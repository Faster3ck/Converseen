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

#include <QApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QCommandLineParser>
#include <Magick++.h>

#include "mainwindowimpl.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#endif

#include "translator.h"
#include "globals.h"

int main(int argc, char ** argv)
{
	InitializeMagick(*argv);

    QCoreApplication::setApplicationName("Converseen");
    QCoreApplication::setApplicationVersion(globals::VERSION);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    QApplication app( argc, argv );

    QCommandLineOption winMagickPathOption({{"m", "debugMagickWindowsPath"}, "Set the default ImageMagick path on Windows (for debug purpose only!).", "C:\\MagickInstallPath"});
    QCommandLineOption importTxtListOption({{"l", "list"}, "Reads a txt file with a list of files to be imported.", "list.txt"});
    QCommandLineOption printSupportedFormats({{"p", "supported-formats"}, "Prints a list of readable/writable supported formats."});

    QCommandLineParser parser;
    parser.setApplicationDescription("Converseen - The Batch Converter and Resizer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(winMagickPathOption);
    parser.addOption(importTxtListOption);
    parser.addOption(printSupportedFormats);

    parser.process(app);

#if defined(Q_OS_OSX)
    QString appDirPath = QApplication::applicationDirPath();

    QDir gs_dir(appDirPath + "/../Resources/ghostscript/gs");
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

#if defined(Q_OS_WIN)
    // example: --debugMagickWindowsPath "C:\Program Files\ImageMagick-7.1.1-Q16-HDRI"

    QString resdir = QApplication::applicationDirPath();

    if (parser.isSet(winMagickPathOption))
        resdir = parser.value(winMagickPathOption);

    qputenv("LD_LIBRARY_PATH", resdir.toLocal8Bit());
    qputenv("MAGICK_HOME", resdir.toLocal8Bit());
    qputenv("MAGICK_CONFIGURE_PATH", resdir.toLocal8Bit());
    qputenv("MAGICK_CODER_MODULE_PATH", resdir.toLocal8Bit() + "\\modules\\coders");
    qputenv("MAGICK_CODER_FILTER_PATH", resdir.toLocal8Bit() + "\\modules\\filters");
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

    // Used in kde context menu
    if (parser.isSet(importTxtListOption)) {
        win.importListFromArgv(parser.value(importTxtListOption));
    }

    // Prints the readable/writable supported formats by IM
    if (parser.isSet(printSupportedFormats)) {
        win.printSupportedFormats();
    }

    win.show();
    app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    return app.exec();
}
