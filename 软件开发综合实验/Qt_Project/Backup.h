#ifndef BACKUP_H
#define BACKUP_H

#include <iostream>
#include <QString>

class Backup
{
public:
    double costTime = 0.0;

    Backup();
    ~Backup();

    void fileBackup(QString src, QString outFilePath);
    void fileDeBackup(QString src, QString outFilePath);
};

#endif // BACKUP_H
