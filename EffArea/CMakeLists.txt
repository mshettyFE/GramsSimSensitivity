file( COPY ${CMAKE_SOURCE_DIR}/EffArea/Commands.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsSphere.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsUnique.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over GramsSim files to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/SenseJob DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over GenCondorScripts.py and EffAreaPlt.py to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/utils/GenCondorScripts.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/EffAreaPlt.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CalcEffArea.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)