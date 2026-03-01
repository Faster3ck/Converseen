cmake_minimum_required(VERSION 3.16.0)

if(WIN32)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CONVERSEEN_VERSION 0.15.2.0)

project(
    converseen
    VERSION ${CONVERSEEN_VERSION}
    LANGUAGES CXX)
set(PROJECT_NAME_CAPITALIZED "Converseen")

option(USE_QT6 "Enable this if you want to use Qt6, instead of Qt5" NO)
option(MACOS_DEPLOY "Enable this if you want to deploy the program on MacOS" NO)

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

if (APPLE AND MACOS_DEPLOY)
message(STATUS "--- MacOS Deploy Build ---")

find_library(GS_LIBRARY NAMES gs)
    if(GS_LIBRARY)
        get_filename_component(GS_LIB_DIR ${GS_LIBRARY} DIRECTORY)
        get_filename_component(GS_PREFIX ${GS_LIB_DIR} DIRECTORY)

        file(GLOB GS_VERSION_DIR
         "${GS_PREFIX}/share/ghostscript/[0-9]*")

        list(GET GS_VERSION_DIR 0 GS_RESOURCE_DIR)

        message(STATUS "Ghostscript resource dir: ${GS_RESOURCE_DIR}")

        install(DIRECTORY "${GS_RESOURCE_DIR}/"
                DESTINATION "${GUI}.app/Contents/Resources/ghostscript")

        # ImageMagick
        get_filename_component(IMAGEMAGICK_LIB_DIR
                       ${ImageMagick_MagickCore_LIBRARY}
                       DIRECTORY)

        get_filename_component(IMAGEMAGICK_PREFIX
                            ${IMAGEMAGICK_LIB_DIR}
                            DIRECTORY)

        message(STATUS "ImageMagick prefix: ${IMAGEMAGICK_PREFIX}")

        set(IMAGEMAGICK_CONFIG_DIR
            "${IMAGEMAGICK_PREFIX}/etc/ImageMagick-7")

        install(DIRECTORY ${IMAGEMAGICK_CONFIG_DIR}
                DESTINATION "${GUI}.app/Contents/Resources")
    endif()
endif()

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
    if (USE_QT6)
	find_program(windeployqt_exe windeployqt6 REQUIRED)
    else ()
	find_program(windeployqt_exe windeployqt REQUIRED)
    endif()
    configure_file(cmake/windeployqt.cmake.in ${CMAKE_BINARY_DIR}/windeployqt.cmake @ONLY)
    set(app_icon_resource_windows "converseen.rc")

    set(CMAKE_WIN32_EXECUTABLE TRUE)
    add_definitions(${QT_DEFINITIONS})
	
	set(CMAKE_INSTALL_BINDIR "${PROJECT_NAME_CAPITALIZED}")

	if (NOT USE_QT6)
		set(OpenSSL_Win32_Path ${CMAKE_BINARY_DIR}/openssl-1.1/x86/bin CACHE PATH "OpenSSL win32 bin path")
	#	# Download openssl if Qt5 is used!
	#	include(ExternalProject)
	#	ExternalProject_Add(
	#		OpenSSL_Win32
	#		PREFIX ${CMAKE_BINARY_DIR}/external 
	#		URL https://download.firedaemon.com/FireDaemon-OpenSSL/openssl-1.1.1w.zip 
	#		DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
	#		DOWNLOAD_NAME openssl-1.1.1w.zip
	#		CONFIGURE_COMMAND ""
	#		BUILD_COMMAND ""
	#		INSTALL_COMMAND ""
	#		# DOWNLOAD_EXTRACT_TIMESTAMP TRUE
	#	)
	endif()
