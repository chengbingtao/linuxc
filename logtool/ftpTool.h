#include <stdio.h>   
#include <curl/curl.h>   
#include <curl/easy.h>
#include <curl/easy.h>
#include <sys/stat.h>

struct FtpFile   //定义一个结构为了传递给my_fwrite函数.可用curl_easy_setopt的CURLOPT_WRITEDATA选项传递  
{   
	char *filename;   
	FILE *stream;   
};

int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) ;

/* parse headers for Content-Length */  
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream)  ; 

/* discard downloaded data */  
size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream)  ;

//write data to upload  
size_t writefunc(void *ptr, size_t size, size_t nmemb, void *stream)  ;

/* read data to upload */  
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream) ;



int upload(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)  ;


// 下载  
int download(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries,char* user,char* pwd) ;

int downloadsimple(char*user,char*pwd,char* ftp_path, char* local_path)   ;
