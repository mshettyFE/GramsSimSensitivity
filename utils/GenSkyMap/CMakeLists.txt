# Create GenSkyMap Executable to be used in BackgroundRecon and SourceRecon
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (GEN_SKY_MAP "GenSkyMap${EXE}")
add_executable(${GEN_SKY_MAP} GenSkyMap.cpp)
# Specify location of linked libraries
target_link_libraries(${GEN_SKY_MAP} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${GEN_SKY_MAP} ${CMAKE_PROJECT_NAME}ReadRoot )
target_link_libraries(${GEN_SKY_MAP} ${CMAKE_PROJECT_NAME}GenSkyMapUtils )

# Copy over GenSkyMap executable to BackgroundRecon, SourceRecon

add_custom_command(TARGET ${GEN_SKY_MAP} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_SKY_MAP}> ${CMAKE_BINARY_DIR}/BackgroundRecon)

add_custom_command(TARGET ${GEN_SKY_MAP} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${GEN_SKY_MAP}> ${CMAKE_BINARY_DIR}/SourceRecon)

# Remove GenSkyMap executable from current directory
add_custom_command(TARGET ${GEN_SKY_MAP}
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${GEN_SKY_MAP}>)