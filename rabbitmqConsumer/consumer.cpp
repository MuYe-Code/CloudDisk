#include "../include/configure.h"
#include <SimpleAmqpClient/SimpleAmqpClient.h>
#include <alibabacloud/oss/OssClient.h>
#include <wfrest/json.hpp>

using Json = nlohmann::json;
using namespace AlibabaCloud::OSS;

int main()
{
    InitializeSdk();
    RabbitMQInfo rabbit;
    //创建一个channel对象
    AmqpClient::Channel::ptr_t channel = AmqpClient::Channel::Create();
    //创建消费者
    channel->BasicConsume(rabbit.queue, rabbit.queue);
    while(1)
    {
        AmqpClient::Envelope::ptr_t envelope;
        //消费一个消息
        bool flag = channel->BasicConsumeMessage(envelope, 10000);
        if(false == flag)
        {
            fprintf(stderr, "No data to backup\n");
        }

        else 
        {
            //fprintf(stderr, "content = %s\n", envelope->Message()->Body().c_str());
            puts("receive data");
            OSSInfo info;
            ClientConfiguration conf;
            OssClient client(info.endpoint, info.accessKeyID, info.accessKeySecret, conf);
            Json message = Json::parse(envelope->Message()->Body());
            std::string ossPath = message["ossPath"];
            std::string filePath = message["filePath"];
            client.PutObject(info.bucket, ossPath, filePath);
        }
    }
    ShutdownSdk();
}