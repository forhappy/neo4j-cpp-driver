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

#include "net/Net.h"
#include "Node.h"
#include "Property.h"
#include "Relationship.h"
#include "Transaction.h"
#include "GraphDatabase.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"

namespace neo4jcpp {

using rapidjson::Document;

bool GraphDatabase::ParseServerRoot(const std::string& response,
        std::string& management_uri, std::string& data_uri)
{
    Document document;
    if (document.Parse<0>(response.c_str()).HasParseError())
		return false;
    if (document.HasMember("management")
            && document.HasMember("data")) {
        management_uri = document["management"].GetString();
        data_uri = document["data"].GetString();
        return true;
    } else return false;
}

bool GraphDatabase::ParseServerRoot(const std::string& response,
    std::string& node_uri, std::string& reference_node_uri,
    std::string& node_index_uri, std::string& relationship_index_uri,
    std::string& extensions_info_uri, std::string& relationship_types_uri,
    std::string& batch_uri, std::string& cypher_uri,
    std::string& neo4j_version)
{
    Document document;
    if (document.Parse<0>(response.c_str()).HasParseError())
		return false;
    if (document.HasMember("node")
            && document.HasMember("reference_node")
            && document.HasMember("node_index")
            && document.HasMember("relationship_index")
            && document.HasMember("extensions_info")
            && document.HasMember("relationship_types")
            && document.HasMember("batch")
            && document.HasMember("cypher")
            && document.HasMember("neo4j_version")) {
        node_uri = document["node"].GetString();
        reference_node_uri = document["reference_node"].GetString();
        node_index_uri = document["node_index"].GetString();
        relationship_index_uri = document["relationship_index"].GetString();
        extensions_info_uri = document["extensions_info"].GetString();
        relationship_types_uri = document["relationship_types"].GetString();
        batch_uri = document["batch"].GetString();
        cypher_uri = document["cypher"].GetString();
        neo4j_version = document["neo4j_version"].GetString();
        return true;
    } else return false;

}

bool GraphDatabase::ParseNodeURI(const std::string& response,
        std::string& node_uri)
{
    Document document;
    if (document.Parse<0>(response.c_str()).HasParseError())
		return false;
    if (document.HasMember("self")) {
        node_uri = document["self"].GetString();
        return true;
    } else return false;

}

bool GraphDatabase::ParseNodeURI(const std::string& response,
        std::string& node_uri, std::string& node_id)
{
    Document document;
    if (document.Parse<0>(response.c_str()).HasParseError())
		return false;
    if (document.HasMember("self")) {
        node_uri = document["self"].GetString();
        size_t last_slash_pos = node_uri.find_last_of("/");
        node_id = node_uri.substr(last_slash_pos + 1);
        return true;
    } else return false;

}

GraphDatabase::GraphDatabase(std::string url):
    graphdb_uri_(url) {}

bool GraphDatabase::Connect()
{
    bool ret = false;
    struct curl_slist *http_headers = NULL; 
    std::string header_accept = "Accept: application/json";
    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_GET, graphdb_uri_, http_headers, buffer);
    if (buffer->header_buffer()->code == 200) {
        if (ParseServerRoot(buffer->recv_buffer()->Data(),
                    management_uri_, data_uri_)) {
            ret = true;
            node_uri_ = data_uri_ + "node";
            reference_node_uri_ = "";
            node_index_uri_ = data_uri_ + "index/node";
            relationship_index_uri_ = data_uri_ + "index/relationship";
            extensions_info_uri_ = data_uri_ + "ext";
            relationship_types_uri_ = data_uri_ + "relationship/types";
            batch_uri_ = data_uri_ + "batch";
            cypher_uri_ = data_uri_ + "cypher";
            neo4j_version_ = "";
        }
    }
    delete buffer;
    curl_slist_free_all(http_headers);
    return ret;
}

bool GraphDatabase::ConnectEx()
{
    bool ret = false;
    struct curl_slist *http_headers = NULL; 
    std::string header_accept = "Accept: application/json";
    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_GET, graphdb_uri_, http_headers, buffer);
    if (buffer->header_buffer()->code == 200) {
        if (ParseServerRoot(buffer->recv_buffer()->Data(),
                    management_uri_, data_uri_)) {
            // Destroy the previous SessionBuffer.
            delete buffer;
            // Create another SessionBuffer for HTTP request.
            buffer = new SessionBuffer(0, 0);
            Net::DoRequest(HTTP_GET, data_uri_, http_headers, buffer);
            if (buffer->header_buffer()->code == 200) {
                if (ParseServerRoot(buffer->recv_buffer()->Data(),
                            node_uri_, reference_node_uri_,
                            node_index_uri_, relationship_index_uri_,
                            extensions_info_uri_, relationship_types_uri_,
                            batch_uri_, cypher_uri_, neo4j_version_)) ret = true;
            }
        }
    }
    delete buffer;
    curl_slist_free_all(http_headers);
    return ret;
}

Transaction GraphDatabase::BeginTx()
{
    return Transaction();
}

Node GraphDatabase::CreateNode(void)
{
    struct curl_slist *http_headers = NULL;
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 1024 * 64);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_POST, node_uri_, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 201) {
        std::string node_uri, node_id;
        if (ParseNodeURI(buffer->recv_buffer()->Data(),
                    node_uri, node_id)) {
            delete buffer;
            return Node(node_uri, node_id);
        }
    } else {
        delete buffer;
        return Node();
    }
}

Node GraphDatabase::CreateNode(Property property)
{
    std::string json = property.Jsonfy();
    struct curl_slist *http_headers = NULL;
    std::string header_content_type = "Content-Type: application/json";
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 1024 * 128);
    buffer->UpdateSendBuffer(json.c_str());

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    http_headers = curl_slist_append(http_headers, header_content_type.c_str());
    Net::DoRequest(HTTP_POST, node_uri_, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 201) {
        std::string node_uri, node_id;
        if (ParseNodeURI(buffer->recv_buffer()->Data(),
                    node_uri, node_id)) {
            delete buffer;
            return Node(node_uri, node_id);
        }
    } else {
        delete buffer;
        return Node();
    }

}


Node GraphDatabase::GetNodeByID(const std::string& id)
{
    struct curl_slist *http_headers = NULL;
    std::string header_accept = "Accept: application/json";
    std::string node_uri = node_uri_ + "/" + id;

    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_GET, node_uri, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 200) {
        std::string node_uri, node_id;
        if (ParseNodeURI(buffer->recv_buffer()->Data(),
                    node_uri, node_id)) {
            return Node(node_uri, node_id);
        }
    } else return Node();
}

Relationship GraphDatabase::GetRelationshipByID(const std::string& id)
{
    return Relationship();
}

const std::string GraphDatabase::GetNeo4jVersion()
{
    if (neo4j_version_.length() == 0)
        return neo4j_version_;
    bool ret = false;
    struct curl_slist *http_headers = NULL; 
    std::string header_accept = "Accept: application/json";
    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_GET, data_uri_, http_headers, buffer);
    if (buffer->header_buffer()->code == 200) {
        if (ParseServerRoot(buffer->recv_buffer()->Data(),
                    node_uri_, reference_node_uri_,
                    node_index_uri_, relationship_index_uri_,
                    extensions_info_uri_, relationship_types_uri_,
                    batch_uri_, cypher_uri_, neo4j_version_)) ret = true;
    }
    delete buffer;
    curl_slist_free_all(http_headers);
    return neo4j_version_;
}

}
