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

#include <QUrl>
#include <QDesktopServices>
#include "dialoginfo.h"
#include "globals.h"

DialogInfo::DialogInfo(QWidget *parent) :
    QDialog(parent){
    setupUi(this);

    connect(pushClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(pushFacebook, SIGNAL(clicked()), this, SLOT(openFacebookPage()));
    connect(pushGPlus, SIGNAL(clicked()), this, SLOT(openGooglePlusPage()));

    QString appVersion;

#ifdef Q_OS_WIN32
    appVersion = QString("%1%2")
            .arg(QCoreApplication::applicationVersion())
            .arg(globals::Globals::signature());
#else
    appVersion = QCoreApplication::applicationVersion();
#endif

    QString title = QString("<p><span style=\" font-size:12pt; font-weight:600;\">Converseen %1</span><br />"
                            "<span style=\" font-size:10pt;\">%2</span></p>")
            .arg(appVersion)
            .arg(tr("The batch image converter and resizer."));

    labelTitle->setText(title);
}

void DialogInfo::openFacebookPage()
{
    QDesktopServices::openUrl(QUrl("https://www.facebook.com/converseen", QUrl::TolerantMode));
}

void DialogInfo::openGooglePlusPage()
{
    QDesktopServices::openUrl(QUrl("https://plus.google.com/u/0/b/112720480720840769800/", QUrl::TolerantMode));
}


void DialogInfo::on_pushDonatePayPal_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=HQA6TBT5354FC", QUrl::TolerantMode));
}

void DialogInfo::on_pushDonateFlattr_clicked()
{
    QDesktopServices::openUrl(QUrl("http://flattr.com/thing/3332139/Converseen", QUrl::TolerantMode));
}
