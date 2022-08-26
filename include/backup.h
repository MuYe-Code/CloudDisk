#ifndef __BACKUP_H__
#define __BACKUP_H__

#include <string>

class Config;

class Backup
{
public:
    Backup(std::string& filepath, std::string& filehash, const Config& config);
};

#endif