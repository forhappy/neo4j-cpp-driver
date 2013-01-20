/* GraphDatabase is the main access point to a running Neo4j instance
 * through HTTP protocol.
 *
 * Copyright (c) 2012-2013 Fu Haiping <haipingf@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _NEO4JCPP_GRAPHDATABASE_H_
#define _NEO4JCPP_GRAPHDATABASE_H_
#include <stdint.h>
#include <stdlib.h>

namespace neo4jcpp {

class Node;
class Property;
class Transaction;
class Relationship;

class GraphDatabase {

public:
    // Construct a restful GraphDatabase instance using the specified url;
    explicit GraphDatabase(std::string url);

    // Connect to the GraphDatabase server, return true if GraphDatabase server
    // is alive.
    bool Connect();
    
    // Connect to the GraphDatabase server, return true if GraphDatabase server
    // is alive, NOTE: ConnectEx will do two HTTP requests.
    bool ConnectEx();

    // Starts a new transaction and associates it with the current thread.
    Transaction BeginTx();

    // Create a new node.
    Node CreateNode(void);

    // Create a new node.
    Node CreateNode(Property property);

    // Looks up a node by id, please note: Neo4j reuses its internal ids
    // when nodes and relationships are deleted, which means it's bad practice 
    // to refer to them this way, use the application generated ids instead.
    Node GetNodeByID(const std::string& id);

    // Looks up a relationship by id, please note: Neo4j reuses its internal ids
    // when nodes and relationships are deleted, which means it's bad practice 
    // to refer to them this way, use the application generated ids instead.
    Relationship GetRelationshipByID(const std::string& id);

    // Get the server root.
    inline const std::string GraphDatabaseURI() const
    { return graphdb_uri_; }
    
    const std::string GetNeo4jVersion();


    ~GraphDatabase() {}

private:
    std::string graphdb_uri_;
    std::string data_uri_;
    std::string management_uri_;

    std::string node_uri_;
    std::string reference_node_uri_;
    std::string node_index_uri_;
    std::string relationship_index_uri_;
    std::string extensions_info_uri_;
    std::string relationship_types_uri_;
    std::string batch_uri_;
    std::string cypher_uri_;
    std::string neo4j_version_;

    bool ParseServerRoot(const std::string& response,
        std::string& management_uri, std::string& data_uri);

    bool ParseServerRoot(const std::string& response,
        std::string& node_uri, std::string& reference_node_uri,
        std::string& node_index_uri, std::string& relationship_index_uri,
        std::string& extensions_info_uri, std::string& relationship_types_uri,
        std::string& batch_uri, std::string& cypher_uri,
        std::string& neo4j_version);

    bool ParseNodeURI(const std::string& response,
        std::string& node_uri);
    bool ParseNodeURI(const std::string& response,
        std::string& node_uri, std::string& node_id);

    // No copying allowed
    GraphDatabase(const GraphDatabase&);
    void operator=(const GraphDatabase&);
};

}
#endif // _NEO4JCPP_GRAPHDATABASE_H_
