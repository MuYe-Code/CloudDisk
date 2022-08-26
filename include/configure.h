#ifndef __CONFIGURR_H__
#define __CONFIGURE_H__

#include <string>

enum StoreType { NONE, OSS };

struct Config
{
    enum StoreType currentStoreType;//存储类型
    bool isAsyncTransferEnable;//是否开启异步转移 目前还未开启
};

struct RabbitMQInfo
{
    std::string url = "amqp://guest:guest@127.0.0.1:5672";
    std::string exchange = "uploadserver.trans";
    std::string queue = "uploadserver.trans.oss";
    std::string routingKey = "oss";
};

struct OSSInfo{
    std::string bucket = "masker-test";
    std::string endpoint = "oss-cn-hangzhou.aliyuncs.com";
    std::string accessKeyID = "LTAI5t8e3rCQ368YBc2BVYaa";
    std::string accessKeySecret = "ndw9QzP8daKYFQIYQqkabOSBqyHc4d";
};

#endif