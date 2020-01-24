/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2020
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
* Contact e-mail: Francesco Mondello <faster3ck@gmail.com>
*
*/

#include <QDesktopServices>

#include "updatechecker.h"
#include "dialogshowupdatemsg.h"
#include "globals.h"

#define VERSION_URL "http://fasterland.net/software-updates/converseen-version.txt"
#define DESTINATION_URL "http://converseen.fasterland.net/update/"

UpdateChecker::UpdateChecker(QObject *parent) :
    QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
    QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));
}

void UpdateChecker::checkForUpdates()
{
    QUrl url(VERSION_URL);
    mNetworkManager->get(QNetworkRequest(url));
}

void UpdateChecker::onNetworkReply(QNetworkReply* reply)
{
    QString replyString;
    if(reply->error() == QNetworkReply::NoError)
    {
        int httpstatuscode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();
        switch(httpstatuscode)
        {
        case 200:
        if (reply->isReadable())
        {
            //Assuming this is a human readable file replyString now contains the file
            replyString = QString::fromUtf8(reply->readAll().data());

            checkIfIsNewVersion(replyString.toInt());
        }
        break;
        }
    }

    reply->deleteLater();
}

void UpdateChecker::checkIfIsNewVersion(int version)
{
    if (version > globals::CURRENT_INTERNAL_VERSION) {
        QString caption = QString(tr("New version is available!"));
        QString message = QString(tr("A new version of %1 is available!\nWould you download it?"))
                .arg(globals::PROGRAM_NAME);

        DialogShowUpdateMsg dlg(0, caption, message);

        if (dlg.exec()) {
            QDesktopServices::openUrl(QUrl(DESTINATION_URL, QUrl::TolerantMode));
        }
    }
}
