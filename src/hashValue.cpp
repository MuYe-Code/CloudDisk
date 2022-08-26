#include "../include/hashValue.h"
#include "../include/unixHeader.h"
#include <openssl/sha.h>

HashValue::HashValue(std::string filepath)
:_filepath(filepath)
{
    // 1 打开文件
    int fd = open(filepath.c_str(), O_RDONLY);

    // 2 计算出hash值
    // init
    SHA_CTX ctx;
    SHA1_Init(&ctx);

    // update
    char buf[4096];
    while (1)
    {
        bzero(buf,sizeof(buf));
        int ret = read(fd, buf, sizeof(buf) - 1);
        if(ret == 0) { break; }

        SHA1_Update(&ctx, buf, ret);
    }
    
    // final
    unsigned char md[20];
    SHA1_Final(md, &ctx);

    // 3  把20个8bit的整数 转换成16进制的字符串
    char frag[3] = {0};
    _sha1sum = "";
    for(int i = 0; i < 20; ++i)
    {
        sprintf(frag, "%02x", md[i]);
        _sha1sum += frag;
    }
}