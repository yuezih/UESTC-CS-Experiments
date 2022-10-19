#ifndef CPR_H
#define CPR_H

#include <iostream>

typedef long long LL;

using namespace std;

class Compressor
{
    const int BUFF_SIZE = 4096;
    const int WRITE_BUFF_SIZE = 512;
    char writeBuffer[512]; // 4096 / 8 = 512

    string getSuffix(string &filePath);
    string getPrefix(string &filePath);
    void flushBuffer(char *buff, ofstream &out);
    void clearBuffer(char *buff, int buffSize);

public:
    double costTime = 0.0;

    Compressor();
    ~Compressor();
    void compress(string srcFilePath, string outDirPath);
    void decompress(string srcFilePath, string outFilePath);
};

#endif
