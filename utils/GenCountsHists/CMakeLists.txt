# Create GenCountsHists
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (GEN_COUNT_HIST_PROG "GenCountsHists${EXE}")
add_executable(${GEN_COUNT_HIST_PROG} GenCountsHists.cpp)

# Specify location of linked libraries
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}GenBackCountsUtils )
target_link_libraries(${GEN_COUNT_HIST_PROG} ${CMAKE_PROJECT_NAME}ReadRoot )


# Copy over executable to SourceRecon/SourceCountHist

add_custom_command(TARGET ${GEN_COUNT_HIST_PROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_COUNT_HIST_PROG}> ${CMAKE_BINARY_DIR}/SourceRecon/SourceCountHist)

# Copy over executable to BackgroundRecon/BackgroundCountHist
add_custom_command(TARGET ${GEN_COUNT_HIST_PROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_COUNT_HIST_PROG}> ${CMAKE_BINARY_DIR}/BackgroundRecon/BackgroundCountHist)

# Remove executable from current directory
add_custom_command(TARGET ${GEN_COUNT_HIST_PROG}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${GEN_COUNT_HIST_PROG}>)