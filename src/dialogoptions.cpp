/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2016
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
#include "inisettings.h"

#define INIFILENAME ".converseen.conf"

DialogOptions::DialogOptions(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(pushOk, SIGNAL(clicked()), this, SLOT(saveOptions()));
    connect(pushCancel, SIGNAL(clicked()), this, SLOT(reject()));

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
        IniSettings::setOverwriteMode(true);
	else
        IniSettings::setOverwriteMode(false);
}

void DialogOptions::setAutoUpdates()
{
    bool enabled;

    if (checkCheckUpdates->isChecked())
        enabled = true;
    else
        enabled = false;

    IniSettings::setAutoChechUpdates(enabled);
}

void DialogOptions::saveOptions()
{
    setLanguage();
    setOverwriteMode();
    setAutoUpdates();

    accept();
}

void DialogOptions::loadSettings()
{
    bool overwriteMode = IniSettings::isOverwriteMode();
	radioAskFirst->setChecked(overwriteMode);

    bool autoUpdates = IniSettings::isAutoChechUpdates();
    if (autoUpdates)
        checkCheckUpdates->setCheckState(Qt::Checked);
    else
        checkCheckUpdates->setCheckState(Qt::Unchecked);

    Translator t;
    
    int idx = comboLangs->findText(t.currentLanguage(), Qt::MatchExactly);
    comboLangs->setCurrentIndex(idx);
}
