#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SCOTCH::scotcherr" for configuration "Release"
set_property(TARGET SCOTCH::scotcherr APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::scotcherr PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libscotcherr.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::scotcherr )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::scotcherr "${_IMPORT_PREFIX}/lib/libscotcherr.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
