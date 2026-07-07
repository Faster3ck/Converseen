#!/usr/bin/env bash

set -euo pipefail
        
IM_REPO="https://github.com/ImageMagick/ImageMagick/releases/download/"
INSTALL_PREFIX="/usr"
SYSCONF_DIR="/etc"

msg() {
    echo -e "\n[INFO] $*\n"
}

# === SCRIPT ===

# Get latest ImageMagick version number from GitHub
gitTags=$(git ls-remote --sort="version:refname" --tags https://github.com/ImageMagick/ImageMagick.git)
lastTagLine=$(echo "$gitTags" | tail -n 1)
latestVersionNumber=$(echo "$lastTagLine" | sed -E 's/.*([0-9]+\.[0-9]+\.[0-9]+-[0-9]+).*/\1/')

# Download ImageMagick
msg "Download ImageMagick..."
wget $IM_REPO/$latestVersionNumber/ImageMagick-$latestVersionNumber.tar.xz

msg "Extracting archive..."
tar -xvf ImageMagick-*.tar.gz

cd ImageMagick-*/

# Configure
msg "Configure build..."
./configure \
  --enable-shared \
  --disable-installed \
  --enable-hdri \
  --without-perl \
  --disable-dependency-tracking \
  --with-gslib=yes \
  --with-openjp2 \
  --with-wmf \
  --without-gvc \
  --with-djvu \
  --without-dps \
  --without-fpx \
  --with-heic=yes \
  --without-rsvg \
  --with-uhdr \
  --with-fftw \
  --with-lqr \
  --without-autotrace \
  --without-dps \
  --without-fpx \
  --without-gcc-arch \
  --with-dejavu-font-dir=/usr/share/fonts/TTF \
  --with-gs-font-dir=/usr/share/fonts/gsfonts \
  --prefix="$INSTALL_PREFIX" \
  --sysconfdir="$SYSCONF_DIR"

# Compilazione
msg "Compiling..."
make -j"$(nproc)"
sudo make install
