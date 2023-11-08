# Copy over GramsSim files to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over EffAreaPlt.py to EffArea
file( COPY ${CMAKE_SOURCE_DIR}/EffArea/CalcEffArea.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)