# Create CalcSensitivity
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (SENSEPROG "CalcSensi${EXE}")
add_executable(${SENSEPROG} CalculateSensitivity.cpp)
# Specify location of linked libraries
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}GenSkyMapUtils )
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}CalcSenseUtils )


# Copy over executable to SourceRecon

add_custom_command(TARGET ${SENSEPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SENSEPROG}> ${CMAKE_BINARY_DIR}/SourceRecon)

# Remove executable from current directory
add_custom_command(TARGET ${SENSEPROG}
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${SENSEPROG}>)