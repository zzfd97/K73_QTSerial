#include "enterwindow.h"
#include "ui_enterwindow.h"
#include "qdebug.h"


EnterWindow::EnterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("k71登录账号"));
    userok=new bool;
    *userok=false;

}

EnterWindow::~EnterWindow()
{
    delete ui;
}

void EnterWindow::on_pushButton_clicked()
{
    QString input_data = ui->textEdit->toPlainText();
    if(input_data==tr("hjw")){
        *userok=true;
         connect(this,SIGNAL(ui->on_pushButton_clicked()),this,SLOT(IsUserWindow()));
    }else{
        *userok=false;
    }
    *userok=true;
    QString output_data = "";
    int length = input_data.length();
    for (int i = 0; i < length; i += 2)
    {
        output_data += input_data.mid(i, 2);
        if (i%2 == 0)
        {
            output_data += " ";
        }
    }

    qDebug()<<input_data;
}

bool EnterWindow::IsUserWindow(void)
{
    return true;
}

void EnterWindow::on_pushButton_2_clicked()
{
    this->hide();
    delete ui;
    delete userok;
}
