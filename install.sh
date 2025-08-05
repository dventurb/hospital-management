#!/bin/bash

# Debian / Ubuntu
install_with_apt() {
  sudo apt update
  sudo apt install -y \
    build-essential \
    pkg-config \
    libhpdf-dev \
    libgtk-4-dev \
    libcjson-dev
}

# Red Hat / CentOS
install_with_dnf() {
  sudo dnf install -y \
    gcc \
    make \
    pkgconf-pkg-config \
    libharu-devel \
    gtk4-devel \
    cjson-devel
}

# Arch Linux
install_with_pacman() {
  sudo pacman -Sy --noconfirm \
    base-devel \
    pkgconf \
    libharu \
    gtk4 \
    cjson
}

if [ -f /etc/arch-release ]; then
  install_with_pacman
elif [ -f /etc/debian_version ]; then
  install_with_apt
elif [ -f /etc/redhat-release ] || [ -f /etc/centos-release ]; then
  install_with_dnf
else
  echo "Your linux distro is not supported currently."
  echo "You need to manualy install those packages: gcc, make, pkg-config, libharu-devel/libhpdf-dev libgtk-4-dev/gtk4-devel/gtk4"
  exit 1
fi

make
echo "INSTALLATION FINISHED"

