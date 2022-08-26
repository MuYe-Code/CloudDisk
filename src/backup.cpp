#include "../include/backup.h"
#include "../include/configure.h"
#include <wfrest/json.hpp>
#include <alibabacloud/oss/OssClient.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using std::string;
using Json = nlohmann::json;
using namespace AlibabaCloud::OSS;

Backup::Backup(string& filepath, string& filehash, const Config& config)
{
    // 可以将文件备份到OSS中
    if(config.currentStoreType == OSS){
        OSSInfo info;
        ClientConfiguration ossConf;
        OssClient client(info.endpoint, info.accessKeyID, info.accessKeySecret, ossConf);
        std::string ossPath = "backup/" + filehash;

        if(config.isAsyncTransferEnable == false)
        { 
            //不开启异步转移 在回复响应给客户端之前，执行putobject
            //puts("isAsyncTransferEnable == false");
            client.PutObject(info.bucket, ossPath, filepath); 
        }
        
        else
        {
            //开启异步转移
            RabbitMQInfo rabbitInfo;
            AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create();
            //上传服务端作为amqp的生产者
            Json fileInfo;
            fileInfo["ossPath"] = ossPath + filehash;
            fileInfo["filePath"] = filepath;
            AmqpClient::BasicMessage::ptr_t message = AmqpClient::BasicMessage::Create(fileInfo.dump());
            channel->BasicPublish(rabbitInfo.exchange, rabbitInfo.routingKey, message);      
        }
    }
}