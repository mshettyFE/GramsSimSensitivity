# Create Reconstruction Executable 

set (RECONPROG "Reconstruct${EXE}")
add_executable(${RECONPROG} Reconstruct.cpp)

## Specify location of linked libraries
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReadRoot )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReconstructUtils )

# Copy over Reconstruction executable to BackgroundRecon, SourceRecon, and EffArea
add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_BINARY_DIR}/BackgroundRecon/SenseJob)

add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_BINARY_DIR}/SourceRecon/SenseJob)

# Remove Reconstruction executable from current directory
add_custom_command(TARGET ${RECONPROG}
                    POST_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E rm $<TARGET_FILE:${RECONPROG}>)