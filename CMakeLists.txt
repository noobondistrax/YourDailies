cmake_minimum_required(VERSION 3.16)

project(YourDailies VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES YourDailies_de_DE.ts)

set(PROJECT_SOURCES
    main.cpp
    resources.qrc
    app/appcontext.h
    app/appcontext.cpp
    app/usersession.h
    app/usersession.cpp
    repository/database.cpp
    repository/database.h
    services/dashboardservice.h
    services/dashboardservice.cpp
    services/userservice.h
    services/userservice.cpp
    services/widgetservice.h
    services/widgetservice.cpp
    ui/designer.h
    ui/designer.cpp
    ui/mainwindow.cpp
    ui/mainwindow.h
    ui/mainwindow.ui
    ui/registration.h
    ui/registration.cpp
    ui/registration.ui
    ui/userHandling.cpp
    ui/userHandling.h
    utils/globals.h
    utils/logger.h
    utils/logger.cpp
    sqlite/sqlite3.c
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(YourDailies
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        services/userservice.h
        services/userservice.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET YourDailies APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(YourDailies SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(YourDailies
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(YourDailies PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(YourDailies PRIVATE sqlite ui repository app utils models servcies)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.YourDailies)
endif()
set_target_properties(YourDailies PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS YourDailies
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(YourDailies)
endif()
