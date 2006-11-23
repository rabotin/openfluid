/**
  \file main.cpp
  \brief implements main

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include <iostream>

#include <wx/log.h>


#include "AppTools.h"
#include "main.h"
#include "setup.h"

using namespace mhydasdk::base;
using namespace mhydasdk::core;


wxString LastError::Message;

// =====================================================================
// =====================================================================


IMPLEMENT_APP_CONSOLE(MHYDASApp);

// =====================================================================
// =====================================================================


bool MHYDASApp::buildModel()
{
  bool ExecStatus;

  std::cout << "* Building model... ";
  std::cout.flush();

  ExecStatus = mp_Engine->buildModel();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::loadData()
{
  bool ExecStatus;

  std::cout << "* Loading data... ";
  std::cout.flush();

  ExecStatus = mp_Engine->loadData();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::checkConsistency()
{

  bool ExecStatus;

  std::cout << "* Preparing data and checking consistency... ";
  std::cout.flush();

  ExecStatus = mp_Engine->prepareDataAndCheckConsistency();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::runSimulation()
{

  bool ExecStatus;

  printDataInfos();

  std::cout << "**** Starting simulation ****" << endl;
  std::cout.flush();

  m_EffectiveStartTime = wxDateTime::Now();

  ExecStatus = mp_Engine->run();

  m_EffectiveEndTime = wxDateTime::Now();

  if (ExecStatus) std::cout << "**** Simulation completed ****" << endl << endl;
  else  std::cout << "**** Simulation aborted ****" << endl << endl;
  std::cout.flush();

  return ExecStatus;
}

// =====================================================================
// =====================================================================


bool MHYDASApp::saveResults()
{

  bool ExecStatus;

  std::cout << "* Saving results... ";
  std::cout.flush();

  ExecStatus = mp_Engine->saveResults();

  printlnExecStatus(ExecStatus);

  return ExecStatus;
}

// =====================================================================
// =====================================================================


void MHYDASApp::printlnExecStatus(bool Status)
{
  if (Status) std::cout << "[OK]" << std::endl;
  else std::cout << "[Error]" << std::endl;
  std::cout.flush();
}


// =====================================================================
// =====================================================================


void MHYDASApp::printMHYDASInfos()
{
  
  int Width = 60;
  wxString VersionInfo = wxT("v ");
  wxString Whites = wxT("");

  VersionInfo = VersionInfo + MAJOR_VERSION + wxT(".") + MINOR_VERSION;

  
  if (BUILD_VERSION != wxT(""))
  {
    
    VersionInfo = VersionInfo + wxT(" (") + BUILD_VERSION + wxT(" build");
     
    #ifdef SVN_REV
    VersionInfo = VersionInfo + wxT(", rev ") + SVN_REVISION;
    #endif
    VersionInfo = VersionInfo + wxT(")");
  }
  
  // on centre le bizness avec des blancs
  for (int i=0;i<((Width-VersionInfo.Length())/2);i++) Whites = Whites + wxT(" ");
  
  VersionInfo = Whites + VersionInfo;
  
  
  std::cout << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << "                       M H Y D A S                         " << std::endl;
  std::cout << "      Distributed hydrological model for agrosystems       " << std::endl;
  std::cout << std::endl;
  std::cout << VersionInfo.mb_str(wxConvUTF8) << std::endl;
  std::cout << std::endl;  
  std::cout << "               LISAH-INRA, Montpellier, France             " << std::endl;
  std::cout << "===========================================================" << std::endl;
  std::cout << std::endl;
  std::cout.flush();
}



// =====================================================================
// =====================================================================

void MHYDASApp::printDataInfos()
{
  int TimeStepsNbr = (mp_CoreData->getRainEvent()->getEventEndingTime().getRawTime() - mp_CoreData->getRainEvent()->getEventStartingTime().getRawTime()) / mp_Engine->getConfig().DeltaT; 
  
  std::cout << std::endl;
  std::cout << "Spatial objects: " << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getSUsCollection()->size() << " Surface Units" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getRSsCollection()->size() << " Reach Segments" << std::endl
            << "   - " << mp_CoreData->getSpatialData()->getGUsCollection()->size() << " Groundwater Units" << std::endl;
  std::cout << "Rain source(s): " << mp_CoreData->getRainEvent()->getRainSourceCollection().size() << std::endl;
  std::cout << "Simulation from " << _C(mp_CoreData->getRainEvent()->getEventStartingTime().asString())
            << " to " << _C(mp_CoreData->getRainEvent()->getEventEndingTime().asString()) << std::endl
            << "         -> " <<  (TimeStepsNbr+1) << " time steps of " << mp_Engine->getConfig().DeltaT << " seconds" << std::endl;
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsList()
{
  ArrayOfPluginsSignatures Signatures = mp_PlugMan->getAvailableFunctionsList();

  std::cout << "Available pluggable functions:" << std::endl;


  if (Signatures.GetCount() > 0)
  {
    for (int i=0;i<Signatures.GetCount();i++) std::cout << "  - " << Signatures[i]->Name.mb_str(wxConvUTF8) << std::endl;
  }
  else
  {
    std::cout << "  (none)" << std::endl;
  }
  std::cout << std::endl;
  std::cout.flush();

}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsVarsPropsParamsReport(wxArrayString VarsPropsParams, wxString Suffix, bool IsXMLFormat)
{
  wxArrayString CurrentStr;  
    

  for (int i=0;i<VarsPropsParams.GetCount();i++)
  {
    CurrentStr = SplitString(VarsPropsParams[i],wxT(";"));
    if (CurrentStr.GetCount() == 5)
    {
      std::cout << Suffix.mb_str(wxConvUTF8);
      if (IsXMLFormat)
      {
        std::cout << "<varpropparam";
        std::cout << " type=\"" << CurrentStr[0].mb_str(wxConvUTF8) << "\"";
        std::cout << " distribution=\"" << CurrentStr[1].mb_str(wxConvUTF8) << "\"";        
        std::cout << " ID=\"" << CurrentStr[2].mb_str(wxConvUTF8) << "\"";                
        std::cout << " description=\"" << CurrentStr[3].mb_str(wxConvUTF8) << "\"";
        std::cout << " unit=\"" << CurrentStr[4].mb_str(wxConvUTF8) << "\"";        
        std::cout << "/>" << std::endl;
      }
      else
      {
        std::cout << CurrentStr[2].mb_str(wxConvUTF8) << ": " << CurrentStr[0].mb_str(wxConvUTF8) << " ";
        if (CurrentStr[1] == wxT("global")) std::cout << "globally distributed ";
        else std::cout << "distributed on " << CurrentStr[1].mb_str(wxConvUTF8) << "s ";
        std::cout << "(" << CurrentStr[4].mb_str(wxConvUTF8) << ")." << std::endl;        
      }   
    }
  }
}


// =====================================================================
// =====================================================================

void MHYDASApp::printPluginsReport(bool IsXMLFormat)
{

  ArrayOfPluginsSignatures Signatures = mp_PlugMan->getAvailableFunctionsList(); 

  // insertion du début du fichier XML
  if (IsXMLFormat)
  {
    std::cout << "<?xml version=\"1.0\" standalone=\"yes\"?>" << std::endl;
    std::cout << "<mhydas>" << std::endl;   
    std::cout << "  <funcsreport>" << std::endl;   
  }
  
  if (Signatures.GetCount() > 0)
  {
    for (int i=0;i<Signatures.GetCount();i++)
    {
      if (IsXMLFormat)
      {
        std::cout << "    <funcdef ID=\"" << Signatures[i]->ID.mb_str(wxConvUTF8) 
                  << "\" name=\"" << Signatures[i]->Name.mb_str(wxConvUTF8) << "\">" << std::endl;
        std::cout << "      <description>" << Signatures[i]->Description.mb_str(wxConvUTF8) << "</description>" << std::endl;
        std::cout << "      <version major=\"" << Signatures[i]->MajorVersion.mb_str(wxConvUTF8) 
                  << "\" minor=\"" << Signatures[i]->MinorVersion.mb_str(wxConvUTF8) << "\"/>" << std::endl;
        std::cout << "      <author name=\"" << Signatures[i]->Author.mb_str(wxConvUTF8) 
                  << "\" email=\"" << Signatures[i]->AuthorEmail.mb_str(wxConvUTF8) << "\"/>" << std::endl;

        std::cout << "      <handleddata>" << std::endl;                  
        printPluginsVarsPropsParamsReport(Signatures[i]->HandledVarsPropsParams,wxT("        "),IsXMLFormat);
        std::cout << "      </handleddata>" << std::endl;                  
                 
      }
      else
      {
        std::cout << "* " << Signatures[i]->ID.mb_str(wxConvUTF8) << std::endl;           
        std::cout << "   - Name: " << Signatures[i]->Name.mb_str(wxConvUTF8) << std::endl;        
        std::cout << "   - Description: " << Signatures[i]->Description.mb_str(wxConvUTF8) << std::endl;                
        std::cout << "   - Version: " << Signatures[i]->MajorVersion.mb_str(wxConvUTF8) << "." << Signatures[i]->MinorVersion.mb_str(wxConvUTF8) << std::endl;                        
        std::cout << "   - Author: " << Signatures[i]->Author.mb_str(wxConvUTF8) << " (" << Signatures[i]->AuthorEmail.mb_str(wxConvUTF8) << ")" << std::endl;                                
        std::cout << "   - Handled data" << std::endl;                  
        printPluginsVarsPropsParamsReport(Signatures[i]->HandledVarsPropsParams,wxT("     . "),IsXMLFormat);
        
      }

      if (IsXMLFormat)
      {
        std::cout << "    </funcdef> " << std::endl;
      }  
      else
      {
        if (i != Signatures.GetCount()-1)
          std::cout << "================================================================================" << std::endl;
      }        
    }
     
  }
  else
  {

  }

  // insertion de la fin du fichier XML
  if (IsXMLFormat)
  {
    std::cout << "  </funcsreport>" << std::endl;   
    std::cout << "</mhydas>" << std::endl;   
  }

  std::cout.flush();

  
}

// =====================================================================
// =====================================================================

bool MHYDASApp::stopAppReturn()
{
  std::cout << std::endl << "Oooops! " << LastError::Message.mb_str(wxConvUTF8) << std::endl;
  std::cout << "Aborting MHYDAS application." << std::endl;
  std::cout << std::endl;
  std::cout.flush();

  return false;
}

// =====================================================================
// =====================================================================



bool MHYDASApp::OnInit()
{
  wxString TmpStr;

  // =========== initialization ===========

  m_OKToRun = true;

  mp_RunEnv = new RuntimeEnvironment(wxPathOnly(GetExecutablePath()));

  mp_PlugMan = new PluginManager(mp_RunEnv);

  LastError::Message = wxT("");

  wxLog::SetTimestamp(NULL);



  // =========== command line parsing ===========

  wxCmdLineParser Parser(CmdLineDesc,argc,argv);

  if (Parser.Parse() != 0)
  {
     std::cout << std::endl;
     return false;
  }

  if (Parser.Found(wxT("v"))) wxLog::SetVerbose(true);

  if (Parser.Found(wxT("f")) || Parser.Found(wxT("r")) || Parser.Found(wxT("x")))
  {
    if (Parser.Found(wxT("f")))
    {
      printMHYDASInfos();
      printPluginsList();
    }  

    if (Parser.Found(wxT("r"))) printPluginsReport(false);
    if (Parser.Found(wxT("x"))) printPluginsReport(true);

    m_OKToRun = false;
  }
  else
  {
    printMHYDASInfos();
    
    if (Parser.Found(wxT("i"),&TmpStr)) mp_RunEnv->setInputDir(TmpStr);
    if (Parser.Found(wxT("o"),&TmpStr)) mp_RunEnv->setOutputDir(TmpStr);
    if (Parser.Found(wxT("g"))) mp_RunEnv->setDateTimeOutputDir();

    wxLogVerbose(wxT("Input dir: ")+mp_RunEnv->getInputDir());
    wxLogVerbose(wxT("Output dir: ")+mp_RunEnv->getOutputDir());
  }

  return true;
}


// =====================================================================
// =====================================================================

int MHYDASApp::OnRun()
{

  if (m_OKToRun)
  {

    m_TotalStartTime = wxDateTime::Now();

    mp_CoreData = new CoreRepository();

    mp_Engine = new Engine(mp_CoreData,mp_RunEnv,mp_PlugMan);


    mp_CoreData->getRainEvent()->enableFirstSerieConstraint(true);

    // chargement, verification et montage du modele
    if (!buildModel()) return stopAppReturn();

    // chargement et verification du jeu de donnees d'entree
    if (!loadData()) return stopAppReturn();

    // verification de la coherence de l'ensemble
    if (!checkConsistency()) return stopAppReturn();

    // simulation
    if (!runSimulation()) return stopAppReturn();

    // sauvegarde des résultats
    if (!saveResults()) return stopAppReturn();

    m_TotalEndTime = wxDateTime::Now();

    std::cout << std::endl;

    wxTimeSpan EffSimTime = m_EffectiveEndTime.Subtract(m_EffectiveStartTime);
    wxTimeSpan TotSimTime = m_TotalEndTime.Subtract(m_TotalStartTime);


    std::cout << "Simulation run time: " << EffSimTime.Format(wxT("%Hh %Mm %Ss")).mb_str(wxConvUTF8) << std::endl;
    std::cout << "     Total run time: " << TotSimTime.Format(wxT("%Hh %Mm %Ss")).mb_str(wxConvUTF8) << std::endl;
    std::cout << std::endl;
  }


  return true;

}



// =====================================================================
// =====================================================================

int MHYDASApp::OnExit()
{

}


// =====================================================================
// =====================================================================


