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

#include <stdlib.h>

#include <string>

#include <curl/curl.h>

// HTTP Method.
#define HTTP_GET     "GET"
#define HTTP_PUT     "PUT"
#define HTTP_POST    "POST"
#define HTTP_DELETE  "DELETE"

#define MAX_SEND_BUFFER_SIZE (1024 * 64)
#define MAX_RECV_BUFFER_SIZE (4 * 1024 * 1024)

namespace neo4jcpp {

struct SendRecvBuffer {
public:
    SendRecvBuffer();
    SendRecvBuffer(const char *str);
    SendRecvBuffer(unsigned int len);
    SendRecvBuffer(const char *str, unsigned int len);

    inline const char* Data(void)
    {
        if (ptr_ != NULL) return ptr_;
        else return NULL;
    }

    inline unsigned int Capacity(void) { return allocated_; }

    ~SendRecvBuffer();

private:
    char *ptr_;
    unsigned int left_;
    unsigned int allocated_;
    
    friend class SessionBuffer;
    friend class CurlCallback;
    friend class Net;
};

struct HeaderBuffer {
    unsigned short code;
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
            if (send_buffer_->ptr_ != NULL) {
                free(send_buffer_->ptr_);
                send_buffer_->ptr_ = NULL;
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

    void UpdateSendBuffer(const char *src);
    void UpdateSendBuffer(const char *src, unsigned int len);

    ~SessionBuffer();

private:
    SendRecvBuffer *send_buffer_;
    SendRecvBuffer *recv_buffer_;
    HeaderBuffer   *header_buffer_;
    
private:
    SessionBuffer(const SessionBuffer&);
    void operator=(const SessionBuffer&);
};

class CurlCallback {
public:
    static unsigned int SendOperationCallbackWithPtrForward(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int SendOperationCallbackWithPtrInplace(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int RecvOperationCallbackPreAlloc(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int RecvOperationCallbackDynAlloc(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
    
    static unsigned int HeaderOperationCallback(
            void *ptr, unsigned int size,
            unsigned int nmemb, void *stream);
};

class Net{
public:
    static void DoRequest(const std::string method,
            const std::string url,
            struct curl_slist *http_headers,
            void *user_data);
};

}

#endif // _NEO4JCPP_NET_H_
