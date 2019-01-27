#include "CurlHelper.h"

/* curl callback function */
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    char *str = (char *) ptr;
    if (nmemb > 0)
    {
        printf("[CurlHelper] received message: %s\n", str);
    }
    return size * nmemb;
}

CurlHelper::CurlHelper()
{
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl init error.\n");
    }

    m_curl = curl_easy_init();
    if (m_curl == NULL)
    {
        ERROR("[CurlHelper] curl init error.\n");
    }

    if (curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, write_data) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl set WRITE_FUNCTION error.\n");
    }
}

CurlHelper::~CurlHelper()
{
    curl_easy_cleanup(m_curl);
}

void CurlHelper::do_post(char *url, char *data)
{
    if (curl_easy_setopt(m_curl, CURLOPT_URL, url) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl set URL error.\n");
    }

    if (curl_easy_setopt(m_curl, CURLOPT_POST, true) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl set POST error.\n");
    }

    if (curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, data) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl set POST_FIELDS error.\n");
    }

    if (curl_easy_perform(m_curl) != CURLE_OK)
    {
        ERROR("[CurlHelper] curl perform error.\n");
    }
    else
    {
        printf("[CurlHelper] do_post DONE successfully.\n");
    }
}
