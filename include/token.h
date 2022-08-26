#include <string>
#ifndef __TOKEN_H__
#define __TOKEN_H__

class Token
{
public:
    Token(std::string username, std::string salt);
    std::string getToken() const {return _token;}
private:
    std::string _username;
    std::string _salt;
    std::string _token;
};

#endif