#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SCOTCH::ptscotchparmetisv3" for configuration "Release"
set_property(TARGET SCOTCH::ptscotchparmetisv3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::ptscotchparmetisv3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libptscotchparmetisv3.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::ptscotchparmetisv3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::ptscotchparmetisv3 "${_IMPORT_PREFIX}/lib/libptscotchparmetisv3.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
