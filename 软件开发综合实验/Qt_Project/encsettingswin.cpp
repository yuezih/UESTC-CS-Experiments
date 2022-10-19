#include "encsettingswin.h"
#include "ui_encsettingswin.h"
#include <QFile>


void EncSettingsWin::update(){
    ui->lineEdit_2->setText(getKey());
    ui->lineEdit->clear();
}

EncSettingsWin::EncSettingsWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncSettingsWin)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_2->setText(getKey());
    ui->lineEdit->setClearButtonEnabled(true);
    ui->lineEdit->clear();

    connect(ui->pushButton,&QPushButton::clicked,this,&EncSettingsWin::ok_run);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&EncSettingsWin::cancel_run);
}

EncSettingsWin::~EncSettingsWin()
{
    delete ui;
}

QString EncSettingsWin::getKey(){
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

void EncSettingsWin::writeKey(QString newKey){
    QFile file("D:/Desktop/CESD/BACKUP_01/key.txt");
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
         qDebug() << "open fail!";
    }
    QTextStream io(&file);
    int b;
    io>>b;
    qDebug("%d",b);
    file.close();
    file.open(QIODevice::Truncate);
    file.close();
    file.open(QIODevice::WriteOnly);
    io<<1;
    QTextStream in(&file);
    in<<newKey;
    file.close();
}

void EncSettingsWin::ok_run(){
    QString newKey = ui->lineEdit->text();
    writeKey(newKey);
    this->close();
}

void EncSettingsWin::cancel_run(){
    this->close();
}
