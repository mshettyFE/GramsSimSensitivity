## Create Extract Executable to be used in Effective  Area Calculation, BackgroundRecon and SourceRecon
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (EXTRACTPROG "Extract${EXE}")
add_executable(${EXTRACTPROG} Extract.cpp)

# Specify location of linked libraries
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}Extract )
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}ReadRoot )


# Copy over Extraction executable to BackgroundRecon, SourceRecon

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/BackgroundRecon/SenseJob)

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/SourceRecon/SenseJob)

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/EffArea/SenseJob)

# Remove Reconstruction executable from current directory
add_custom_command(TARGET ${EXTRACTPROG}
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${EXTRACTPROG}>)