#include <iostream>
#include <string.h>
#include <cstring>
#include <io.h>
#include <stdio.h>
#include <fstream>
#include <Packer.h>
#include <QString>
#include <QDebug>
#include <algorithm>
#include <direct.h>

Packer::Packer(){}
Packer::~Packer(){}

string fileList[100];
int fileIndex = 0;

void Packer::listFiles(QString dir)
{

    string AbsDir = dir.toStdString();
    QString dirNew = dir;
    dirNew += "\\*.*";    // 在目录后面加上"\\*.*"进行第一次搜索
    char *chars_dirNew;
    QByteArray ba_dirNew = dirNew.toLatin1();
    chars_dirNew = ba_dirNew.data();

    intptr_t handle;
    _finddata_t findData;

    handle = _findfirst(chars_dirNew, &findData);
    if (handle == -1) return;   // 检查是否成功
    do
    {
        if (findData.attrib & _A_SUBDIR)
        {
            if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
                continue;
            //cout << findData.name << "\t<dir>\n";
            // 在目录后面加上"\\"和搜索到的目录名进行下一次搜索
            dirNew = dir;
            dirNew += "\\";
            dirNew += findData.name;
            listFiles(dirNew);
        }
        else{
            AbsDir = dir.toStdString();
            AbsDir = AbsDir + "/" + findData.name;
//            AbsDir = AbsDir.substr()
            replace(AbsDir.begin(),AbsDir.end(),'\\','/');
            fileList[fileIndex] = AbsDir;
            fileIndex ++;
            QString qAbsDir = QString::fromLocal8Bit(AbsDir);
//            qDebug() << qAbsDir << "\n";
        }
    } while (_findnext(handle, &findData) == 0);

    _findclose(handle);    // 关闭搜索句柄
}

void Packer::write_file(QString dir, QString outFilePath)
{
    listFiles(dir);
    char name[200];
    // 合成之后的文件名
    string Des_FileName = outFilePath.toStdString();
    ofstream fout(Des_FileName);
    FILE* f = fopen(Des_FileName.c_str(), "rb+");
    int cnt = fileIndex; // 文件个数
//    qDebug() << cnt;
    //保存将要合并的各文件
    int len[100];
    char* buf[100];
    FILE* fp[100];
    string Source_Array_FileName[100];
    for (int i = 0; i != cnt; ++i) {
        Source_Array_FileName[i] = fileList[i];
        fp[i] = fopen(fileList[i].c_str(), "rb+"); // 逐个打开文件
    }
    for (int i = 0; i != cnt; ++i) {
        len[i] = filelength(fileno(fp[i])); // 获取文件长度
        buf[i] = new char[len[i]]; // 新建缓冲区存储文件
        memset(buf[i], 0x0, len[i]); // 缓冲区初始化
        fread(buf[i], len[i], 1, fp[i]); // 缓冲区写入
        fclose(fp[i]);
    }

    int iCount = cnt;
    fseek(f, 0, SEEK_SET); // 把文件指针指向文件的开头
    fwrite(&iCount, sizeof(int), 1, f); // 标记
//    qDebug() << dir.length();

    for (int i = 0; i != cnt; ++i) {
        memset(name, 0x0, 200);
        string strFileName = Source_Array_FileName[i].c_str();
        strFileName = strFileName.substr(dir.length());
//        qDebug() << strFileName.c_str();
        strcpy(name, strFileName.c_str());

        fwrite(name, 200, 1, f); // 写入文件名
        fwrite(&len[i], sizeof(int), 1, f); // 写入文件长度
        fwrite(buf[i], len[i], 1, f); // 写入文件内容
    }
    fclose(f);

    for (int i = 0; i != cnt; ++i) {
        delete[] buf[i];
    }
}

void Packer::split(QString dir,QString outFilePath){
    char name[200];
    string strBlock;
    strBlock = dir.toStdString();
    FILE* F = fopen(strBlock.c_str(), "rb+");
    int iCount = 0;
    fseek(F, 0, SEEK_SET);
    fread(&iCount, sizeof(int), 1, F);

    for (int i = 0; i < iCount; i++)
    {
        memset(name, 0x0, 200);
        fread(&name, 200, 1, F);
        // 输出名称
//        cout << name << endl;
        string strName = name;
//        qDebug() << strName.c_str();

        int iLen = 0;
        // 读取文件长度
        fread(&iLen, sizeof(int), 1, F);
        char* buff = new char[iLen];
        // 读取文件内容
        fread(buff, iLen, 1, F);
        char fileLen[10];
        sprintf(fileLen, "%d", iLen);

        // 保存路径
//        string strDes = outFilePath.toStdString();
        string strDes = outFilePath.toStdString();
        strDes += strName;

//        qDebug() << strDes.c_str();
        string dirPath = strDes.c_str();
        dirPath = dirPath.substr(0,dirPath.find_last_of('/'));
//        qDebug() << QString::fromLocal8Bit(dirPath);
        if (access(dirPath.c_str(), 0) == -1){
            mkdir(dirPath.c_str());
        }
        FILE* file = fopen(strDes.c_str(), "wb+");
        fwrite(buff, iLen, 1, file);
        fclose(file);
    }

    fclose(F);
}

void Packer::packup(QString dir,QString outFilePath){
//    string dirPath = dir.toStdString();
//    if (outFilePath == ""){
//        outFilePath = QString::fromLocal8Bit(dirPath.substr(dirPath.find_last_of('/')));
//    }
    write_file(dir,outFilePath);
    qDebug() << "file packed up!";
}

int Packer::CopyFile(char *SourceFile,char *NewFile)
{
    ifstream in;
    ofstream out;
    in.open(SourceFile,ios::binary);//打开源文件
    if(in.fail())//打开源文件失败
    {
       cout<<"Error 1: Fail to open the source file."<<endl;
       in.close();
       out.close();
       return 0;
    }
    out.open(NewFile,ios::binary);//创建目标文件
    if(out.fail())//创建文件失败
    {
       cout<<"Error 2: Fail to create the new file."<<endl;
       out.close();
       in.close();
       return 0;
    }
    else//复制文件
    {
       out<<in.rdbuf();
       out.close();
       in.close();
       return 1;
    }
}
