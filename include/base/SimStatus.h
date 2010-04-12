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
  \file SimStatus.h
  \brief Header of simulation status management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __SIMSTATUS_H__
#define __SIMSTATUS_H__


#include "openfluid-core.h"


namespace openfluid { namespace base {


/**
  Class giving static information on simulation
*/
class SimulationInfo
{
  protected:
    openfluid::core::DateTime m_StartTime;
    openfluid::core::DateTime m_EndTime;
    int m_TimeStep;

    unsigned int m_StepsCount;



  public:

    /**
      Constructor
    */
    SimulationInfo(openfluid::core::DateTime StartTime,
                   openfluid::core::DateTime EndTime,
                   int TimeStep);

    /**
      Destructor
    */
    ~SimulationInfo();

    /**
      Returns the value of a time step (in seconds)

      \return the number of second in a time step
    */
    int getTimeStep() const { return m_TimeStep; };

    /**
      Returns the number of time step for the simulation

      \return the number of time steps
    */
    unsigned int getStepsCount() const { return m_StepsCount; };

    /**
      Returns the date and time of the beginning of the simulation

      \return the DateTime of the beginning of the simulation
    */
    openfluid::core::DateTime getStartTime() const { return m_StartTime; };

    /**
      Returns the date and time of the ending of the simulation

      \return the DateTime of the ending of the simulation
    */
    openfluid::core::DateTime getEndTime() const { return m_EndTime; };


};

/**
  Class giving dynamic information on simulation
*/
class SimulationStatus : public SimulationInfo
{
  private:

    unsigned int m_CurrentStep;

    openfluid::core::DateTime m_CurrentTime;

    bool m_IsFirstStep;

    bool m_IsLastStep;

  public:
    /**
      Constructor
    */
    SimulationStatus(openfluid::core::DateTime StartTime,
                     openfluid::core::DateTime EndTime,
                     int TimeStep);


    /**
      Destructor
    */
    ~SimulationStatus();


    bool switchToNextStep();

    /**
      Returns the current time step of the simulation

      \return the current time step number
    */
    unsigned int getCurrentStep() const { return m_CurrentStep; };

    /**
      Returns the current time of the simulation

      \return the current time
    */
    openfluid::core::DateTime getCurrentTime() const { return m_CurrentTime; };

    /**
      Returns true if the current step is the first step of the simulation
    */
    bool isFirstStep() const { return m_IsFirstStep; };

    /**
      Returns true if the current step is the last step of the simulation
    */
    bool isLastStep() const { return m_IsLastStep; };


};



} } // namespace openfluid::core


#endif




