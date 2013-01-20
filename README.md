neo4j-cpp-driver
================
Neo4j cpp driver, under heavy development and not stable right now,
some of the GraphDatabase's API may change in future.

Example
=======

GraphDatabase
-------------
        int main(int argc, const char *argv[])
        {
            GraphDatabase graphdb("http://127.0.0.1:7474");
            graphdb.Connect();
        #if 1
            PropertyValue value("hello");
            Property p;
            p.AddProperty("hi", value);
            Node node = graphdb.CreateNode(p);
        #else
            Node node = graphdb.CreateNode();
        #endif
            std::cout << node.GetID() << std::endl;
            std::cout << node.GetSelfURI() << std::endl;
            return 0;
        }

License
=======
Copyright (c) 2012-2013 Fu Haiping haipingf AT gmail DOT com

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301 USA
