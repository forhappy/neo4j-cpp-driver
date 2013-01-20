/* PropertyValue class 
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
#include <string>

#include "PropertyValue.h"
namespace neo4jcpp {
    
PropertyValue::PropertyValue(const PropertyValue& rhs)
{
    if (rhs.IsValid()) {
        if (rhs.CheckType(NEO4J_Bool)) {
            this->bool_value_ = rhs.GetBool();
            this->type_ = NEO4J_Bool;
        } else if (rhs.CheckType(NEO4J_Char)) {
            this->char_value_ = rhs.GetChar();
            this->type_ = NEO4J_Char; 
        } else if (rhs.CheckType(NEO4J_Short)) {
            this->short_value_ = rhs.GetShort();
            this->type_ = NEO4J_Short; 
        } else if (rhs.CheckType(NEO4J_Integer)) {
            this->int_value_ = rhs.GetInt();
            this->type_ = NEO4J_Integer; 
        } else if (rhs.CheckType(NEO4J_Long)) {
            this->long_value_ = rhs.GetLong();
            this->type_ = NEO4J_Long; 
        } else if (rhs.CheckType(NEO4J_LongLong)) {
            this->long_long_value_ = rhs.GetLongLong();
            this->type_ = NEO4J_LongLong; 
        } else if (rhs.CheckType(NEO4J_Float)) {
            this->float_value_ = rhs.GetFloat();
            this->type_ = NEO4J_Float; 
        }  else if (rhs.CheckType(NEO4J_Double)) {
            this->double_value_ = rhs.GetDouble();
            this->type_ = NEO4J_Double; 
        }  else if (rhs.CheckType(NEO4J_String)) {
            this->string_value_ = rhs.GetString();
            this->type_ = NEO4J_String; 
        }
        this->valid_ = rhs.IsValid();
    }
}

PropertyValue& PropertyValue::operator=(const PropertyValue& rhs)
{
    if (rhs.IsValid()) {
        if (rhs.CheckType(NEO4J_Bool)) {
            this->bool_value_ = rhs.GetBool();
            this->type_ = NEO4J_Bool;
        } else if (rhs.CheckType(NEO4J_Char)) {
            this->char_value_ = rhs.GetChar();
            this->type_ = NEO4J_Char; 
        } else if (rhs.CheckType(NEO4J_Short)) {
            this->short_value_ = rhs.GetShort();
            this->type_ = NEO4J_Short; 
        } else if (rhs.CheckType(NEO4J_Integer)) {
            this->int_value_ = rhs.GetInt();
            this->type_ = NEO4J_Integer; 
        } else if (rhs.CheckType(NEO4J_Long)) {
            this->long_value_ = rhs.GetLong();
            this->type_ = NEO4J_Long; 
        } else if (rhs.CheckType(NEO4J_LongLong)) {
            this->long_long_value_ = rhs.GetLongLong();
            this->type_ = NEO4J_LongLong; 
        } else if (rhs.CheckType(NEO4J_Float)) {
            this->float_value_ = rhs.GetFloat();
            this->type_ = NEO4J_Float; 
        }  else if (rhs.CheckType(NEO4J_Double)) {
            this->double_value_ = rhs.GetDouble();
            this->type_ = NEO4J_Double; 
        }  else if (rhs.CheckType(NEO4J_String)) {
            this->string_value_ = rhs.GetString();
            this->type_ = NEO4J_String; 
        }
        this->valid_ = rhs.IsValid();
    }
    return *this;
}

const bool PropertyValue::GetString(std::string& value)
{
    if (CheckType(NEO4J_String)) {
        value = string_value_;
        return true;
    } else return false;
}

const bool PropertyValue::GetValue(void *value)
{
    if (type_ != NEO4J_InvalidType) {
        switch(Type()) {
            case NEO4J_Bool:
                *(bool *)value = GetBool();
                break;
            case NEO4J_Char:
                *(char *)value = GetChar();
                break;
            case NEO4J_Short:
                *(short *)value = GetShort();
                break;
            case NEO4J_Integer:
                *(int *)value = GetInt();
                break;
            case NEO4J_Long:
                *(long *)value = GetLong();
                break;
            case NEO4J_LongLong:
                *(long long *)value = GetLongLong();
                break;
            case NEO4J_Float:
                *(float *)value = GetFloat();
                break;
            case NEO4J_Double:
                *(double *)value = GetDouble();
                break;
            case NEO4J_String:
            case NEO4J_InvalidType:
                return false;
        }
        return true;
    } else return false;
}

const bool PropertyValue::GetValue(ValueType type, void *value)
{
    if (type != NEO4J_InvalidType) {
        if (CheckType(type))
            return GetValue(value);
    }
    return false;
}

}
