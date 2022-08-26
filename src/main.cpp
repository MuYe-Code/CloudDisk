#include "../include/server.h"
#include "../include/signinModule.h"
#include "../include/signupModule.h"
#include "../include/usrInfoModule.h"
#include "../include/uploadModule.h"
#include "../include/downloadModule.h"
#include "../include/unixHeader.h"

#include <workflow/WFFacilities.h>

#include <iostream>
using std::endl;
static WFFacilities::WaitGroup waitGroup(1);

void sigHandler(int signo){
    // wake
    fprintf(stderr,"One work is done!\n");
    waitGroup.done();
}

int main()
{
    signal(SIGINT, sigHandler);
    SigninModule    signin;
    SignupModule    signup;
    UsrInfoModule   usrInfo;
    UploadModule    upload;
    DownloadModule  download;

    CloudDiskServer server;
    server.setAsyncTransfer(true);
    server.addModule(signin);
    server.addModule(signup);
    server.addModule(usrInfo);
    server.addModule(download);
    server.addModule(upload);
    server.start();
    waitGroup.wait();
    server.stop();
}