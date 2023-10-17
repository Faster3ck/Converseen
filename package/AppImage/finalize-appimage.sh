#! /bin/bash

set -x
set -e

PROGRAM_NAME="Converseen"
PROGRAM_VERSION="0.11.0.1"
BUILD_VERSION="1"
ARCH=$(uname -m)

OUTPUT_NAME=$PROGRAM_NAME-$PROGRAM_VERSION-$BUILD_VERSION-$ARCH.AppImage

# Download appimagetool
wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod a+x appimagetool-x86_64.AppImage

OLD_DIR=$(readlink -f ../../)
CURR_DIR=$(readlink -f $(dirname $(dirname $0)))

# Extract AppImage
./Converseen-x86_64.AppImage --appimage-extract

# Replace AppRun file
mv squashfs-root AppDir
cp -vf AppRun AppDir/AppRun

# Recreate AppImage
./appimagetool-x86_64.AppImage -v AppDir --sign
mv Converseen-x86_64.AppImage $OUTPUT_NAME
chmod a+x $OUTPUT_NAME

# Clean Environment
rm -rf AppDir
rm -f appimagetool-x86_64.AppImage

# Generate sig file
gpg2 --output $OUTPUT_NAME.sig --detach-sig $OUTPUT_NAME

