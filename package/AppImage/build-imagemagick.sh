#!/usr/bin/env bash

set -euo pipefail

IM_URL="https://mirror.dogado.de/imagemagick/ImageMagick.tar.bz2"
IM_ARCHIVE="ImageMagick.tar.bz2"
INSTALL_PREFIX="/usr"
SYSCONF_DIR="/etc"

msg() {
    echo -e "\n[INFO] $*\n"
}

# === SCRIPT ===

# Download ImageMagick
msg "Download ImageMagick..."
wget -O "$IM_ARCHIVE" "$IM_URL"

msg "Extracting archive..."
tar -xvf "$IM_ARCHIVE"

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
