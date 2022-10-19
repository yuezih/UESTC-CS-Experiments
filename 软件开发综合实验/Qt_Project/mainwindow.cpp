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
#include "Compressor.cpp"
#include <fstream>
#include "Packer.h"
#include "Encryption.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(450,150);
    //setFixedHeight(140);
    ui->radioButton->setChecked(true);
    ui->lineEdit_input->setClearButtonEnabled(true);
    ui->lineEdit_output->setClearButtonEnabled(true);
    statusBar()->showMessage("请先选择输入输出文件.");
    taskThread = NULL;

    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::btn_run);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::btn_input_file);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::btn_output_file);
    connect(ui->actionEncSettings,&QAction::triggered,this,&MainWindow::act_EncSettings);
//    connect(ui->actionhelp,&QAction::triggered,this,&MainWindow::act_help);
//    connect(ui->actionabout,&QAction::triggered,this,&MainWindow::act_about);
    void (MainWindow::*func)(QString);
    func = &MainWindow::sourceFilePathChange;
    connect(this,func,this,&MainWindow::generateOutputPath);

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls) {
        QString file_name = url.toLocalFile();
//        if(QFileInfo(file_name).isDir()){
//            QMessageBox::warning(this,"警告","请选择单个的输入文件，暂不支持多文件操作！");
//            return;
//        }
        ui->lineEdit_input->setText(file_name);
        emit sourceFilePathChange(file_name);
    }
}

void MainWindow::taskCompleted(double costTime){
    //setAllWidgetsStatus(true);
    ui->pushButton_3->setEnabled(true);
    statusBar()->showMessage(QString("执行完毕"));
}

void MainWindow::onUiShowMessage(QString msg){
    statusBar()->showMessage(msg);
}

void MainWindow::btn_run(){

    //setAllWidgetsStatus(false);
    //isRunning = true;
    if(taskThread){
        taskThread->quit();
        taskThread->wait();
        delete taskThread;
        taskThread = NULL;
    }

    QString inFilePath = ui->lineEdit_input->text();
    QString outFilePath = ui->lineEdit_output->text();
    QFile inFile(inFilePath);
    if(!inFile.exists()){
        QMessageBox::warning(this,"警告","输入文件不存在！");
        return;
    }
//    else if(QFileInfo(inFilePath).isDir()){
//        QMessageBox::warning(this,"警告","请选择单个的输入文件，暂不支持多文件操作！");
//        return;
//    }
    else if(outFilePath == ""){
        QMessageBox::warning(this,"警告","请先选择一个输出路径！");
        return;
    }

    bool isBackup = true;
    if(ui->radioButton->isChecked())
        isBackup = true;
    else if(ui->radioButton2->isChecked())
        isBackup = false;

    string s_infile = inFilePath.toStdString();
    if (!isBackup && s_infile.find(".backup") == string::npos){
        QMessageBox::warning(this,"警告","该文件不是备份文件");
        return;
    }
    if (isBackup && s_infile.find(".backup") != string::npos){
        QMessageBox::warning(this,"警告","该文件已经是备份文件，请勿多重备份");
        return;
    }

    statusBar()->showMessage("请稍等，" + QString(isBackup ? "备份" : "还原") + "中..");

    //setAllWidgetsStatus(false);
    ui->pushButton_3->setEnabled(false);
    taskThread = new TaskThread(this);
    connect(taskThread,(void(TaskThread::*)(double))&TaskThread::done,this,(void(MainWindow::*)(double))&MainWindow::taskCompleted);
    connect(taskThread,(void(TaskThread::*)(QString))&TaskThread::updateMsg,this,(void(MainWindow::*)(QString))&MainWindow::onUiShowMessage);
    taskThread->setTask(inFilePath,outFilePath,isBackup);
    taskThread->start();
}

void MainWindow::btn_input_file(){
    QString selectPath = QFileDialog::getOpenFileName(this,"选择输入文件",QDir::currentPath());
    ui->lineEdit_input->setText(selectPath);
    if(!selectPath.isEmpty())emit sourceFilePathChange(selectPath);
}
void MainWindow::btn_output_file(){
    QString selectFile = QFileDialog::getSaveFileName(this,"选择输出文件",QDir::currentPath());
    if(!selectFile.isEmpty())ui->lineEdit_output->setText(selectFile);
}

//void MainWindow::act_help(){
//    QMessageBox::information(this,"提示","没有帮助");
//}

//void MainWindow::act_about(){
//    QMessageBox::information(this,"没毛病","By Melonl");
//}

void MainWindow::act_EncSettings(){
    qApp->processEvents();
    encsettings.update();
    encsettings.show();
}

