#!/bin/sh

set -eux

ARCH="$(uname -m)"
VERSION="$(pacman -Q converseen | awk '{print $2; exit}')"
URUNTIME="https://raw.githubusercontent.com/pkgforge-dev/Anylinux-AppImages/refs/heads/main/useful-tools/uruntime2appimage.sh"
SHARUN="https://raw.githubusercontent.com/pkgforge-dev/Anylinux-AppImages/refs/heads/main/useful-tools/quick-sharun.sh"

export ADD_HOOKS="self-updater.bg.hook"
export ICON=/usr/share/icons/hicolor/256x256/apps/converseen.png
export DESKTOP=/usr/share/applications/net.fasterland.converseen.desktop
export UPINFO="gh-releases-zsync|${GITHUB_REPOSITORY%/*}|${GITHUB_REPOSITORY#*/}|latest|*$ARCH.AppImage.zsync"
export OUTNAME=Converseen-"$VERSION"-anylinux-"$ARCH".AppImage

# Deploy dependencies
wget --retry-connrefused --tries=30 "$SHARUN" -O ./quick-sharun
chmod +x ./quick-sharun
./quick-sharun \
	/usr/bin/converseen       \
	/usr/lib/libSvtAv1Enc.so  \
	/usr/lib/libavcodec.so

cat >> ./AppDir/.env << 'EOF'
# Set Ghostscript env
GS_LIB="${SHARUN_DIR}/share/ghostscript/Resource/Init"
GS_FONTPATH="${SHARUN_DIR}/share/ghostscript/Resource/Font"
GS_OPTIONS="-sGenericResourceDir=${SHARUN_DIR}/share/ghostscript/Resource/ -sICCProfilesDir=${SHARUN_DIR}/share/ghostscript/iccprofiles/"
EOF

# Copy Ghostscript resources
cp -rv /usr/share/ghostscript ./AppDir/share

# Copy Converseen resources
mkdir -p ./AppDir/share/applications
mkdir -p ./AppDir/share/metainfo
mkdir -p ./AppDir/share/kio/servicemenus

cp -v /usr/share/applications/net.fasterland.converseen.desktop ./AppDir/share/applications
cp -v /usr/share/metainfo/converseen.appdata.xml ./AppDir/share/metainfo
cp -v /usr/share/kio/servicemenus/converseen_import.desktop ./AppDir/share/kio/servicemenus

# Remove useless files
rm ./AppDir/bin/convert
rm ./AppDir/bin/magick

# MAKE APPIMAGE WITH URUNTIME
wget --retry-connrefused --tries=30 "$URUNTIME" -O ./uruntime2appimage
chmod +x ./uruntime2appimage
./uruntime2appimage

mkdir -p ./dist
mv -v ./*.AppImage* ./dist
