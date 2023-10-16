## Copy over command files
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/Commands.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsSphere.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CommandsUnique.txt DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over GramsSim files to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over EffAreaPlt.py to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CalcEffArea.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)