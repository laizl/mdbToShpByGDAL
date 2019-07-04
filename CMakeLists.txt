PROJECT(autoTracktool)

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix for Debug mode, usually d on windows")

IF(COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0003 NEW)
	
	# disable autolinking to qtmain as we have our own main() functions (new in Qt 5.1)
	if((${CMAKE_MAJOR_VERSION} EQUAL 2 OR ${CMAKE_MAJOR_VERSION} GREATER 2) AND
		(${CMAKE_MINOR_VERSION} EQUAL 8 OR ${CMAKE_MINOR_VERSION} GREATER 8) AND
		${CMAKE_PATCH_VERSION} GREATER 10)
		cmake_policy(SET CMP0020 OLD)
	endif()
ENDIF(COMMAND CMAKE_POLICY)

FIND_PACKAGE(GDAL)

# To select a specific version of QT define DESIRED_QT_VERSION
# via cmake -DDESIRED_QT_VERSION=5
# QUIET option disables messages if the package cannot be found.

IF  (DESIRED_QT_VERSION)
    IF  (DESIRED_QT_VERSION MATCHES 5)
        FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)
    ELSEIF (DESIRED_QT_VERSION MATCHES 4)
        FIND_PACKAGE(Qt4 REQUIRED)
    ENDIF()

ELSE()

    FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

    IF ( Qt5Widgets_FOUND )
        # CMake 2.8.8 or greater required
        IF(NOT VALID_BUILDER_VERSION)
            MESSAGE(
            SEND_ERROR
            "Qt5 requires CMake version 2.8.8 or greater!\n"
            "Update CMake or set DESIRED_QT_VERSION to less than 5
            or disable OSG_USE_QT."
            )
        ENDIF( )
    ENDIF( )

    IF ( NOT Qt5Widgets_FOUND )
        FIND_PACKAGE(Qt4 REQUIRED)
    ENDIF()
ENDIF()

#If we have found Qt5, let's try to top off by getting the webkit as well
IF ( Qt5Widgets_FOUND )
message(STATUS "wfc qt5-003")
    FIND_PACKAGE(Qt5OpenGL REQUIRED)
    FIND_PACKAGE(Qt5WebKitWidgets QUIET)

    IF(COMMAND cmake_policy)
        IF(${CMAKE_MAJOR_VERSION} GREATER 2)
            # Qt5 qt5_use_modules usage was causing "Policy CMP0043 is not set: Ignore COMPILE_DEFINITIONS_<Config> properties." warnings
            cmake_policy(SET CMP0043 NEW)
        ENDIF()
    ENDIF()
ENDIF()

IF(Qt5Widgets_FOUND)
  message(STATUS "Qt: Using version 5")
  FIND_PACKAGE(Qt5Core QUIET)
  FIND_PACKAGE(Qt5Gui QUIET)
  FIND_PACKAGE(Qt5Widgets QUIET)
  FIND_PACKAGE(Qt5Sql QUIET)
        
  IF ( Qt5Core_FOUND AND Qt5Widgets_FOUND AND Qt5Gui_FOUND AND Qt5Sql_FOUND)
  message(STATUS "wfc qt5-002")  
  SET(QT_INCLUDES ${Qt5Widgets_INCLUDE_DIRS})
  ENDIF ()
ELSEIF(QT4_FOUND)
  message(STATUS "Qt: Using version 4")
  INCLUDE(${QT_USE_FILE})
  #SET(QT_INCLUDES ${QT_INCLUDES} ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}${QT_QTOPENGL_INCLUDE_DIR} )
  SET(QT_ALL_LIBRARIES ${QT_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTWEBKIT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTXML_LIBRARY})
ENDIF ()

# Expose CMAKE_INCLUDE_PATH and CMAKE_LIBARY_PATH to the GUI so users
# may set these values without needing to manipulate the environment.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} CACHE STRING "You may add additional search paths here. Use ; to separate multiple paths.")
# We are proposing that a new variable called CMAKE_PREFIX_PATH be introduced
# to CMake to compliment CMAKE_INCLUDE_PATH and CMAKE_LIBRARY_PATH.
# A formal feature request has been submited to CMake, Bug #4947.
# It is intended for those users who have common prefixes for their INCLUDE
# and LIBRARY locations. So if users have headers in /usr/local/include
# and libraries in /usr/local/lib, the common prefix is /usr/local.
# It should also cover the case where headers and libraries are
# in the same directory.
# Our proposal expects that FIND_* commands will automatically search for
# CMAKE_PREFIX_PATH right after CMAKE_INCLUDE_PATH or CMAKE_LIBRARY_PATH.
# Obviously, since CMake does not currently support this, we must write
# our Find*.cmake modules to explicitly support this. Otherwise, this variable
# will have no impact.
# This is unofficial so this may be removed or changed at anytime.
SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} CACHE STRING "(EXPERIMENTAL) You may add additional search paths here. Use ; to separate multiple paths.")

