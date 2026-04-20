#!/bin/bash
set -e  # exit on error

#   List of dependent packages
packages=(
  build-essential
  cmake
  libssl-dev
  libcurl4-openssl-dev
  libgpiod-dev
  pkg-config
  libgtk-3-dev
  libwebkit2gtk-4.1-dev
  libcurl4-openssl-dev
  ninja-build
)

#   Ensuring that all the necessary dependecies have been installed.
echo "Installing dependencies..."
sudo apt update
sudo apt install -y "${packages[@]}"

echo "Configuring project..."
#   Removing the previous CMakeCache.txt
#rm build/CMakeCache.txt
#rm -rf build/CMakeFiles

cmake -S . -B build -DBUILD_TESTING=ON -DCMAKE_BUILD_TYPE=Release

echo "Building..."
cmake --build build -j$(3)

echo "Done."

#   Asking the user if they wish to run the exectuable after it has been built.
read -p "Run the executable now? [y/N]: " answer
case "$answer" in
  ([yY][eE][sS]|[yY])
    ./build/Tromboneless
    ;;
  (*)
    echo "Skipping execution."
    ;;
esac
