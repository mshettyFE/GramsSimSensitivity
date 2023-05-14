# The source code to compile  for options
file(GLOB UtilSrc src/Options.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/Options.cpp
    )

# The library's name will be lib(project)Utilities and will
# be located in /util within the build directory. .
set(LibName ${CMAKE_PROJECT_NAME}Utilities)
add_library (${LibName} SHARED ${UtilSrc})

# These utilities depend on ROOT and Xerces-C (for XML).

target_link_libraries(${LibName} ${ROOT_LIBRARIES} )
target_link_libraries(${LibName} ${XercesC_LIBRARIES} )

# Make sure the compiler can find include files for our library
# when other libraries or executables link to it.
target_include_directories (${CMAKE_PROJECT_NAME}Utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create Extraction Utilities
# The source code to compile
file(GLOB ExtractSrc src/ExtractUtils.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/ExtractUtils.cpp
    )

set(ExtractLibName ${CMAKE_PROJECT_NAME}Extract)
add_library (${ExtractLibName} SHARED ${ExtractSrc})

target_link_libraries(${ExtractLibName} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}Extract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)