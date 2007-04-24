/**
  \file HayamiRSPlug.h
  \brief implementation of HayamiRSPlug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HayamiRSFunc.h"
#include "HayamiRSPlug.h"


mhydasdk::base::Plugin* GetMHYDASPlugin()
{
  return new HayamiRSPlug();
}


// =====================================================================
// =====================================================================


HayamiRSPlug::HayamiRSPlug()
          : mhydasdk::base::Plugin()
{
  mp_Signature = new mhydasdk::base::Signature();

  mp_Signature->Author = wxT("Jean-Christophe FABRE");
  mp_Signature->AuthorEmail = wxT("fabrejc@ensam.inra.fr");
  mp_Signature->ID = wxT("hayamirs");
  mp_Signature->FunctionType = mhydasdk::base::SIMULATION;
  mp_Signature->Name = wxT("Hayami hydrological transfer on reach segments");
  mp_Signature->Description = wxT("");

  mp_Signature->HandledVarsPropsParams.Add(wxT("pvar;RS;qoutput;-;-"));
  
  mp_Signature->HandledVarsPropsParams.Add(wxT("prop;RS;nmanning;-;-"));

}


// =====================================================================
// =====================================================================


HayamiRSPlug::~HayamiRSPlug()
{

}


// =====================================================================
// =====================================================================


mhydasdk::base::Function* HayamiRSPlug::getFunction(mhydasdk::core::CoreRepository* CoreData)
{
  return new HayamiRSFunction(CoreData);
}


