#include <stdio.h>
#include <strings.h>
#include <curl/curl.h>
// Http Module includes
#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"


/* Define prototypes of our functions in this module */
static void register_hooks(apr_pool_t *pool);
static int sts_handler(request_rec *r);

/* Define our module as an entity and assign a function for registering hooks  */

module AP_MODULE_DECLARE_DATA sts_module =
{
    STANDARD20_MODULE_STUFF,
    NULL,            // Per-directory configuration handler
    NULL,            // Merge handler for per-directory configurations
    NULL,            // Per-server configuration handler
    NULL,            // Merge handler for per-server configurations
    NULL,            // Any directives we may have for httpd
    register_hooks   // Our hook registering function
};

/* register_hooks: Adds a hook to the httpd process */
static void register_hooks(apr_pool_t *pool) 
{
    
    /* Hook the request handler */
    ap_hook_handler(sts_handler, NULL, NULL, APR_HOOK_LAST);
}

/* The handler function for our module.
 * This is where all the fun happens!
 */

static int sts_handler(request_rec *r)
{
    
    // if(!r->handler || strcmp(r->handler, "example-handler")) return(DECLINED);

   
    
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    //const apr_array_header_t    *fields;
    //int                         i;
    //apr_table_entry_t           *e = 0;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    char* authorization = apr_table_get(r->headers_in, "Authorization");

    ap_rprintf(r, "%s", authorization);

    // fields = apr_table_elts(r->headers_in);
    // e = (apr_table_entry_t *) fields->elts;
    // for(i = 0; i < fields->nelts; i++) {
    //     ap_rprintf(r, "%s: %s\n", e[i].key, e[i].val);
    //     ap_rprintf(r, "%s", "Marceloooo");
    // }

    return 0;
}

//----------------------------------------

int validateToken(char *url_token_info, char *access_token)
{
    char url_sts_validation[1000];
    sprintf(url_sts_validation, url_token_info, access_token);

    printf("%s", url_sts_validation);

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    long response_code = 500;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url_sts_validation);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    printf("Retorno: %zu", response_code);

    return (int)response_code;
}

int main()
{
    return validateToken("http://localhost:8080/token_info?token=%s", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJVU0VSX0lEIjoiMSIsInVzZXJuYW1lIjoidGVudGF0aXZhZmMifQ.NN9MXTfabh_tf9VeJbHoc7tGNd6fSf4jTfThSNkTBaY");
}

