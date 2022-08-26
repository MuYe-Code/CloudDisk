#ifndef __USRINFOMODULE_H__
#define __USRINFOMODULE_H__

#include "serverModule.h"

class UsrInfoModule
:public ServerModule
{
public:
    void addModule(wfrest::HttpServer& server, Config& config) override;
};

#endif