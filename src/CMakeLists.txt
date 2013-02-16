INCLUDE_DIRECTORIES(
	${NEO4JCPP_BINARY_DIR}
	${NEO4JCPP_SOURCE_DIR}/
	${NEO4JCPP_SOURCE_DIR}/src/
	${NEO4JCPP_SOURCE_DIR}/include/
	)

SET(CORE_SRC
    net/Net.cc
    GraphDatabase.cc
    Node.cc
    Property.cc
    PropertyValue.cc
    Relationship.cc
    Transaction.cc
    Util.cc
    )

ADD_LIBRARY(neo4jclient_static STATIC ${CORE_SRC})
ADD_LIBRARY(neo4jclient_shared SHARED ${CORE_SRC})
SET_TARGET_PROPERTIES(neo4jclient_static PROPERTIES OUTPUT_NAME "neo4jclient") 
SET_TARGET_PROPERTIES(neo4jclient_shared PROPERTIES OUTPUT_NAME "neo4jclient") 
SET_TARGET_PROPERTIES(neo4jclient_shared PROPERTIES VERSION 0.0.1 SOVERSION 1) 
