#determine system: Linux,MacOSX,Windows
if (NOT DEFINED OSNAME)
    set(OSNAME NOTFOUND CACHE STRING "Name of the OS. Possible values: LINUX, MACOSX, WINDOWS")
    
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR UNIX AND NOT APPLE OR CYGWIN)
        set(OSNAME "LINUX")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR UNIX AND NOT APPLE OR CYGWIN)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR APPLE)
       set(OSNAME "MACOSX")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR APPLE)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Win" OR WIN32)
       set(OSNAME "WINDOWS")
    endif(${CMAKE_SYSTEM_NAME} MATCHES "Win" OR WIN32)
    
    if(OSNAME)
        message(STATUS "Detected OS: " ${OSNAME})

        set(OSNAME ${OSNAME} CACHE STRING "Name of the OS. Possible values: LINUX, MACOSX, WINDOWS" FORCE)
    else(OSNAME)
        message(FATAL_ERROR "Can not determine OS. Supported types: Linux, MacOSX, Windows")
    endif(OSNAME)
endif (NOT DEFINED OSNAME)


