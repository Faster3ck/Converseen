#! /bin/bash

set -x
set -e

export MAGICK_HOME=$HOME/ImageMagick7-devel
export PATH="$MAGICK_HOME/bin:$PATH"
export LD_LIBRARY_PATH="$MAGICK_HOME/lib:$LD_LIBRARY_PATH"

# building in temporary directory to keep system clean
# use RAM disk if possible (as in: not building on CI system like Travis, and RAM disk is available)
TEMP_BASE=/tmp
BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" appimage-build-XXXXXX)


# make sure to clean up build dir, even if errors occur
cleanup () {

    if [ -d "$BUILD_DIR" ]; then

        rm -rf "$BUILD_DIR"

    fi
}

trap cleanup EXIT

# store repo root as variable
REPO_ROOT=$(readlink -f ../../)
OLD_CWD=$(readlink -f .)

# switch to build dir
cd "$BUILD_DIR"

# configure build files with qmake
# we need to explicitly set the install prefix, as CMake's default is /usr/local for some reason...
qmake "$REPO_ROOT" IM_INSTALL_PREFIX=$MAGICK_HOME

# build project and install files into AppDir
make -j$(nproc)
make install INSTALL_ROOT=AppDir

# now, build AppImage using linuxdeploy and linuxdeploy-plugin-qt
# download linuxdeploy and its Qt plugin
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

# make them executable
chmod +x linuxdeploy*.AppImage

# make sure Qt plugin finds QML sources so it can deploy the imported files
export QML_SOURCES_PATHS="$REPO_ROOT"/src

# Copy ImageMagick configuration files
mkdir -p $BUILD_DIR/AppDir/.config/ImageMagick
cp -v $MAGICK_HOME/etc/ImageMagick-7/* $BUILD_DIR/AppDir/.config/ImageMagick

# Add SSL libraries
mkdir -p $BUILD_DIR/AppDir/usr/lib

cp $(ldconfig -p | grep libssl.so.1 | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
cp $(ldconfig -p | grep libcrypto.so.1 | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
#cp $(ldconfig -p | grep libicui18n.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
#cp $(ldconfig -p | grep libicuuc.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
#cp $(ldconfig -p | grep libicudata.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib

# Fix metadata directory and filename
mv $BUILD_DIR/AppDir/usr/share/appdata $BUILD_DIR/AppDir/usr/share/metainfo
mv $BUILD_DIR/AppDir/usr/share/metainfo/converseen.appdata.xml $BUILD_DIR/AppDir/usr/share/metainfo/net.fasterland.converseen.appdata.xml

# Create AppImage package
./linuxdeploy-x86_64.AppImage --appdir AppDir -i "$REPO_ROOT"/res/converseen.png -d "$REPO_ROOT"/res/net.fasterland.converseen.desktop  --plugin qt --output appimage

# move built AppImage back into original CWD
mv Converseen*.AppImage "$OLD_CWD"
