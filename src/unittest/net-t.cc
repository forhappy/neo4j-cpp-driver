#include <string.h>

#include <iostream>
#include <string>
#include "Net.h"

using namespace std;
using namespace neo4jcpp;

int main()
{
    std::string url = "http://127.0.0.1:7474/db/data/node/421/properties/hi";
    struct curl_slist *http_headers = NULL;
    string header_content_type = "Content-Type: application/json";
    string header_accept = "Accept: application/json";
    SessionBuffer *buffer = new SessionBuffer(0, 1024 * 64);
    // SessionBuffer *buffer = new SessionBuffer(0, 0);
    char value[] = "true";
    buffer->UpdateSendBuffer(value);
    
	http_headers = curl_slist_append(http_headers, header_content_type.c_str());
	http_headers = curl_slist_append(http_headers, header_accept.c_str());

    Net::DoRequest(HTTP_PUT, url, http_headers, buffer);
    curl_slist_free_all(http_headers);

    if (buffer->header_buffer()->code == 400) {
        std::cout << buffer->recv_buffer()->Data() << std::endl;
    }
    delete buffer;
}
