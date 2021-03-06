/*

This file is part of cltwitter - a command-line utility for Twitter

Copyright 2008-2010 Martin Häger <martin.haeger@gmail.com>

cltwitter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

cltwitter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with cltwitter.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "network_helpers.h"

void* my_realloc(void* ptr, size_t size) {
  if (ptr)
    return realloc(ptr, size);
  else
    return malloc(size);
}

size_t ignore_data(void *ptr, size_t size, size_t nmemb, void *stream) {
  return size*nmemb;
}

size_t write_to_memory(void* ptr, size_t size, size_t nmemb, void* data) {
  size_t realsize = size*nmemb;
  
  memory* mem = (memory*)data;
  if (realsize + 1 > mem->capacity - mem->size) {
    mem->capacity = MAX(mem->capacity*2, mem->size + realsize + 1);
    mem->memory = my_realloc(mem->memory, mem->capacity);
  }

  if (mem->memory) { // my_realloc succeeded
    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0; // zero-terminate
  }
  
  return realsize;
}

char *response_message(unsigned long response_code) {
  switch (response_code) {
    case OK:
      return "Request was successful.";
    case NOT_MODIFIED:
      return "Resource has not been modified since last request (but the request was successful).";
    case BAD_REQUEST:
      return "Request was malformed. This is most likely a bug, please report it.";
    case NOT_AUTHORIZED:
      return "Unable to authorize. Make sure that the login credentials are correct.";
    case FORBIDDEN:
      return "Request was forbidden. If this problem persists, please report it as a bug.";
    case NOT_FOUND:
      return "Requested resource was not found. If this problem persists, please report it as a bug.";
    case INTERNAL_SERVER_ERROR:
      return "Something is broken on Twitter's servers. Please try again later, or report this " \
             "to the Twitter team if the problem persists.";
    case BAD_GATEWAY:
      return "Twitter is currently down or being upgraded. Please try again later.";
    case SERVICE_UNAVAILABLE:
      return "Twitter's servers are overloaded at the moment. Please try again later.";
    default:
      return "Unknown response code. Please report this as a bug (including the response code).";
  }
}
