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
  \file CoreRepository_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_corerepository
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::core::CoreRepository* Repos;
  openfluid::core::MemoryMonitor* MemMon;

  Repos = openfluid::core::CoreRepository::getInstance();
  MemMon = openfluid::core::MemoryMonitor::getInstance();
  MemMon->setPacketAndKeep(500,10);
  Repos->setMemoryMonitor(MemMon);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::core::CoreRepository* Repos;
  openfluid::core::MemoryMonitor* MemMon;
  int i, PcsOrder;
  openfluid::core::UnitsCollection* UnitsColl;
  openfluid::core::UnitsList_t::iterator UnitsIt, PrevUnitsIt;
  openfluid::core::Unit* U;


  Repos = openfluid::core::CoreRepository::getInstance();
  MemMon = openfluid::core::MemoryMonitor::getInstance();
  MemMon->setPacketAndKeep(500,10);
  Repos->setMemoryMonitor(MemMon);

  for (i=1;i<=250;i++)
  {
    PcsOrder = (i%7)+1;
    Repos->addUnit(openfluid::core::Unit("UnitClassA",i,PcsOrder));
  }

  for (i=1;i<=7325;i++)
  {
    PcsOrder = (i%31)+1;
    Repos->addUnit(openfluid::core::Unit("UnitClassB",i,PcsOrder));
  }

  Repos->sortUnitsByProcessOrder();

  // *** Units count and process order
  UnitsColl = Repos->getUnits("UnitClassA");
  BOOST_REQUIRE_EQUAL(UnitsColl->getList()->size(),250);

  PrevUnitsIt = UnitsColl->getList()->begin();
  for (UnitsIt = UnitsColl->getList()->begin(); UnitsIt != UnitsColl->getList()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->getList()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }

  UnitsColl = Repos->getUnits("UnitClassB");
  BOOST_REQUIRE_EQUAL(UnitsColl->getList()->size(),7325);

  PrevUnitsIt = UnitsColl->getList()->begin();
  for (UnitsIt = UnitsColl->getList()->begin(); UnitsIt != UnitsColl->getList()->end();++UnitsIt)
  {

    if (UnitsIt != UnitsColl->getList()->begin())
    {
      BOOST_REQUIRE_GE(UnitsIt->getProcessOrder(),PrevUnitsIt->getProcessOrder());
    }
    PrevUnitsIt = UnitsIt;
  }


  // *** existing classes
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("UnitClassA"),true);
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("UnitClassB"),true);
  BOOST_REQUIRE_EQUAL(Repos->isUnitsClassExist("WrongClass"),false);


  // *** existing units
  BOOST_REQUIRE(Repos->getUnit("UnitClassA",17) != NULL);
  BOOST_REQUIRE(Repos->getUnit("UnitClassA",1000) == NULL);
  U = Repos->getUnit("UnitClassA",17);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassA");
  BOOST_REQUIRE_EQUAL(U->getID(),17);

  BOOST_REQUIRE(Repos->getUnit("UnitClassB",1333) != NULL);
  BOOST_REQUIRE(Repos->getUnit("UnitClassB",10000) == NULL);
  U = Repos->getUnit("UnitClassB",1333);
  BOOST_REQUIRE_EQUAL(U->getClass(),"UnitClassB");
  BOOST_REQUIRE_EQUAL(U->getID(),1333);

  BOOST_REQUIRE(Repos->getUnit("WrongClass",1) == NULL);
}

// =====================================================================
// =====================================================================
