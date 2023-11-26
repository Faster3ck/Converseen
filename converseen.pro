TEMPLATE = app
QT = gui core network widgets
CONFIG += qt \
 release \
 warn_on

win32:CONFIG += entrypoint

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.13
    ICON = res/converseen.icns
}

 DESTDIR =  bin
 OBJECTS_DIR =  build
 MOC_DIR =  build
 UI_DIR =  build
 FORMS =  ui/mainwindow.ui \
  ui/dialogconversionstatus.ui \
  ui/dialogquality.ui \
  ui/dialogoptions.ui \
  ui/dialoginfo.ui \
    ui/dialogmultipageeditor.ui \
    ui/dialogshowupdatemsg.ui
 HEADERS =  src/mainwindowimpl.h \
  src/converter.h \
  src/dialogconversionstatus.h \
  src/dialogquality.h \
    src/magickdefine.h \
  src/translator.h \
  src/dialogoptions.h \
  src/dialoginfo.h \
  src/pixtreewidget.h \
    src/formats.h \
    src/inisettings.h \
    src/sizeutil.h \
    src/mylabelpreviewer.h \
    src/thumbnailgeneratorthread.h \
    src/pushcolorchooser.h \
    src/cachingsystem.h \
    src/combofilters.h \
    src/updatechecker.h \
    src/globals.h \
    src/Modules/multipageconverter.h \
    src/dialogmultipageeditor.h \
    src/dialogshowupdatemsg.h
 SOURCES =  src/mainwindowimpl.cpp \
    src/magickdefine.cpp \
  src/main.cpp \
  src/converter.cpp \
  src/dialogconversionstatus.cpp \
  src/dialogquality.cpp \
  src/translator.cpp \
  src/dialogoptions.cpp \
  src/dialoginfo.cpp \
  src/pixtreewidget.cpp \
    src/formats.cpp \
    src/inisettings.cpp \
    src/sizeutil.cpp \
    src/mylabelpreviewer.cpp \
    src/thumbnailgeneratorthread.cpp \
    src/pushcolorchooser.cpp \
    src/cachingsystem.cpp \
    src/combofilters.cpp \
    src/updatechecker.cpp \
    src/globals.cpp \
    src/Modules/multipageconverter.cpp \
    src/dialogmultipageeditor.cpp \
    src/dialogshowupdatemsg.cpp

 !win32 {
     QMAKE_CXXFLAGS += -O2 \
      -Wall \
      -W \
      -pthread \
      -DMAGICKCORE_HDRI_ENABLE=1 \
      -DMAGICKCORE_QUANTUM_DEPTH=16
 }

 win32 {
    INCLUDEPATH += "$$IM_INSTALL_PREFIX\include"
 } else {
    
    isEmpty(IM_INSTALL_PREFIX) {
        INCLUDEPATH +=  /usr/include/ImageMagick-7
    }
    else {
	INCLUDEPATH +=  $$IM_INSTALL_PREFIX/include/ImageMagick-7
    }
    
    message(ImageMagick Include path: $$INCLUDEPATH)
 }

 win32 {
    LIBS += -L"$$IM_INSTALL_PREFIX\lib" -lCORE_RL_Magick++_ -lCORE_RL_MagickCore_ -lCORE_RL_MagickWand_
 } else {
    isEmpty(IM_INSTALL_PREFIX) {
        LIB_PREFIX +=  /usr/lib
    }
    else {
        LIB_PREFIX +=  $$IM_INSTALL_PREFIX/lib
    }
    
    message(ImageMagick Library path: $$LIB_PREFIX)

    LIBS += -L$$LIB_PREFIX  -lMagick++-7.Q16HDRI  -lMagickWand-7.Q16HDRI  -lMagickCore-7.Q16HDRI
 }

 TRANSLATIONS +=  loc/converseen_it_IT.ts \
 loc/converseen_hu_HU.ts \
 loc/converseen_cs_CZ.ts \
 loc/converseen_fr_FR.ts \
 loc/converseen_pt_BR.ts \
 loc/converseen_de_DE.ts \
 loc/converseen_es_CL.ts \
 loc/converseen_tr_TR.ts \
 loc/converseen_ru_RU.ts \
 loc/converseen_pl_PL.ts \
 loc/converseen_ja_JP.ts \
 loc/converseen_da_DK.ts \
 loc/converseen_uk_UA.ts \
 loc/converseen_sv_SE.ts \
 loc/converseen_zh_CN.ts
 
 RESOURCES +=  resources.qrc
 INSTALL_PREFIX =  /usr
 target.path =  $${INSTALL_PREFIX}/bin
 SHARE_DIR =  $${INSTALL_PREFIX}/share
 translations.path = $${SHARE_DIR}/converseen/loc
 translations.files = loc/converseen_it_IT.qm \
 loc/converseen_hu_HU.qm \
 loc/converseen_cs_CZ.qm \
 loc/converseen_fr_FR.qm \
 loc/converseen_pt_BR.qm \
 loc/converseen_de_DE.qm \
 loc/converseen_es_CL.qm \
 loc/converseen_tr_TR.qm \
 loc/converseen_ru_RU.qm \
 loc/converseen_pl_PL.qm \
 loc/converseen_ja_JP.qm \
 loc/converseen_da_DK.qm \
 loc/converseen_uk_UA.qm \
 loc/converseen_sv_SE.qm \
 loc/converseen_zh_CN.qm
 icon16.path = $${SHARE_DIR}/icons/hicolor/16x16/apps
 icon16.files += res/icons/16x16/converseen.png
 icon32.path = $${SHARE_DIR}/icons/hicolor/32x32/apps
 icon32.files += res/icons/32x32/converseen.png
 icon48.path = $${SHARE_DIR}/icons/hicolor/48x48/apps
 icon48.files += res/icons/48x48/converseen.png
 icon64.path = $${SHARE_DIR}/icons/hicolor/64x64/apps
 icon64.files += res/icons/64x64/converseen.png
 icon128.path = $${SHARE_DIR}/icons/hicolor/128x128/apps
 icon128.files += res/icons/128x128/converseen.png
 icon256.path = $${SHARE_DIR}/icons/hicolor/256x256/apps
 icon256.files += res/icons/256x256/converseen.png
 icon512.path = $${SHARE_DIR}/icons/hicolor/512x512/apps
 icon512.files += res/icons/512x512/converseen.png

 desktopFile.path = $${SHARE_DIR}/applications
 desktopFile.files = res/converseen.desktop
 kdeservicemenu.path = $${SHARE_DIR}/kservices5/ServiceMenus
 kdeservicemenu.files = res/converseen_import.desktop
 appdata.path = /usr/share/appdata/
 appdata.files = converseen.appdata.xml

 isEmpty(QMAKE_LRELEASE) {
  QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
 }

 updateqm.input = TRANSLATIONS
 updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
 updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
 updateqm.CONFIG += no_link

 QMAKE_EXTRA_COMPILERS += updateqm
 PRE_TARGETDEPS += compiler_updateqm_make_all

 INSTALLS += target \
 translations \
 icon16 \
 icon32 \
 icon48 \
 icon64 \
 icon128 \
 icon256 \
 icon512 \
 desktopFile \
 kdeservicemenu \
 appdata
