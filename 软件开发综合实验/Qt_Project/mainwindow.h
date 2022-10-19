#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <taskthread.h>
#include "encsettingswin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void sourceFilePathChange(QString newPath);

private slots:
    void btn_run();
    void btn_input_file();
    void btn_output_file();
//    void act_help();
//    void act_about();
    void act_EncSettings();

    void taskCompleted(double costTime);
    void onUiShowMessage(QString msg);

private:
    Ui::MainWindow *ui;
    TaskThread* taskThread;
    EncSettingsWin encsettings;

    void generateOutputPath(QString sourcePath);
    void setAllWidgetsStatus(bool status);
    QString readFileHeader(QString compressedFilePath);

};
#endif // MAINWINDOW_H
