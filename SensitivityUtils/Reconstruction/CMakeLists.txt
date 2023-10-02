# Create Reconstruction Executable 

set (RECONPROG "Reconstruct${EXE}")
add_executable(${RECONPROG} Reconstruct.cpp)

## Specify location of linked libraries
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReadRoot )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReconstructUtils )

# Copy over Reconstruction executable to Background, Source, and EffArea
add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_BINARY_DIR}/Source/GramsWork)

add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_BINARY_DIR}/Background/GramsWork)

add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_BINARY_DIR}/EffArea/GramsWork)
