#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include <QThread>
#include <QString>
#include "Compressor.h"
#include "Packer.h"
#include "Backup.h"

class TaskThread : public QThread
{
    Q_OBJECT

    QString srcPath;
    QString outPath;
    bool isBackup;

public:
    TaskThread(QObject* parent = 0);
    void setTask(QString src,QString out,bool isBackup);

    signals:
    void done(double costTime);
    void updateMsg(QString msg);

protected:
    void run();
};

#endif // TASKTHREAD_H
