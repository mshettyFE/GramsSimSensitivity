# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Display all CMAKE variables
#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

set (PROG "test${EXE}")
add_executable (${PROG} Extract.cpp)
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}Utilities )
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}Extract )