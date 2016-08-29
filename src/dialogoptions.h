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

#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QSettings>
#include <QDir>
#include "ui_dialogoptions.h"

class DialogOptions : public QDialog, private Ui::DialogOptions {
    Q_OBJECT
public:
    DialogOptions(QWidget *parent = 0);
private:
    void setLanguage();
    void setOverwriteMode();
    void setAutoUpdates();
    void loadSettings();

    QSettings *settings;
private slots:
    void saveOptions();
};

#endif // DIALOGOPTIONS_H
