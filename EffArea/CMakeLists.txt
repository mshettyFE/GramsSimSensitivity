file( COPY ${CMAKE_SOURCE_DIR}/EffArea/Commands.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsSphere.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsUnique.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Build CalcEffArea Functions
file(GLOB CalcEffAreaFuncSrc src/CalcEffAreaFunctions.cpp
                    ${PROJECT_SOURCE_DIR}/EffArea/src/CalcEffAreaFunctions.cpp
    )

set(CalcEffAreaLib ${CMAKE_PROJECT_NAME}CalcEffAreaFuncs)
add_library (${CalcEffAreaLib} SHARED ${CalcEffAreaFuncSrc})

target_link_libraries(${CalcEffAreaLib} ${ROOT_LIBRARIES} )
target_link_libraries(${CalcEffAreaLib} ${CMAKE_PROJECT_NAME}ReadRoot )

target_include_directories (${CMAKE_PROJECT_NAME}CalcEffAreaFuncs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create CalcEffArea.cpp executable
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)
include_directories(include ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Set Program name to account for compilation on mac OS
set (CALC_AREA "AggregateEffArea${EXE}")
add_executable(${CALC_AREA} CalcEffArea.cpp)
# Specify location of linked libraries
target_link_libraries(${CALC_AREA} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${CALC_AREA} ${CMAKE_PROJECT_NAME}CalcEffAreaFuncs)
