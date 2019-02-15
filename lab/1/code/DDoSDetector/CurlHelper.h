#ifndef CURL_HELPER_H
#define CURL_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>

#define ERROR(msg) fprintf(stderr, msg); \
                   exit(EXIT_FAILURE)

class CurlHelper
{
public:
    CurlHelper();
    virtual ~CurlHelper();
    void do_post(char *url,char *data);

private:
    CURL *m_curl;
};

#endif /* CURL_HELPER_H */
