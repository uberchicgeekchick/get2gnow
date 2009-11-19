lib_LTLIBRARIES = liboauth.la
include_HEADERS = liboauth.h 

liboauth_la_SOURCES=liboauth.c ../../config.h liboauth.xmalloc.c liboauth.xmalloc.h liboauth.oauth_http.c
liboauth_la_LIBADD=-lm -lcrypto @CURL_LIBS@
liboauth_la_LDFLAGS=-g -Wall -version-info @VERSION_INFO@
liboauth_la_CFLAGS=-Wall @LIBOAUTH_CFLAGS@

