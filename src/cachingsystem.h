/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2018
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

#ifndef CACHINGSYSTEM_H
#define CACHINGSYSTEM_H

#include <QCache>
#include <QImage>

class CachedImageObject
{
public:
    QImage thumbnail;
    QString id;
    int orig_w;
    int orig_h;
    double orig_dens_x;
    double orig_dens_y;
};

class CachingSystem
{
public:
    static void init();
    static void insert(QString id, QImage thumbnail, int orig_w, int orig_h, double orig_dens_x, double orig_dens_y);
    static bool find(QString key);
    static void clear();
    static void remove(QString key);

    static QImage thumbnail();
    static int originalWidth();
    static int originalHeight();
    static double originalDensityX();
    static double originalDensityY();

    static QCache<QString, CachedImageObject> *imageCache;
private:
    static CachedImageObject *currObj;
    static int m_orig_w;
};

#endif // CACHINGSYSTEM_H
