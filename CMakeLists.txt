cmake_minimum_required(VERSION 3.5)

project(fingers_control LANGUAGES CXX)

# Définir la norme C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Trouver OpenCV
find_package(OpenCV REQUIRED)

# Trouver Qt5 (assurez-vous que Qt5 est bien installé sur votre machine)
find_package(Qt5 REQUIRED COMPONENTS Core)

# Ajouter les fichiers source
add_executable(fingers_control main.cpp
    CameraService.cpp CameraService.h MobileNetSSDService.cpp MobileNetSSDService.h
    ressources.qrc
)

# Lier Qt5 et OpenCV avec votre projet
target_link_libraries(fingers_control Qt5::Core ${OpenCV_LIBS})

# Installer votre programme
include(GNUInstallDirs)
install(TARGETS fingers_control
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