else()
    add_definitions(
        -Wall
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

if(APPLE AND MACOS_DEPLOY)
    set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/res/converseen.icns)
    set_source_files_properties(${APP_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )
    
    set(CMAKE_MACOSX_RPATH ON)

    add_executable(${GUI} MACOSX_BUNDLE
        ${converseen_SOURCES}
        ${converseen_UIS_H}
        ${converseen_MOC_SRCS}
        ${converseen_RESOURCES}
        ${APP_ICON}
    )

    
else()
    add_executable(${GUI}
        ${converseen_SOURCES}
        ${converseen_UIS_H}
        ${converseen_MOC_SRCS}
        ${converseen_RESOURCES}
        $<$<BOOL:${WIN32}>:${app_icon_resource_windows}>
    )
endif()


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
    file(GLOB ImageMagick_DLLs ${ImageMagick_EXECUTABLE_DIR}/CORE_RL_*.dll)
    file(GLOB ImageMagick_Filters ${ImageMagick_EXECUTABLE_DIR}/modules/filters/*.dll)
    file(GLOB ImageMagick_Coders ${ImageMagick_EXECUTABLE_DIR}/modules/coders/*.dll)
    file(GLOB ImageMagick_XMLs ${ImageMagick_EXECUTABLE_DIR}/*.xml)
    file(GLOB Win32_DLLs 
        ${ImageMagick_EXECUTABLE_DIR}/mfc140*.dll
        ${ImageMagick_EXECUTABLE_DIR}/msvcp140*.dll
        ${ImageMagick_EXECUTABLE_DIR}/vc*140.dll
    )

    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/converseen.exe DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${ImageMagick_DLLs} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${ImageMagick_Filters} DESTINATION ${CMAKE_INSTALL_BINDIR}/modules/filters)
    install(FILES ${ImageMagick_Coders} DESTINATION ${CMAKE_INSTALL_BINDIR}/modules/coders)
    install(FILES ${ImageMagick_XMLs} DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${ImageMagick_EXECUTABLE_DIR}/sRGB.icc DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${ImageMagick_EXECUTABLE_DIR}/NOTICE.txt RENAME ImageMagick_License.txt DESTINATION ${CMAKE_INSTALL_BINDIR})
    install(FILES ${Win32_DLLs} DESTINATION ${CMAKE_INSTALL_BINDIR})
	
	if (NOT USE_QT6)
		install(FILES package/Windows/qt.conf DESTINATION ${CMAKE_INSTALL_BINDIR})
		install(FILES ${OpenSSL_Win32_Path}/libcrypto-1_1.dll DESTINATION ${CMAKE_INSTALL_BINDIR})
		install(FILES ${OpenSSL_Win32_Path}/libssl-1_1.dll DESTINATION ${CMAKE_INSTALL_BINDIR})
	endif()
	
	install(SCRIPT ${CMAKE_BINARY_DIR}/windeployqt.cmake)
	install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qtdeploy/ DESTINATION ${CMAKE_INSTALL_BINDIR} FILES_MATCHING PATTERN "*")
    
    foreach(LANG ${converseen_LANG})
        string(REGEX REPLACE ".*/" "" TS_FILE ${LANG})
        string(REGEX REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE} DESTINATION ${CMAKE_INSTALL_BINDIR}/loc)
    endforeach()


elseif(APPLE AND MACOS_DEPLOY)

    install(TARGETS ${GUI}
            BUNDLE DESTINATION .
            RUNTIME DESTINATION bin)

    foreach(LANG ${converseen_LANG})
        string(REGEX REPLACE ".*/" "" TS_FILE ${LANG})
        string(REGEX REPLACE ".ts" ".qm" QM_FILE ${TS_FILE})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QM_FILE} DESTINATION "${GUI}.app/Contents/Resources/loc")
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
	set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
	
    set(CPACK_PACKAGE_VENDOR "Francesco Mondello")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Converseen the Batch Image Converter and Resizer")
    set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_PACKAGE_HOMEPAGE_URL "https://converseen.fasterland.net")
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

    set(CPACK_WIX_PRODUCT_GUID "B35C58D3-FBD2-4A81-8371-588F51000033")
    set(CPACK_WIX_UPGRADE_GUID "B35C58D3-FBD2-4A81-8371-588F51EC180F")

    include(CPack)
endif()

# =========================
# macOS Bundle Fix
# =========================

if(APPLE AND MACOS_DEPLOY AND USE_QT6)
    set_target_properties(${GUI} PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    qt_generate_deploy_app_script(
        TARGET ${GUI}
        OUTPUT_SCRIPT qt_deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${qt_deploy_script})
endif()

# ======================================================================================= #
# This CMakeLists.txt file will be refactored and modernized as soon as possible! :D      #
# ======================================================================================= #
