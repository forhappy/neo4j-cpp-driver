/* Property class 
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

#include <map>
#include <string>

#include "Property.h"
#include "PropertyValue.h"

namespace neo4jcpp {

void Property::AddProperty(std::string key, PropertyValue value)
{
    properties_.insert(std::pair<std::string, PropertyValue>(key, value));
}

void Property::Clear()
{
    properties_.clear();
}

const PropertyValue Property::GetProperty(std::string key) const
{
    if (key.length() == 0) return PropertyValue();
    std::map<std::string, PropertyValue>::const_iterator it;
    it = properties_.find(key);
    if (it != properties_.end()) return it->second;
    else return PropertyValue();
}

const PropertyValue& Property::GetProperty(std::string key,
        PropertyValue& default_value) const
{
    if (key.length() == 0) return default_value;
    std::map<std::string, PropertyValue>::const_iterator it;
    it = properties_.find(key);
    if (it != properties_.end()) return it->second;
    else return default_value;
}

bool Property::HasProperty(std::string key) const
{
    if (key.length() == 0) return false;
    if (properties_.count(key) > 0) return true;
    else return false;
}

void Property::RemoveProperty(std::string key)
{
    if (key.length() == 0) return;
    std::map<std::string, PropertyValue>::iterator it;
    it = properties_.find(key);
    if (it != properties_.end()) properties_.erase(it);
    else return;
}

}

