# Entry classes
file(GLOB Entries
    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/GramsG4Entry.cpp
    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/DetSimEntry.cpp
    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/ExtractionEntry.cpp
    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/RecoEntry.cpp
    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/Entry.cpp
)
set(EntryLibName ${CMAKE_PROJECT_NAME}Entries)
add_library (${EntryLibName} STATIC ${Entries})
target_link_libraries(${EntryLibName} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}Entries PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Options class
file(GLOB UtilSrc
                    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/Options.cpp
    )
set(LibName ${CMAKE_PROJECT_NAME}SenseUtilities)
add_library (${LibName} STATIC ${UtilSrc})
# Add ROOT and Xerces dependencies
target_link_libraries(${LibName} ${ROOT_LIBRARIES} )
target_link_libraries(${LibName} ${XercesC_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}SenseUtilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create Extraction Utilities
file(GLOB ExtractSrc
                    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/ExtractUtils.cpp
    )
set(ExtractLibName ${CMAKE_PROJECT_NAME}Extract)
add_library (${ExtractLibName} STATIC ${ExtractSrc})

target_link_libraries(${ExtractLibName} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}Extract PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create ReadRootFiles library
file(GLOB ReadRootSrc
                    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/ReadRootFiles.cpp
    )

set(ReadRootLib ${CMAKE_PROJECT_NAME}ReadRoot)
add_library (${ReadRootLib} STATIC ${ReadRootSrc})

target_link_libraries(${ReadRootLib} ${ROOT_LIBRARIES} )
target_link_libraries(${ReadRootLib} ${CMAKE_PROJECT_NAME}Entries)
target_include_directories (${CMAKE_PROJECT_NAME}ReadRoot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create ReconstructUtils library
file(GLOB ReconstructSrc 
                    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/ReconstructUtils.cpp
    )

set(ReconstructLib ${CMAKE_PROJECT_NAME}ReconstructUtils)
add_library (${ReconstructLib} STATIC ${ReconstructSrc})

target_link_libraries(${ReconstructLib} ${ROOT_LIBRARIES} )
target_include_directories (${CMAKE_PROJECT_NAME}ReconstructUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create GenSkyMapUtils library
file(GLOB GenBackCountsSrc
                    ${PROJECT_SOURCE_DIR}/SensitivityUtils/src/GenSkyMapUtils.cpp
    )

set(GenBackLib ${CMAKE_PROJECT_NAME}GenSkyMapUtils)
add_library (${GenBackLib} STATIC ${GenBackCountsSrc})

target_link_libraries(${GenBackLib} ${ROOT_LIBRARIES} )
# We need to link Eigen as well because writing your own matrix library is always a terrible idea
# And something I have done more than I care to admit...
target_link_libraries(${GenBackLib} ${Eigen_LIBRARIES} )
target_link_libraries(${ReadRootLib} ${CMAKE_PROJECT_NAME}Entries)
target_include_directories (${CMAKE_PROJECT_NAME}GenSkyMapUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create Extract, Reconstruct and GenCountsHists helper executables
add_subdirectory(Extraction)
add_subdirectory(Reconstruction)
add_subdirectory(GenSkyMap)
