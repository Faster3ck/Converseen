/*
* This file is part of Converseen, an open-source batch image converter
* and resizer.
*
* (C) Francesco Mondello 2009 - 2023
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

#include <QApplication>
#include <QLibraryInfo>
#include <QTextCodec>
#include <QDir>
#include <Magick++.h>
#include "mainwindowimpl.h"
#include "translator.h"
#include "globals.h"

int main(int argc, char ** argv)
{
	InitializeMagick(*argv);

    QCoreApplication::setApplicationVersion(globals::VERSION);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app( argc, argv );

#if defined(Q_OS_OSX)
    QString appDirPath = QApplication::applicationDirPath();

    QDir gs_dir(appDirPath + "/../Resources/ghostscript/10.01.2");
    QString gs_path = gs_dir.absolutePath();

    QString magick_configure_path = appDirPath + "/../Resources/ImageMagick-7";
    QString gs_lib = gs_path + "/Resource/Init/";
    QString gs_fontpath = gs_path + "/Resource/Font/";
    QString gs_options = QString("-sGenericResourceDir=%1/Resource/ -sICCProfilesDir=%1/iccprofiles/").arg(gs_path);

    qputenv("MAGICK_CONFIGURE_PATH", magick_configure_path.toLocal8Bit());
    qputenv("GS_LIB", gs_lib.toLocal8Bit());
    qputenv("GS_FONTPATH", gs_fontpath.toLocal8Bit());
    qputenv("GS_OPTIONS", gs_options.toLocal8Bit());
#endif

    // Default traslations for Qt apps
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
        QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    // Converseen translations
    Translator T;

    QTranslator *t = T.translation();

    if (t != NULL)
        app.installTranslator(t);

    MainWindowImpl win;

    if (argc > 1) {
        if (QString::fromStdString(argv[1]) == "--list") {
            win.importListFromArgv(QString::fromStdString(argv[2]));
        }

        // Prints the readable/writable supported formats by IM
        if (QString::fromStdString(argv[1]) == "--supported-formats") {
            win.printSupportedFormats();
        }
    }

    win.show();
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    return app.exec();
}
