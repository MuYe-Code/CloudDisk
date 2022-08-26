#include "../include/server.h"
#include "../include/serverModule.h"
#include "../include/unixHeader.h"

#include <alibabacloud/oss/OssClient.h>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace AlibabaCloud::OSS;

CloudDiskServer::CloudDiskServer()
{
    _config.currentStoreType = OSS;         //存储在OSS中
    _config.isAsyncTransferEnable = false;  //不开启异步转移
    InitializeSdk();
}

CloudDiskServer::~CloudDiskServer()
{ }

void CloudDiskServer::start()
{ 
    if(0 != _server.track().start(1234))
    { 
        perror("server start");
        exit(-1);    
    }
    
    _server.list_routes();
}

void CloudDiskServer::stop()
{ 
    _server.stop();
    ShutdownSdk();
}

void  CloudDiskServer::addModule(ServerModule & module)
{
    module.addModule(_server, _config);
}

void CloudDiskServer::setAsyncTransfer(bool flag)
{
    _config.isAsyncTransferEnable = flag;
}



