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

#include "Net.h"

namespace neo4jcpp {

SendRecvBuffer::SendRecvBuffer(): ptr_(NULL), left_(0), allocated_(0)
{
}

SendRecvBuffer::SendRecvBuffer(unsigned int len): left_(len), allocated_(len)
{
    ptr_ = new char[len + 1];
    assert(ptr_ != NULL);
    memset(ptr_, 0, len + 1);
}

SendRecvBuffer::SendRecvBuffer(const char *src)
{
    if (src == NULL) {
        ptr_ = NULL;
        left_ = 0;
        allocated_ = 0;
    } else {
        size_t src_len = strlen(src);
        ptr_ = new char[src_len + 1];
        assert(ptr_ != NULL);
        memset(ptr_, 0, src_len + 1);
        memcpy(ptr_, src, src_len);
        left_ = src_len;
        allocated_ = src_len;
    }
}

SendRecvBuffer::SendRecvBuffer(const char *src, unsigned int len)
{
    if (src == NULL) {
        ptr_ = NULL;
        left_ = 0;
        allocated_ = 0;
    } else {
        ptr_ = new char[len + 1];
        assert(ptr_ != NULL);
        memset(ptr_, 0, len + 1);
        memcpy(ptr_, src, len);
        left_ = len;
        allocated_ = len;
    }
}

SendRecvBuffer::~SendRecvBuffer()
{
    if (ptr_ != NULL) {
        delete[] ptr_;
        ptr_ = NULL;
        left_ = 0;
        allocated_ = 0;
    }
}

SessionBuffer::SessionBuffer()
{
    send_buffer_ = new SendRecvBuffer(MAX_SEND_BUFFER_SIZE);
    if (send_buffer_ == NULL) exit(-1);

    recv_buffer_ = new SendRecvBuffer(MAX_RECV_BUFFER_SIZE);
    if (recv_buffer_ == NULL) exit(-1);

    header_buffer_ = new HeaderBuffer();
    if (header_buffer_ == NULL) exit(-1);
    header_buffer_->code = -1;

}

SessionBuffer::SessionBuffer(
        unsigned int snd_buffer_size = MAX_SEND_BUFFER_SIZE,
        unsigned int rcv_buffer_size = MAX_RECV_BUFFER_SIZE)
{

    if (snd_buffer_size == 0)
        send_buffer_ = new SendRecvBuffer();
    else send_buffer_ = new SendRecvBuffer(snd_buffer_size);
    if (send_buffer_ == NULL) exit(-1);

    if (rcv_buffer_size == 0)
        recv_buffer_ = new SendRecvBuffer();
    else recv_buffer_ = new SendRecvBuffer(rcv_buffer_size);
    if (recv_buffer_ == NULL) exit(-1);

    header_buffer_ = new HeaderBuffer();
    if (header_buffer_ == NULL) exit(-1);
    header_buffer_->code = -1;
}

SessionBuffer::~SessionBuffer()
{
    if (send_buffer_ != NULL) {
        if (send_buffer_->ptr_ != NULL) {
            delete[] send_buffer_->ptr_;
            send_buffer_->ptr_ = NULL;
        }
        delete send_buffer_;
        send_buffer_ = NULL;
    }

    if (recv_buffer_!= NULL) {
        if (recv_buffer_->ptr_ != NULL) {
            delete[] recv_buffer_->ptr_;
            recv_buffer_->ptr_ = NULL;
        }
        delete recv_buffer_;
        recv_buffer_= NULL;
    }

    if (header_buffer_ != NULL) {
        delete header_buffer_;
        header_buffer_ = NULL;
    }
}

void SessionBuffer::UpdateSendBuffer(const char *src)
{
    if (send_buffer_ != NULL) {
        if (send_buffer_->ptr_ != NULL) {
            delete[] send_buffer_->ptr_;
            send_buffer_->ptr_ = NULL;
        }
        delete send_buffer_;
    }
    send_buffer_ = new SendRecvBuffer(src);
}

void SessionBuffer::UpdateSendBuffer(const char *src, unsigned int len)
{
    if (send_buffer_ != NULL) {
        if (send_buffer_->ptr_ != NULL) {
            delete[] send_buffer_->ptr_;
            send_buffer_->ptr_ = NULL;
        }
        delete send_buffer_;
    }
    send_buffer_ = new SendRecvBuffer(src, len);
}

unsigned int
CurlCallback::SendOperationCallbackWithPtrForward(
        void *ptr, unsigned int size, 
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *send_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_send = size * nmemb; 

    if(bytes_per_send < 1)
        return 0;
    if(send_buffer->left_ > 0) {
        if (send_buffer->left_ > bytes_per_send) {
            memcpy((void *)ptr,
                    (const void *)(send_buffer->ptr_), bytes_per_send);
            send_buffer->ptr_ += bytes_per_send; /* advance pointer */
            send_buffer->left_ -= bytes_per_send; /* less data left_ */
            return bytes_per_send;
        } else {
            memcpy((void *)ptr,
                    (const void *)(send_buffer->ptr_), send_buffer->left_);
            unsigned int last_sent_bytes = send_buffer->left_;
            send_buffer->left_ -= bytes_per_send; /* less data left_ */
            return last_sent_bytes;
        }
    } else return 0;
}

unsigned int
CurlCallback::SendOperationCallbackWithPtrInplace(
        void *ptr, unsigned int size, 
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *send_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_send = size * nmemb; 

    if(bytes_per_send < 1)
        return 0;
    if(static_cast<int>(send_buffer->left_) > 0) {
        if (send_buffer->left_ > bytes_per_send) {
            memcpy((void *)ptr, (const void *)(send_buffer->ptr_ 
                        + send_buffer->allocated_ - send_buffer->left_),
                    bytes_per_send);
            send_buffer->left_ -= bytes_per_send; /* less data left_ */
            return bytes_per_send;
        } else {
            memcpy((void *)ptr, (const void *)(send_buffer->ptr_ +
                        send_buffer->allocated_ - send_buffer->left_),
                    send_buffer->left_);
            unsigned int last_sent_bytes = send_buffer->left_;
            send_buffer->left_ -= bytes_per_send; /* less data left_ */
            return last_sent_bytes;
        }
    } else return 0;
}

unsigned int
CurlCallback::RecvOperationCallbackPreAlloc(
        void *ptr, unsigned int size,
        unsigned int nmemb, void *stream)
{
    SendRecvBuffer *recv_buffer = (SendRecvBuffer *)stream;
    unsigned int bytes_per_recv = size * nmemb;
    if (recv_buffer->left_ > 0) {
        unsigned int offset = recv_buffer->allocated_ - recv_buffer->left_;
        if (recv_buffer->left_ > bytes_per_recv) {
            strncpy(recv_buffer->ptr_ + offset,
                    (const char *)ptr, size * nmemb);
            recv_buffer->left_ -= bytes_per_recv;
            return bytes_per_recv;
        } else {
            strncpy(recv_buffer->ptr_ + offset,
                    (const char *)ptr, recv_buffer->left_);
            unsigned int last_recv_bytes = recv_buffer->left_;
            recv_buffer->left_ -= bytes_per_recv;
            return last_recv_bytes;
        }
    } else {
        fprintf(stderr, "%s\n", "Receive buffer overflow!");
        return 0;
    }
}

unsigned int
CurlCallback::RecvOperationCallbackDynAlloc(
        void *ptr, unsigned int size,
        unsigned int nmemb, void *stream)
{
    unsigned int recv_size = size * nmemb;
    SendRecvBuffer *recv_buffer = (SendRecvBuffer *)stream;
    
    recv_buffer->ptr_ = (char *)realloc(recv_buffer->ptr_,
            recv_buffer->allocated_ + recv_size + 1);
    if (recv_buffer->ptr_ == NULL) {
        /* out of memory! */ 
        exit(EXIT_FAILURE);
    }
    memcpy(&(recv_buffer->ptr_[recv_buffer->allocated_]), ptr, recv_size);
    recv_buffer->allocated_ += recv_size;
    recv_buffer->ptr_[recv_buffer->allocated_] = 0;
    return recv_size;
}

unsigned int
CurlCallback::HeaderOperationCallback(
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

void Net::DoRequest(
        const std::string method,
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
            // NOTE: when doing GET, SessionBuffer must be initialized with
            // the following arguments:
            //
            //     SessionBuffer *buffer = new SessionBuffer(size, 0),
            //
            // that is, recv_buffer_ has to be initialized with 0 in size.
            //
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                    CurlCallback::RecvOperationCallbackDynAlloc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method == HTTP_PUT) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_INFILESIZE, send_buffer->Capacity());
            curl_easy_setopt(curl, CURLOPT_READFUNCTION,
                    CurlCallback::SendOperationCallbackWithPtrInplace);
            curl_easy_setopt(curl, CURLOPT_READDATA, send_buffer);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                    CurlCallback::RecvOperationCallbackPreAlloc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method  == HTTP_POST || method == HTTP_PUT) {
            // I added a duplicate PUT handler here for purpose.
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            if (send_buffer->ptr_ != NULL)
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send_buffer->ptr_);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                    CurlCallback::RecvOperationCallbackPreAlloc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        } else if (method == HTTP_DELETE) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                    CurlCallback::RecvOperationCallbackPreAlloc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, recv_buffer);
        }
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION,
                CurlCallback::HeaderOperationCallback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, header_buffer);
        if (http_headers != NULL)
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
        // curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return;
}

}

