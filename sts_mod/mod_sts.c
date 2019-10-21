#include <stdio.h>
#include <locale.h>
#include <strings.h>
#include <curl/curl.h>
// Http Module includes
#include "apr_hash.h"
#include "ap_config.h"
#include "ap_provider.h"
#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"

static void register_hooks(apr_pool_t *pool);
static int sts_handler(request_rec *r);
static char *string_after(char *str, char *search);
static int validate_token(char *url_token_info);

typedef struct
{
    const char *sts_url_validation_service;
} sts_config;

static sts_config config;

const char *sts_set_url_validation_service(cmd_parms *cmd, void *cfg, const char *arg)
{
    config.sts_url_validation_service = arg;
    return NULL;
}

static const command_rec sts_directives[] =
    {
        AP_INIT_TAKE1("StsUrlValidation", sts_set_url_validation_service, NULL, RSRC_CONF, "The url to the validation service"),
        {NULL}};

static void register_hooks(apr_pool_t *pool)
{
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
            char *url_template = config.sts_url_validation_service;
            char *token = string_after(authorization, "Bearer ");
            int size_template = (int)strlen(url_template) * sizeof(char);
            int size_token = (int)strlen(token) * sizeof(char);
            char *url = (char *)malloc(size_template + size_token);
            sprintf(url, url_template, token);
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

module AP_MODULE_DECLARE_DATA sts_module =
    {
        STANDARD20_MODULE_STUFF,
        NULL,           // Per-directory configuration handler
        NULL,           // Merge handler for per-directory configurations
        NULL,           // Per-server configuration handler
        NULL,           // Merge handler for per-server configurations
        sts_directives, // Any directives we may have for httpd
        register_hooks  // Our hook registering function
};

static char *string_after(char *str, char *search)
{
    if ((int)strlen(str) <= (int)strlen(search))
    {
        return "";
    }

    char *retorno = (char *)malloc((int)strlen(str) * sizeof(char));

    int j = 0;

    for (int i = 0; i < (int)strlen(str); i++)
    {
        if (i < ((int)strlen(search)) && str[i] == search[i])
        {
            continue;
        }
        else
        {
            retorno[j++] = str[i];
        }
    }

    retorno[j++] = '\0';

    return retorno;
}

static int validate_token(char *url_token_info)
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