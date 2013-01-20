/* Node class.
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
#ifndef _NEO4JCPP_NODE_H_
#define _NEO4JCPP_NODE_H_

#include "AbstractProperty.h"

namespace neo4jcpp {

class AbstractProperty;
class PropertyValue;
class Relationship;
class RelationshipType;

class Node: public AbstractProperty {
public:
    Node() {}
    Node(const std::string id, const std::string self_uri)
    { id_ = id; self_uri_ = self_uri; }

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

    // Create a relationship between this node and another node.
    Relationship CreateRelationshipTo(Node another, RelationshipType type);

    // Delete this node if it has no relationships attached to it.
    void Delete();

    // Returns the unique id of this node.
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

    ~Node() {}

private:
    std::string id_;
    std::string self_uri_;
    
    // No copying allowed
    Node(const Node&);
    void operator=(const Node&);
};

}

#endif // _NEO4JCPP_NODE_H_

