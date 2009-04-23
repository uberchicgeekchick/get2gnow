/* -*- Mode: C; shift-width: 8; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * {project} is:
 * 	Copyright (c) 2006-2009 Kaity G. B. <uberChick@uberChicGeekChick.Com>
 * 	Released under the terms of the RPL
 *
 * For more information or to find the latest release, visit our
 * website at: http://uberChicGeekChick.Com/?projects={project}
 *
 * Writen by an uberChick, other uberChicks please meet me & others @:
 * 	http://uberChicks.Net/
 *
 * I'm also disabled. I live with a progressive neuro-muscular disease.
 * DYT1+ Early-Onset Generalized Dystonia, a type of Generalized Dystonia.
 * 	http://Dystonia-DREAMS.Org/
 *
 *
 *
 * Unless explicitly acquired and licensed from Licensor under another
 * license, the contents of this file are subject to the Reciprocal Public
 * License ("RPL") Version 1.5, or subsequent versions as allowed by the RPL,
 * and You may not copy or use this file in either source code or executable
 * form, except in compliance with the terms and conditions of the RPL.
 *
 * All software distributed under the RPL is provided strictly on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, AND
 * LICENSOR HEREBY DISCLAIMS ALL SUCH WARRANTIES, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, QUIET ENJOYMENT, OR NON-INFRINGEMENT. See the RPL for specific
 * language governing rights and limitations under the RPL.
 *
 * The User-Visible Attribution Notice below, when provided, must appear in each
 * user-visible display as defined in Section 6.4 (d):
 * 
 * Initial art work including: design, logic, programming, and graphics are
 * Copyright (C) 2009 Kaity G. B. and released under the RPL where sapplicable.
 * All materials not covered under the terms of the RPL are all still
 * Copyright (C) 2009 Kaity G. B. and released under the terms of the
 * Creative Commons Non-Comercial, Attribution, Share-A-Like version 3.0 US license.
 * 
 * Any & all data stored by this Software created, generated and/or uploaded by any User
 * and any data gathered by the Software that connects back to the User.  All data stored
 * by this Software is Copyright (C) of the User the data is connected to.
 * Users may lisences their data under the terms of an OSI approved or Creative Commons
 * license.  Users must be allowed to select their choice of license for each piece of data
 * on an individual bases and cannot be blanketly applied to all of the Users.  The User may
 * select a default license for their data.  All of the Software's data pertaining to each
 * User must be fully accessible, exportable, and deletable to that User.
 */

/********************************************************
 *          My art, code, & programming.                *
 ********************************************************/



/********************************************************
 *        Project headers, eg #include "config.h"       *
 ********************************************************/
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <openssl/hmac.h>
#include <libxml/parser.h>

#include "config.h"
#include "oauth.h"

#include "app.h"
#include "main.h"
#include "gconf.h"
#include "network.h"
#include "keyring.h"
#include "debug.h"
#include "gtkbuilder.h"


/********************************************************
 *          Variable definitions.                       *
 ********************************************************/
#define DEBUG_DOMAIN "OAuth"
typedef enum {
	Access,
	Request,
} OAuthToken;

typedef struct{
	OAuthService	*service;
	GtkWindow	*dialog;
	GtkLinkButton	*link_button;
} OAuthAuthorization;
#define GtkBuilderUI "oauth-authorize.ui"


/********************************************************
 *          Static method & function prototypes         *
 ********************************************************/
static Account *oauth_account_new(const gchar *auth_uri);
static void oauth_account_close(Account *account);

static void oauth_init_service(OAuthService *service);
static void oauth_init_twitter(OAuthService *service);

static gboolean oauth_load_access_token(OAuthService *service);

static gchar *oauth_get_token(OAuthService *service, OAuthToken token);
static void oauth_set_token(SoupMessage *msg, OAuthService *service);
static void oauth_get_access_token(OAuthService *service);
static void oauth_set_access_token(OAuthService *service);

static gchar *oauth_generate_nonce(void);
static char *oauth_sign(OAuthService *service, RequestMethod method, gchar *request_uri, gchar *base_string);

static void oauth_prompt_response( GtkWidget *widget, gint response, OAuthAuthorization *auth);
static void oauth_prompt_destroy( GtkWidget *widget, OAuthAuthorization *auth);
static void oauth_authorization_prompt_show(OAuthService *service);

static void oauth_free(OAuthService *service);


/********************************************************
 *   'Here be Dragons'...art, beauty, fun, & magic.     *
 ********************************************************/
