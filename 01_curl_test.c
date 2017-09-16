#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <curl/curl.h>

#define POSTDATA "{\"username\":\"lalala\",\"password\":\"123123\"}"

size_t callback(char * ptr, size_t size, size_t nmemb, void * userdata)
{
    FILE * fp = (FILE*)userdata;

    int count = size * nmemb;
    int fwrite_len = 0;

    fwrite_len = fwrite(ptr, size, nmemb, fp);
    printf("读取数据的总长度：%d\n", count);
    printf("已经成功写入文件的长度: %d\n", fwrite_len);

    return fwrite_len;
}

int main()
{
    CURL * curl = NULL;
    CURLcode res;
    int ret = 0;

    FILE * fp = NULL;

    fp = fopen("./file.txt", "w");
    if(fp == NULL){
        printf("open file error");
        ret = -1;
        goto END;
    }

    //初始化句柄
    curl = curl_easy_init();
    if(curl == NULL){
        printf("curl easy init error\n");
        ret = -1;
        goto END;
    }
    
    //给句柄设置一些参数
    //给curl句柄设置一个目标地址
    res = curl_easy_setopt(curl, CURLOPT_URL, "http://101.200.170.171:8080/");
    if(res != CURLE_OK){
        printf("set easy_opt curl error res = %d \n", res);
        goto END;
    }
    
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,POSTDATA);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,callback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    //将curl发出请求，向远程服务器，慈航代码才会真正的向服务器发送请求才会
    //触发socket的connect
    //此函数是一个同步(阻塞)的请求，如果服务器没有响应，此函数会阻塞
    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
        printf("set easy_perform error res = %d\n", res);
        goto END;
    }

    //处理远程服务器返回的响应

END:
    //释放句柄                                        
    curl_easy_cleanup(curl);                        
    return 0;
}
