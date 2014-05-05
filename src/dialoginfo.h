/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009-2014
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

#ifndef DIALOGINFO_H
#define DIALOGINFO_H

#include "ui_dialoginfo.h"

class DialogInfo : public QDialog, private Ui::DialogInfo {
    Q_OBJECT
public:
    DialogInfo(QWidget *parent = 0);

private slots:
    void openFacebookPage();
    void openGooglePlusPage();
    void on_pushDonatePayPal_clicked();
    void on_pushDonateFlattr_clicked();
};

#endif // DIALOGINFO_H