Account *oauth_account_new(const gchar *auth_uri){
	debug(DEBUG_DOMAIN, "Loading account: %s", auth_uri );
	Conf *conf=conf_get();
	Account *account=g_new0(Account, 1);
	account->auth_uri=g_strdup(auth_uri);
	
	gchar *prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, account->auth_uri);
	conf_get_bool(conf, prefs_auth_path, &account->auto_connect);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, account->auth_uri);
	conf_get_string(conf, prefs_auth_path, &account->username);
	g_free(prefs_auth_path);
	
#ifdef HAVE_GNOME_KEYRING
	account->password=NULL;
	gchar *keyring_key=g_strdup_printf("%s@%s", account->username, account->auth_uri );
	if(G_STR_EMPTY(account->username))
		account->password=NULL;
	else if(!(keyring_get_password(keyring_key, &account->password)))
		account->password=NULL;
	g_free(keyring_key);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, account->auth_uri);
	conf_get_string(conf, prefs_auth_path, &account->password);
	g_free(prefs_auth_path);
#endif
	
	debug( DEBUG_DOMAIN, "Loaded %s account:\n\tusername: %s; password: %s; auto_connect: %s", account->auth_uri, account->username, account->password, (account->auto_connect?"TRUE":"FALSE") );
	
	return account;
}//oauth_account_new

void oauth_account_save(OAuthService *service, const gchar *username, const gchar *password, gboolean auto_connect){
	Conf *conf=conf_get();
	gchar *prefs_auth_path=NULL;
	
	service->account->auto_connect=auto_connect;
	service->account->username=g_strdup(username);
	service->account->password=g_strdup(password);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_AUTO_CONNECT, service->account->auth_uri);
	conf_set_bool(conf, prefs_auth_path, service->account->auto_connect);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_USERNAME, service->account->auth_uri);
	conf_set_string(conf, prefs_auth_path, service->account->username);
	g_free(prefs_auth_path);

	debug( DEBUG_DOMAIN, "Saved %s account:\n\tusername: %s; password: %s; auto_connect: %s", service->account->auth_uri, service->account->username, service->account->password, (service->account->auto_connect?"TRUE":"FALSE") );

#ifdef HAVE_GNOME_KEYRING
	gchar *keyring_key=g_strdup_printf("%s@%s", service->account->username, service->account->auth_uri );
	keyring_set_password( keyring_key, service->account->password);
	g_free(keyring_key);
#else
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_PASSWORD, service->account->auth_uri);
	conf_set_string(conf, prefs_auth_path, service->account->password);
	g_free(prefs_auth_path);
#endif
	
	oauth_init_service(service);
}//account_save

void oauth_account_close(Account *account){
	debug(DEBUG_DOMAIN, "Closing %s account", account->auth_uri );
	g_free(account->auth_uri);
	g_free(account->username);
	g_free(account->password);
	g_free(account);
}//oauth_account_close

OAuthService *oauth_init(const gchar *auth_uri){
	debug(DEBUG_DOMAIN, "Loading instance of: %s service", auth_uri );
	OAuthService *service=g_new0(OAuthService, 1);
	service->connection=network_get_connection();
	service->account=oauth_account_new(auth_uri);
	service->oauth_enabled=FALSE;
	oauth_init_service(service);
	if(!(service->account->username && service->account->password && service->oauth_enabled )){
		debug(DEBUG_DOMAIN, "Returning un-setup, or non-OAuth service: %s", service->account->auth_uri );
		return service;
	}
	
	debug(DEBUG_DOMAIN, "Loading OAuth tokens and authentication for: %s", service->account->auth_uri );
	if(!(oauth_load_access_token(service)))
		oauth_get_access_token(service);
	
	return service;
}//oauth_init

static void oauth_init_service(OAuthService *service){
	debug(DEBUG_DOMAIN, "Checking OAuth support for: %s", service->account->auth_uri );
	if(g_str_equal(service->account->auth_uri, "twitter.com"))
		oauth_init_twitter(service);
}//oauth_init_service

