#include "taskthread.h"

TaskThread::TaskThread(QObject* parent):QThread(parent){}

void TaskThread::run(){
    Backup bck;
    if(isBackup)
        bck.fileBackup(srcPath,outPath);
    else
        bck.fileDeBackup(srcPath,outPath);
    emit done(bck.costTime);

}

void TaskThread::setTask(QString src,QString out, bool isBackup){
    srcPath = src;
    outPath = out;
    this->isBackup = isBackup;
}
