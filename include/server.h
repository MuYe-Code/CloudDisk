#ifndef __SERVER_H__
#define __SERVER_H__

#include "configure.h"
#include <wfrest/HttpServer.h>

class ServerModule;

class CloudDiskServer
{
public:
    CloudDiskServer();
    ~CloudDiskServer();
    void addModule(ServerModule & module);
    void start();
    void stop();
    void setAsyncTransfer(bool flag = false);
private:
    wfrest::HttpServer _server;
    Config _config;
};

#endif