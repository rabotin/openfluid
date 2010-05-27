/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file PluginManager.cpp
  \brief implements the plugins management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include <openfluid/config.hpp>

#include <openfluid/machine/DynamicLib.hpp>
#include <openfluid/machine/PluginManager.hpp>
#include <openfluid/tools.hpp>
#include <openfluid/base/OFException.hpp>

#include <iostream>

namespace openfluid { namespace machine {


PluginManager* PluginManager::mp_Singleton = NULL;


// =====================================================================
// =====================================================================


PluginManager::PluginManager()
{

}

// =====================================================================
// =====================================================================

PluginManager::~PluginManager()
{

}


// =====================================================================
// =====================================================================


PluginManager* PluginManager::getInstance()
{
  if (mp_Singleton == NULL) mp_Singleton = new PluginManager();
  return mp_Singleton;
}


// =====================================================================
// =====================================================================


ModelItemInstance* PluginManager::buildPluginContainer(std::string PluginFilename)
{
  std::string PluginFile = openfluid::base::RuntimeEnvironment::getInstance()->getPluginFullPath(PluginFilename);
  ModelItemInstance* Plug = NULL;

  DynamicLib *PlugLib = new DynamicLib(PluginFile);

  // library loading
  if (PluginFile.length()>0 && PlugLib->load())
  {
    Plug = new ModelItemInstance();
    Plug->Filename = PluginFile;

    if (PlugLib->hasSymbol(PLUGSDKVERSION_PROC_NAME))
    {
      openfluid::base::GetSDKVersionProc SDKProc;

      SDKProc = (openfluid::base::GetSDKVersionProc)PlugLib->getSymbol(PLUGSDKVERSION_PROC_NAME);
      Plug->SDKCompatible = (openfluid::config::FULL_VERSION == SDKProc());
    }
    else Plug->SDKCompatible = false;


    if (Plug->SDKCompatible)
    {

      // checks if the handle proc exists
      if (PlugLib->hasSymbol((PLUGFUNCTION_PROC_NAME)) && PlugLib->hasSymbol(PLUGSIGNATURE_PROC_NAME))
      {

        // hooks the handle proc
        openfluid::base::GetSignatureProc SignProc = (openfluid::base::GetSignatureProc)PlugLib->getSymbol(PLUGSIGNATURE_PROC_NAME);


        if (SignProc != NULL)
        {
          Plug->Signature = SignProc();

          if (Plug->Signature == NULL)
            throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Signature from plugin file " + PluginFilename + " cannot be instanciated");

          openfluid::base::GetPluggableFunctionProc PlugProc = (openfluid::base::GetPluggableFunctionProc)PlugLib->getSymbol(PLUGFUNCTION_PROC_NAME);
          if (PlugProc != NULL)
          {
            Plug->Function = PlugProc();

            if (Plug->Function == NULL)
              throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Function from plugin file " + PluginFilename + " cannot be instanciated");

          }
          else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find function in plugin file " + PluginFilename);
        }
        else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Unable to find signature in plugin file " + PluginFilename);

        // unloads the library
        //PlugLib->unload();
        //delete PlugLib;

      }
      else throw openfluid::base::OFException("kernel","PluginManager::buildPluginContainer","Format error in plugin file " + PluginFilename);
    }
  }
  else throw openfluid::base::OFException(/*"kernel"*/strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__,/*"PluginManager::buildPluginContainer"*/ __FUNCTION__,"Unable to find plugin file " + PluginFilename);

  return Plug;
}


// =====================================================================
// =====================================================================


ArrayOfModelItemInstance PluginManager::getAvailableFunctions(const std::string Pattern)
{
  ArrayOfModelItemInstance PluginsContainers;
  std::vector<std::string> PluginsPaths = openfluid::base::RuntimeEnvironment::getInstance()->getPluginsPaths();
  std::vector<std::string> PluginFiles;
  std::vector<std::string> TmpFiles;
  unsigned int i,j;


  for (i=0;i<PluginsPaths.size();i++)
  {
    TmpFiles = openfluid::tools::GetFilesByExt(PluginsPaths[i],openfluid::config::PLUGINS_EXT,false,true);
    for (j=0;j<TmpFiles.size();j++) PluginFiles.push_back(TmpFiles[j]);
  }


  ModelItemInstance* CurrentPlug;

  for (i=0;i<PluginFiles.size();i++)
  {
    CurrentPlug = buildPluginContainer(PluginFiles[i]);

    if (CurrentPlug != NULL && CurrentPlug->SDKCompatible)
    {
      if (Pattern != "")
      {
        if (openfluid::tools::WildcardMatching(Pattern,CurrentPlug->Signature->ID))
          PluginsContainers.push_back(CurrentPlug);
      }
      else PluginsContainers.push_back(CurrentPlug);
    }

  }

  return PluginsContainers;
}


// =====================================================================
// =====================================================================


ModelItemInstance* PluginManager::getPlugin(std::string PluginName,
                                          openfluid::base::ExecutionMessages* ExecMsgs,
                                          openfluid::core::CoreRepository* CoreData)
{

  ModelItemInstance* Plug = buildPluginContainer(PluginName+openfluid::config::PLUGINS_EXT);



  if (Plug != NULL && Plug->SDKCompatible)
  {
    Plug->Function->setDataRepository(CoreData);
    Plug->Function->setExecutionMessages(ExecMsgs);
    Plug->Function->setFunctionEnvironment(openfluid::base::RuntimeEnvironment::getInstance()->getFunctionEnvironment());
    return Plug;
  }


  return NULL;
}


} } //namespaces

