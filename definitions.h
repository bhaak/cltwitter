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

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "config.h"

#ifdef _WIN32
  #define CURL_STATICLIB
  #define PCRE_STATIC
  #define SNPRINTF _snprintf
  #define HOME "USERPROFILE"
  #define DS "\\"
#else
  #define SNPRINTF snprintf
  #define HOME "HOME"
  #define DS "/"
#endif

#define USERAGENT_HEADER PACKAGE " (" VERSION ")"
#define CONFIG_FILENAME ".cltwitter"
#define TOKEN_FILENAME ".cltwitter.token"
#define CACHE_FILENAME ".cltwitter_users.cache"
#define MAX_MESSAGE_LENGTH 140
#define MAX_USERNAME_PWD_LENGTH 100
#define MAX_TOKEN_LENGTH 400
#define TWITTER_UPDATE_URL "http://twitter.com/statuses/update.xml"
#define TWITTER_FRIENDS_URL "http://twitter.com/statuses/friends.xml"
#define TWITPIC_UPLOAD_URL "https://twitpic.com/api/upload"
#define TWITPIC_POST_URL "https://twitpic.com/api/uploadAndPost"
#define IS_GD_API "http://is.gd/api.php?longurl="
#define OAUTH_REQUEST_TOKEN_URL "http://twitter.com/oauth/request_token"
#define OAUTH_AUTHORIZE_URL "http://twitter.com/oauth/authorize"
#define OAUTH_ACCESS_TOKEN_URL "http://twitter.com/oauth/access_token"

/* from http://dev.twitter.com/  Consumer key and Consumer secret */
#define OAUTH_CONSUMER_KEY consumer_key_not_set
#define OAUTH_CONSUMER_SECRET consumer_secret_not_set

/* from http://dev.twitter.com/ "My Access Token" */
#define OAUTH_TOKEN access_token_not_set
#define OAUTH_TOKEN_SECRET access_token_secret_not_set

#define URL_REGEX "\\b(\?:(\?:https\?|ftp|file)://|www\\.|ftp\\.)(\?:\\([-A-Z0-9+&@#/%=~_|$\?!:,.]*\\)|[-A-Z0-9+&@#/%=~_|$\?!:,.])*(\?:\\([-A-Z0-9+&@#/%=~_|$\?!:,.]*\\)|[A-Z0-9+&@#/%=~_|$])"
#define UPLOAD_ONLY_FLAG "--upload-only"

#define DATA_LENGTH 3*MAX_MESSAGE_LENGTH + 8
#define USERPWD_LENGTH 2*MAX_USERNAME_PWD_LENGTH + 2
#define S(x) STRINGIFY(x)
#define STRINGIFY(x) #x
#define COMPLAIN_AND_EXIT(FORMAT, ...) { fprintf(stderr, FORMAT, ##__VA_ARGS__); exit(-1); }
#define MAX(a, b) ( ((a) > (b)) ? (a) : (b) )
#define TRUE 1
#define FALSE 0
typedef unsigned char bool;

enum http_response_code {
  OK = 200,
  NOT_MODIFIED = 304,
  BAD_REQUEST = 400,
  NOT_AUTHORIZED = 401,
  FORBIDDEN = 403,
  NOT_FOUND = 404,
  INTERNAL_SERVER_ERROR = 500,
  BAD_GATEWAY = 502,
  SERVICE_UNAVAILABLE = 503
};

enum cltwitter_modes { CLTWITTER_STDIN, CLTWITTER_ARG, CLTWITTER_TWITPIC, CLTWITTER_TWITPIC_UPLOAD_ONLY };
typedef enum cltwitter_modes cltwitter_mode;

typedef struct { 
  char username[MAX_USERNAME_PWD_LENGTH];
  char password[MAX_USERNAME_PWD_LENGTH];
} config;

typedef struct { 
  char *key;
  char *secret;
} token;

#endif /* DEFINITIONS_H */
