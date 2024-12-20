cmake_minimum_required(VERSION 3.16.0)

if(WIN32)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CONVERSEEN_VERSION 0.12.2.4)

project(
    converseen
    VERSION ${CONVERSEEN_VERSION}
    LANGUAGES CXX)
set(PROJECT_NAME_CAPITALIZED "Converseen")

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
    src/magickdefine.h
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

if(WIN32)
    find_program(windeployqt_exe windeployqt REQUIRED)
    configure_file(cmake/windeployqt.cmake.in ${CMAKE_BINARY_DIR}/windeployqt.cmake @ONLY)
    set(app_icon_resource_windows "converseen.rc")

    set(CMAKE_WIN32_EXECUTABLE TRUE)
    add_definitions(${QT_DEFINITIONS})

    include(ExternalProject)
    ExternalProject_Add(
        OpenSSL_Win32
        PREFIX ${CMAKE_BINARY_DIR}/external 
        URL https://download.firedaemon.com/FireDaemon-OpenSSL/openssl-1.1.1w.zip 
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        DOWNLOAD_NAME openssl-1.1.1w.zip
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        # DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
else()
    add_definitions(
        -Wall
        -std=c++17
        -fPIC
        -DMAGICKCORE_HDRI_ENABLE=0
        -DMAGICKCORE_QUANTUM_DEPTH=16
        ${QT_DEFINITIONS}
    )
endif()

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
    $<$<BOOL:${WIN32}>:${app_icon_resource_windows}>
)

if (USE_QT6)
    target_link_libraries(${GUI}
        Qt6::Core
        Qt6::Gui
        Qt6::Network
        Qt6::Widgets
        ${ImageMagick_LIBRARIES}
    )
else()
    target_link_libraries(${GUI}
        Qt5::Core
        Qt5::Gui
        Qt5::Network
        Qt5::Widgets
        ${ImageMagick_LIBRARIES}
    )
endif()


if(WIN32)
    #set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)

    #include(InstallRequiredSystemLibraries)

    file(GLOB ImageMagick_DLLs ${ImageMagick_EXECUTABLE_DIR}/CORE_RL_*.dll)
    file(GLOB ImageMagick_Filters ${ImageMagick_EXECUTABLE_DIR}/modules/filters/*.dll)
    file(GLOB ImageMagick_Coders ${ImageMagick_EXECUTABLE_DIR}/modules/coders/*.dll)
    file(GLOB ImageMagick_XMLs ${ImageMagick_EXECUTABLE_DIR}/*.xml)
    file(GLOB OpenSSL_x86_DLLs ${CMAKE_BINARY_DIR}/external/src/OpenSSL_Win32/x86/bin/*.dll)
    #file(GLOB Win32_DLLs ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}/*.dll)
    file(GLOB Win32_DLLs 
        ${ImageMagick_EXECUTABLE_DIR}/mfc140*.dll
        ${ImageMagick_EXECUTABLE_DIR}/msvcp140*.dll
        ${ImageMagick_EXECUTABLE_DIR}/vc*140.dll
    )

    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/converseen.exe DESTINATION .)
    install(FILES package/Windows/qt.conf DESTINATION .)
    install(FILES ${ImageMagick_DLLs} DESTINATION .)
    install(FILES ${ImageMagick_Filters} DESTINATION ./modules/filters)
    install(FILES ${ImageMagick_Coders} DESTINATION ./modules/coders)
    install(FILES ${ImageMagick_XMLs} DESTINATION .)
    install(FILES ${ImageMagick_EXECUTABLE_DIR}/sRGB.icc DESTINATION .)
    install(FILES ${ImageMagick_EXECUTABLE_DIR}/NOTICE.txt RENAME ImageMagick_License.txt DESTINATION .)
    install(FILES ${OpenSSL_x86_DLLs} DESTINATION .)
    install(FILES ${Win32_DLLs} DESTINATION .)
    #install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION .)
    
    install(SCRIPT ${CMAKE_BINARY_DIR}/windeployqt.cmake)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qtdeploy/ DESTINATION . FILES_MATCHING PATTERN "*")

    foreach(LANG ${converseen_LANG})
        string(REGEX REPLACE ".*/" "" TS_FILE ${LANG})
        string(REGEX REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE} DESTINATION ./loc)
    endforeach()
else()
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${GUI} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES res/net.fasterland.converseen.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES res/converseen_import.desktop DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kio/servicemenus)
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
endif()

    # CPack
    set(CPACK_PACKAGE_VENDOR "Francesco Mondello")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Converseen the Batch Image Converter and Resizer")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_HOMEPAGE_URL "https:/converseen.fasterland.net")
    set(CPACK_PACKAGE_CONTACT "Contact Author <faster3ck@gmail.net>")
    set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/COPYING.txt")
    set(CPACK_GENERATOR WIX ZIP)
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME_CAPITALIZED}")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME_CAPITALIZED}")
    set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME} "${PROJECT_NAME_CAPITALIZED}")
    set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME} "${PROJECT_NAME_CAPITALIZED}")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME_CAPITALIZED}")

if(WIN32)
    # WIX (Windows .msi installer)
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/res/converseen.ico")
    set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/package/Windows/wix/CPACK_WIX_UI_BANNER.BMP")
    set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/package/Windows/wix/CPACK_WIX_UI_DIALOG.BMP")
    set(CPACK_WIX_PROPERTY_ARPHELPLINK "${CPACK_PACKAGE_HOMEPAGE_URL}")
    set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${CPACK_PACKAGE_HOMEPAGE_URL}")
    set(CPACK_WIX_ROOT_FEATURE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_WIX_LIGHT_EXTRA_FLAGS "-dcl:high")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/COPYING.txt")

    set(CPACK_WIX_PRODUCT_GUID "B35C58D3-FBD2-4A81-8371-588F51000009")
    set(CPACK_WIX_UPGRADE_GUID "B35C58D3-FBD2-4A81-8371-588F51EC180F")

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/wixCheckInnoVersion.xml.in" wixCheckInnoVersion.xml)
    set(CPACK_WIX_PATCH_FILE "wixCheckInnoVersion.xml")

    configure_file(cmake/RemoveInnoVersion.vbs.in ${CMAKE_BINARY_DIR}/RemoveInnoVersion.vbs)

    include(CPack)
endif()

