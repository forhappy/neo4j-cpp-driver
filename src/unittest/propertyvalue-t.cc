#include <map>
#include <string>
#include <iostream>
#include "PropertyValue.h"

using namespace neo4jcpp;

int main()
{
    PropertyValue value(9);
    // value.SetInt(0);
    if (value.IsValid()) {
        PropertyValue::ValueType type = value.Type();
        std::cout << "Valid value:" << std::endl;
        if (value.CheckType(PropertyValue::NEO4J_Integer))
            std::cout << value.GetInt() << std::endl;
            std::cout << type << std::endl;
    } else std::cout << "Invalid value." << std::endl;

    int v;
    if (value.GetValue(PropertyValue::NEO4J_Bool, &v) == true)
        std::cout << "v as bool: " << v << std::endl;
    if (value.GetValue(PropertyValue::NEO4J_Integer, &v) == true)
        std::cout << "v as int: " << v << std::endl;

}
