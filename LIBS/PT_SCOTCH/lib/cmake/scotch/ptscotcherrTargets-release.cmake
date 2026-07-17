#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SCOTCH::ptscotcherr" for configuration "Release"
set_property(TARGET SCOTCH::ptscotcherr APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::ptscotcherr PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libptscotcherr.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::ptscotcherr )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::ptscotcherr "${_IMPORT_PREFIX}/lib/libptscotcherr.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
