cmake_minimum_required(VERSION 3.16.0)

project(converseen)

option(USE_QT6 "Enable this if you want to use Qt6, instead of Qt5" NO)

set(GUI "converseen")
set(QT_USE_QTNETWORK TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

if (USE_QT6)
    find_package(Qt6 COMPONENTS Core Gui Network Widgets LinguistTools REQUIRED)
else ()
    find_package(Qt5 COMPONENTS Core Gui Network Widgets LinguistTools REQUIRED)
endif()

find_package(ImageMagick REQUIRED COMPONENTS Magick++ MagickWand MagickCore)

set(converseen_SOURCES
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
    src/magickdefine.cpp
    src/Modules/multipageconverter.cpp
)

set(converseen_HEADERS
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
    src/magickdefine.cpp
    src/Modules/multipageconverter.h
)

set(converseen_UIS
    ui/dialogconversionstatus.ui
    ui/dialoginfo.ui
    ui/dialogmultipageeditor.ui
    ui/dialogshowupdatemsg.ui
    ui/dialogoptions.ui
    ui/dialogquality.ui
    ui/mainwindow.ui
)

set(converseen_RESOURCES
    resources.qrc
)

set(converseen_LANG
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
    loc/converseen_da_DK.ts
    loc/converseen_uk_UA.ts
    loc/converseen_sv_SE.ts
    loc/converseen_zh_CN.ts
)

if (USE_QT6)
    qt6_wrap_ui(converseen_UIS_H ${converseen_UIS})
    qt6_wrap_cpp(converseen_MOC_SRCS ${converseen_HEADERS})
    qt6_add_resources(converseen_RESOURCES ${converseen_RESOURCES})
    qt6_add_translation(converseen_RESOURCES ${converseen_LANG})
else ()
    qt5_wrap_ui(converseen_UIS_H ${converseen_UIS})
    qt5_wrap_cpp(converseen_MOC_SRCS ${converseen_HEADERS})
    qt5_add_resources(converseen_RESOURCES ${converseen_RESOURCES})
    qt5_add_translation(converseen_RESOURCES ${converseen_LANG})
endif()

add_definitions(
    -Wall
    -std=c++17
    -fPIC
    -DMAGICKCORE_HDRI_ENABLE=0
    -DMAGICKCORE_QUANTUM_DEPTH=16
    ${QT_DEFINITIONS}
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${ImageMagick_INCLUDE_DIRS}
    ${ImageMagick_Magick++_INCLUDE_DIRS}
    ${ImageMagick_MagickCore_INCLUDE_DIRS}
    .
)

add_executable(${GUI}
    ${converseen_SOURCES}
    ${converseen_UIS_H}
    ${converseen_MOC_SRCS}
    ${converseen_RESOURCES}
)

if (USE_QT6)
    target_link_libraries(${GUI}
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        Qt6::Widgets
        ${ImageMagick_LIBRARIES}
    )
else ()
    target_link_libraries(${GUI}
        Qt5::Core
        Qt5::Gui
        Qt5::Network
        Qt5::Widgets
        ${ImageMagick_LIBRARIES}
    )
endif()

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${GUI} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES res/net.fasterland.converseen.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
install(FILES res/converseen_import.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kservices5/ServiceMenus)
install(FILES converseen.appdata.xml DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/metainfo/)
install(FILES res/icons/16x16/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/16x16/apps)
install(FILES res/icons/32x32/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/32x32/apps)
install(FILES res/icons/48x48/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/48x48/apps)
install(FILES res/icons/64x64/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/64x64/apps)
install(FILES res/icons/128x128/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/128x128/apps)
install(FILES res/icons/256x256/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps)
install(FILES res/icons/512x512/converseen.png DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/512x512/apps)

foreach(LANG ${converseen_LANG})
    string(REGEX REPLACE ".*/" "" TS_FILE ${LANG})
    string(REGEX REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE} DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/converseen/loc)
endforeach()
