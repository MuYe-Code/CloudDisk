#include <string>
#ifndef __HASH_VALUE_H__
#define __HASH_VALUE_H__

class HashValue
{
public:
    HashValue(std::string filepath);
    std::string getSha1sum() const {return _sha1sum;}
private:
    std::string _sha1sum;
    std::string _filepath;
};

#endif