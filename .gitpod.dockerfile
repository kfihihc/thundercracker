FROM gitpod/workspace-full

RUN sudo apt-get update \
 && sudo apt-get install -y --no-install-recommends \
    g++ doxygen upx-ucl python-imaging openocd \
    uuid-dev libusb-1.0-0-dev mesa-common-dev \
    libglu1-mesa-dev libasound2-dev ia32-libs \
 && sudo rm -rf /var/lib/apt/lists/* \
