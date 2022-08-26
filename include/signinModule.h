#ifndef __SIGNINMODULE_H__
#define __SIGNINMODULE_H__

#include "serverModule.h"

class SigninModule
:public ServerModule
{
public:
    void addModule(wfrest::HttpServer& server, Config& config) override;
};

#endif