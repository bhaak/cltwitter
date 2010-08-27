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

#include "definitions.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <curl/curl.h>
#include "application_helpers.h"
#include "network_helpers.h"
#include "string_io_helpers.h"
#include "oauth_helpers.h"

int main(int argc, char *argv[]) {
  /* definitions */
  cltwitter_mode mode;
  size_t length = 0, oauth_postargs_length = 0;
  char *input, *trimmed_input,
       *signed_update_url, *oauth_postargs;
  config *cfg = NULL;
  CURL *curl;
  CURLcode res;
  long response_code;
  
  /* parse command line arguments */
  if (argc == 2) {
    mode = CLTWITTER_ARG;
    input = argv[1];
  } else {
    COMPLAIN_AND_EXIT("Error: No message\n");
  }
 
  /* remove leading/trailing whitespace from input */
  trimmed_input = trim(input); 
  length = strlen(trimmed_input);
  
  /* check message length */
  if (length == 0 || length > MAX_MESSAGE_LENGTH) {
    if (cfg) free(cfg);
    COMPLAIN_AND_EXIT("Error: Message must be 1-" S(MAX_MESSAGE_LENGTH) " characters long, yours was %lu.\n", (unsigned long)length);
  }
  
  /* send update */
  curl = curl_easy_init();
  
  if (!curl) {
    if (cfg) free(cfg);
    COMPLAIN_AND_EXIT("Error: Couldn't init connection mechanism. Tweet not sent.\n");
  }
  
  curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT_HEADER); 
    
  {
    oauth_postargs_length = strlen(trimmed_input) + 8;
    oauth_postargs = calloc(oauth_postargs_length, sizeof(char));
    if (!oauth_postargs) { curl_easy_cleanup(curl); COMPLAIN_AND_EXIT("Error: Memory allocation error.\n"); }
    
    SNPRINTF(oauth_postargs, oauth_postargs_length, "status=%s", trimmed_input);   
    signed_update_url = my_oauth_sign_url(TWITTER_UPDATE_URL, 1, &oauth_postargs, OA_HMAC, OAUTH_CONSUMER_KEY, OAUTH_CONSUMER_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET);
    if (mode == CLTWITTER_STDIN) free(input);
    
    if (!signed_update_url) { free(oauth_postargs); curl_easy_cleanup(curl); COMPLAIN_AND_EXIT("Error: Signing of OAuth request URL failed. Tweet not sent.\n"); }  

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ignore_data);
    curl_easy_setopt(curl, CURLOPT_URL, signed_update_url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, oauth_postargs);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, USERAGENT_HEADER);
  
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_cleanup(curl);
    free(signed_update_url);
    free(oauth_postargs);
    if (res != CURLE_OK)
      COMPLAIN_AND_EXIT("(Twitter) Error: %s\n", curl_easy_strerror(res));
    if (!(response_code == OK || response_code == NOT_MODIFIED))
      COMPLAIN_AND_EXIT("(Twitter) Error: %s (#%lu)\n", response_message(response_code), response_code);
  }
  
  return 0;
}
