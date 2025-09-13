cmake_minimum_required(VERSION 3.10)
project(Web2AppManager)

set(CMAKE_CXX_STANDARD 17)

# Find GTK3 package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include directories
include_directories(${GTK3_INCLUDE_DIRS})

# Add executable
add_executable(web2app-manager src/main.cpp)

# Link libraries
target_link_libraries(web2app-manager ${GTK3_LIBRARIES})

# Add compile definitions
add_definitions(${GTK3_CFLAGS_OTHER})

# Set install destination
install(TARGETS web2app-manager DESTINATION /usr/local/bin)
