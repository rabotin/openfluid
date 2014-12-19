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
 \file WareSrcSyntaxManager.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <QDir>
#include <QFile>
#include <QDomDocument>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>
#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcFiletypeManager* WareSrcFiletypeManager::mp_Instance = 0;

// =====================================================================
// =====================================================================


WareSrcFiletypeManager::WareSrcFiletypeManager()
{
  //TODO get formats from conf file

  m_Formats["keyword"].setForeground(QColor("#0000FF"));
  m_Formats["keyword"].setFontWeight(QFont::Bold);

  m_Formats["datatype"].setFontWeight(QFont::Bold);

  m_Formats["preprocessor"].setForeground(QColor("#FF00FF"));
  m_Formats["preprocessor"].setFontWeight(QFont::Bold);

  m_Formats["deprecated"].setForeground(QColor("#0000FF"));
  m_Formats["deprecated"].setFontWeight(QFont::Bold);
  m_Formats["deprecated"].setFontStrikeOut(true);

  m_Formats["quoted"].setForeground(QColor("#FF0000"));

  m_Formats["function"].setForeground(QColor("#0000FF"));

  m_Formats["comment"].setForeground(QColor("#00FF00"));
  m_Formats["comment"].setFontItalic(true);

  // TODO replace with CMake-defined path
  QDir WaresdevDir(
      QString("%1/share/openfluid/waresdev").arg(
          QString::fromStdString(
              openfluid::base::RuntimeEnvironment::getInstance()
                  ->getInstallPrefix())));

  parseFiletypeFile(WaresdevDir.filePath("filetypes.ofdev.xml"));

  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin();
      it != m_WareSrcFiletypes.end(); ++it)
  {
    if (!it.value().m_HlFilename.isEmpty())
      it.value().m_HlRules = parseSyntaxFile(
          WaresdevDir.filePath(it.value().m_HlFilename));
  }

}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::~WareSrcFiletypeManager()
{

}


// =====================================================================
// =====================================================================


QDomElement WareSrcFiletypeManager::openWaresdevFile(const QString& FilePath)
{
  QDomDocument Doc;
  QFile File(FilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::openWaresdevFile",
        QString("syntax file not found: %1").arg(FilePath).toStdString());

  QString Msg;
  int Line, Col;
  if (!Doc.setContent(&File, &Msg, &Line, &Col))
  {
    File.close();
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::openWaresdevFile",
        QString("error in syntax file: %1 (line %2, column %3").arg(Msg).arg(
            Line).arg(Col).toStdString());
  }
  File.close();

  QDomElement Elem = Doc.documentElement();

  if (Elem.tagName() != "openfluid")
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::openWaresdevFile",
        "syntax file not well formed (missing 'openfluid' tag)");

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "waresdev")
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::openWaresdevFile",
        "syntax file not well formed (missing 'waresdev' tag)");

  return Elem;
}


// =====================================================================
// =====================================================================


