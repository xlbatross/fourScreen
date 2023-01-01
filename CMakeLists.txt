cmake_minimum_required(VERSION 3.5)

project(fourScreen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_OSX_ARCHITECTURES x86_64)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# opencv
if(WIN32)
    set(OpenCV_DIR "C:/openCV-MinGW")
elseif(APPLE)
    set(OpenCV_DIR "/usr/local/Cellar/opencv/4.6.0_1/lib/cmake/opencv4")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})
# opencv end

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network) # 네트워크 추가

set(PROJECT_SOURCES
        main.cpp
        mainwidget.cpp
        mainwidget.h
        mainwidget.ui
        request.h
        request.cpp
        response.h
        response.cpp
        receivethread.h
        receivethread.cpp
)
if (WIN32)
    set(WINDOWS_SOURCES
        clientw.h
        clientw.cpp
    )
elseif (APPLE)
    set(MAC_SOURCES
        clientl.cpp
        clientl.h
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            ${WINDOWS_SOURCES}
            ${MAC_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network # 네트워크 추가
    ${LIBOPENCV}
    ${OpenCV_LIBRARIES}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
