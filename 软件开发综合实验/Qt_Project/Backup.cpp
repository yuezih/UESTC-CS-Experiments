#include "Backup.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <fstream>
#include "Packer.h"
#include "Encryption.h"
#include "Compressor.h"
#include "direct.h"

Backup::Backup(){}
Backup::~Backup(){}

void Backup::fileBackup(QString src, QString outFilePath){
    string recSrc = src.toStdString();
    string recDes = outFilePath.toStdString();
    QString QrecSrc = src;
    QString QrecDes = outFilePath;
    string del;
    // recSrc = desktop/test
    // recDes = desktop/backup/test.backup
    // 打包
    recDes = recDes.substr(0, recDes.length()-7);
    recDes += ".packup";
    // recSrc = desktop/test
    // recDes = desktop/backup/test.packup
//    qDebug() << "recSrc1:" << QString::fromLocal8Bit(recSrc);
//    qDebug() << "recDes1:" << QString::fromLocal8Bit(recDes);
    char *ch_src = const_cast<char *>(recSrc.c_str());
    char *ch_des = const_cast<char *>(recDes.c_str());
    del = recDes;
    Packer p = Packer();
    if (QFileInfo(src).isDir()){
        QrecSrc = src;
        QrecDes = QString::fromLocal8Bit(recDes);
        p.write_file(QrecSrc, QrecDes);
        qDebug() << QrecSrc << "," << QrecDes;
    }
    else{
        p.CopyFile(ch_src,ch_des);
    }

    // recSrc = desktop/test
    // recDes = desktop/backup/test.packup
    // 压缩
    recSrc = recDes;
    recDes = QrecDes.toStdString();
    recDes = recDes.substr(0,recDes.length()-7);
    recDes += ".haff";
    // recSrc = desktop/backup/test.packup
    // recDes = desktop/backup/test.haff
//    qDebug() << "recSrc2:" << QString::fromLocal8Bit(recSrc);
//    qDebug() << "recDes2:" << QString::fromLocal8Bit(recDes);
    Compressor c = Compressor();
    c.compress(recSrc, recDes);
    qDebug() << QString::fromLocal8Bit(recSrc) << "," << QString::fromLocal8Bit(recDes);
    char *ch_del = const_cast<char *>(del.c_str());
    remove(ch_del);

    // 加密
    Encryption e = Encryption();
    e.handle_file(recDes);
//    qDebug() << e.getKey();
}

void Backup::fileDeBackup(QString src, QString outFilePath){
    // src = backup/1.backup
    string recSrc = src.toStdString();
    string recDes;
    QString QrecSrc = src;
    QString QrecDes;

    // 解密
    Encryption de = Encryption();
    de.handle_file(recSrc);
    recSrc = recSrc.substr(0, recSrc.length()-7) + ".haff";

    // 解压
    recDes = recSrc.substr(0, recSrc.length()-5);
    recDes += ".packup";
    Compressor dc = Compressor();
    dc.decompress(recSrc, recDes);
    remove(const_cast<char *>(recSrc.c_str()));

    // 解包

    QrecSrc = QString::fromLocal8Bit(recDes);
    QrecDes = outFilePath;
    recSrc = QrecSrc.toStdString();
    recSrc = recSrc.substr(0,recDes.length()-7);
//    qDebug() << "recSrc:" << QString::fromLocal8Bit(recSrc);
//    qDebug() << "recDes:" << QString::fromLocal8Bit(recDes);
    Packer dp = Packer();
    string subPath = recSrc.substr(recSrc.find_last_of('/'), recSrc.length());
    subPath = QrecDes.toStdString() + subPath;
    char *ch_src = const_cast<char *>(recDes.c_str());
    char *ch_des;
    if (recSrc.find('.') == recSrc.npos){
        if (access(subPath.c_str(), 0) ==  -1){
            mkdir(subPath.c_str());
//            qDebug() << QString::fromLocal8Bit(subPath) << "  1";
        }
        else {
            subPath += "_new";
            mkdir(subPath.c_str());
//            qDebug() << QString::fromLocal8Bit(subPath) << "  2";
        }
        dp.split(QrecSrc, QString::fromLocal8Bit(subPath));
    }
    else {
        if (access(subPath.c_str(), 0) != -1)
            subPath = subPath.substr(0, subPath.find_last_of('.')) + "_new" + subPath.substr(subPath.find_last_of('.'), subPath.length());
        ch_src = const_cast<char *>(recDes.c_str());
        ch_des = const_cast<char *>(subPath.c_str());
        dp.CopyFile(ch_src, ch_des);
        qDebug() <<  QString::fromLocal8Bit(recSrc);
    }
    remove(ch_src);
}