void WareSrcFiletypeManager::parseFiletypeFile(const QString& FilePath)
{
  QDomElement Elem = openWaresdevFile(FilePath);

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "filetypes")
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::parseFiletypeFile",
        "language file not well formed (missing 'filetypes' tag)");

  for (QDomElement TypeElem = Elem.firstChildElement();
      !TypeElem.isNull() && TypeElem.tagName() == "filetype"; TypeElem =
          TypeElem.nextSiblingElement())
  {
    QString TypeName = TypeElem.attribute("name");
    if (TypeName.isEmpty())
      throw openfluid::base::FrameworkException(
          "WareSrcFiletypeManager::parseFiletypeFile",
          "language file not well formed ('filetype' tag has no 'name' attribute)");

    QDomNode ExtNode = TypeElem.namedItem("extensions");
    if (ExtNode.isNull() || !ExtNode.isElement())
      throw openfluid::base::FrameworkException(
          "WareSrcFiletypeManager::parseFiletypeFile",
          "language file not well formed (missing 'extensions' tag)");

    WareSrcFiletype Type;

    Type.m_Extensions = ExtNode.toElement().text();

    for (QDomElement TypeInfoElem = TypeElem.firstChildElement();
        !TypeInfoElem.isNull(); TypeInfoElem =
            TypeInfoElem.nextSiblingElement())
    {
      if (TypeInfoElem.tagName() == "icon")
      {
        Type.m_IconPath = TypeInfoElem.attribute("qresname");
        m_IconsByFileExtensionList[Type.m_Extensions] = Type.m_IconPath;
      }
      else if (TypeInfoElem.tagName() == "edition"
          && TypeInfoElem.attribute("internal") == "yes")
      {
        for (QDomElement EditionElem = TypeInfoElem.firstChildElement();
            !EditionElem.isNull(); EditionElem =
                EditionElem.nextSiblingElement())
        {
          if (EditionElem.tagName() == "highlighting")
            Type.m_HlFilename = EditionElem.attribute("specsfile");
          else if (EditionElem.tagName() == "highlighting")
            Type.m_ComplFilename = EditionElem.attribute("specsfile");
        }

      }
    }

    m_WareSrcFiletypes[TypeName] = Type;
  }

}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::parseSyntaxFile(
    const QString& FilePath)
{
  QDomElement Elem = openWaresdevFile(FilePath);

  // TODO remove useless 'language' tag
  Elem = Elem.firstChildElement();
//  if (Elem.tagName() != "language" || Elem.attribute("name") != FileTypeName)
//    throw openfluid::base::FrameworkException(
//        "WareSrcFiletypeManager::parseSyntaxFile",
//        QString(
//            "syntax file not well formed (missing 'language' tag or language name is not '%1')")
//            .arg(FileTypeName).toStdString());

  Elem = Elem.firstChildElement();
  if (Elem.tagName() != "highlighting")
    throw openfluid::base::FrameworkException(
        "WareSrcFiletypeManager::parseSyntaxFile",
        "no 'highlighting' tag in syntax file");

  HighlightingRules_t Rules;

  for (QDomElement HlElem = Elem.firstChildElement(); !HlElem.isNull(); HlElem =
      HlElem.nextSiblingElement())
  {
    if (HlElem.tagName() == "list")
    {
      QString StyleName = HlElem.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = HlElem.elementsByTagName("item");
        for (int i = 0; i < Items.size(); i++)
        {
          Rules.append(
              HighlightingRule(
                  QRegExp(
                      QString("\\b%1\\b").arg(Items.at(i).toElement().text())),
                  Format));
        }
      }
    }
    else if (HlElem.tagName() == "rule")
    {
      QString StyleName = HlElem.attribute("style");
      if (m_Formats.contains(StyleName))
      {
        QTextCharFormat Format = m_Formats.value(StyleName);

        QDomNodeList Items = HlElem.elementsByTagName("pattern");
        for (int i = 0; i < Items.size(); i++)
        {
          QDomElement Pattern = Items.at(i).toElement();

          // 'value' must be set and different of ''
          QString SimplePatternValue = Pattern.attribute("value", "");
          QString BeginPatternValue = Pattern.attribute("start", "");
          QString EndPatternValue = Pattern.attribute("end", "");
          if (!SimplePatternValue.isEmpty())
          {
            Rules.append(HighlightingRule(QRegExp(SimplePatternValue), Format));
          }
          else if (!BeginPatternValue.isEmpty() && !EndPatternValue.isEmpty())
          {
            Rules.append(
                HighlightingRule(QRegExp(BeginPatternValue),
                                 QRegExp(EndPatternValue), Format));
          }
        }
      }
    }
  }

  return Rules;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager* WareSrcFiletypeManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new WareSrcFiletypeManager();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


QString WareSrcFiletypeManager::getFileType(const QString& FileName)
{
  for (QMap<QString, WareSrcFiletype>::iterator it = m_WareSrcFiletypes.begin();
      it != m_WareSrcFiletypes.end(); ++it)
  {
    if (QDir::match(it.value().m_Extensions, FileName))
      return it.key();
  }

  return "";
}


// =====================================================================
// =====================================================================


QMap<QString, QString> WareSrcFiletypeManager::getIconsByFileExtensionList()
{

  return m_IconsByFileExtensionList;
}


// =====================================================================
// =====================================================================


WareSrcFiletypeManager::HighlightingRules_t WareSrcFiletypeManager::getHighlightingRules(
    const QFile& File)
{
  QString FileType = getFileType(QFileInfo(File).fileName());

  return m_WareSrcFiletypes.value(FileType, WareSrcFiletype()).m_HlRules;
}


// =====================================================================
// =====================================================================


} } } // namespaces