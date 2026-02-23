cmake_minimum_required(VERSION 3.16)

project(converseen
    VERSION 0.15.1.3
    LANGUAGES CXX
)

set(PROJECT_NAME_CAPITALIZED "Converseen")
option(USE_QT6 "Build with Qt6 (default ON)" ON)

include(GNUInstallDirs)

# =========================
# Qt selection (Qt6-first)
# =========================
if(USE_QT6)
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Network Widgets LinguistTools)
    set(QT_LIBS Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets)
    set(QT_LUPDATE Qt6::lupdate)
else()
    find_package(Qt5 REQUIRED COMPONENTS Core Gui Network Widgets LinguistTools)
    set(QT_LIBS Qt5::Core Qt5::Gui Qt5::Network Qt5::Widgets)
endif()

# =========================
# ImageMagick
# =========================
find_package(ImageMagick REQUIRED COMPONENTS Magick++ MagickCore MagickWand)

# =========================
# Global CMake Modern Setup
# =========================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

# =========================
# Sources
# =========================
set(SRC
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

set(UI
    ui/dialogconversionstatus.ui
    ui/dialoginfo.ui
    ui/dialogmultipageeditor.ui
    ui/dialogshowupdatemsg.ui
    ui/dialogoptions.ui
    ui/dialogquality.ui
    ui/mainwindow.ui
)

set(RESOURCES resources.qrc)

set(LANG_TS
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

# =========================
# Executable
# =========================
if(APPLE)

    set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/res/converseen.icns")

    set(CMAKE_MACOSX_RPATH ON)

    add_executable(converseen MACOSX_BUNDLE
        ${SRC}
        ${UI}
        ${RESOURCES}
        ${APP_ICON}
    )

    set_source_files_properties(${APP_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources"
    )

    set_target_properties(converseen PROPERTIES
        MACOSX_BUNDLE_ICON_FILE "converseen.icns"
    )

else()
    add_executable(converseen
        ${SRC} ${UI} ${RESOURCES}
    )

endif()

# =========================
# Target Properties
# =========================
target_compile_features(converseen PRIVATE cxx_std_17)

target_compile_definitions(converseen PRIVATE
    MAGICKCORE_HDRI_ENABLE=0
    MAGICKCORE_QUANTUM_DEPTH=16
)

target_include_directories(converseen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${ImageMagick_INCLUDE_DIRS}
)

target_link_libraries(converseen PRIVATE
    ${QT_LIBS}
    ${ImageMagick_LIBRARIES}
)

# =========================
# Translations
# =========================

if(USE_QT6)

    qt_add_translations(converseen
        TS_FILES ${LANG_TS}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )

else()

    qt5_add_translation(QM_FILES ${LANG_TS})

endif()

# =========================
# Windows Packaging
# =========================
if(WIN32)

    set(CMAKE_INSTALL_BINDIR "${PROJECT_NAME_CAPITALIZED}")

    install(TARGETS converseen
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # ----------------------------
    # ImageMagick runtime
    # ----------------------------
    file(GLOB IM_DLLS "${ImageMagick_EXECUTABLE_DIR}/CORE_RL_*.dll")
    file(GLOB IM_XML  "${ImageMagick_EXECUTABLE_DIR}/*.xml")
    file(GLOB IM_FILTERS "${ImageMagick_EXECUTABLE_DIR}/modules/filters/*.dll")
    file(GLOB IM_CODERS  "${ImageMagick_EXECUTABLE_DIR}/modules/coders/*.dll")

    install(FILES ${IM_DLLS}
        DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(FILES ${IM_XML}
        DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(FILES ${ImageMagick_EXECUTABLE_DIR}/sRGB.icc
        DESTINATION ${CMAKE_INSTALL_BINDIR})

    install(FILES ${ImageMagick_EXECUTABLE_DIR}/NOTICE.txt
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        RENAME ImageMagick_License.txt)

    install(FILES ${IM_FILTERS}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/modules/filters)

    install(FILES ${IM_CODERS}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/modules/coders)

    # Languages
    install(FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/loc)

    # ----------------------------
    # windeployqt
    # ----------------------------
    if(USE_QT6)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt6 REQUIRED)
    else()
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt REQUIRED)
    endif()

    install(CODE "
        execute_process(
            COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/converseen.exe\"
        )
    ")

endif()

# =========================
# Linux Install
# =========================
if(UNIX AND NOT APPLE)

    install(TARGETS converseen
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # Desktop entry
    install(FILES res/net.fasterland.converseen.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)

    # KDE service menu
    install(FILES res/converseen_import.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/kio/servicemenus)

    # AppStream metadata
    install(FILES converseen.appdata.xml
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)

    # Icons
    foreach(size 16 32 48 64 128 256 512)
        install(FILES res/icons/${size}x${size}/converseen.png
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/${size}x${size}/apps)
    endforeach()

    # Languages
    install(FILES ${QM_FILES}
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/converseen/loc)

endif()

# =========================
# macOS Bundle + Deploy
# =========================
if(APPLE)

    # ----------------------------
    # RPATH
    # ----------------------------
    set_target_properties(converseen PROPERTIES
        INSTALL_RPATH "@executable_path/../Frameworks"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    # ----------------------------
    # Install bundle
    # ----------------------------
    install(TARGETS converseen
        BUNDLE DESTINATION .)

    # ----------------------------
    # ImageMagick config
    # ----------------------------
    get_filename_component(IM_LIB_DIR
        ${ImageMagick_MagickCore_LIBRARY}
        DIRECTORY)

    get_filename_component(IM_PREFIX
        ${IM_LIB_DIR}
        DIRECTORY)

    set(IM_CONFIG_DIR "${IM_PREFIX}/etc/ImageMagick-7")

    if(EXISTS "${IM_CONFIG_DIR}")
        install(DIRECTORY ${IM_CONFIG_DIR}
            DESTINATION converseen.app/Contents/Resources)
    endif()

    # ----------------------------
    # Languages
    # ----------------------------
    install(FILES ${QM_FILES}
        DESTINATION converseen.app/Contents/Resources/loc)

    # ----------------------------
    # Ghostscript
    # ----------------------------
    find_library(GS_LIBRARY NAMES gs)

    if(GS_LIBRARY)
        get_filename_component(GS_LIB_DIR ${GS_LIBRARY} DIRECTORY)
        get_filename_component(GS_PREFIX ${GS_LIB_DIR} DIRECTORY)

        file(GLOB GS_VERSION_DIR
            "${GS_PREFIX}/share/ghostscript/[0-9]*")

        list(GET GS_VERSION_DIR 0 GS_RESOURCE_DIR)

        if(EXISTS "${GS_RESOURCE_DIR}")
            install(DIRECTORY "${GS_RESOURCE_DIR}/"
                DESTINATION "converseen.app/Contents/Resources/ghostscript")
        endif()
    endif()

    # ----------------------------
    # Qt deploy
    # ----------------------------
    if(USE_QT6)

        qt_generate_deploy_app_script(
            TARGET converseen
            OUTPUT_SCRIPT qt_deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
        )

        install(SCRIPT ${qt_deploy_script})

    endif()

endif()

# =========================
# CPack
# =========================
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)

set(CPACK_PACKAGE_VENDOR "Francesco Mondello")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Converseen - Batch Image Converter")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_HOMEPAGE_URL "https://converseen.fasterland.net")
set(CPACK_PACKAGE_CONTACT "faster3ck@gmail.net")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME_CAPITALIZED}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME_CAPITALIZED}")

if(WIN32)

    set(CPACK_GENERATOR WIX ZIP)

    # -------------------------
    # WIX Configuration
    # -------------------------
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_SOURCE_DIR}/res/converseen.ico")
    set(CPACK_WIX_UI_BANNER "${CMAKE_SOURCE_DIR}/package/Windows/wix/CPACK_WIX_UI_BANNER.BMP")
    set(CPACK_WIX_UI_DIALOG "${CMAKE_SOURCE_DIR}/package/Windows/wix/CPACK_WIX_UI_DIALOG.BMP")

    set(CPACK_WIX_PROPERTY_ARPHELPLINK "${CPACK_PACKAGE_HOMEPAGE_URL}")
    set(CPACK_WIX_PROPERTY_ARPURLINFOABOUT "${CPACK_PACKAGE_HOMEPAGE_URL}")
    set(CPACK_WIX_ROOT_FEATURE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")

    set(CPACK_WIX_LIGHT_EXTRA_FLAGS "-dcl:high")

    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING.txt")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/COPYING.txt")

    set(CPACK_WIX_PRODUCT_GUID "B35C58D3-FBD2-4A81-8371-588F51000032")
    set(CPACK_WIX_UPGRADE_GUID "B35C58D3-FBD2-4A81-8371-588F51EC180F")

endif()

include(CPack)

