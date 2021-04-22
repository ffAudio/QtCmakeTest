cmake_minimum_required(VERSION 3.17)

set(PROJECT_NAME QtCmakeTest)

set(PROJECT_SOURCES
        Sources/main.cpp
        Sources/mainwindow.cpp
        Sources/mainwindow.h
        Sources/mainwindow.ui
        Sources/AudioPluginManager.cpp
        Sources/AudioPluginManager.h
)

project("${PROJECT_NAME}" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Configure JUCE sources
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

# Configure QT
source_group("QT Resources" REGULAR_EXPRESSION "\.ui$")
source_group("QT generated" REGULAR_EXPRESSION "moc_")
source_group("QT generated" REGULAR_EXPRESSION "mocs_.*\.cpp$")

# The source tree should uhhh, still look like the source tree, yo
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "" FILES ${SourceFiles})

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_subdirectory(JUCE)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(QtCmakeTest SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(QtCmakeTest
            ${PROJECT_SOURCES}
        )
    endif()
endif()

set_target_properties("${PROJECT_NAME}" PROPERTIES FOLDER "")

juce_add_plugin(juceLib
    PRODUCT_NAME "JUCE Lib"
    VERSION 1.0.0
    FORMATS Standalone)

target_compile_definitions(juceLib
    PUBLIC
    JUCE_PLUGINHOST_AU=1
    JUCE_PLUGINHOST_VST3=1)

target_link_libraries(juceLib
    PUBLIC
    juce::juce_core
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_utils
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    juceLib
    Qt${QT_VERSION_MAJOR}::Widgets)
