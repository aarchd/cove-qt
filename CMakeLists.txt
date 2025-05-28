cmake_minimum_required(VERSION 3.16)

project(cove VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    WaylandClient
)

find_package(LayerShellQt REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/dock.cpp
    src/helper/configmanager.cpp
    src/helper/desktopfile.cpp
    src/helper/desktopiconloader.cpp
)

qt_add_resources(${PROJECT_NAME} "qml"
    FILES
        qml/Main.qml
        qml/Dock.qml
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::WaylandClient
    LayerShellQtInterface
)

target_include_directories(${PROJECT_NAME} PRIVATE src)
