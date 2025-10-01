#!/bin/sh

set -eux

ARCH="$(uname -m)"
EXTRA_PACKAGES="https://raw.githubusercontent.com/pkgforge-dev/Anylinux-AppImages/refs/heads/main/useful-tools/get-debloated-pkgs.sh"

pacman -Syu --noconfirm \
	base-devel       \
	curl             \
	git              \
	djvulibre        \
	jbigkit          \
	libheif          \
	libjpeg-turbo    \
	libjxl           \
	libraw           \
	libtiff          \
	libultrahdr      \
	libwebp          \
	libwmf           \
	libzip           \
	openexr          \
	openjpeg2        \
	libxcb           \
	libxcursor       \
	libxi            \
	libxkbcommon     \
	libxkbcommon-x11 \
	libxrandr        \
	libxtst          \
	qt6ct            \
	qt6-wayland      \
	wget             \
	xorg-server-xvfb \
	rav1e            \
	ghostscript      \
	ffmpeg           \
	zsync

	if [ "$(uname -m)" = "x86_64" ]; then
	  pacman -S --noconfirm --needed svt-av1
	fi

echo "Installing debloated packages..."
echo "---------------------------------------------------------------"
wget --retry-connrefused --tries=30 "$EXTRA_PACKAGES" -O ./get-debloated-pkgs.sh
chmod +x ./get-debloated-pkgs.sh
./get-debloated-pkgs.sh --add-opengl --prefer-nano qt6-base-mini gtk3-mini libxml2-mini opus-mini