static void oauth_init_twitter(OAuthService *service){
	debug(DEBUG_DOMAIN, "Initalizing OAuth support for: %s", service->account->auth_uri );
	service->oauth_enabled=TRUE;
	service->consumer_key=g_strdup("OiO5jfOQFIHzu48IKnWfg");
	service->consumer_secret=g_strdup("KBTFIF9SMPg9jGLhvV1jeEsnO3ntWwqOUU0gIJo");
	service->authorize_uri=g_strdup("/oauth/authorize");
	service->token_access_uri=g_strdup("/oauth/access_token");
	service->token_request_uri=g_strdup("/oauth/request_token");
	debug(DEBUG_DOMAIN, "Enabled OAuth support for: %s", service->account->auth_uri );
}//oauth_init_twitter

static gboolean oauth_load_access_token(OAuthService *service){
	debug(DEBUG_DOMAIN, "Attempting to load OAuth access for: %s", service->account->auth_uri );
	Conf *gconf=conf_get();
	
	gchar *prefs_auth_path=g_strdup_printf(PREFS_AUTH_ACCESS_TOKEN, service->account->auth_uri);
	if(!( (conf_get_string(gconf, prefs_auth_path, &service->access_token)) && service->access_token )){
		g_free(prefs_auth_path);
		return FALSE;
	}
	service->token=g_strdup(service->access_token);
	g_free(prefs_auth_path);
	
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ACCESS_SECRET, service->account->auth_uri);
	conf_get_string(gconf, prefs_auth_path, &service->access_secret);
	service->access_secret=g_strdup(service->access_secret);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "Loaded OAuth Access: access_token==%s & access_secret==%s", service->access_token, service->access_secret);
	return TRUE;
}//oauth_load_access_token


static void oauth_get_access_token(OAuthService *service){
	gchar *request_uri=g_strdup_printf("https://%s%s", service->account->auth_uri, service->token_request_uri );
	debug(DEBUG_DOMAIN, "Retrieving OAuth request token from: %s", request_uri );
	SoupMessage *msg=soup_message_new("POST", request_uri);
	gchar *request_data=oauth_get_token(service, Request);
	debug(DEBUG_DOMAIN, "Sending OAuth request data: %s", request_data );
	soup_message_headers_replace(
					msg->request_headers,
					"WWW-Authenticate",
					g_strdup_printf("OAuth realm=\"https://%s/\"", service->account->auth_uri)
	);
	soup_message_set_request(
					msg,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_TAKE,
					request_data,
					strlen( request_data )
	);
	soup_session_send_message( service->connection, msg );
	debug(DEBUG_DOMAIN, "Parsing OAuth request token from: %s", request_uri );
	oauth_set_token(msg, service);
	g_free(request_uri);
	if(G_STR_EMPTY(service->token)){
		debug(DEBUG_DOMAIN, "Failed to get or parse required request token.");
		return;
	}
	
	oauth_authorization_prompt_show(service);
}//oauth_get_access_token

static void oauth_prompt_response( GtkWidget *widget, gint response, OAuthAuthorization *auth ){
	if( response != GTK_RESPONSE_OK )
		return gtk_widget_destroy(widget);
	gchar *request_uri=g_strdup_printf("https://%s%s", auth->service->account->auth_uri, auth->service->token_access_uri );
	debug(DEBUG_DOMAIN, "Retrieving OAuth access token from: %s", request_uri );
	SoupMessage *msg=soup_message_new("POST", request_uri);
	gchar *request_data=oauth_get_token(auth->service, Access);
	debug(DEBUG_DOMAIN, "Sending OAuth access data: %s", request_data );
	soup_message_headers_replace(
					msg->request_headers,
					"WWW-Authenticate",
					g_strdup_printf("OAuth realm=\"https://%s/\"", auth->service->account->auth_uri)
	);
	soup_message_set_request(
					msg,
					"application/x-www-form-urlencoded",
					SOUP_MEMORY_TAKE,
					request_data,
					strlen( request_data )
	);
	soup_session_send_message( auth->service->connection, msg );
	debug(DEBUG_DOMAIN, "Parsing OAuth access token from: %s", request_uri );
	oauth_set_token(msg, auth->service);
	g_free(request_uri);
	if(G_STR_EMPTY(auth->service->token)){
		debug(DEBUG_DOMAIN, "Failed to get or parse required access token.");
		return;
	}
	
	oauth_set_access_token(auth->service);
}//oauth_prompt_response

