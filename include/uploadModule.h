#ifndef __UPLOADMODULE_H__
#define __UPLOADMODULE_H__

#include "serverModule.h"

class UploadModule
:public ServerModule
{
public:
    void addModule(wfrest::HttpServer& server, Config& config) override;
};

#endif