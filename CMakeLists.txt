PROJECT(converseen)

cmake_minimum_required(VERSION 2.4)
  
SET(GUI "converseen")  

ADD_DEFINITIONS( ${OPTIMIZATION_FLAGS} )
  
SET(converseen_UIS  
    ui/dialogconversionstatus.ui
    ui/dialoginfo.ui
    ui/dialogoptions.ui
    ui/dialogquality.ui
    ui/mainwindow.ui
)  
  
SET(converseen_SRCS  
    src/pushcolorchooser.cpp
    src/main.cpp
    src/dialogoptions.cpp
    src/inisettings.cpp
    src/translator.cpp
    src/converter.cpp
    src/pixtreewidget.cpp
    src/imageinformations.cpp
    src/sizeutil.cpp
    src/thumbnailgeneratorthread.cpp
    src/dialogquality.cpp
    src/mylabelpreviewer.cpp
    src/mainwindowimpl.cpp
    src/dialoginfo.cpp
    src/whereiam.cpp
    src/formats.cpp
    src/dialogconversionstatus.cpp
)  
  
SET(converseen_MOC_HDRS  
    src/pushcolorchooser.h
    src/dialogoptions.h
    src/inisettings.h
    src/translator.h
    src/converter.h
    src/pixtreewidget.h
    src/imageinformations.h
    src/sizeutil.h
    src/thumbnailgeneratorthread.h
    src/dialogquality.h
    src/mylabelpreviewer.h
    src/mainwindowimpl.h
    src/dialoginfo.h
    src/whereiam.h
    src/formats.h
    src/dialogconversionstatus.h
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
    loc/converseen_pl_PL.ts
    loc/converseen_hu_HU.ts
    loc/converseen_de_DE.ts
    loc/converseen_it_IT.ts
    loc/converseen_ru_RU.ts
)

FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(ImageMagick REQUIRED)
FIND_PACKAGE(ImageMagick COMPONENTS Magick++)
FIND_PACKAGE(ImageMagick COMPONENTS MagickWand)
FIND_PACKAGE(ImageMagick COMPONENTS MagickCore)

INCLUDE(${QT_USE_FILE})
QT4_WRAP_UI(converseen_UIS_H ${converseen_UIS})  
QT4_WRAP_CPP(converseen_MOC_SRCS ${converseen_MOC_HDRS})  
QT4_ADD_RESOURCES(converseen_RESOURCES ${converseen_RESOURCES})
QT4_ADD_TRANSLATION(converseen_RESOURCES ${converseen_LANG}) 

ADD_DEFINITIONS(
	-Wall
	-W
	-pthread
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
  
TARGET_LINK_LIBRARIES(${GUI} ${QT_LIBRARIES} ${ImageMagick_LIBRARIES} ${ImageMagick_Magick++_LIBRARIES})

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/converseen DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES res/converseen.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)
install(FILES res/converseen.png DESTINATION ${CMAKE_INSTALL_PREFIX}/share/pixmaps)
install(FILES res/converseen_import.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kde4/services/ServiceMenus)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_cs_CZ.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_de_DE.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_es_CL.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_fr_FR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_hu_HU.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_it_IT.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_pl_PL.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_pt_BR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_tr_TR.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/converseen_ru_RU.qm DESTINATION ${CMAKE_INSTALL_PREFIX}/share/converseen)
