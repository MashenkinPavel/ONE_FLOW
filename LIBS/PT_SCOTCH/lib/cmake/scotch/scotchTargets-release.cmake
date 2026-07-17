#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SCOTCH::scotch" for configuration "Release"
set_property(TARGET SCOTCH::scotch APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::scotch PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libscotch.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::scotch )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::scotch "${_IMPORT_PREFIX}/lib/libscotch.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
