#! /bin/bash

set -x
set -e

#export QT_SELECT=opt-qt514

export GS_VERSION="9.55.0"

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
        if [[ "$BUILD_DIR" == /tmp/appimage-build-* ]]; then
            rm -rf "$BUILD_DIR"
        # Debugging notice for safety check.
        #else
        #    echo "Safety check failed: $BUILD_DIR is not in /tmp/appimage-build-*"
        fi
    fi
}

trap cleanup EXIT

# store repo root as variable
REPO_ROOT=$(readlink -f ../../)
OLD_CWD=$(readlink -f .)

# Fix content rating line in converseen.appdata.xml
cp ../../converseen.appdata.xml ../../converseen.appdata_orig.xml
sed -i 's/<content_rating type="oars-1.1" \/>/<content_rating type="oars-1.1">none<\/content_rating>/' ../../converseen.appdata.xml

# switch to build dir
cd "$BUILD_DIR"

# configure build files with cmake
cmake "$REPO_ROOT" -DUSE_QT6=yes -DCMAKE_INSTALL_PREFIX="$BUILD_DIR/AppDir/usr"

# build project and install files into AppDir
make -j$(nproc)
make install

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
cp $(ldconfig -p | grep libx265.so.209 | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
cp $(ldconfig -p | grep libde265.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib

cp $(ldconfig -p | grep libdav1d.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
cp $(ldconfig -p | grep libaom.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
# cp $(ldconfig -p | grep libavcodec.so.58 | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
# cp $(ldconfig -p | grep libsharpyuv.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib

cp -rv /usr/lib/x86_64-linux-gnu/libheif $BUILD_DIR/AppDir/usr/lib

#cp $(ldconfig -p | grep libicui18n.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
#cp $(ldconfig -p | grep libicuuc.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib
#cp $(ldconfig -p | grep libicudata.so | head -n1 | tr ' ' '\n' | grep /) $BUILD_DIR/AppDir/usr/lib

# Add Ghostscript Resources
cp -rvf /usr/share/ghostscript $BUILD_DIR/AppDir/usr/share
rm -rvf $BUILD_DIR/AppDir/usr/share/ghostscript/${GS_VERSION}/iccprofiles
mkdir -p $BUILD_DIR/AppDir/usr/share/ghostscript/${GS_VERSION}/iccprofiles
cp -rvf /usr/share/ghostscript/${GS_VERSION}/iccprofiles/* $BUILD_DIR/AppDir/usr/share/ghostscript/${GS_VERSION}/iccprofiles

# Fix metadata directory and filename
mv $BUILD_DIR/AppDir/usr/share/metainfo/converseen.appdata.xml $BUILD_DIR/AppDir/usr/share/metainfo/net.fasterland.converseen.appdata.xml

# Create AppImage package
./linuxdeploy-x86_64.AppImage --appdir AppDir -i "$REPO_ROOT"/res/converseen.png -d "$REPO_ROOT"/res/net.fasterland.converseen.desktop  --plugin qt --output appimage

# move built AppImage back into original CWD
mv Converseen*.AppImage "$OLD_CWD"
