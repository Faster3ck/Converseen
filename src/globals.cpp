/*
* This file is part of Kawaii Emoji Messenger, a multiplatform Facebook client
* based on the idea of bringing emoji on desktop.
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

#include <QScreen>
#include <QGuiApplication>
#include "globals.h"

QString globals::Globals::m_sig;

void globals::Globals::setSignature(const QString &signature)
{
    m_sig = signature;
}

QString globals::Globals::signature()
{
    using namespace globals;
    return m_sig;
}

QString globals::Globals::magickVersion()
{
    QString imVersion;
    const char *version = MagickCore::GetMagickVersion(nullptr);

    imVersion = QString::fromStdString(version);

    return imVersion;
}

qreal globals::Globals::scaleFactor()
{
    qreal scale = 1.0;

    if (QGuiApplication::screens().count() > 0) {
        QScreen *screen = QGuiApplication::screens().at(0);
        scale = screen->devicePixelRatio();
    }

    return scale;
}
