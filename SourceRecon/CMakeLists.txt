# Create CalcSensitivity executable
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (SENSEPROG "CalcSensi${EXE}")
add_executable(${SENSEPROG} CalculateSensitivity.cpp)

# Specify location of linked libraries
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}GenBackCountsUtils )
target_link_libraries(${SENSEPROG} ${CMAKE_PROJECT_NAME}CalcSenseUtils )