/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file LoopsFunc.cpp
  \brief Implements ...
*/


#include "LoopsFunc.h"


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(LoopsFunction);


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.loops"));
  DECLARE_SIGNATURE_NAME(("test function for units loops"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


LoopsFunction::LoopsFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


LoopsFunction::~LoopsFunction()
{


}


// =====================================================================
// =====================================================================


bool LoopsFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool LoopsFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{

/*
  Tests are based on the following spatial domain,
  with classes TestUnits (TU) and OtherUnits (OU),
  represented here as class.ID


TU.1         TU.2
  |            |
  -->  TU.22 <--
         |
         --> TU.18
               |
    TU.52 --> OU.5 <-- OU.13
               |
               --> OU.25



<?xml version="1.0" standalone="yes"?>
<openfluid>
  <domain>
    <definition>

      <unit class="TestUnits" ID="1" pcsorder="1">
        <to class="TestUnits" ID="22" />
      </unit>
      <unit class="TestUnits" ID="2" pcsorder="1">
        <to class="TestUnits" ID="22" />
      </unit>
      <unit class="TestUnits" ID="52" pcsorder="1">
        <to class="OtherUnits" ID="5" />
      </unit>
      <unit class="OtherUnits" ID="13" pcsorder="1">
        <to class="OtherUnits" ID="5" />
      </unit>


      <unit class="TestUnits" ID="22" pcsorder="2">
        <to class="TestUnits" ID="18" />
      </unit>


      <unit class="TestUnits" ID="18" pcsorder="3">
        <to class="OtherUnits" ID="5" />
      </unit>


      <unit class="OtherUnits" ID="5" pcsorder="4">
        <to class="OtherUnits" ID="25" />
      </unit>
      <unit class="OtherUnits" ID="25" pcsorder="5">
      </unit>


    </definition>
  </domain>
</openfluid>


*/


  openfluid::core::Unit *TU = NULL;
  openfluid::core::Unit *OU = NULL;
  openfluid::core::Unit *ToUnit = NULL;
  openfluid::core::Unit *FromUnit = NULL;
  openfluid::core::UnitsPtrList_t *ToList = NULL;
  openfluid::core::UnitsPtrList_t *FromList = NULL;
  unsigned int CountInside;

  DECLARE_UNITS_ORDERED_LOOP(11)
  DECLARE_UNITS_ORDERED_LOOP(1)
  DECLARE_UNITS_ORDERED_LOOP(2)
  DECLARE_UNITS_LIST_LOOP(1)



  // ===== loop inside loop =====

  CountInside = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    BEGIN_UNITS_ORDERED_LOOP(2,"TestUnits",TU)
      CountInside++;
    END_LOOP

  END_LOOP

  if (CountInside != 25)
    OPENFLUID_RaiseError("tests.loops","runStep()","wrong units for while loop inside loop");


  // ===== unit class =====

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)
    if (TU->getClass() != "TestUnits")
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong units class");
  END_LOOP



  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)

    if (TU->getID() == 22)
    {
      if (TU->getFromUnits("TestUnits") == NULL || TU->getFromUnits("TestUnits")->size() != 2)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for TestUnit 22");

      if (TU->getToUnits("TestUnits") == NULL || TU->getToUnits("TestUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(TestUnits) count for TestUnit 22");

    }

  END_LOOP


  BEGIN_UNITS_ORDERED_LOOP(11,"OtherUnits",OU)

    if (OU->getID() == 5)
    {
      if (OU->getFromUnits("TestUnits") == NULL || OU->getFromUnits("TestUnits")->size() != 2)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) count for OtherUnit 5");

      if (OU->getFromUnits("OtherUnits") == NULL || OU->getFromUnits("OtherUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) count for OtherUnit 5");

      if (OU->getToUnits("OtherUnits") == NULL || OU->getToUnits("OtherUnits")->size() != 1)
        OPENFLUID_RaiseError("tests.loops","runStep()","wrong To-units(OtherUnits) count for OtherUnit 5");


      FromList = OU->getFromUnits("TestUnits");
      BEGIN_UNITS_LIST_LOOP(1,FromList,FromUnit)

        if (FromUnit->getID() != 18 && FromUnit->getID() != 52)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(TestUnits) content for OtherUnit 5");

      END_LOOP

      FromList = OU->getFromUnits("OtherUnits");
      BEGIN_UNITS_LIST_LOOP(1,FromList,FromUnit)

        if (FromUnit->getID() != 13)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong from-units(OtherUnits) content for OtherUnit 5");

      END_LOOP


      ToList = OU->getToUnits("OtherUnits");
      BEGIN_UNITS_LIST_LOOP(1,ToList,ToUnit)

        if (ToUnit->getID() != 25)
          OPENFLUID_RaiseError("tests.loops","runStep()","wrong to-units(OtherUnits) content for OtherUnit 5");

      END_LOOP



    }

  END_LOOP


  // ===== process order =====

  unsigned int LastPcsOrd;
  std::string LastStr, CurrentStr, IDStr;

  LastPcsOrd = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)
    if (TU->getProcessOrder() < LastPcsOrd)
    {

      openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
      openfluid::tools::ConvertValue(TU->getProcessOrder(),&CurrentStr);
      openfluid::tools::ConvertValue(TU->getID(),&IDStr);
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit TestUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
    }

    LastPcsOrd = TU->getProcessOrder();
  END_LOOP

  LastPcsOrd = 0;
  BEGIN_UNITS_ORDERED_LOOP(1,"OtherUnits",OU)
    if (OU->getProcessOrder() < LastPcsOrd)
    {
      openfluid::tools::ConvertValue(LastPcsOrd,&LastStr);
      openfluid::tools::ConvertValue(OU->getProcessOrder(),&CurrentStr);
      openfluid::tools::ConvertValue(OU->getID(),&IDStr);
      OPENFLUID_RaiseError("tests.loops","runStep()","wrong process order at unit OtherUnits#"+ IDStr + " (last ord: "+LastStr+", current ord: "+CurrentStr+")");
    }

    LastPcsOrd = TU->getProcessOrder();
  END_LOOP



  return true;
}

// =====================================================================
// =====================================================================


bool LoopsFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

