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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "net.h"

namespace neo4jcpp {

SessionBuffer::SessionBuffer()
{
    send_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
    if (send_buffer_ == NULL) exit(-1);
    memset(send_buffer_, 0, sizeof(SendRecvBuffer));
    send_buffer_->ptr = new char[MAX_SEND_BUFFER_SIZE];
    memset(send_buffer_->ptr, 0, MAX_SEND_BUFFER_SIZE);
    send_buffer_->left = MAX_SEND_BUFFER_SIZE;
    send_buffer_->allocated = MAX_SEND_BUFFER_SIZE;

    recv_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
    if (recv_buffer_ == NULL) exit(-1);
    memset(recv_buffer_, 0, sizeof(SendRecvBuffer));
    recv_buffer_->ptr = new char[MAX_RECV_BUFFER_SIZE];
    memset(recv_buffer_->ptr, 0, MAX_RECV_BUFFER_SIZE);
    recv_buffer_->left = MAX_RECV_BUFFER_SIZE;
    recv_buffer_->allocated = MAX_RECV_BUFFER_SIZE;

    header_buffer_ = (HeaderBuffer *) malloc(sizeof(HeaderBuffer));
    if (header_buffer_ == NULL) exit(-1);
    header_buffer_->code = -1;

}

SessionBuffer::SessionBuffer(
        unsigned int snd_buffer_size = MAX_SEND_BUFFER_SIZE,
        unsigned int rcv_buffer_size = MAX_RECV_BUFFER_SIZE)
{

    if (snd_buffer_size > 0) {
        send_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
        if (send_buffer_ == NULL) exit(-1);
        memset(send_buffer_, 0, sizeof(SendRecvBuffer));
        send_buffer_->ptr = new char[snd_buffer_size];
        memset(send_buffer_->ptr, 0, snd_buffer_size);
        send_buffer_->left = snd_buffer_size;
        send_buffer_->allocated = snd_buffer_size;
    } else {
        send_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
        if (send_buffer_ == NULL) exit(-1);
        memset(send_buffer_, 0, sizeof(SendRecvBuffer));
        send_buffer_->ptr = NULL;
        send_buffer_->left = 0;
        send_buffer_->allocated = 0;
    }

    if (rcv_buffer_size > 0) {
        recv_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
        if (recv_buffer_ == NULL) exit(-1);
        memset(recv_buffer_, 0, sizeof(SendRecvBuffer));
        recv_buffer_->ptr = new char[rcv_buffer_size];
        memset(recv_buffer_->ptr, 0, rcv_buffer_size);
        recv_buffer_->left = rcv_buffer_size;
        recv_buffer_->allocated = rcv_buffer_size;
    } else {
        recv_buffer_ = (SendRecvBuffer *) malloc(sizeof(SendRecvBuffer));
        if (recv_buffer_ == NULL) exit(-1);
        memset(recv_buffer_, 0, sizeof(SendRecvBuffer));
        recv_buffer_->ptr = NULL;
        recv_buffer_->left = 0;
        recv_buffer_->allocated = 0;
    }

    header_buffer_ = (HeaderBuffer *) malloc(sizeof(HeaderBuffer));
    if (header_buffer_ == NULL) exit(-1);
    header_buffer_->code = -1;
}

SessionBuffer::~SessionBuffer()
{
    if (send_buffer_ != NULL) {
        if (send_buffer_->ptr != NULL) {
            delete send_buffer_->ptr;
            send_buffer_->ptr = NULL;
        }
        free(send_buffer_);
        send_buffer_ = NULL;
    }

    if (recv_buffer_!= NULL) {
        if (recv_buffer_->ptr != NULL) {
            delete recv_buffer_->ptr;
            recv_buffer_->ptr = NULL;
        }
        free(recv_buffer_);
        recv_buffer_= NULL;
    }
}

unsigned int
Callback::SendOperationCallback(
        void *ptr, unsigned int size, 
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *send_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_send = size * nmemb; 

    if(bytes_per_send < 1)
        return 0;
    if(send_buffer->left > 0) {
        if (send_buffer->left > bytes_per_send) {
            memcpy((void *)ptr,
                    (const void *)(send_buffer->ptr), bytes_per_send);
            send_buffer->ptr += bytes_per_send; /* advance pointer */
            send_buffer->left -= bytes_per_send; /* less data left */
            return bytes_per_send;
        } else {
            memcpy((void *)ptr,
                    (const void *)(send_buffer->ptr), send_buffer->left);
            unsigned int last_sent_bytes = send_buffer->left;
            send_buffer->left -= bytes_per_send; /* less data left */
            return last_sent_bytes;
        }
    } else return 0;
}

unsigned int
Callback::SendOperationCallback2nd(
        void *ptr, unsigned int size, 
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *send_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_send = size * nmemb; 

    if(bytes_per_send < 1)
        return 0;
    if(static_cast<int>(send_buffer->left) > 0) {
        if (send_buffer->left > bytes_per_send) {
            memcpy((void *)ptr, (const void *)(send_buffer->ptr 
                        + send_buffer->allocated - send_buffer->left),
                    bytes_per_send);
            send_buffer->left -= bytes_per_send; /* less data left */
            return bytes_per_send;
        } else {
            memcpy((void *)ptr, (const void *)(send_buffer->ptr +
                        send_buffer->allocated - send_buffer->left),
                    send_buffer->left);
            unsigned int last_sent_bytes = send_buffer->left;
            send_buffer->left -= bytes_per_send; /* less data left */
            return last_sent_bytes;
        }
    } else return 0;
}

unsigned int
Callback::RecvOperationCallback(
        void *ptr, unsigned int size,
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *recv_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_recv = size * nmemb;
    if (recv_buffer->left > 0) {
        unsigned int offset = recv_buffer->allocated - recv_buffer->left;
        if (recv_buffer->left > bytes_per_recv) {
            strncpy(recv_buffer->ptr + offset,
                    (const char *)ptr, size * nmemb);
            recv_buffer->left -= bytes_per_recv;
            return bytes_per_recv;
        } else {
            strncpy(recv_buffer->ptr + offset,
                    (const char *)ptr, recv_buffer->left);
            unsigned int last_recv_bytes = recv_buffer->left;
            recv_buffer->left -= bytes_per_recv;
            return last_recv_bytes;
        }
    } else {
        fprintf(stderr, "%s\n", "Receive buffer overflow!");
        return 0;
    }
}

unsigned int
Callback::RecvOperationCallback2nd(
        void *ptr, unsigned int size,
        unsigned int nmemb, void *stream)
{
    unsigned int recv_size = size * nmemb;
    SendRecvBuffer *recv_buffer = (SendRecvBuffer *)stream;
    
    recv_buffer->ptr = (char *)realloc(recv_buffer->ptr,
            recv_buffer->allocated + recv_size + 1);
    if (recv_buffer->ptr == NULL) {
        /* out of memory! */ 
        exit(EXIT_FAILURE);
    }
    memcpy(&(recv_buffer->ptr[recv_buffer->allocated]), ptr, recv_size);
    recv_buffer->allocated += recv_size;
    recv_buffer->ptr[recv_buffer->allocated] = 0;
    return recv_size;
}

unsigned int
Callback::HeaderOperationCallback(
        void *ptr, unsigned int size,
        unsigned int nmemb, void *stream)
{
    HeaderBuffer *header_buffer = (HeaderBuffer *)stream;
    int rcode = 0;
    unsigned int code = 0;

    rcode = sscanf((const char *)ptr, "HTTP/1.1 %u\n", &code);
    if (rcode != 0) {
        header_buffer->code= code;
    }

    return size * nmemb;
}

void CurlOperation::DoRequest(
        const std::string method,
        const std::string resource,
        const std::string url,
        struct curl_slist *http_headers,
        void *user_data)
{
    assert(user_data != NULL);

    CURL *curl = NULL;
    SessionBuffer *params = (SessionBuffer *)user_data;

    SendRecvBuffer *send_buffer = params->send_buffer();
    SendRecvBuffer *recv_buffer = params->recv_buffer();
    HeaderBuffer *header_buffer = params->header_buffer();

    curl = curl_easy_init();
    if (curl != NULL) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, static_cast<CURLoption>(CURL_HTTP_VERSION_1_1), 1L);
        if (method == HTTP_GET) {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback::RecvOperationCallback2nd);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method == HTTP_PUT) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_INFILESIZE, send_buffer->allocated);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, Callback::SendOperationCallback2nd);
            curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback::RecvOperationCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method  == HTTP_POST) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer->ptr);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback::RecvOperationCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method == HTTP_DELETE) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callback::RecvOperationCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        }
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, Callback::HeaderOperationCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
        if (http_headers != NULL)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return;
}

}