static void oauth_set_access_token(OAuthService *service){
	Conf *gconf=conf_get();
	service->access_token=g_strdup(service->token);
	gchar *prefs_auth_path=g_strdup_printf(PREFS_AUTH_ACCESS_TOKEN, service->account->auth_uri);
	conf_set_string(gconf, prefs_auth_path, service->access_token );
	g_free(prefs_auth_path);
	
	service->access_secret=g_strdup(service->token_secret);
	prefs_auth_path=g_strdup_printf(PREFS_AUTH_ACCESS_SECRET, service->account->auth_uri);
	conf_set_string(gconf, prefs_auth_path, service->access_secret);
	g_free(prefs_auth_path);
	
	debug(DEBUG_DOMAIN, "Saved OAuth Access: access_token==%s & access_secret==%s", service->access_token, service->access_secret );
}//oauth_get_access_token

static void oauth_prompt_destroy( GtkWidget *widget, OAuthAuthorization *auth ){
	gtk_widget_destroy(widget);
}

static void oauth_authorization_prompt_show(OAuthService *service){
	debug(DEBUG_DOMAIN, "Prompting for OAuth autorization from: https://%s%s?oauth_token=%s", service->account->auth_uri, service->authorize_uri, service->token);
	OAuthAuthorization *auth=g_new(OAuthAuthorization, 1);
	auth->service=service;

	GtkBuilder *ui=gtkbuilder_get_file(
						GtkBuilderUI,
						"oauth_authorize_dialog", &auth->dialog,
						"oauth_authorize_link_button", &auth->link_button,
						NULL
	);
	
	gtkbuilder_connect( ui, auth,
				"oauth_authorize_dialog", "destroy", oauth_prompt_destroy,
				"oauth_authorize_dialog", "response", oauth_prompt_response,
				NULL
	);

	gchar *auth_uri=g_strdup_printf("https://%s%s?oauth_token=%s", service->account->auth_uri, service->authorize_uri, service->token);
	gtk_link_button_set_uri(auth->link_button, auth_uri);
	g_free(auth_uri);
	
	gtk_window_set_transient_for(GTK_WINDOW(auth->dialog), app_get_window());
	gtk_window_set_modal(GTK_WINDOW(auth->dialog), TRUE);
	
	gtk_widget_show_all( GTK_WIDGET( auth->dialog ) );
	
}//oauth_prompt_for_authorization

static void oauth_set_token(SoupMessage *msg, OAuthService *service){
	debug(DEBUG_DOMAIN, "Parsing OAuth oauth_token & oauth_token_secret from:\n\t%s", msg->response_body->data);
	gchar **tokens=g_strsplit_set(msg->response_body->data, "?=&", -1);
	int i=0;
	for(i=0; tokens[i]; i++){
		if(g_str_equal("oauth_token", tokens[i]))
			service->token=g_strdup(tokens[++i]);
		else if(g_str_equal("oauth_token_secret", tokens[i]))
			service->token_secret=g_strdup(tokens[++i]);
	}
	g_strfreev(tokens);
}//oauth_set_token

static gchar *oauth_get_token(OAuthService *service, OAuthToken token){
	if(!(service->oauth_enabled))
		return NULL;
	switch(token){
		case Request: case Access: break;
		default: return NULL;
	}
	long unsigned int timestamp=time(NULL);
	gchar *nonce=oauth_generate_nonce();
	gchar *base_string=NULL, *request_uri=NULL, *signature=NULL, *request_data=NULL;
	switch(token){
		case Request:
			request_uri=g_strdup_printf("https://%s%s", service->account->auth_uri, service->token_request_uri );
			base_string=g_strdup_printf("oauth_consumer_key=%s&oauth_nonce=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%li&oauth_version=1.0", service->consumer_key, nonce, timestamp);
			signature=oauth_sign(service, POST, request_uri, base_string);
			request_data=g_strdup_printf("%s?oauth_consumer_key=%s&oauth_nonce=%s&outh_signature=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%li&oauth_version=1.0", request_uri, service->consumer_key, nonce, signature, timestamp);
			break;
		case Access:
			request_uri=g_strdup_printf("https://%s%s", service->account->auth_uri, service->token_access_uri );
			base_string=g_strdup_printf("oauth_consumer_key=%s&oauth_nonce=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%li&oauth_token=%s&oauth_version=1.0", service->consumer_key, nonce, timestamp, service->token);
			signature=oauth_sign(service, POST, request_uri, base_string);
			request_data=g_strdup_printf("%s?oauth_consumer_key=%s&oauth_nonce=%s&outh_signature=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%li&oauth_token=%s&oauth_version=1.0", request_uri, service->consumer_key, nonce, signature, timestamp, service->token);
			break;
	}//switch
	
	g_free(nonce);
	g_free(signature);
	g_free(base_string);
	g_free(request_uri);
	return request_data;
}//oauth_get_token


