cmake_minimum_required(VERSION 3.16)

project(cove VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -flto -march=native -pipe -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -Wl,--gc-sections")

set(CMAKE_AUTOMOC ON)
set(CMAKE_STRIP ON)

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
    src/launcher.cpp
    src/helper/configmanager.cpp
    src/helper/desktopfile.cpp
    src/helper/battery.cpp
)

qt_add_resources(${PROJECT_NAME} "qml"
    FILES
        qml/launcher/Main.qml
        qml/launcher/Launcher.qml
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
