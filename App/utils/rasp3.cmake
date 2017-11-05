SET(CMAKE_SYSTEM_NAME Linux)
#SET(CMAKE_SYSTEM_VERSION 1)

SET(DEVROOT $ENV{HOME}/crosscomp)

SET(ALTSYSRT /media/tf/Vbox/rootfs2)

SET(PIROOT ${DEVROOT}/rootfs)
SET(PITOOLS ${DEVROOT}/tools)

SET(TOOLROOT ${PITOOLS}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${TOOLROOT}/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLROOT}/bin/arm-linux-gnueabihf-g++)

SET(CMAKE_SYSROOT ${ALTSYSRT})
SET(CMAKE_FIND_ROOT_PATH ${ALTSYSRT})


# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
