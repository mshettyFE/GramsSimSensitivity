## Create Extract Executable to be used in Effective  Area Calculation, Background and Source
# Search for header files
include_directories(include ${PROJECT_SOURCE_DIR}/SensitivityUtils/include)

# Set Program name to account for compilation on mac OS
set (EXTRACTPROG "Extract${EXE}")
add_executable(${EXTRACTPROG} Extract.cpp)

# Specify location of linked libraries
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}ReadRoot)
target_link_libraries(${EXTRACTPROG} ${CMAKE_PROJECT_NAME}Extract)

add_custom_command(TARGET ${EXTRACTPROG}
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${EXTRACTPROG}> ${CMAKE_SOURCE_DIR}/GramsWork)