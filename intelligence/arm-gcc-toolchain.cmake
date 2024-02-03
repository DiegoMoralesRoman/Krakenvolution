message("${BoldMagenta}USING ARM TOOLCHAIN${Reset}")
# Set the system name to tell CMake we're cross-compiling
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross-compiler
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# You may need to set the root of your target system (sysroot) and other variables
# set(CMAKE_FIND_ROOT_PATH /path/to/arm/sysroot)
