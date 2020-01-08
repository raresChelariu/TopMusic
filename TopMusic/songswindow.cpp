#include "songswindow.h"
#include "ui_songswindow.h"
#include "apptop.h"
SongsWindow::SongsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SongsWindow)
{
    ui->setupUi(this);
    initTableHeader();
    printf("OOooo\n");
    fflush(stdout);
    this->update();
    printf("Eeee\n");
    fflush(stdout);
    initTableContent();
}

SongsWindow::~SongsWindow()
{
    delete ui;
}
void SongsWindow::initTableHeader()
{
    QStringList tableHeader;
    this->ui->tableSongs->setColumnCount(6);
    tableHeader << "Name" << "Artist" << "Genre" << "Description" << "URL" << "Vote No.";
    this->ui->tableSongs->verticalHeader()->setVisible(false);
    this->ui->tableSongs->setHorizontalHeaderLabels(tableHeader);
    this->ui->tableSongs->verticalHeader()->setDefaultSectionSize(50);
    this->ui->tableSongs->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    this->ui->tableSongs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->ui->tableSongs->setFocusPolicy(Qt::NoFocus);
    this->ui->tableSongs->setSelectionMode(QAbstractItemView::NoSelection);
    for(int i = 0; i < this->ui->tableSongs->columnCount(); i++)
        this->ui->tableSongs->setColumnWidth(i, this->ui->tableSongs->width()/this->ui->tableSongs->columnCount());
}
void SongsWindow::initTableContent()
{
    printf("initTable\n");
    fflush(stdout);
    char msg[256];
    char *lineSong;
    long long songNo = 0;

    strcpy(msg, "GET_SONGS");
    printf("%s\n", msg);
    fflush(stdout);
    write(AppTop::client->sd, msg, 256);

    read(AppTop::client->sd, &songNo, sizeof(songNo));
    for (long long i = 0; i < songNo; i++)
    {
        read(AppTop::client->sd, lineSong, 256);
        printf("%s\n", lineSong, 256);
        fflush(stdout);
    }    
}
