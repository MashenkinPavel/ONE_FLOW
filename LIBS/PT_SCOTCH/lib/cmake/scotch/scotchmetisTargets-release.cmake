#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SCOTCH::scotchmetisv3" for configuration "Release"
set_property(TARGET SCOTCH::scotchmetisv3 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::scotchmetisv3 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libscotchmetisv3.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::scotchmetisv3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::scotchmetisv3 "${_IMPORT_PREFIX}/lib/libscotchmetisv3.a" )

# Import target "SCOTCH::scotchmetisv5" for configuration "Release"
set_property(TARGET SCOTCH::scotchmetisv5 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SCOTCH::scotchmetisv5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libscotchmetisv5.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SCOTCH::scotchmetisv5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_SCOTCH::scotchmetisv5 "${_IMPORT_PREFIX}/lib/libscotchmetisv5.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
