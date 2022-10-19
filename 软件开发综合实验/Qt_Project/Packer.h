#ifndef PACKER_H
#define PACKER_H

#include <iostream>
#include <QString>

using namespace std;

class Packer
{

public:
    double costTime = 0.0;

    Packer();
    ~Packer();
    void packup(QString dir,QString outFilePath);
    void write_file(QString dir, QString outFilePath);
    void split(QString dir,QString outFilePath);
    void listFiles(QString dir);
    int CopyFile(char *SourceFile,char *NewFile);

};

#endif // PACKER_H
