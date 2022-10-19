#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <queue>
#include "HaffCode.h"
//#include "Compressor.cpp"

using namespace std;



HaffCode::HaffCode(string path, SourceType SrcType) : filePath(path)
{
    costTimes = clock();
    compressedFileInfo = NULL;
    initHaffTree();
    if (SrcType == CompressFile)
    {
        readDataFromSrc(path);
        sortDESC();
        createHaffTree();
        generateHaffCode();
        //optimizeQuery();
    }
    else if (SrcType == DecompressFile)
    {
        readFileHeader(path);
        createHaffTree();
    }
    costTimes = (double)(clock() - costTimes) / (double)CLOCKS_PER_SEC;
    //cout << costTimes << endl;
}
HaffCode::~HaffCode()
{
    delete[] haffTree;
    if (compressedFileInfo)
        delete compressedFileInfo;
}

void HaffCode::initHaffTree()
{
    haffTree = new HaffNode[TREE_LENGTH];
    for (int i = 0; i < TREE_LENGTH; i++)
    {
        haffTree[i].byte = i;
        haffTree[i].weight = 0;
        haffTree[i].parentIndex = -1;
        haffTree[i].lChildIndex = -1;
        haffTree[i].rChileIndex = -1;
        haffTree[i].codeLength = 0;
        haffTree[i].code[0] = '\0';
    }
}
void HaffCode::readDataFromSrc(string &path)
{
    fstream inFile;
    inFile.open(path, ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cout << "file open error" << endl;
        inFile.close();
        return;
    }
    char c = 0;
    inFile.read(&c, 1);
    while (!inFile.eof())
    {
        /*
        if (inFile.peek() == EOF)
            continue;
            */

        if (!haffTree[(unsigned char)c].weight)
            ++byteCounts;
        ++haffTree[(unsigned char)c].weight;
        ++fileSize;
        //cout << c;
        inFile.read(&c, 1);
    }
    cout << endl;
    //cout << byteCounts << "," << fileSize << endl;
    inFile.close();
}

void HaffCode::readFileHeader(string &path)
{
    fstream inFile;
    inFile.open(path, ios::in | ios::binary);
    if (!inFile.is_open())
    {
        cout << "open file error";
        inFile.close();
        return;
    }
    char comma;
    int suffixLen;
    char *suffix = new char[suffixLen];
    LL fileLength;
    inFile >> suffixLen;
    inFile >> comma;
    inFile.read(suffix, suffixLen);
    inFile >> comma;
    inFile >> fileLength;
    inFile >> comma;
    inFile >> byteCounts;
    inFile >> comma;

    char byte;
    LL a;
    for (int i = 0; i < byteCounts; i++)
    {
        //inFile >> byte;
        inFile.read(&byte, 1);
        //cout << (int)byte << ",";
        haffTree[i].byte = byte;
        inFile >> comma;
        inFile >> a;
        haffTree[i].weight = a;
        //cout << haffTree[i].weight << " ";
        inFile >> comma;
    }
    cout << endl;

    compressedFileInfo = new CompressedFileInfo(suffix, inFile.tellg());
    compressedFileInfo->byteCounts = byteCounts;
    compressedFileInfo->fileLength = fileLength;
    compressedFileInfo->suffixLen = suffixLen;
    inFile.close();
}

void HaffCode::printFileInfoStruct()
{
    if (!compressedFileInfo)
        return;
    cout << "suffix length: " << compressedFileInfo->suffixLen << endl
         << "suffix: " << compressedFileInfo->suffix.c_str() << endl
         << "file length: " << compressedFileInfo->fileLength << endl
         << "byte counts: " << compressedFileInfo->byteCounts << endl
         << endl;
}

