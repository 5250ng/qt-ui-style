cmake_minimum_required(VERSION 3.16)

project(QtUiStyle VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(QT_UI_STYLE_BUILD_TESTS "Build QtUiStyle tests" ON)
option(QT_UI_STYLE_BUILD_EXAMPLES "Build QtUiStyle examples" OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 6.4 REQUIRED COMPONENTS Widgets)

add_library(QtUiStyleCore STATIC
    include/QtUiStyle/StyleManager.h
    include/QtUiStyle/TitleBar.h
    include/QtUiStyle/BaseFramelessWindow.h
    include/QtUiStyle/BaseFramelessDialog.h
    src/StyleManager.cpp
    src/TitleBar.cpp
    src/BaseFramelessWindow.cpp
    src/BaseFramelessDialog.cpp
)
add_library(QtUiStyle::Core ALIAS QtUiStyleCore)
set_target_properties(QtUiStyleCore PROPERTIES
    EXPORT_NAME Core
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_link_libraries(QtUiStyleCore PUBLIC Qt6::Widgets)
target_include_directories(QtUiStyleCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

add_library(QtUiStyleDialogs STATIC
    include/QtUiStyle/StyledMessageBox.h
    include/QtUiStyle/StyledInputDialog.h
    include/QtUiStyle/StyledColorDialog.h
    include/QtUiStyle/StyledFileDialog.h
    src/StyledMessageBox.cpp
    src/StyledInputDialog.cpp
    src/StyledColorDialog.cpp
    src/StyledFileDialog.cpp
)
add_library(QtUiStyle::Dialogs ALIAS QtUiStyleDialogs)
set_target_properties(QtUiStyleDialogs PROPERTIES
    EXPORT_NAME Dialogs
    POSITION_INDEPENDENT_CODE ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_link_libraries(QtUiStyleDialogs PUBLIC QtUiStyle::Core)
target_include_directories(QtUiStyleDialogs
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(MSVC)
    target_compile_options(QtUiStyleCore PRIVATE /W4)
    target_compile_options(QtUiStyleDialogs PRIVATE /W4)
else()
    target_compile_options(QtUiStyleCore PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(QtUiStyleDialogs PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS QtUiStyleCore QtUiStyleDialogs
    EXPORT QtUiStyleTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT QtUiStyleTargets
    FILE QtUiStyleTargets.cmake
    NAMESPACE QtUiStyle::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtUiStyle
)

configure_package_config_file(
    cmake/QtUiStyleConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/QtUiStyleConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtUiStyle
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/QtUiStyleConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/QtUiStyleConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/QtUiStyleConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/QtUiStyle
)

if(QT_UI_STYLE_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 6.4 REQUIRED COMPONENTS Test)
    add_executable(test_qt_ui_style tests/test_qt_ui_style.cpp)
    target_link_libraries(test_qt_ui_style PRIVATE QtUiStyle::Dialogs Qt6::Test)
    add_test(NAME qt_ui_style COMMAND test_qt_ui_style)
    set_tests_properties(qt_ui_style PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
endif()

if(QT_UI_STYLE_BUILD_EXAMPLES)
    add_executable(qt_ui_style_example examples/basic.cpp)
    target_link_libraries(qt_ui_style_example PRIVATE QtUiStyle::Core)
endif()
