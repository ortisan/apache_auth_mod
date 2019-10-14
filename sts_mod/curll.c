#include <stdio.h>
#include <strings.h>
#include <curl/curl.h>

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