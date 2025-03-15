cmake_minimum_required(VERSION 3.5)

project(fingers_control LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver OpenCV
find_package(OpenCV REQUIRED)

# Ajouter les fichiers sources
add_executable(fingers_control main.cpp
    CameraService.cpp CameraService.h MobileNetSSDService.cpp MobileNetSSDService.h ressources.qrc
    assets/MobileNetSSD/deploy.prototxt assets/MobileNetSSD/mobilenet_iter_73000.caffemodel images/pokeball.png ressources/MobileNetSSD/deploy.prototxt)

# Inclure les headers OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Lier OpenCV aux bibliothèques du projet
target_link_libraries(fingers_control ${OpenCV_LIBS})

# Installation
include(GNUInstallDirs)
install(TARGETS fingers_control
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
