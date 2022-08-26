#include "../include/token.h"
#include "../include/unixHeader.h"

Token::Token(std::string username, std::string salt)
:_username(username),
_salt(salt)
{
    // timestamp
    time_t now = time(nullptr);
    struct tm *ptm = localtime(&now);
    char timestamp[40];
    sprintf(timestamp,"%04d%02d%02d%02d%02d%02d", 
            ptm->tm_year+1900, ptm->tm_mon+1, 
            ptm->tm_mday, ptm->tm_hour, 
            ptm->tm_min, ptm->tm_sec);
    std::string toMD5Str = timestamp + _salt;

    //fprintf(stderr,"toMD5Str = %s\n", toMD5Str.c_str());
    // MD5(timestamp+salt) --> MD5res

    unsigned char md[16];
    MD5((const unsigned char *)toMD5Str.c_str(), toMD5Str.size(), md);
    
    char frag[3] = {0};
    _token = "";
    for(int i = 0; i < 16; ++i)
    {
        sprintf(frag,"%02x", md[i]);
        _token += frag;
    }

    // MD5res+data --> token
    char date[9] = {0};
    sprintf(date, "%02d%02d%02d%02d", ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
    _token += date;
    fprintf(stderr, "token = %s\n", _token.c_str());
}