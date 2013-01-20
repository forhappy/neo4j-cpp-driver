/* AbstractProperty class.
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
#ifndef _NEO4JCPP_ABSTRACTPROPERTY_H_
#define _NEO4JCPP_ABSTRACTPROPERTY_H_

#include <map>
#include <vector>
#include <string>

namespace neo4jcpp {
class PropertyValue;

class AbstractProperty {
public:
    // Associates the specified property value with
    // the specified key in this Property map.
    virtual void AddProperty(std::string key, PropertyValue value) = 0;

    // Removes all of the mappings from this property map.
    virtual void ClearProperties() = 0;

    // Returns the PropertyValue to which the specified key
    // is mapped.
    virtual const PropertyValue GetProperty(std::string key) const = 0;
    
    // Returns the PropertyValue to which the specified key
    // is mapped, default_value will be returned if no property
    // was associated with the given key.
    virtual const PropertyValue& GetProperty(std::string key,
            PropertyValue& default_value) const = 0;

    // Returns true if this property map contains a mapping
    // for the specified key.
    virtual bool HasProperty(std::string key) const = 0;

    // Remove the mapping for a key from this Property map
    // if it is present.
    virtual void RemoveProperty(std::string key) = 0;
};

}

#endif // _NEO4JCPP_ABSTRACTPROPERTY_H_

