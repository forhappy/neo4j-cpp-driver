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

namespace neo4jcpp {

class Node;
class Transaction;
class Relationship;

class GraphDatabase {

public:
    // Construct a restful GraphDatabase instance using the specified url;
    explicit GraphDatabase(std::string url);

    // Construct a restful GraphDatabase instance using the specified url
    // and port;
    GraphDatabase(std::string url, std::string port);

    // Starts a new transaction and associates it with the current thread.
    Transaction BeginTx();

    // Create a new node.
    Node CreateNode(void);

    // Looks up a node by id, please note: Neo4j reuses its internal ids
    // when nodes and relationships are deleted, which means it's bad practice 
    // to refer to them this way, use the application generated ids instead.
    Node GetNodeByID(uint32_t id);

    // Looks up a relationship by id, please note: Neo4j reuses its internal ids
    // when nodes and relationships are deleted, which means it's bad practice 
    // to refer to them this way, use the application generated ids instead.
    Relationship GetRelationshipByID(uint32_t id);

    // Get the server root.
    inline const std::string GraphDatabaseURI() const
    { return graphdb_uri_; }

    // Get the server port.
    inline const std::string GraphDatabasePort() const
    { return port_; }

    ~GraphDatabase() {}

private:
    std::string graphdb_uri_;
    std::string port_;
    
    // No copying allowed
    GraphDatabase(const GraphDatabase&);
    void operator=(const GraphDatabase&);
};

}
#endif // _NEO4JCPP_GRAPHDATABASESERVICE_H_
