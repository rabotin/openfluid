/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.

*/


/**
  @file LandPrimitivesSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


BEGIN_SIMULATOR_SIGNATURE("tests.primitives.land")

  DECLARE_NAME("");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("10.07");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


  DECLARE_UPDATED_UNITSGRAPH("Created units graph from scratch");
  DECLARE_UPDATED_UNITSCLASS("VU","Units class for virtual units, parents of OU and TU units");
  DECLARE_UPDATED_UNITSCLASS("TU","Test Units class");
  DECLARE_UPDATED_UNITSCLASS("OU","Other Units class");
  DECLARE_UPDATED_UNITSCLASS("MU","Matrix Units class");
END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class LandPrimitivesSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    LandPrimitivesSimulator() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~LandPrimitivesSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {  }

    // =====================================================================
    // =====================================================================


    void prepareData()
    {

 /*
      TU.1         TU.2
        |            |
        -->  TU.22 <--
               |
               --> TU.18
                     |
          TU.52 --> OU.5 <-- OU.13
                     |
                     --> OU.25

       VU1 <-> VU2

   with:
   TU1, TU2, TU22, TU18 are children of VU1
   TU52, OU5, OU13, OU25 are children of VU2
*/
      OPENFLUID_AddUnit("VU",1,1);
      OPENFLUID_AddUnit("VU",2,2);
      OPENFLUID_AddUnit("TU",1,1);
      OPENFLUID_AddUnit("TU",2,1);
      OPENFLUID_AddUnit("TU",22,2);
      OPENFLUID_AddUnit("TU",18,3);
      OPENFLUID_AddUnit("TU",52,1);
      OPENFLUID_AddUnit("OU",5,4);
      OPENFLUID_AddUnit("OU",13,1);
      OPENFLUID_AddUnit("OU",25,5);

      OPENFLUID_AddFromToConnection("VU",1,"VU",2);
      OPENFLUID_AddFromToConnection("VU",2,"VU",1);
      OPENFLUID_AddFromToConnection("TU",1,"TU",22);
      OPENFLUID_AddFromToConnection("TU",2,"TU",22);
      OPENFLUID_AddFromToConnection("TU",22,"TU",18);
      OPENFLUID_AddFromToConnection("TU",18,"OU",5);
      OPENFLUID_AddFromToConnection("TU",52,"OU",5);
      OPENFLUID_AddFromToConnection("OU",13,"OU",5);
      OPENFLUID_AddFromToConnection("OU",5,"OU",25);

      OPENFLUID_AddChildParentConnection("TU",1,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",2,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",22,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",18,"VU",1);
      OPENFLUID_AddChildParentConnection("TU",52,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",5,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",13,"VU",2);
      OPENFLUID_AddChildParentConnection("OU",25,"VU",2);


      unsigned int Cols = 5;
      unsigned int Rows = 7;
      OPENFLUID_BuildUnitsMatrix("MU",Cols,Rows);


      unsigned int UnitsCount;

      OPENFLUID_GetUnitsCount("VU",UnitsCount);
      if (UnitsCount != 2)
        OPENFLUID_RaiseError("tests.primitives.land","incorrect number of VU units");

      OPENFLUID_GetUnitsCount("TU",UnitsCount);
      if (UnitsCount != 5)
        OPENFLUID_RaiseError("tests.primitives.land","incorrect number of TU units");

      OPENFLUID_GetUnitsCount("OU",UnitsCount);
      if (UnitsCount != 3)
        OPENFLUID_RaiseError("tests.primitives.land","incorrect number of OU units");

      OPENFLUID_GetUnitsCount("MU",UnitsCount);
      if (UnitsCount != (Cols*Rows))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect number of MU units");


      OPENFLUID_GetUnitsCount(UnitsCount);
      if (UnitsCount != (10+Cols*Rows))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect total number of units");


      openfluid::core::SpatialUnit* CurrentUnit = NULL;

      CurrentUnit = OPENFLUID_GetUnit("TU",1);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",22))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit TU#1");

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",22))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit TU#2");

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (OPENFLUID_IsUnitConnectedTo(CurrentUnit,"TU",522))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit TU#2 (522)");


      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",1) || !OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit TU#22");

      CurrentUnit = OPENFLUID_GetUnit("TU",18);
      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit TU#18");

      CurrentUnit = OPENFLUID_GetUnit("OU",5);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",18))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection for unit OU#5");


      CurrentUnit = OPENFLUID_GetUnit("TU",1);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection for unit TU#1");

      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection for unit TU#22");

      CurrentUnit = OPENFLUID_GetUnit("TU",18);
      if (OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection for unit TU#18");

      CurrentUnit = OPENFLUID_GetUnit("OU",5);
      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection for unit OU#5");


      openfluid::core::SpatialUnit* TU;


      OPENFLUID_UNITS_ORDERED_LOOP("TU", TU)
      {
        OPENFLUID_SetAttribute(TU,"indatadbl",double(TU->getID())*0.1);
        OPENFLUID_SetAttribute(TU,"indatastr","C0DE1");
      }





      CurrentUnit = OPENFLUID_GetUnit("TU",2);

      if (CurrentUnit == NULL)
        OPENFLUID_RaiseError("tests.primitives.land","incorrect unit TU#2 before deletion");

      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (!OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection before deletion of unit TU#2");


      CurrentUnit = OPENFLUID_GetUnit("VU",1);
      if (!OPENFLUID_IsUnitParentOf(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection before deletion of unit TU#2");


      OPENFLUID_DeleteUnit("TU",2);

      CurrentUnit = OPENFLUID_GetUnit("TU",2);
      if (CurrentUnit != NULL)
        OPENFLUID_RaiseError("tests.primitives.land","incorrect deletion for unit TU#2");


      CurrentUnit = OPENFLUID_GetUnit("TU",22);
      if (OPENFLUID_IsUnitConnectedFrom(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection after deletion of unit TU#2");

      CurrentUnit = OPENFLUID_GetUnit("VU",1);
      if (OPENFLUID_IsUnitParentOf(CurrentUnit,"TU",2))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect parent-child connection after deletion of unit TU#2");



      CurrentUnit = OPENFLUID_GetUnit("OU",13);

      if (!OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect from-to connection between units OU#13 and OU#5 before removing");

      OPENFLUID_RemoveFromToConnection("OU",13,"OU",5);

      if (OPENFLUID_IsUnitConnectedTo(CurrentUnit,"OU",5))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect removing of from-to connection between units OU#13 and OU#5");


      CurrentUnit = OPENFLUID_GetUnit("TU",18);

      if (!OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect child-parent connection between units TU#18 and VU#1 before removing");

      OPENFLUID_RemoveChildParentConnection("TU",18,"VU",1);

      if (OPENFLUID_IsUnitChildOf(CurrentUnit,"VU",1))
        OPENFLUID_RaiseError("tests.primitives.land","incorrect removing of child-parent connection between units TU#18 and VU#1");


    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {
      openfluid::core::SpatialUnit* TU;
      double DblValue;
      std::string StrValue;

      OPENFLUID_UNITS_ORDERED_LOOP("TU", TU)
      {
        OPENFLUID_GetAttribute(TU,"indatadbl",DblValue);
        if (!openfluid::scientific::isVeryClose(double(TU->getID())*0.1,DblValue))
          OPENFLUID_RaiseError("tests.primitives.land","attributes error for indatadbl");

        OPENFLUID_GetAttribute(TU,"indatastr",StrValue);
        if (StrValue != "C0DE1")
          OPENFLUID_RaiseError("tests.primitives.land","attributes error for indatastr");

      }

      return DefaultDeltaT();
    }

    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      return Never();
    }

    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_SIMULATOR_CLASS(LandPrimitivesSimulator)

