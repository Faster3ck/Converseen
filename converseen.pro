TEMPLATE = app
QT = gui core network widgets
CONFIG += qt \
 release \
 warn_on \
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
  src/translator.h \
  src/dialogoptions.h \
  src/whereiam.h \
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
  src/main.cpp \
  src/converter.cpp \
  src/dialogconversionstatus.cpp \
  src/dialogquality.cpp \
  src/translator.cpp \
  src/dialogoptions.cpp \
  src/whereiam.cpp \
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

 QMAKE_CXXFLAGS += -O2 \
  -Wall \
  -W \
  -pthread \
  -DMAGICKCORE_HDRI_ENABLE=0 \
  -DMAGICKCORE_QUANTUM_DEPTH=16
  
 INCLUDEPATH +=  /usr/include/ImageMagick-7

 LIBS +=  -L/usr/lib  -lMagick++-7.Q16HDRI  -lMagickWand-7.Q16HDRI  -lMagickCore-7.Q16HDRI

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
 loc/converseen_uk_UA.ts
 
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
loc/converseen_uk_UA.qm
  
 icon.path = $${SHARE_DIR}/pixmaps
 icon.files = res/converseen.png
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
	     icon \
	     desktopFile \
	     kdeservicemenu \
	     appdata
