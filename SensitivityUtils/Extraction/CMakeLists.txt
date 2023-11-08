## Create Extract Executable to be used in Effective  Area Calculation, Background and Source
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (EXTRACTPROG "Extract${EXE}")
add_executable(${EXTRACTPROG} Extract.cpp)

# Specify location of linked libraries
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}Extract )
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}ReadRoot )

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/Source/GramsWork)

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/Background/GramsWork)

add_custom_command(TARGET ${EXTRACTPROG} 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_BINARY_DIR}/EffArea/GramsWork)
