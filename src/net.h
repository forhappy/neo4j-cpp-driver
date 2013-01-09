/* Net operations.
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

#ifndef _NEO4JCPP_NET_H_
#define _NEO4JCPP_NET_H_

#include <string>

#include <curl/curl.h>

// HTTP Method.
#define HTTP_GET     "GET"
#define HTTP_PUT     "PUT"
#define HTTP_HEAD    "HEAD"
#define HTTP_POST    "POST"
#define HTTP_DELETE  "DELETE"

#define MAX_SEND_BUFFER_SIZE (1024 * 64)
#define MAX_RECV_BUFFER_SIZE (4 * 1024 * 1024)

namespace neo4jcpp {

struct SendRecvBuffer {
    char *ptr; //< 缓冲区首指针 /
    unsigned int left; //< 缓冲区剩余大小 /
    unsigned int allocated; //< 缓冲区总大小 /
};

struct HeaderBuffer {
    unsigned short code; //< 返回码 /
};

class SessionBuffer {
public:
    SessionBuffer();
    SessionBuffer(unsigned int snd_buffer_size,
            unsigned int rcv_buffer_size);

    inline SendRecvBuffer * send_buffer() const
    { return send_buffer_; }

    inline void set_send_buffer(SendRecvBuffer *send_buffer)
    {
        if (send_buffer_ != NULL) {
            if (send_buffer_->ptr != NULL) {
                free(send_buffer_->ptr);
                send_buffer_->ptr = NULL;
            }
            delete send_buffer_;
        }
        send_buffer_ = send_buffer;
    }

    inline SendRecvBuffer * recv_buffer() const
    {return recv_buffer_;}

    inline void set_recv_buffer(SendRecvBuffer *recv_buffer)
    { recv_buffer_ = recv_buffer;}

    inline HeaderBuffer * header_buffer() const
    { return header_buffer_;}

    inline void set_header_buffer(HeaderBuffer *header_buffer)
    { header_buffer_ = header_buffer;}

    ~SessionBuffer();

private:
    SendRecvBuffer *send_buffer_; //< 发送缓冲区 /
    SendRecvBuffer *recv_buffer_; //< 接收缓冲区 /
    HeaderBuffer   *header_buffer_; //< 头部缓冲区 /
    
private:
    SessionBuffer(const SessionBuffer&);
    void operator=(const SessionBuffer&);
};

class Callback {
public:
    static unsigned int SendOperationCallback(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int SendOperationCallback2nd(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int RecvOperationCallback(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int RecvOperationCallback2nd(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int HeaderOperationCallback(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
};

class CurlOperation {
public:
    static void DoRequest(const std::string method,
            const std::string resource,
            const std::string url,
            struct curl_slist *http_headers,
            void *user_data);
    
    static void DoRequest2nd(const std::string method,
            const std::string resource,
            const std::string url,
            struct curl_slist *http_headers,
            void *user_data);
};

}

#endif // _NEO4JCPP_NET_H_
