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
 \file WareSrcWidgetCollection.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcWidgetCollection.hpp>

#include <QTabWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/ui/waresdev/WareSrcWidget.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::WareSrcWidgetCollection() :
    mp_Manager(openfluid::waresdev::WareSrcManager::getInstance())
{

}


// =====================================================================
// =====================================================================


WareSrcWidgetCollection::~WareSrcWidgetCollection()
{

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openPath(const QString& Path, bool IsStandalone,
                                       QTabWidget* TabWidget)
{
  openfluid::waresdev::WareSrcManager::PathInfo Info = mp_Manager->getPathInfo(
      Path);

  // TODO manage other workspaces later
  if (!Info.m_IsInCurrentWorkspace)
    return;

  if (Info.m_isAWare || Info.m_isAWareFile)
  {
    openfluid::ui::waresdev::WareSrcWidget* Widget =
        m_WareSrcWidgetByPath.value(Info.m_AbsolutePathOfWare, 0);

    if (!Widget)
    {
      Widget = new WareSrcWidget(Info, IsStandalone, TabWidget);

      TabWidget->addTab(Widget, Info.m_WareName);

      m_WareSrcWidgetByPath[Info.m_AbsolutePathOfWare] = Widget;

      if (Info.m_isAWare)
        Widget->openDefaultFiles();
    }

    if (Info.m_isAWareFile)
      Widget->openFile(Info);

    TabWidget->setCurrentWidget(Widget);
  }

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::setCurrent(const QString& Path,
                                         QTabWidget* TabWidget)
{
  openfluid::waresdev::WareSrcManager::PathInfo Info = mp_Manager->getPathInfo(
      Path);

  // TODO manage other workspaces later
  if (!Info.m_IsInCurrentWorkspace)
    return;

  if (Info.m_isAWare || Info.m_isAWareFile)
  {
    openfluid::ui::waresdev::WareSrcWidget* Widget =
        m_WareSrcWidgetByPath.value(Info.m_AbsolutePathOfWare, 0);

    if (Widget)
    {
      if (Info.m_isAWare || Widget->setCurrent(Info))
        TabWidget->setCurrentWidget(Widget);
    }

  }

}


// =====================================================================
// =====================================================================


void WareSrcWidgetCollection::openExplorer(QTabWidget* TabWidget)
{
  if (WareSrcWidget* Widget = qobject_cast<WareSrcWidget*>(
      TabWidget->currentWidget()))
    Widget->openExplorer();
  else
    QDesktopServices::openUrl(
        QUrl::fromLocalFile(mp_Manager->getWaresdevPath()));
}


// =====================================================================
// =====================================================================


} } } //namespaces