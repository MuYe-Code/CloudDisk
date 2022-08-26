#ifndef __SERVERMODULE_H__
#define __SERVERMODULE_H__

#include <wfrest/HttpServer.h>

class Config;

class ServerModule
{
public:
    virtual void addModule(wfrest::HttpServer& server, Config& config) = 0; 
};

#endif