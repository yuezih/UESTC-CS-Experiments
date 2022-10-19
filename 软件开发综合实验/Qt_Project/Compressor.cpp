#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "Compressor.h"
#include "HaffCode.h"
#include "QDebug"


Compressor::Compressor() {}
Compressor::~Compressor() {}

// 文件类型
string Compressor::getSuffix(string &filePath)
{
    return filePath.substr(filePath.find_last_of('.') + 1);
}
// 位置
string Compressor::getPrefix(string &filePath)
{
    return filePath.substr(filePath.find_last_of('\\') + 1, filePath.find_last_of('.') - filePath.find_last_of('\\') - 1);
}

void Compressor::flushBuffer(char *buff, ofstream &out)
{
    memset(writeBuffer, '\0', WRITE_BUFF_SIZE);
    unsigned char ch = 0;
    int byte_count = strlen(buff) / 8; //Discard decimal part

    int index;
    for (index = 0; index < byte_count; index++)
    {
        ch = 0;
        for (int j = 0; j < 8; j++)
        {
            //Optimization point
            if (buff[index * 8 + j] == '1')
            {
                //ch += pow(2, 7 - j);
                ch += (2 << (7 - j - 1));
                if (j == 7)
                    ++ch;
            }
        }
        writeBuffer[index] = ch;
    }
    writeBuffer[index] = '\0';
    out.write(writeBuffer, byte_count);
    strcpy(buff, buff + byte_count * 8); //reserve < 8 bits
}

void Compressor::compress(string srcFilePath, string outFilePath)
{
//    costTime = (double)clock();
//    std::locale::global(std::locale(""));
    fstream src;
    src.open(srcFilePath, ios::in | ios::binary);
    if (!src)
    {
        qDebug() << QString::fromLocal8Bit(srcFilePath);
        cout << "Source file not exist." << endl;
        src.close();
        return;
    }

    if (!remove(outFilePath.c_str()))
    {
        cout << "Remove old file." << endl;
    }

    HaffCode hc(srcFilePath, HaffCode::SourceType::CompressFile);
    //hc.printCode(hc.byteCounts * 2 - 1);
    //hc.printCodeInfo();

    ofstream out;
    out.open(outFilePath, ios::out | ios::binary);
    string fileSuffix = getSuffix(srcFilePath); // 获取文件类型
    char *suffix = new char[128]; 
    memset(suffix, '\0', 128);
    strncat(suffix, fileSuffix.c_str(), max(128, (int)fileSuffix.size()));
    out << (int)fileSuffix.size() << ','
        << suffix << ','
        << (LL)hc.fileSize << ','
        << (int)hc.byteCounts << ',';
    for (int i = 0; i < hc.byteCounts; i++)
    {
        out << (char)hc.haffTree[i].byte << ','
            << (LL)hc.haffTree[i].weight << ',';
    }
    hc.enableOptimization(); //enable optimization for query
    cout << endl;
    char buff[BUFF_SIZE];
    memset(&buff, '\0', BUFF_SIZE);
    char byte;
    src.read(&byte, 1);
    //LL sz = 0;
    while (!src.eof())
    {
        //++sz;
        //cout << strlen(buff) << " ";
        if (BUFF_SIZE - strlen(buff) > 256)
        {
            //cout << byte << " ";
            strcat(buff, hc.getCode((unsigned char)byte));
            src.read(&byte, 1);
        }
        else
            flushBuffer(buff, out);
    }
    //cout << sz << endl;
    flushBuffer(buff, out); //Maybe there are < 8 bits left in the buff
    if (strlen(buff) > 0)   //proc these bits
    {
        strcat(buff, "0000000"); //make the remaining bits > 8
        flushBuffer(buff, out);
        memset(buff, '\0', BUFF_SIZE);
    }

    out.close();
    src.close();

//    costTime = (double)(clock() - costTime)/(double)CLOCKS_PER_SEC;
}
void Compressor::decompress(string srcFilePath, string outFilePath)
{
    costTime = (double)clock();
    if (!remove(outFilePath.c_str()))
    {
        cout << "Remove old file." << endl;
    }

    HaffCode hc(srcFilePath, HaffCode::DecompressFile);
    CompressedFileInfo &info = *hc.compressedFileInfo;
    //hc.printFileInfoStruct();
    //hc.printCode(hc.byteCounts * 2 - 1);

    ofstream outFile;
    outFile.open(outFilePath, ios::out | ios::binary);
    if (!outFile)
    {
        cout << "output file create error." << endl;
        outFile.close();
        return;
    }

    int reFileLen = 0;
    int buffCur = 0;
    int byteCur = 0;
    int rootIndex = hc.byteCounts * 2 - 2;
    int treeCur = rootIndex;
    int writeCur = 0;
    char buff[BUFF_SIZE];
    memset(buff, '\0', BUFF_SIZE);
    memset(writeBuffer, '\0', WRITE_BUFF_SIZE);
    fstream inFile;
    inFile.open(srcFilePath, ios::in | ios::binary);
    if (!inFile)
    {
        qDebug() << QString::fromLocal8Bit(srcFilePath);
        cout << "source file not exist." << endl;
        inFile.close();
        return;
    }
    inFile.seekg(info.filePos);
    inFile.read(buff, BUFF_SIZE - 1);
    char c;
    while (reFileLen < info.fileLength)
    {
        if (writeCur >= WRITE_BUFF_SIZE - 1)
        {
            outFile.write(writeBuffer, writeCur);
            writeCur = 0;
        }

        if (hc.haffTree[treeCur].lChildIndex == -1)
        {
            c = hc.haffTree[treeCur].byte;
            writeBuffer[writeCur] = c;
            ++writeCur;
            //cout << hc.haffTree[treeCur].byte << " ";
            writeBuffer[writeCur] = '\0';
            treeCur = rootIndex;
            ++reFileLen;
        }
        else
        {
            if ((buff[buffCur] >> (7 - byteCur)) & 1 == 1)
                treeCur = hc.haffTree[treeCur].rChileIndex;
            else
                treeCur = hc.haffTree[treeCur].lChildIndex;
            if (byteCur < 7)
                ++byteCur;
            else
            {
                byteCur = 0;
                ++buffCur;
                if (buffCur >= BUFF_SIZE - 1)
                {
                    inFile.read(buff, BUFF_SIZE - 1);
                    buffCur = 0;
                }
            }
        }
    }
    outFile.write(writeBuffer, writeCur);
    inFile.close();
    outFile.close();

    costTime = (double)(clock() - costTime)/(double) CLOCKS_PER_SEC;
}

