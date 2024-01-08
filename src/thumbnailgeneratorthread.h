/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2024
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

#ifndef THUMBNAILGENERATORTHREAD_H
#define THUMBNAILGENERATORTHREAD_H

#include <string>
#include <iostream>
#include <Magick++.h>

#include <QThread>
#include <QObject>
#include <QPixmap>
#include <QPixmapCache>
#include "cachingsystem.h"

using namespace Magick;
using namespace std;

class ThumbnailGeneratorThread : public QThread
{
    Q_OBJECT
public:
    ThumbnailGeneratorThread(QObject *parent);
    void run();

    void setFileName(QString fileName);
    void setThumbnailGeneration(bool generate);

private:
    void createThumbnail();

    QImage* toQImage(const Image &image);
    QString m_fileName;
    bool m_generateThumbnail;

signals:
    void pixmapGenerated(QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);

public slots:

};

#endif // THUMBNAILGENERATORTHREAD_H
