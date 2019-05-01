# Intel-Realsense2-OpenCV-Imaging-Test-
Sample for verifying whether the installation of librealsense2 was successful (only tested under Linux (Debian with Kernels 4.10 and 4.19, where 4.10 is probably required to be installed). Also, a sampel for conversion to OpenCV datastructures is provided.

## Current Way of Installing
- install linux 4.10 kernel (deb file present)
- install linux 4.10 headers (deb file present)
- reboot
- in advanced grub options choose the newly installed kernel 
- install according to https://github.com/IntelRealSense/librealsense/blob/development/doc/installation.md
- Ubuntu 14/16/18 with LTS kernel 
- some aspects may or may not work, however building the library does via cmake and make
- the firmware is already installed on the camera device (UR Inventory-Nr. 000419)
- use sample project to test whether your installation of the realsense2 lib was successful 
