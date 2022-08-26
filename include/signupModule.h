#ifndef __SIGNUPMODULE_H__
#define __SIGNUPMODULE_H__

#include "serverModule.h"

class SignupModule
:public ServerModule
{
public:
    void addModule(wfrest::HttpServer& server, Config& config) override;
};

#endif