const gchar *oauth_get_auth_headers(OAuthService *service, RequestMethod method, gchar *request_uri){
	if(!(service->oauth_enabled))
		return NULL;
	long unsigned int timestamp=time(NULL);
	gchar *nonce=oauth_generate_nonce();
	gchar *base_string=g_strdup_printf("oauth_consumer_key=%s&oauth_token=%s&oauth_signature_method=HMAC-SHA1&oauth_timestamp=%li&oauth_nonce=%s,oauth_version=1.0", service->consumer_key, service->access_token, timestamp, nonce );
	gchar *signature=oauth_sign(service, method, request_uri, base_string);
	gchar *headers=g_strdup_printf("WWW-Authenticate: OAuth realm=\"https://%s/\",\n\toauth_consumer_key=\"%s\",\n\toauth_token=\"%s\",\n\toauth_signature_method=\"HMAC-SHA1\",\n\toauth_signature=\"%s\",\n\toauth_timestamp=\"%li\",\n\toauth_nonce=\"%s\",\n\toauth_version=\"1.0\"", service->account->auth_uri, service->consumer_key, service->access_token, signature, timestamp, nonce );
	g_free(nonce);
	return headers;
}//oauth_get_request_headers

static char *oauth_sign(OAuthService *service, RequestMethod method, gchar *request_uri, gchar *base_string){
	unsigned char signature[EVP_MAX_MD_SIZE];
	unsigned int signature_length=0;
	gchar *key=NULL, *message=NULL;
	
	key=g_strdup_printf("%s&%s", service->consumer_secret, (service->token_secret?service->token_secret:""));
	gchar *encoded_request_uri=url_encode(request_uri), *encoded_base_string=url_encode(base_string);
	message=g_strdup_printf("%s&%s&%s", (method==POST?"POST":"GET"), encoded_request_uri, encoded_base_string);
	g_free(encoded_request_uri);
	g_free(encoded_base_string);
	
	HMAC(
		EVP_sha1(),
		(const char *)key, strlen(key), 
		(unsigned char *)message, strlen(message),
		signature, &signature_length
	);
	
	gchar *base64_encoded_signature=g_base64_encode((const guchar *)signature, signature_length);
	gchar *encoded_signature=url_encode(base64_encoded_signature);
	g_free(base64_encoded_signature);
	debug(DEBUG_DOMAIN, "Signing OAuth request.  Signature: %s; Key: %s\n\t%s", encoded_signature, key, message);
	
	g_free(key);
	base_string=NULL;
	
	return encoded_signature;
}//oauth_sign

static gchar *oauth_generate_nonce(void){
	debug(DEBUG_DOMAIN, "Generating OAuth nonce.");
	gchar *nonce=NULL;
	static gboolean seed_random=TRUE;
	const char *chars = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ" "0123456789_";
	unsigned int max=strlen(chars);
	int i, len;
	
	if(seed_random){
		srand( time(NULL) * getpid() );
		seed_random=FALSE;
	} // seed random number generator
	
	len=15+floor(rand()*16.0/(double)RAND_MAX);
	nonce=(gchar *)xmalloc((len+1)*sizeof(gchar));
	for(i=0;i<len; i++)
		nonce[i]=chars[ rand() % max ];
	nonce[i]='\0';
	
	return nonce;
}//oauth_generate_nonce

static void oauth_free(OAuthService *service){
	if(service->authorize_uri) g_free(service->authorize_uri);
	if(service->token_access_uri) g_free(service->token_access_uri);
	if(service->token_request_uri) g_free(service->token_request_uri);
	
	if(service->consumer_secret) g_free(service->consumer_secret);
	if(service->consumer_key) g_free(service->consumer_key);
	
	if(service->token) g_free(service->token);
	if(service->token_secret) g_free(service->token_secret);
	
	if(service->access_token) g_free(service->access_token);
	if(service->access_secret) g_free(service->access_secret);
}//oauth_free

void oauth_deinit(OAuthService *service){
	if(!service) return;
	
	debug(DEBUG_DOMAIN, "Unloading instance of: %s service", service->account->auth_uri );
	service->connection=NULL;
	oauth_account_close(service->account);
	service->account=NULL;
	if(service->oauth_enabled)
		oauth_free(service);
	
	g_free(service);
}//oauth_deinit
/********************************************************
 *                       eof                            *
 ********************************************************/

