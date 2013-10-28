/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2013
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: Faster <faster3ck@gmail.com>
*
*/

#include <QMessageBox>
#include "dialogoptions.h"
#include "translator.h"
#include <QDebug>

#define INIFILENAME ".converseen.conf"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    QString iniPath = QDir::homePath() + "/" + INIFILENAME;

#ifdef Q_WS_WIN
    if ((QSysInfo::windowsVersion() == QSysInfo::WV_VISTA) || (QSysInfo::windowsVersion() == QSysInfo::WV_WINDOWS7)) {
        iniPath = QDir::homePath() + "/AppData/Roaming/Converseen" + INIFILENAME;
    }
#endif

    settings = new QSettings(iniPath, QSettings::IniFormat);

    connect(pushOk, SIGNAL(clicked()), this, SLOT(saveOptions()));
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(close()));

    Translator t;
    for (int i = 0; i < t.loadTranslationFiles().count(); i++)
        comboLangs->addItem(t.loadTranslationFiles().at(i).second);

    comboLangs->addItem("English");
    loadSettings();
}

void DialogOptions::setLanguage()
{
    Translator t;
    QString lang;

    if (comboLangs->currentText() == "English")
        lang = "English";
    else
        lang = t.loadTranslationFiles()
            .at(comboLangs->currentIndex())
            .first;

    if (lang != t.loadCurrentTranslationName()) {
        t.saveSettings(lang);

        QMessageBox::warning(0, tr("Warning!"),
                                  tr("Please, restart Converseen to apply current language!"));
    }
}

void DialogOptions::setOverwriteMode()
{
    if (radioAskFirst->isChecked())
        settings->setValue("Options/Overwrite mode", false);

    if (radioOverwrite->isChecked())
        settings->setValue("Options/Overwrite mode", true);
}

void DialogOptions::saveOptions()
{
    setLanguage();
    setOverwriteMode();

    close();
}

void DialogOptions::loadSettings()
{
    if (!settings->contains("Options/Overwrite mode"))
        radioAskFirst->setChecked(true);
    else {
        if (settings->value("Options/Overwrite mode").toBool())
            radioOverwrite->setChecked(true);
        else
            radioAskFirst->setChecked(true);
    }

    Translator t;
    
    qDebug() << "Lingua corrente " << t.currentLanguage();
    
    int idx = comboLangs->findText(t.currentLanguage(), Qt::MatchExactly);
    comboLangs->setCurrentIndex(idx);
}
