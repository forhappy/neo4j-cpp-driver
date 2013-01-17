/*
 * =============================================================================
 *
 *       Filename:  t.cc
 *
 *    Description:  testcase.
 *
 *        Created:  01/10/2013 11:30:47 AM
 *
 *         Author:  Fu Haiping (forhappy), haipingf@gmail.com
 *        Company:  ICT ( Institute Of Computing Technology, CAS )
 *
 * =============================================================================
 */
#include <string.h>

#include <iostream>
#include <string>
#include "Net.h"

using namespace std;
using namespace neo4jcpp;

int main()
{
    std::string url = "http://127.0.0.1:7474/db/data/node/421/properties/hello";
    struct curl_slist *http_headers = NULL;
    string header_content_type = "Content-Type: application/json";
    SessionBuffer *buffer = new SessionBuffer(0, 1024 * 1024);
    char value[] = "\"hello neo4jcpp\"";
    size_t value_len = strlen(value);
    buffer->send_buffer()->ptr = new char[value_len + 1];
    strncpy(buffer->send_buffer()->ptr, value, value_len);
    buffer->send_buffer()->left = value_len;
    buffer->send_buffer()->allocated = value_len;

	http_headers = curl_slist_append(http_headers, header_content_type.c_str());

    CurlOperation::DoRequest(HTTP_PUT, url, http_headers, buffer);

    if (buffer->header_buffer()->code == 204) {
        std::cout << buffer->recv_buffer()->ptr << std::endl;
    }
    delete buffer;
}
