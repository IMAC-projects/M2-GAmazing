# Find Libc3ga
#
#  C3GA_FOUND        - True if Libc3ga found.
#  C3GA_INCLUDE_DIRS - where to find Libc3ga include files
#  C3GA_LIBRARIES    - where to find Libc3ga binary libraries


# find include path
find_path (C3GA_INCLUDE_DIR 
		NAMES c3ga
		HINTS /usr/local/include)

# find library file
find_library (C3GA_LIBRARY 
		NAMES c3ga
		HINTS /usr/local/lib/ )


set(C3GA_LIBRARIES ${C3GA_LIBRARY})
set(C3GA_INCLUDE_DIRS ${C3GA_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set C3GA_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (C3GA DEFAULT_MSG 
                                   C3GA_LIBRARIES C3GA_INCLUDE_DIRS)

mark_as_advanced(C3GA_INCLUDE_DIR C3GA_LIBRARIE)