# This is for an advanced option to give aggressive warnings
# under different compilers. If yours is not implemented, this option
# will not be made available.
IF(CMAKE_COMPILER_IS_GNUCXX)
    # To be complete, we might also do GNUCC flags,
    # but everything here is C++ code.
    # -Wshadow and -Woverloaded-virtual are also interesting flags, but OSG
    # returns too many hits.
    # FYI, if we do implement GNUCC, then -Wmissing-prototypes in another
    # interesting C-specific flag.
    SET(OSG_AGGRESSIVE_WARNING_FLAGS -Wall -Wparentheses -Wno-long-long -Wno-import -pedantic -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wunused)

    # Previous included -Wformat=2 in OSG_AGGRESSIVE_WARNING_FLAGS but had to remove it due to standard library errors


ELSEIF(MSVC)
        #disable specific warning level 4 warnings:
        #C4100 'identifier' : unreferenced formal parameter
        #C4127 Error Message conditional expression is constant
        #C4706 assignment within conditional expression
        SET(OSG_AGGRESSIVE_WARNING_FLAGS /W4 /wd4706 /wd4127 /wd4100)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        SET(OSG_AGGRESSIVE_WARNING_FLAGS  -Wall -Wparentheses -Wno-long-long -Wno-import -pedantic -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wunused -Wno-overloaded-virtual)

        # CMake lacks an elseif, so other non-gcc, non-VS compilers need
        # to be listed below. If unhandled, OSG_AGGRESSIVE_WARNING_FLAGS should
        # remain unset.

        IF (APPLE)
            # set standard lib, clang defaults to c++0x
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++98")
            set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libstdc++")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98 -stdlib=libstdc++ -Wno-overloaded-virtual -Wno-conversion")
            set(WARNING_CFLAGS "")
        ENDIF()
ENDIF()


# We have some custom .cmake scripts not in the official distribution.
# Maybe this can be used override existing behavior if needed?
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

SET(CMAKE_DEBUG_POSTFIX  "d" CACHE STRING "add a postfix, usually d on windows")
SET(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd" CACHE STRING "add a postfix, usually empty on windows")
SET(CMAKE_MINSIZEREL_POSTFIX "s" CACHE STRING "add a postfix, usually empty on windows")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


INCLUDE_DIRECTORIES(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${GDAL_INCLUDE_DIR})


SET(UI_FILES
    #autotracktool.ui
)

SET(MOC_HDRS
    autotracktool.h
)

set(LIB_QT_RCS
    #autotracktool.qrc
)

IF(Qt5Widgets_FOUND)
    QT5_ADD_RESOURCES( LIB_RC_SRCS ${LIB_QT_RCS} )
    QT5_WRAP_UI( UI_HDRS ${UI_FILES} )
    QT5_WRAP_CPP( UI_SRCS ${UI_HDRS} )
    SET(MOC_SRCS)
    FOREACH( MOC_HDR ${MOC_HDRS} )
        GET_FILENAME_COMPONENT( MOC_HDR_ABS ${MOC_HDR} ABSOLUTE )
        QT5_WRAP_CPP( MOC_SRC ${MOC_HDR} OPTIONS "-f${MOC_HDR_ABS}" )
        LIST( APPEND MOC_SRCS ${MOC_SRC} )
    ENDFOREACH()
ELSE()
    INCLUDE( ${QT_USE_FILE} )
    QT4_ADD_RESOURCES( LIB_RC_SRCS ${LIB_QT_RCS} )
    QT4_WRAP_UI( UI_HDRS ${UI_FILES} )
    QT4_WRAP_CPP( UI_SRCS ${UI_HDRS} )
    QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} OPTIONS "-f" )
ENDIF()

SET(TARGET_H
    ${UI_HDRS}
    ${LIB_QT_RCS}
    autotracktool.h
	NtAccessManager.h
)

SET(TARGET_SRC
    ${UI_SRCS}
    ${MOC_SRCS}
    #Resources/image/autotracktool.rc
    autotracktool.cpp
	NtAccessManager.cpp
    main.cpp
)


IF(Qt5Widgets_FOUND)
    SET(TARGET_ADDED_LIBRARIES
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
		Qt5::Sql
    )
ELSE(Qt5Widgets_FOUND)
    SET(TARGET_ADDED_LIBRARIES
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
    )
ENDIF(Qt5Widgets_FOUND)

INCLUDE(OsgEarthMacroUtils)

#### end var setup  ###
IF(Qt5Widgets_FOUND)
SETUP_APPLICATION(autoTracktool)
ELSE(Qt5Widgets_FOUND)
SETUP_APPLICATION(autoTracktool)
ENDIF(Qt5Widgets_FOUND)

OPTION(NRL_STATIC_LIBRARIES "Link osgEarth against static GDAL and cURL, including static OpenSSL, Proj4, JPEG, PNG, and TIFF." OFF)
if(NOT NRL_STATIC_LIBRARIES)
  LINK_WITH_VARIABLES(autoTracktool GDAL_LIBRARY)
else(NOT NRL_STATIC_LIBRARIES)
  LINK_WITH_VARIABLES(autoTracktool)
endif(NOT NRL_STATIC_LIBRARIES)
