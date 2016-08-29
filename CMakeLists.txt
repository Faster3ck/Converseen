PROJECT(converseen)

cmake_minimum_required(VERSION 2.6)
  
SET(GUI "converseen")  
SET(QT_USE_QTNETWORK TRUE)

ADD_DEFINITIONS( ${OPTIMIZATION_FLAGS} )
  
SET(converseen_UIS  
    ui/dialogconversionstatus.ui
    ui/dialoginfo.ui
    ui/dialogmultipageeditor.ui
    ui/dialogshowupdatemsg.ui
    ui/dialogoptions.ui
    ui/dialogquality.ui
    ui/mainwindow.ui
)  
  
SET(converseen_SRCS  
    src/main.cpp
    src/cachingsystem.cpp
    src/combofilters.cpp
    src/converter.cpp
    src/dialogconversionstatus.cpp
    src/dialoginfo.cpp
    src/dialogmultipageeditor.cpp
    src/dialogoptions.cpp
    src/dialogquality.cpp
    src/dialogshowupdatemsg.cpp
    src/formats.cpp
    src/globals.cpp
    src/inisettings.cpp
    src/mainwindowimpl.cpp
    src/mylabelpreviewer.cpp
    src/pixtreewidget.cpp
    src/pushcolorchooser.cpp
    src/sizeutil.cpp
    src/thumbnailgeneratorthread.cpp
    src/translator.cpp
    src/updatechecker.cpp
    src/whereiam.cpp
    src/Modules/multipageconverter.cpp
)  
  
SET(converseen_MOC_HDRS  
    src/cachingsystem.h
    src/combofilters.h
    src/converter.h
    src/dialogconversionstatus.h
    src/dialoginfo.h
    src/dialogmultipageeditor.h
    src/dialogoptions.h
    src/dialogquality.h
    src/dialogshowupdatemsg.h
    src/formats.h
    src/globals.h
    src/inisettings.h
    src/mainwindowimpl.h
    src/mylabelpreviewer.h
    src/pixtreewidget.h
    src/pushcolorchooser.h
    src/sizeutil.h
    src/thumbnailgeneratorthread.h
    src/translator.h
    src/updatechecker.h
    src/whereiam.h
    src/Modules/multipageconverter.h
)
  
SET(converseen_RESOURCES  
    resources.qrc
)

SET(converseen_LANG
    loc/converseen_tr_TR.ts
    loc/converseen_fr_FR.ts
    loc/converseen_es_CL.ts
    loc/converseen_pt_BR.ts
    loc/converseen_cs_CZ.ts
    loc/converseen_hu_HU.ts
    loc/converseen_de_DE.ts
    loc/converseen_it_IT.ts
    loc/converseen_ru_RU.ts
    loc/converseen_pl_PL.ts
    loc/converseen_ja_JP.ts
)

FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5LinguistTools REQUIRED)
FIND_PACKAGE(ImageMagick REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++)
FIND_PACKAGE(ImageMagick COMPONENTS MagickWand)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore)

INCLUDE_DIRECTORIES(${Qt5Core_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Gui_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Network_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Qt5Widgets_INCLUDE_DIRS})
QT5_WRAP_UI(converseen_UIS_H ${converseen_UIS})
QT5_WRAP_CPP(converseen_MOC_SRCS ${converseen_MOC_HDRS})
QT5_ADD_RESOURCES(converseen_RESOURCES ${converseen_RESOURCES})
QT5_ADD_TRANSLATION(converseen_RESOURCES ${converseen_LANG})

ADD_DEFINITIONS(
	-Wall
	-W
	-std=c++11
	-fPIC
	-DMAGICKCORE_HDRI_ENABLE=0
	-DMAGICKCORE_QUANTUM_DEPTH=16
	${QT_DEFINITIONS}
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${ImageMagick_INCLUDE_DIRS} ${ImageMagick_Magick++_INCLUDE_DIRS} . )
  
ADD_EXECUTABLE(${GUI}  
    ${converseen_SRCS}  
    ${converseen_UIS_H}  
    ${converseen_MOC_SRCS}  
    ${converseen_RESOURCES}  
    ${converseen_RCS} 
)  
  
TARGET_LINK_LIBRARIES(${GUI} ${Qt5Network_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${ImageMagick_LIBRARIES} ${ImageMagick_Magick++_LIBRARIES})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/converseen DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES res/converseen.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES res/converseen.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
install(FILES res/converseen_import.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kde4/services/ServiceMenus)
install(FILES converseen.appdata.xml DESTINATION /usr/share/appdata/)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_cs_CZ.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_de_DE.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_es_CL.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_fr_FR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_hu_HU.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_it_IT.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_pt_BR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_tr_TR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_ru_RU.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_pl_PL.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_ja_JP.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen/loc)
