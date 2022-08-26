#ifndef __DOWNLOADMODULE_H__
#define __DOWNLOADMODULE_H__

#include "serverModule.h"

class DownloadModule
:public ServerModule
{
public:
    void addModule(wfrest::HttpServer& server, Config& config) override;
};

#endif