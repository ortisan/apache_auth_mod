#include <stdio.h>
#include <strings.h>
#include <curl/curl.h>
// Http Module includes
#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

static void register_hooks(apr_pool_t *pool);
static int sts_handler(request_rec *r);
static char *string_after(char *str, char *search);
static int validate_token(char *url_token_info);

module AP_MODULE_DECLARE_DATA sts_module =
    {
        STANDARD20_MODULE_STUFF,
        NULL,          // Per-directory configuration handler
        NULL,          // Merge handler for per-directory configurations
        NULL,          // Per-server configuration handler
        NULL,          // Merge handler for per-server configurations
        NULL,          // Any directives we may have for httpd
        register_hooks // Our hook registering function
};

static void register_hooks(apr_pool_t *pool)
{
    //ap_hook_handler(sts_handler, NULL, NULL, APR_HOOK_LAST);
    ap_hook_quick_handler(sts_handler, NULL, NULL, APR_HOOK_LAST);
}

static int sts_handler(request_rec *r)
{
    char *authorization = apr_table_get(r->headers_in, "Authorization");
    if (!authorization)
    {
        ap_rputs("Sem authorization", r);
    }
    else
    {
        if ((int)strlen(authorization) > 0)

        {
            char *token = string_after(authorization, "Bearer ");
            char url[5000];
            // TODO CONFIGURATION
            sprintf(url, "http://localhost:8080/token_info?token=%s", token);
            ap_rputs(url, r);
            int status_validation = validate_token(url);
            if (status_validation == 200)
            {
                return DECLINED;
            }
            return status_validation;
        }
    }
    return OK;
}

char *string_after(char *str, char *search)
{
    if ((int)strlen(str) <= (int)strlen(search))
    {
        return "";
    }

    char *retorno = (char *)malloc((int)strlen(str) * sizeof(char));

    int j = 0;

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == search[i])
        {
            continue;
        }
        else
        {
            retorno[j++] = str[i];
        }
    }

    return retorno;
}

int validate_token(char *url_token_info)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    long response_code = 500;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url_token_info);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            return (int)401;
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_cleanup(curl);
    }

    return (int)response_code;
}