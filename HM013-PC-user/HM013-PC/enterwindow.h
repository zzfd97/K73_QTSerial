#ifndef ENTERWINDOW_H
#define ENTERWINDOW_H

#include <QWidget>

namespace Ui {
class EnterWindow;
}

class EnterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EnterWindow(QWidget *parent = nullptr);
    ~EnterWindow();
//signals:

private slots:
    void on_pushButton_clicked();
    bool IsUserWindow(void);
    void on_pushButton_2_clicked();

private:
    Ui::EnterWindow *ui;
    bool *userok;
};

#endif // ENTERWINDOW_H
