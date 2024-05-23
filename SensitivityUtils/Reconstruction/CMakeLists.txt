# Create Reconstruction Executable 
include_directories(include ${PROJECT_SOURCE_DIR}/SensitivityUtils/include)

set (RECONPROG "Reconstruct${EXE}")
add_executable(${RECONPROG} Reconstruct.cpp )

## Specify location of linked libraries
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}Entries )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReconstructUtils )
target_link_libraries(${RECONPROG} ${CMAKE_PROJECT_NAME}ReadRoot )

# Copy over Reconstruction executable to GramsWork
add_custom_command(TARGET ${RECONPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${RECONPROG}> ${CMAKE_SOURCE_DIR}/GramsWork/)