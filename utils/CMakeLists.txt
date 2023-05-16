# The source code to compile for options
file(GLOB UtilSrc src/Options.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/Options.cpp
    )

# The library's name will be lib(project)SenseUtilities and will
# be located in /util within the build directory. .
set(LibName ${CMAKE_PROJECT_NAME}SenseUtilities)
add_library (${LibName} SHARED ${UtilSrc})

# These utilities depend on ROOT and Xerces-C (for XML).

target_link_libraries(${LibName} ${ROOT_LIBRARIES} )
target_link_libraries(${LibName} ${XercesC_LIBRARIES} )

# Make sure the compiler can find include files for our library
# when other libraries or executables link to it.
target_include_directories (${CMAKE_PROJECT_NAME}SenseUtilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create Extraction Utilities
# The source code to compile
file(GLOB ExtractSrc src/ExtractUtils.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/ExtractUtils.cpp
    )

set(ExtractLibName ${CMAKE_PROJECT_NAME}Extract)
add_library (${ExtractLibName} SHARED ${ExtractSrc})

target_link_libraries(${ExtractLibName} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}Extract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create Extract Executable to be used in Effective  Area Calculation and Condor Generation
# Search for header files in utils
include_directories(include ${PROJECT_SOURCE_DIR}/utils/include)

# Set Program name to account for compilation on mac OS
set (PROG "Extract${EXE}")
add_executable(${PROG} Extract.cpp)
# Specify location of linked libraries
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}SenseUtilities )
target_link_libraries(${PROG} ${CMAKE_PROJECT_NAME}Extract )

# Put Extract file in SenseJob location at 3 locations

set_target_properties( ${PROG} 
   PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
   "${CMAKE_BINARY_DIR}/BackgroundRecon/SenseJob" 
   )
set_target_properties( ${PROG} 
   PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
   "${CMAKE_BINARY_DIR}/SourceRecon/SenseJob" 
   )
set_target_properties( ${PROG} 
   PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
   "${CMAKE_BINARY_DIR}/EffArea/SenseJob" 
   )