void HaffCode::createHaffTree()
{

    /*
    // abandoned.
    priority_queue<HaffNode *, vector<HaffNode *>, Functor> queue;
    for (int i = 0; i < byteCounts; i++)
        queue.push(&haffTree[i]);

    */

    int min1 = 0, min2 = 1;
    int cur = byteCounts;
    for (int i = 1; i < byteCounts; i++)
    {
        min1 = cur - 1;
        for (int j = cur - 1; j >= 0; j--)
            if (haffTree[j].parentIndex == -1 && haffTree[j].weight <= haffTree[min1].weight)
                min1 = j;
        min2 = -1;
        for (int j = cur - 1; j >= 0; j--)
        {
            if (min2 == -1)
            {
                if (haffTree[j].parentIndex == -1 && j != min1)
                    min2 = j;
            }
            else if (j != min1 && haffTree[j].parentIndex == -1 && haffTree[j].weight <= haffTree[min2].weight)
                min2 = j;
        }

        haffTree[cur].weight = haffTree[min1].weight + haffTree[min2].weight;
        haffTree[cur].lChildIndex = min1;
        haffTree[cur].rChileIndex = min2;
        haffTree[min1].parentIndex = cur;
        haffTree[min2].parentIndex = cur;
        cur++;
    }
}
void HaffCode::generateHaffCode()
{
    int rootIndex = byteCounts * 2 - 2;
    haffTree[rootIndex].code[0] = '\0';
    for (int i = rootIndex - 1; i >= 0; i--)
    {
        strcpy_s(haffTree[i].code, haffTree[haffTree[i].parentIndex].code);
        strcat(haffTree[i].code, (haffTree[haffTree[i].parentIndex].rChileIndex == i ? "1" : "0"));
        haffTree[i].codeLength = strlen(haffTree[i].code);
    }
    for (int i = 0; i < byteCounts; i++)
        longestLength = max(longestLength, haffTree[i].codeLength);
}

void HaffCode::printCode()
{
    printCode(byteCounts);
}
void HaffCode::printCode(int n)
{
    cout.flush();
    cout << "index\tbyte\tweight\tlchild\trchild\tparent\tcodelength\tcode" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << i << "\t"
             << (int)haffTree[i].byte << "\t"
             << haffTree[i].weight << "\t"
             << haffTree[i].lChildIndex << "\t"
             << haffTree[i].rChileIndex << "\t"
             << haffTree[i].parentIndex << "\t"
             << haffTree[i].codeLength << "\t"
             << haffTree[i].code << "\t"
             << endl;
    }
}

void HaffCode::printCodeInfo()
{
    for (int i = 0; i < 16; i++)
        cout << "=";
    cout << "HAFFCODE INFO";
    for (int i = 0; i < 16; i++)
        cout << "=";
    cout << endl;
    cout << "file path: " << filePath.c_str() << endl;
    cout << "file size: ";
    if (fileSize >= 1024 * 1024)
        cout << (double)fileSize / (1024.0 * 1024.0) << " m" << endl;
    else if (fileSize < 1024 * 1024 && fileSize > 1024)
        cout << (double)fileSize / 1024.0 << " kb" << endl;
    else
        cout << fileSize << " bytes" << endl;

    cout << "code counts: " << byteCounts << endl
         << "longest code length: " << longestLength << endl
         << "cost time: " << costTimes << " s" << endl
         << endl;
}

const char *HaffCode::getCode(unsigned char ch)
{
    if (optimization)
    {
        int l = 0, r = byteCounts - 1;
        int mid;
        while (l <= r)
        {
            mid = (l + r) / 2;
            if (haffTree[mid].byte == ch)
                return haffTree[mid].code;
            else if (haffTree[mid].byte > ch)
                r = mid - 1;
            else if (haffTree[mid].byte < ch)
                l = mid + 1;
        }
        return NULL;
    }
    else
    {
        for (int i = 0; i < byteCounts; i++)
        {
            if (haffTree[i].byte == ch)
            {
                return haffTree[i].code;
            }
        }
        cout << "getCode error" << endl;
        return NULL;
    }
    //return haffTree[ch].code;
}

void HaffCode::optimizeQuery()
{
    sort(haffTree, haffTree + byteCounts, [&](HaffNode a, HaffNode b) {
        return a.byte < b.byte;
    });
}

void HaffCode::sortDESC()
{
    sort(haffTree, haffTree + 256, [&](HaffNode a, HaffNode b) {
        return a.weight > b.weight;
    });
}

void HaffCode::enableOptimization()
{
    optimization = true;
    optimizeQuery();
}
