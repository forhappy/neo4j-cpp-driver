/* PropertyValue class, PropertyValue is a tiny wrapper
 * of basic types in C++, this implementation may be a little tricky
 * and can waste some extra memory.
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
#ifndef _NEO4JCPP_PROPERTYVALUE_H_
#define _NEO4JCPP_PROPERTYVALUE_H_

#include <string>

namespace neo4jcpp {

class PropertyValue {
public:
    typedef enum ValueType {
        NEO4J_InvalidType = -1,
        NEO4J_Bool, NEO4J_Char, NEO4J_Short,
        NEO4J_Integer, NEO4J_Long, NEO4J_LongLong,
        NEO4J_Float, NEO4J_Double, NEO4J_String
    } ValueType;

    PropertyValue(): valid_(false), type_(NEO4J_InvalidType) {}
    PropertyValue(bool value):
        bool_value_(value), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Bool) {}

    PropertyValue(char value):
        bool_value_(false), char_value_(value),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Char) {}

    PropertyValue(short value):
        bool_value_(false), char_value_('\0'),
        short_value_(value), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Short) {}

    PropertyValue(int value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(value),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Integer) {}

    PropertyValue(long value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(value), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Long) {}

    PropertyValue(long long value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(value),
        float_value_(0.0f), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_LongLong) {}

    PropertyValue(float value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(value), double_value_(0.0),
        string_value_(""), valid_(true),
        type_(NEO4J_Float) {}

    PropertyValue(double value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(value),
        string_value_(""), valid_(true),
        type_(NEO4J_Double) {}

    PropertyValue(std::string value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(value), valid_(true),
        type_(NEO4J_String) {}

    PropertyValue(char* value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(value), valid_(true),
        type_(NEO4J_String) {}

    PropertyValue(const char* value):
        bool_value_(false), char_value_('\0'),
        short_value_(0), int_value_(0),
        long_value_(0L), long_long_value_(0LL),
        float_value_(0.0f), double_value_(0.0),
        string_value_(value), valid_(true),
        type_(NEO4J_String) {}
    
    PropertyValue(const PropertyValue&);
    PropertyValue& operator=(const PropertyValue&);

    void SetBool(bool value) 
    {
        if (valid_ == false) valid_ = true;
        bool_value_ = value;
        type_ = NEO4J_Bool;
    }
    // Get value as bool. 
    const bool GetBool() const { return bool_value_; }

    // Set value as char. 
    void SetChar(char value)
    {
        if (valid_ == false) valid_ = true;
        char_value_ = value;
        type_ = NEO4J_Char;
    }
    // Get value as char. 
    const char GetChar() const { return char_value_; }

    // Set value as short. 
    void SetShort(short value)
    {
        if (valid_ == false) valid_ = true;
        short_value_ = value;
        type_ = NEO4J_Short;
    }
    // Get value as short. 
    const short GetShort() const { return short_value_; }

    // Set value as int.
    void SetInt(int value)
    {
        if (valid_ == false) valid_ = true;
        int_value_ = value;
        type_ = NEO4J_Integer;
    }
    // Get value as int.
    const int GetInt() const { return int_value_; }

    // Set value as long.
    void SetLong(long value)
    {
        if (valid_ == false) valid_ = true;
        long_value_ = value;
        type_ = NEO4J_Long;
    }
    // Get value as long.
    const long GetLong() const { return long_value_; }

    // Set value as long long.
    void SetLongLong(long long value)
    {
        if (valid_ == false) valid_ = true;
        long_long_value_= value;
        type_ = NEO4J_LongLong;
    }
    // Get value as long long.
    const long long GetLongLong() const { return long_long_value_; }

    // Set value as float.
    void SetFloat(float value)
    {
        if (valid_ == false) valid_ = true;
        float_value_ = value;
        type_ = NEO4J_Float;
    }
    // Get value as float.
    const float GetFloat() const { return float_value_; }

    // Set value as double.
    void SetDouble(double value)
    {
        if (valid_ == false) valid_ = true;
        double_value_ = value;
        type_ = NEO4J_Double;
    }
    // Get value as double.
    const double GetDouble() const { return double_value_; }

    // Set value as std::string.
    void SetString(std::string value)
    {
        if (valid_ == false) valid_ = true;
        string_value_ = value;
        type_ = NEO4J_String;
    }
    // Get value as std::string and returns the original std::string value,
    // No type checks are conducted if you call this routine.
    const std::string GetString() const { return string_value_; }
    // Get value as std::string, if the original ValueType of PropertyValue
    // is NEO4J_String it will return true and original std::string value will
    // be stored in the out parameter *value*, otherwise it will return false.
    // you should always call this routine if possible.
    const bool GetString(std::string& value);

    // Get value as its natural ValueType, NOTE: if the original ValueType is
    // NEO4J_String, you have to call GetString(std::string& value) instead.
    // This function always do type checking, it will return true if the original
    // ValueType is NEO4J_Bool, NEO4J_Char, NEO4J_Short, NEO4J_Integer, NEO4J_Long,
    // NEO4J_LongLong, NEO4J_Float, NEO4J_Double, and the original value will be
    // stored in where *value* pointer points to, otherwise it will return false as
    // the other function does.
    const bool GetValue(void *value);
    // Get value as ValueType type, if the original ValueType of PropertyValue
    // matchs *type*, it will return true and the original value will be stored
    // in *value*, otherwise it will return false, this function is faster than
    // the previous GetValue(void *value).
    const bool GetValue(ValueType type, void *value);

    // Check whether this PropertyValue is valid or not,
    // that is, if PropertyValue was set to any C++ basic types before,
    // it would be valid, otherwise it is an invalid PropertyValue.
    const bool IsValid() const { return valid_; }

    // Returns the type of PropertyValue.
    const ValueType Type() const { return type_; }

    // Returns true if the original ValueType matchs the *expected* types.
    const bool CheckType(ValueType expected) const
    {
        if (IsValid())
            return Type() == expected;
        else return false;
    }

private:
    // Bool value.
    bool bool_value_;
    // Char value.
    char char_value_;
    // Short value.
    short short_value_;
    // Integer value.
    int  int_value_;
    // Long value.
    long long_value_;
    // Long Long value.
    long long long_long_value_;
    // Float value.
    float float_value_;
    // Double value.
    double double_value_;
    // std::string value.
    std::string string_value_;
    // PropertyValue contains valid value, bool, char, etc.
    bool valid_;
    // Value type which indicates the PropertyValue type.
    ValueType type_;
};

}

#endif // _NEO4JCPP_PROPERTY_H_

