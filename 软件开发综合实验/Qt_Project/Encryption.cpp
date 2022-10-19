#include "Encryption.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <io.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <QString>
#include <QFile>
#include <QDebug>

Encryption::Encryption(){}
Encryption::~Encryption(){}

unordered_map <string,bool> encry;
char key[1000] = { 0 };
char buf[50] = { 0 };
string key_name = "abc";

QString Encryption::getKey(){
    QString primKey = "abc";
    QFile file("D:/Desktop/CESD/BACKUP_01/key.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"Can't open the file!";
    }
    QTextStream stream(&file);
    primKey = stream.readLine();
    file.close();
    return primKey;
}

void Encryption::handle_file(string path) // .haff -> .enc   .enc -> .haff
{
    string skey = getKey().toStdString();
    char *kkey = const_cast<char *>(skey.c_str());
    string path1, path2;
    if (path.find(".backup") == string::npos){
        path1 = path;
        path2 = path1.substr(0, path1.length()-5) + ".backup";
    }
    else {
        path1 = path;
        path2 = path1.substr(0, path1.length()-7) + ".haff";
    }

    FILE* f = fopen(path.c_str(), "rb");
    FILE* fw = fopen(path2.c_str(), "wb");
    const int buffer_size = 1024;
    int read_size = 0;
    char buffer[1024*10 + 1] = { 0 };
    int i;

    memset(buffer, 0, buffer_size + 1);
    while (read_size = fread(buffer, sizeof(char), buffer_size, f)) {
        for (i = 0; i < read_size; i++) {
            buffer[i] ^=  kkey[i%strlen(kkey)];
        }
         fwrite(buffer, sizeof(char), read_size, fw);
    }

    fclose(f);
    fclose(fw);
    if (path.find(".backup") == string::npos) remove(path.c_str());
}

