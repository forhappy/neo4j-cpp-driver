INCLUDE_DIRECTORIES(
	${NEO4JCPP_BINARY_DIR}
	${NEO4JCPP_SOURCE_DIR}/include/
	${NEO4JCPP_SOURCE_DIR}/src/
	)

ADD_EXECUTABLE(graphdb graphdb.cc)
TARGET_LINK_LIBRARIES(graphdb neo4jclient_static curl)

ADD_EXECUTABLE(node-GetProperty node-GetProperty.cc)
TARGET_LINK_LIBRARIES(node-GetProperty neo4jclient_static curl)

