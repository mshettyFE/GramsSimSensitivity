# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (PROG "Extract${EXE}")
add_executable (${PROG} Extract.cpp)
# Specify location of linked libraries
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}Utilities )
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}Extract )

# Put Extract file in SenseJob location

set_target_properties( ${PROG} 
   PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
   "${CMAKE_CURRENT_BINARY_DIR}/SenseJob" 
   )