/* Relationship class.
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

#include <stdio.h>
#include <string.h>

#include <string>

#include <curl/curl.h>

#include "Util.h"
#include "Node.h"
#include "Relationship.h"
#include "Property.h"
#include "PropertyValue.h"
#include "RelationshipType.h"
#include "net/Net.h"
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"

namespace neo4jcpp {

void Relationship::AddProperty(std::string key, PropertyValue value)
{
    if (key.length() == 0 || !value.IsValid()) return;

    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties/" + key; 
    std::string header_content_type = "Content-Type: application/json";
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 1024 * 128);
    if (value.Type() == PropertyValue::NEO4J_Bool) {
        char strbool[8] = {0};
        if (value.GetBool() == true) {
            memcpy(strbool, "true", 4);
        } else memcpy(strbool, "false", 5);
        buffer->UpdateSendBuffer(strbool);
    } else if (value.Type() == PropertyValue::NEO4J_Char) {
        char strchar[8] = {0};
        char char_value = value.GetChar();
        sprintf(strchar, "\"%c\"", char_value);
        buffer->UpdateSendBuffer(strchar);
    } else if (value.Type() == PropertyValue::NEO4J_Short
            || value.Type() == PropertyValue::NEO4J_Integer) {
        char strint[32] = {0};
        int int_value = value.GetInt();
        sprintf(strint, "%d", int_value);
        buffer->UpdateSendBuffer(strint);
    } else if (value.Type() == PropertyValue::NEO4J_Long) {
        char strlong[80] = {0};
        long long_value = value.GetLong();
        sprintf(strlong, "%ld", long_value);
        buffer->UpdateSendBuffer(strlong);
    } else if (value.Type() == PropertyValue::NEO4J_LongLong) {
        char strlonglong[96] = {0};
        long long longlong_value = value.GetLongLong();
        sprintf(strlonglong, "%lld", longlong_value);
        buffer->UpdateSendBuffer(strlonglong);
    } else if (value.Type() == PropertyValue::NEO4J_String) {
        std::string string_value = value.GetString();
        string_value = "\"" + string_value + "\"";
        buffer->UpdateSendBuffer(string_value.c_str());
    } else {
        buffer->UpdateSendBuffer("\"\"");
    }

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    http_headers = curl_slist_append(http_headers, header_content_type.c_str());
    Net::DoRequest(HTTP_PUT, property_uri, http_headers, buffer);
    if (buffer->header_buffer()->code == 204);
    // clean ups.
    delete buffer;
    curl_slist_free_all(http_headers);
}

void Relationship::ClearProperties()
{
    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties"; 
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 64);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_DELETE, property_uri, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 204);
    delete buffer;
    return;
}

void Relationship::Delete()
{
    struct curl_slist *http_headers = NULL;
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 64);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_DELETE, self_uri_, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 204);
    delete buffer;
    return;
}

const PropertyValue Relationship::GetProperty(std::string key) const
{
    if (key.length() == 0) return PropertyValue();

    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties/" + key; 
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_PUT, property_uri, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 200) {
        long long int_value = 0;
        if (buffer->recv_buffer()->Data() == "true") {
            delete buffer;
            return PropertyValue(true);
        } else if (buffer->recv_buffer()->Data() == "false") {
            delete buffer;
            return PropertyValue(false);
        } else if (Util::safe_strtoll(buffer->recv_buffer()->Data(),
                    &int_value)) {
            delete buffer;
            return PropertyValue(int_value);
        } else {
            std::string strvalue = buffer->recv_buffer()->Data();
            delete buffer;
            return PropertyValue(strvalue);
        }
    }
}
    
const PropertyValue& Relationship::GetProperty(std::string key,
            PropertyValue& default_value) const
{
    if (key.length() == 0) return default_value;

    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties/" + key; 
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_PUT, property_uri, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 200) {
        long long int_value = 0;
        if (buffer->recv_buffer()->Data() == "true") {
            default_value.SetBool(true);
        } else if (buffer->recv_buffer()->Data() == "false") {
            default_value.SetBool(false);
        } else if (Util::safe_strtoll(buffer->recv_buffer()->Data(),
                    &int_value)) {
            default_value.SetInt(int_value);
        } else {
            std::string strvalue = buffer->recv_buffer()->Data();
            default_value.SetString(strvalue);
        }
    }
    delete buffer;
    return default_value;
}

bool Relationship::HasProperty(std::string key) const
{
    bool ret = false;
    if (key.length() == 0) return false;

    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties/" + key; 
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 0);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_GET, property_uri, http_headers, buffer);
    if (buffer->header_buffer()->code == 200) {
        ret = true;
    } else if (buffer->header_buffer()->code == 404) {
        ret = false;
    }

    delete buffer;
    curl_slist_free_all(http_headers);
    return ret;
}

void Relationship::RemoveProperty(std::string key)
{
    if (key.length() == 0) return;

    struct curl_slist *http_headers = NULL;
    std::string property_uri = self_uri_ + "/properties/" + key; 
    std::string header_accept = "Accept: application/json";

    SessionBuffer *buffer = new SessionBuffer(0, 64);

    http_headers = curl_slist_append(http_headers, header_accept.c_str());
    Net::DoRequest(HTTP_DELETE, property_uri, http_headers, buffer);
    curl_slist_free_all(http_headers);
    if (buffer->header_buffer()->code == 204);
    delete buffer;
    return;
}

}
