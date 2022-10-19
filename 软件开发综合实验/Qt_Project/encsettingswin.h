#ifndef ENCSETTINGSWIN_H
#define ENCSETTINGSWIN_H

#include <QDialog>

namespace Ui {
class EncSettingsWin;
}

class EncSettingsWin : public QDialog
{
    Q_OBJECT

public:
    explicit EncSettingsWin(QWidget *parent = nullptr);
    ~EncSettingsWin();
    QString getKey();
    void writeKey(QString newKey);
    void update();

private slots:
    void ok_run();
    void cancel_run();
private:
    Ui::EncSettingsWin *ui;
};

#endif // ENCSETTINGSWIN_H
