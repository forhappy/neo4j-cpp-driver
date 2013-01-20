#include <map>
#include <string>
#include <iostream>
#include "Property.h"
#include "PropertyValue.h"

using namespace neo4jcpp;

int main()
{
    PropertyValue pv1(9);
    Property p;
    p.AddProperty("hello", pv1);
    std::cout << p.HasProperty("helloo") << std::endl;
}
