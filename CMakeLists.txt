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

FIND_PACKAGE(MDB)

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
#set(CMAKE_AUTORCC ON)

INCLUDE_DIRECTORIES(${MDB_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${GDAL_INCLUDE_DIR} ${GLIB_INCLUDE_DIR} ${GLIB_INCLUDE_64_DIR})

SET(TARGET_H
    autotracktool.h
    NtAccessManager.h
)

SET(TARGET_SRC
    autotracktool.cpp
	NtAccessManager.cpp
    main.cpp
)

INCLUDE(OsgEarthMacroUtils)

#### end var setup  ###
IF(Qt5Widgets_FOUND)
SETUP_APPLICATION(autoTracktool)
ELSE(Qt5Widgets_FOUND)
SETUP_APPLICATION(autoTracktool)
ENDIF(Qt5Widgets_FOUND)

OPTION(NRL_STATIC_LIBRARIES "Link osgEarth against static GDAL and cURL, including static OpenSSL, Proj4, JPEG, PNG, and TIFF." OFF)
if(NOT NRL_STATIC_LIBRARIES)
  LINK_WITH_VARIABLES(autoTracktool GDAL_LIBRARY MDB_LIBRARY)
else(NOT NRL_STATIC_LIBRARIES)
  LINK_WITH_VARIABLES(autoTracktool)
endif(NOT NRL_STATIC_LIBRARIES)
