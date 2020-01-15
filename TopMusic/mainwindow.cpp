#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apptop.h"
#include "songswindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AppTop::client->connectToServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLogin_clicked()
{
    char user[128];
    char pass[128];
    char msg[256];
    memset(user, 0, 128);
    memset(pass, 0, 128);
    memset(msg, 0, 256);

    strcpy(user, ui->leUsername->text().toStdString().c_str());
    strcpy(pass, ui->lePassword->text().toStdString().c_str());
    sprintf(msg, "%s;%s", user, pass);

    write(AppTop::client->sd, msg, 256);
    //printf("1111%s111", msg);
    fflush(stdout);
    read(AppTop::client->sd, msg, 256);
    if (!strcmp(msg, "LOGIN_VALID"))
    {
        AppTop::clientName = ui->leUsername->text().toStdString();
        SongsWindow* window = new SongsWindow();
        window->show();
        this->hide();
        return;
    }
    this->setEnabled(false);
    QMessageBox* msgBox = new QMessageBox();
    msgBox->setText(QString("User and password are not valid! Try again!"));
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
    this->setEnabled(true);
}