//void MainWindow::generateOutputPath(QString sourcePath){
//    qDebug() << "收到QString : " << sourcePath.toUtf8().data();

//    int pos1 = sourcePath.lastIndexOf('/');
//    int pos2 = sourcePath.lastIndexOf('.');
//    if(pos2 <= pos1)pos2 = -1; // path included '.'
//    int fileNameLen = (pos2 == -1 ? pos1 + sourcePath.length() : pos2-pos1-1);
//    QString outputPath = sourcePath.mid(0,pos1+1);
//    outputPath += sourcePath.mid(pos1+1,fileNameLen);
//    qDebug() << "pos1: " << pos1 << ", pos2: " << pos2;
//    if(!ui->radioButton2->isChecked()){
//        if(QFileInfo(outputPath + ".haff").isFile())
//            if (sourcePath == (outputPath + ".haff") ||QMessageBox::No ==  QMessageBox::question(this,"是否覆盖","检测到存在 " + sourcePath.mid(pos1+1,fileNameLen) + ".haff"))
//                while(QFileInfo(outputPath + ".haff").isFile()){
//                    outputPath += "_new";
//                }
//        outputPath += ".haff";
//    }
//    else{
//        QString suffix = readFileHeader(sourcePath);
//        if(!suffix.isEmpty() && !suffix.isNull()){
//            while(QFileInfo(outputPath + "." + suffix).isFile())
//                outputPath += "_new";
//            outputPath += ("." + suffix);
//        }
//        else outputPath += ".dehaff";
//    }
//    qDebug() << "生成输出路径:" << outputPath;
//    ui->lineEdit_output->setText(outputPath);
//}

void MainWindow::generateOutputPath(QString sourcePath){
//    int pos1 = sourcePath.lastIndexOf('/');
//    int pos2 = sourcePath.lastIndexOf('.');
//    if(pos2 <= pos1) pos2 = -1; // path included '.'
//    int fileNameLen = (pos2 == -1 ? pos1 + sourcePath.length() : pos2-pos1-1);
//    QString outputPath = sourcePath.mid(0,pos1+1);
//    outputPath += sourcePath.mid(pos1+1,fileNameLen);
//    qDebug() << "pos1: " << pos1 << ", pos2: " << pos2;
//    string str_outputPath = sourcePath.toStdString();

    QString outputPath = sourcePath;
    if(!ui->radioButton2->isChecked()){
        if(QFileInfo(outputPath + ".backup").isFile())
            if (sourcePath == (outputPath + ".backup"))
                while(QFileInfo(outputPath + ".backup").isFile()){
                    outputPath += "_new";
                }
        outputPath += ".backup";
    }
    else{
//        QString suffix = readFileHeader(sourcePath);
//        qDebug() << suffix;
//        if(!suffix.isEmpty() && !suffix.isNull()){
//            while(QFileInfo(outputPath + "." + suffix).isFile())
//                outputPath += "_new";
//            outputPath += ("." + suffix);
//        }
//        else outputPath += ".dehaff";
        string str_srcPath = sourcePath.toStdString();
        str_srcPath = str_srcPath.substr(0,str_srcPath.find_last_of('/'));
        outputPath = QString::fromLocal8Bit(str_srcPath);
    }
//    qDebug() << "生成输出路径:" << outputPath;
    ui->lineEdit_output->setText(outputPath);
}

void MainWindow::setAllWidgetsStatus(bool status){
    ui->pushButton->setEnabled(status);
    ui->pushButton_2->setEnabled(status);
    ui->pushButton_3->setEnabled(status);
    //ui->pushButton_3->setText(status ? "执行":"中止");
    ui->lineEdit_input->setEnabled(status);
    ui->lineEdit_output->setEnabled(status);
    ui->radioButton->setEnabled(status);
    ui->radioButton2->setEnabled(status);
}

QString MainWindow::readFileHeader(QString compressedFilePath){
    fstream inFile;
    inFile.open(compressedFilePath.toStdString(),ios::in | ios::binary);
    int suffixLen;
    char comma;
    inFile >> suffixLen;
    inFile >> comma;
    char* suf = new char[suffixLen+1];
    memset(suf,'\0',suffixLen);
    inFile.read(suf,suffixLen);
    suf[suffixLen] = '\0';
    qDebug() << suf;
    inFile.close();
    if(suffixLen < 0 || comma != ',')return "";
    return QString(QString::fromStdString(string(suf)));
}


MainWindow::~MainWindow()
{
    delete ui;
    if(taskThread){
        taskThread->quit();
        taskThread->wait();
        delete taskThread;
        taskThread = NULL;
    }
}
