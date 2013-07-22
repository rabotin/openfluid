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
 \file GeoRasterValue_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_georastervalue
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <tests-config.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoRasterValue.hpp>


// =====================================================================
// =====================================================================

class GeoRasterValueSub: public openfluid::core::GeoRasterValue
{
  public:

    GeoRasterValueSub(std::string PrefixPath, std::string RelativePath) :
        openfluid::core::GeoRasterValue(PrefixPath, RelativePath)
    {
    }

    GDALDataset* getData()
    {
      return mp_Data;
    }

    std::string getAbsolutePath()
    {
      return computeAbsolutePath(m_FilePath, m_FileName);
    }

    void tryToOpenSource()
    {
      openfluid::core::GeoRasterValue::tryToOpenSource();
    }
};

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoRasterValue/dem.jpeg");

  BOOST_CHECK_EQUAL(Val->getType(),
                    openfluid::core::UnstructuredValue::GeoRasterValue);

  BOOST_CHECK_EQUAL(
      Val->getAbsolutePath(),
      boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR + "/GeoRasterValue/dem.jpeg").string());

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(
      CONFIGTESTS_INPUT_DATASETS_DIR, "GeoRasterValue/wrongfile.jpeg");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Jpeg)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoRasterValue/dem.jpeg");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Gtiff)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoRasterValue/dem.Gtiff");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Img)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoRasterValue/dem.img");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Ascii)
{
  GeoRasterValueSub* Val = new GeoRasterValueSub(CONFIGTESTS_INPUT_DATASETS_DIR,
                                                 "GeoRasterValue/dem.asc");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->getData());

  delete Val;
}

// =====================================================================
// =====================================================================


