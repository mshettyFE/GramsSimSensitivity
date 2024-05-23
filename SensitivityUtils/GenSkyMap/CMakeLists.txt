# Create GenSkyMap
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (GEN_COUNT_HIST_PROG "GenSkyMap${EXE}")
add_executable(${GEN_COUNT_HIST_PROG} GenSkyMap.cpp)

# Specify location of linked libraries
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}GenSkyMapUtils )
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}ReadRoot )


# Copy over executable to GramsWork

add_custom_command(TARGET ${GEN_COUNT_HIST_PROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_COUNT_HIST_PROG}> ${CMAKE_SOURCE_DIR}/GramsWork)