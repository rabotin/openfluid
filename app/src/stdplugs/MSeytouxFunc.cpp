/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#include "MSeytouxFunc.h"


MorelSeytouxFunc::MorelSeytouxFunc(mhydasdk::core::CoreRepository *CoreData)
                : Function(CoreData)
{
  SU_VARIABLE_TO_CREATE("runoff");
  SU_VARIABLE_TO_CREATE("infiltration");
}

// =====================================================================
// =====================================================================

MorelSeytouxFunc::~MorelSeytouxFunc()
{

}



// =====================================================================
// =====================================================================

bool MorelSeytouxFunc::initParams(mhydasdk::core::ParamsMap Params)
{

  // std::cout << "Momo initParams()" << std::endl;
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::initializeRun()
{

  bool IsOK =  true;


  return IsOK;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::checkConsistency()
{
  // std::cout << "Momo checkConsistency()" << std::endl;

  return Function::checkConsistency();
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::runStep(mhydasdk::base::SimulationStatus* SimStatus)
{

  // std::cout << "coucou c'est Momo run " << mp_CoreData->getSpatialData()->getSUsCollection()->size();
  return true;
}


// =====================================================================
// =====================================================================


bool MorelSeytouxFunc::finalizeRun()
{
  // std::cout << "Momo finalize()" << std::endl;
  return true;
}
