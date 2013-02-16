/* Relationship class.
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

#ifndef _NEO4JCPP_RELATIONSHIP_H_
#define _NEO4JCPP_RELATIONSHIP_H_

#include "RelationshipType.h"

namespace neo4jcpp {

class Relationship: public AbstractProperty {
public:
    Relationship() {}
    
    explicit Relationship(const std::string self_uri)
    { self_uri_ = self_uri; valid_ = true; }

    Relationship(const std::string id, const std::string self_uri)
    { id_ = id; self_uri_ = self_uri; valid_ = true; }

    Relationship(const std::string id, const std::string self_uri,
            const Node& start_node, const Node& end_node,
            const RelationshipType& relationshi_type)
        : id_(id), self_uri_(self_uri),
        start_node_(start_node), end_node_(end_node),
        relationshi_type_(relationshi_type) {}

    // Associates the specified property value with
    // the specified key in this Property map.
    void AddProperty(std::string key, PropertyValue value);

    // Removes all of the mappings from this property map.
    void ClearProperties();

    // Returns the PropertyValue to which the specified key
    // is mapped.
    const PropertyValue GetProperty(std::string key) const;
    
    // Returns the PropertyValue to which the specified key
    // is mapped, default_value will be returned if no property
    // was associated with the given key.
    const PropertyValue& GetProperty(std::string key,
            PropertyValue& default_value) const;

    // Returns true if this property map contains a mapping
    // for the specified key.
    bool HasProperty(std::string key) const;

    // Remove the mapping for a key from this Property map
    // if it is present.
    void RemoveProperty(std::string key);
    
    // Delete this relationship if it has no relationships attached to it.
    void Delete();

    // Returns the unique id of this relationship.
    inline const std::string GetID() const
    { return id_; }

    // Get self URI.
    inline const std::string GetSelfURI() const
    { return self_uri_; }

    // Set ID.
    inline void SetID(const std::string id)
    { id_ = id; }

    // Set self URI.
    inline void SetSelfURI(const std::string self_uri)
    { self_uri_ = self_uri; }

    // Valid relationship or not
    inline bool IsValid() const { return valid_; }

    // Make this relationship valid;
    inline bool SetValid()
    { valid_ = true; }

    Node GetStartNode() const { return start_node_; }
    Node GetEndNode() const { return end_node_; }
    Node GetOtherNode(Node node) const
    {
        if (node.GetID() == start_node_.GetID())
            return start_node_;
        else
            return end_node_;
    }

    ~Relationship() {}
    
    RelationshipType GetRelationshipType() const;
    
    bool IsRelationshipType(RelationshipType type) const;


private:
    // Valid relationship or not.
    bool valid_;
    std::string id_;
    std::string self_uri_;
    Node start_node_;
    Node end_node_;
    RelationshipType relationshi_type_;
};

}
#endif // _NEO4JCPP_RELATIONSHIP_H_
