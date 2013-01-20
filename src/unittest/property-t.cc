#include <map>
#include <string>
#include <iostream>
#include "Property.h"
#include "PropertyValue.h"

using namespace neo4jcpp;

int main()
{
    PropertyValue pv1(9);
    PropertyValue pv2("forhappy");
    PropertyValue pv3(true);
    Property p;
    p.AddProperty("hello", pv1);
    p.AddProperty("hi", pv2);
    p.AddProperty("his", pv2);
    p.AddProperty("bool", pv3);
    std::cout << p.Jsonfy() << std::endl;
}
