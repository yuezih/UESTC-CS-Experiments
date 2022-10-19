#ifndef HC_H
#define HC_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <string>
#include <queue>

typedef long long LL;

using namespace std;

class Compressor;

struct HaffNode
{
    unsigned char byte;
    LL weight;
    int parentIndex;
    int lChildIndex;
    int rChileIndex;
    char code[256];
    int codeLength;
};

struct Functor // for priority_queue,now abandoned.
{
    bool operator()(HaffNode *a, HaffNode *b)
    {
        return a->weight > b->weight;
    }
};

struct CompressedFileInfo
{
    int suffixLen;
    string suffix;
    LL fileLength;
    int byteCounts;
    streampos filePos;

    CompressedFileInfo(string suffixx, streampos pos) : suffix(suffixx), filePos(pos), suffixLen(0), fileLength(0), byteCounts(0) {}
};

class HaffCode
{
    string filePath;
    HaffNode *haffTree;
    const int TREE_LENGTH = 511;
    int byteCounts = 0;
    LL fileSize = 0;
    CompressedFileInfo *compressedFileInfo;
    bool optimization = false;

    void initHaffTree();
    void readDataFromSrc(string &path);
    void readFileHeader(string &path);
    void createHaffTree();
    void generateHaffCode();
    //Optimization point
    void optimizeQuery(); //optimization for query performance
    void printCode(int n);
    void sortDESC();

    friend class Compressor;

public:
    enum SourceType
    {
        DecompressFile,
        CompressFile
    };
    double costTimes = 0.0;
    int longestLength = 0;

    HaffCode(string path, SourceType op);
    ~HaffCode();
    void printCode();
    void printCodeInfo();
    void printFileInfoStruct();
    void enableOptimization();

    const char *getCode(unsigned char ch);
};

#endif
