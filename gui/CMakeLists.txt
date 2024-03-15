project(qfcpp VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    if(NOT EXISTS resources/Resource.rc)
        file(WRITE resources/Resource.rc "IDI_ICON1               ICON    \"../../images/Logo.ico\"")
    endif()
endif()

configure_file(
    ${TOP_DIR}/gui/resources/Resource.qrc.in
    ${TOP_DIR}/gui/resources/Resource.qrc @ONLY
)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

file(GLOB UI_FILES ui/*.ui)
file(GLOB TS_FILES languages/*.ts)
file(GLOB RC_FILES resources/*.*rc)

set(QFCPP_SRCS
    ${TOP_DIR}/gui/src/QtMain.cpp
    ${TOP_DIR}/gui/src/Config.cpp
    ${TOP_DIR}/gui/src/ErrorHandler.cpp
    ${TOP_DIR}/gui/src/Util.cpp
    ${TOP_DIR}/gui/src/Emulator.cpp
    ${TOP_DIR}/gui/src/ColorSquareLabel.cpp
    ${TOP_DIR}/gui/src/SettingDialog.cpp
    ${TOP_DIR}/gui/src/InputConfigureDialog.cpp
    ${TOP_DIR}/gui/src/MainWindow.cpp
    ${TOP_DIR}/gui/include/Config.hpp
    ${TOP_DIR}/gui/include/ErrorHandler.hpp
    ${TOP_DIR}/gui/include/Util.hpp
    ${TOP_DIR}/gui/include/RomInfo.hpp
    ${TOP_DIR}/gui/include/Emulator.hpp
    ${TOP_DIR}/gui/include/ColorSquareLabel.hpp
    ${TOP_DIR}/gui/include/SettingDialog.hpp
    ${TOP_DIR}/gui/include/InputConfigureDialog.hpp
    ${TOP_DIR}/gui/include/MainWindow.hpp
    ${UI_FILES}
    ${RC_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_create_translation(QM_FILES ${QFCPP_SRCS} ${TS_FILES})
    qt_add_executable(qfcpp MANUAL_FINALIZATION ${QFCPP_SRCS} ${QM_FILES})
else()
    qt5_create_translation(QM_FILES ${QFCPP_SRCS} ${TS_FILES})
    add_executable(qfcpp ${QFCPP_SRCS} ${QM_FILES})
endif()

target_include_directories(qfcpp PRIVATE
    $<BUILD_INTERFACE:${TOP_DIR}/gui/include>
)

target_link_libraries(qfcpp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    fcpp
    fcpp_io
    fcpp_util
)

set_target_properties(qfcpp PROPERTIES
    AUTOUIC_SEARCH_PATHS ui
    MACOSX_BUNDLE_GUI_IDENTIFIER tianzerl.fcpp.qfcpp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(qfcpp PRIVATE
    QFCPP_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    QFCPP_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
    QFCPP_BUILD_DATE="${DATE_TODAY}"
    QFCPP_BUILD_YEAR="${DATE_YEAR}"
    QFCPP_VERSION_STR="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
)

if(QT_VERSION_MAJOR GREATER_EQUAL 6)
    qt_finalize_executable(qfcpp)
endif()

install(
    TARGETS qfcpp
    ARCHIVE DESTINATION qfcpp/lib
    LIBRARY DESTINATION qfcpp/lib
    RUNTIME DESTINATION bin
)
