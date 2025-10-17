/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2025
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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

#define VERSION_URL "https://fasterland.net/software-updates/converseen-version.txt"
#define DESTINATION_URL "https://converseen.fasterland.net/update/"

class UpdateChecker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateChecker(QObject *parent = 0);
    void checkForUpdates();
    bool isUpdateAvailable();

private:
    void checkIfIsNewVersion(int version);

    QNetworkAccessManager* mNetworkManager;
    bool m_update_available;

private slots:
    void onNetworkReply(QNetworkReply* reply);

signals:
    void updateAvailable(const bool &isAvailable);
};

#endif // UPDATECHECKER_H
