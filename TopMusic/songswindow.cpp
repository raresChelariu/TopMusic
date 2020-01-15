#include "songswindow.h"
#include "ui_songswindow.h"
#include "apptop.h"
#include <QMessageBox>
#include <QTimer>
#include <string>

#define COL_VOTES_ID 5
SongsWindow::SongsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SongsWindow)
{
    ui->setupUi(this);

    ui->labelUserLogged->setText(QString("Logged as : ").append(AppTop::clientName.c_str()));
    initTableHeader();
    initComboBoxSort();
    printf("OOooo\n");
    fflush(stdout);
    printf("Eeee\n");
    fflush(stdout);
    initTableContent();
    this->ui->comboBoxGenre->addItems({"Blues", "DnB", "HipHop", "House", "Jazz", "Rock"});
    this->update();
    this->updateTimer = new QTimer(this);
    this->updateTimer->setInterval(3000);
    this->updateTimer->start();

    connect(updateTimer, SIGNAL(timeout()), this, SLOT(on_update_songs()));
}

void SongsWindow::on_update_songs()
{
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
    this->ui->tableSongs->setSelectionBehavior(QAbstractItemView::SelectRows);
    for(int i = 0; i < this->ui->tableSongs->columnCount(); i++)
        this->ui->tableSongs->setColumnWidth(i, this->ui->tableSongs->width()/this->ui->tableSongs->columnCount());
}
void SongsWindow::on_btnAddSong_clicked()
{
    if (ui->leName->text().isEmpty() ||
        ui->leArtist->text().isEmpty() ||
        ui->leDescription->text().isEmpty() ||
        ui->leURL->text().isEmpty() ||
        ui->comboBoxGenre->currentText().isEmpty())
    {
        QMessageBox* msgBox = new QMessageBox();
        msgBox->setText("Fill all fields");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
    char* myNewSong = (char*) malloc(256);
    memset(myNewSong, 0, 256);

    sprintf(myNewSong, "%s;%s;%s;%s;%s;0",
            ui->leName->text().toStdString().c_str(),
            ui->leArtist->text().toStdString().c_str(),
            ui->comboBoxGenre->currentText().toStdString().c_str(),
            ui->leDescription->text().toStdString().c_str(),
            ui->leURL->text().toStdString().c_str());
    printf("Hola\n");
    write(AppTop::client->sd, "ADD_SONG", 256);
    write(AppTop::client->sd, myNewSong, 256);
}

void SongsWindow::initTableContent()
{
    printf("initTable\n");
    fflush(stdout);
    char msg[256];
    char *lineSong = (char*) malloc(256);
    memset(lineSong, 0, 256);

    long long songNo = 0;

    strcpy(msg, "GET_SONGS");
    printf("%s\n", msg);
    fflush(stdout);
    write(AppTop::client->sd, msg, 256);

    char* p;
    read(AppTop::client->sd, &songNo, sizeof(songNo));
    ui->tableSongs->setRowCount(songNo);
    for (long long i = 0; i < songNo; i++)
    {
        read(AppTop::client->sd, lineSong, 256);
        printf("%s\n", lineSong);
        p = strtok(lineSong, ";");

        ui->tableSongs->setItem(i, 0, new QTableWidgetItem(QString(p)));
        p = strtok(NULL, ";");

        ui->tableSongs->setItem(i, 1, new QTableWidgetItem(QString(p)));
        p = strtok(NULL, ";");
        ui->tableSongs->setItem(i, 2, new QTableWidgetItem(QString(p)));
        p = strtok(NULL, ";");
        ui->tableSongs->setItem(i, 3, new QTableWidgetItem(QString(p)));
        p = strtok(NULL, ";");
        ui->tableSongs->setItem(i, 4, new QTableWidgetItem(QString(p)));
        p = strtok(NULL, ";");
        ui->tableSongs->setItem(i, 5, new QTableWidgetItem(QString(p)));
        fflush(stdout);
    }    
    this->update();
}

void SongsWindow::on_btnVote_clicked()
{
    QItemSelectionModel* select = ui->tableSongs->selectionModel();
    if (select->hasSelection() == false)
    {
        QMessageBox* msgBox = new QMessageBox();
        this->setEnabled(false);
        msgBox->setText(QString("No row is selected !"));
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
        this->setEnabled(true);
        return;
    }
    QModelIndexList selection = select->selectedRows();
    int rowNo;
    for (int i = 0; i < selection.count(); i++)
    {
        rowNo = selection.at(i).row();
        int noVotes = 1 + std::stoi(ui->tableSongs->item(rowNo, COL_VOTES_ID)->text().toStdString());
        ui->tableSongs->setItem(rowNo, COL_VOTES_ID, new QTableWidgetItem(QString::fromStdString(std::to_string(noVotes))));
        printf("%d\n", noVotes);
        write(AppTop::client->sd, "UPDATE_VOTE_NO", 256);
        write(AppTop::client->sd, &rowNo, 4);
        write(AppTop::client->sd, &noVotes, 4);
        fflush(stdout);
    }
}
void SongsWindow::initComboBoxSort()
{
    ui->comboBoxSort->addItem("Sort by Genre");
    ui->comboBoxSort->addItem("Sort by Artist");
    ui->comboBoxSort->addItem("Sort by Title");
}

void SongsWindow::on_btnSortList_clicked()
{
    write(AppTop::client->sd, "SORT_BY", 256);
    if (ui->comboBoxGenre->currentIndex() == 0)
        write(AppTop::client->sd, "GENRE", 256);
    if (ui->comboBoxGenre->currentIndex() == 1)
        write(AppTop::client->sd, "TITLE", 256);
    if (ui->comboBoxGenre->currentIndex() == 2)
        write(AppTop::client->sd, "ARTIST", 256);
    printf("List sorted!\n");
    fflush(stdout);
    initTableContent();
}
