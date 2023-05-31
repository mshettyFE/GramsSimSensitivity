# Create GenSkyMap Executable to be used in BackgroundRecon and SourceRecon
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (GEN_BACK_COUNT "GenBackCounts${EXE}")
add_executable(${GEN_BACK_COUNT} GenBackCounts.cpp)
# Specify location of linked libraries
target_link_libraries(${GEN_BACK_COUNT} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${GEN_BACK_COUNT} ${CMAKE_PROJECT_NAME}ReadRoot )
target_link_libraries(${GEN_BACK_COUNT} ${CMAKE_PROJECT_NAME}GenBackCountsUtils )

# Copy over GenSkyMap executable to BackgroundRecon, SourceRecon

add_custom_command(TARGET ${GEN_BACK_COUNT} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_BACK_COUNT}> ${CMAKE_BINARY_DIR}/BackgroundRecon/BackgroundCount)

# Remove GenSkyMap executable from current directory
add_custom_command(TARGET ${GEN_BACK_COUNT}
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${GEN_BACK_COUNT}>)