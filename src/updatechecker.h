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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = 0);
    void checkForUpdates();

private:
    void checkIfIsNewVersion(int version);

    QNetworkAccessManager* mNetworkManager;

private slots:
    void onNetworkReply(QNetworkReply* reply);
};

#endif // UPDATECHECKER_H
