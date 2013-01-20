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
#include "AbstractProperty.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

namespace neo4jcpp {

using namespace rapidjson;

void Property::AddProperty(std::string key, PropertyValue value)
{
    properties_.insert(std::pair<std::string, PropertyValue>(key, value));
}

void Property::ClearProperties()
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

std::string Property::Jsonfy()
{
    const char json[] = "{}";
    Document document;
    if (document.Parse<0>(json).HasParseError()) return "{}";

    std::map<std::string, PropertyValue>::const_iterator it;
    for(it = properties_.begin(); it != properties_.end(); it++) {
        Value json_value;
        std::string key = it->first;
        PropertyValue property_value = it->second; 
        switch(property_value.Type()) {
            case PropertyValue::NEO4J_Bool:
                json_value.SetBool(property_value.GetBool());
                break;
            case PropertyValue::NEO4J_Char:
                json_value.SetInt(property_value.GetChar());
                break;
            case PropertyValue::NEO4J_Short:
                json_value.SetInt(property_value.GetShort());
                break;
            case PropertyValue::NEO4J_Integer:
                json_value.SetInt(property_value.GetInt());
                break;
            case PropertyValue::NEO4J_Long:
                json_value.SetInt(property_value.GetLong());
                break;
            case PropertyValue::NEO4J_LongLong:
                json_value.SetInt64(property_value.GetLongLong());
                break;
            case PropertyValue::NEO4J_Float:
                json_value.SetDouble(property_value.GetFloat());
                break;
            case PropertyValue::NEO4J_Double:
                json_value.SetDouble(property_value.GetDouble());
                break;
            case PropertyValue::NEO4J_String:
                json_value.SetString(property_value.GetString().c_str());
                break;
        }
        document.AddMember(key.c_str(), json_value, document.GetAllocator());
    }

    StringBuffer sbuffer;
    Writer<StringBuffer> writer(sbuffer);
    document.Accept(writer);
    std::string output = sbuffer.GetString();
    return output;
}

}

