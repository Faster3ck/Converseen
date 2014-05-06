/*
* This file is part of Kawaii Emoji Messenger, a multiplatform Facebook client
* based on the idea of bringing emoji on desktop.
*
* (C) Francesco Mondello 2014
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

#include "updatechecker.h"
#include <QMessageBox>
#include <QDesktopServices>
#include "globals.h"

#define VERSION_URL "http://fasterland.net/software-updates/converseen-version.txt"
#define DESTINATION_URL "http://converseen.sf.net/"

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
        int ret = QMessageBox::warning(0, tr("New version is available!"),
                                       tr("A new version of Converseen is available!\nWould you download it?"),
                                       QMessageBox::Ok | QMessageBox::No);

        if (ret == QMessageBox::Ok)
            QDesktopServices::openUrl(QUrl(DESTINATION_URL, QUrl::TolerantMode));
    }
}
