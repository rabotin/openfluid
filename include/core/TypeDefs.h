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
  \file TypeDefs.h
  \brief header of data types definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

#include <vector>
#include <list>
#include <map>
#include "Vector.h"


namespace openfluid { namespace core {



/**
  Type definition for the ID of units
*/
typedef unsigned int UnitID_t;

/**
  Type definition for the process order of units
*/
typedef int PcsOrd_t;

/**
  Type definition for the class of units
*/
typedef std::string UnitClass_t;

/**
  Type definition for name of input data
*/
typedef std::string InputDataName_t;

/**
  Type definition for name of a variable
*/
typedef std::string VariableName_t;

/**
  Type definition for a time step value
*/
typedef unsigned int TimeStep_t;


typedef std::pair<UnitClass_t,UnitID_t> UnitClassID_t;

typedef std::pair<UnitClassID_t,UnitClassID_t> UnitsLink_t;


/**
 Type definition for scalar values
 */
typedef double ScalarValue;

/**
 Type definition for vector values. This type is based on the openfluid::core::Vector template.
 \sa openfluid::core::Vector

 <i>usage examples:</i>
 \code
  // create an empty vector
  openfluid::core::VectorValue TmpValue;

  // give a size of 10 to this vector
  TmpValue = openfluid::core::VectorValue(10);

  // give a size of 10 to this vector and fill it with a value of2
  TmpValue = openfluid::core::VectorValue(10,2);

  // read the value at index 6 in put it in a variable MyVar
  MyVar = TmpValue[6];
  // or
  MyVar = TmpValue.at(6);

  // set or modify the value 15 at index 3
  TmpValue.setElement(3,15);
 \endcode

 */
typedef Vector<ScalarValue> VectorValue;

typedef std::string InputDataValue;


/**
 Type definition for a time serie of scalar values
 */
typedef std::vector<ScalarValue> SerieOfScalarValue;


/**
 Type definition for a time serie of vectorvalues
 */
typedef std::vector<VectorValue> SerieOfVectorValue;

// =====================================================================
// =====================================================================


typedef std::string FuncParamKey_t;
typedef std::string FuncParamValue_t;

/**
  Hash table for parameters (distributed properties, distributed initial conditions, ...)
*/
typedef std::map<FuncParamKey_t,FuncParamValue_t> FuncParamsMap_t;


// =====================================================================
// =====================================================================
/**
  Map associating a float value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,float> IDFloatMap;

/**
  Map associating a double precision value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,double> IDDoubleMap;

/**
  Map associating an openfluid::core::ScalarValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,ScalarValue> IDScalarValueMap;

/**
  Map associating an integer value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,int> IDIntMap;

/**
  Map associating a boolean value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,bool> IDBoolMap;

/**
  Map associating an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,VectorValue> IDVectorValueMap;

/**
  Map associating a pointer to an openfluid::core::VectorValue value to a unit, indexed by its ID
*/
typedef std::map<UnitID_t,VectorValue*> IDVectorValuePtrMap;

typedef std::map<UnitID_t,SerieOfScalarValue> IDSerieOfScalarValueMap;

typedef std::map<UnitID_t,SerieOfScalarValue*> IDSerieOfScalarValuePtrMap;




} } // namespaces


#endif /*TYPEDEFS_H_*/
