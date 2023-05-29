# The source code to compile for options
file(GLOB UtilSrc src/Options.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/Options.cpp
    )

# The library's name will be lib(project)SenseUtilities and will
# be located in /util within the build directory. .
set(LibName ${CMAKE_PROJECT_NAME}SenseUtilities)
add_library (${LibName} STATIC ${UtilSrc})

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
add_library (${ExtractLibName} STATIC ${ExtractSrc})

target_link_libraries(${ExtractLibName} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}Extract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create ReadRootFiles library
file(GLOB ReadRootSrc src/ReadRootFiles.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/ReadRootFiles.cpp
    )

set(ReadRootLib ${CMAKE_PROJECT_NAME}ReadRoot)
add_library (${ReadRootLib} STATIC ${ReadRootSrc})

target_link_libraries(${ReadRootLib} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}ReadRoot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create ReconstructUtils library
file(GLOB ReconstructSrc src/ReconstructUtils.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/ReconstructUtils.cpp
    )

set(ReconstructLib ${CMAKE_PROJECT_NAME}ReconstructUtils)
add_library (${ReconstructLib} STATIC ${ReconstructSrc})

target_link_libraries(${ReconstructLib} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}ReconstructUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create GenSkyMapUtils library
file(GLOB GenSkyMapSrc src/GenSkyMapUtils.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/GenSkyMapUtils.cpp
    )

set(GenSkyMapLib ${CMAKE_PROJECT_NAME}GenSkyMapUtils)
add_library (${GenSkyMapLib} STATIC ${GenSkyMapSrc})

target_link_libraries(${GenSkyMapLib} ${ROOT_LIBRARIES} )
target_link_libraries(${GenSkyMapLib} ${Eigen_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}GenSkyMapUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create CalcSenseUtils library
file(GLOB CalcSenseSrc src/CalcSenseUtils.cpp
                    ${PROJECT_SOURCE_DIR}/util/src/CalcSenseUtils.cpp
    )

set(CalcSenseLib ${CMAKE_PROJECT_NAME}CalcSenseUtils)
add_library (${CalcSenseLib} STATIC ${CalcSenseSrc})

target_link_libraries(${CalcSenseLib} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}CalcSenseUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Copy GenEnergySpectrum.py over to Background
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenEnergySpectrum.py DESTINATION ${CMAKE_BINARY_DIR}/BackgroundRecon)

# Copy over GenCondorScripts.py to BackgroundRecon and SourceRecon
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenCondorScripts.py DESTINATION ${CMAKE_BINARY_DIR}/BackgroundRecon)
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenCondorScripts.py DESTINATION ${CMAKE_BINARY_DIR}/SourceRecon)

# Copy over GenCondorScriptsHist.py to BackgroundRecon and SourceRecon
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenCondorScriptsHist.py DESTINATION ${CMAKE_BINARY_DIR}/BackgroundRecon)
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenCondorScriptsHist.py DESTINATION ${CMAKE_BINARY_DIR}/SourceRecon)

# Create Extract and Reconstruct helper executables
add_subdirectory(Extraction)
add_subdirectory(Reconstruction)
add_subdirectory(GenSkyMap)
add_subdirectory(CalculateSensitivity)