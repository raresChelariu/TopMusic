#ifndef SONGSWINDOW_H
#define SONGSWINDOW_H

#include <QMainWindow>

namespace Ui {
class SongsWindow;
}

class SongsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SongsWindow(QWidget *parent = 0);
    void initTableHeader();
    void initTableContent();
    void initComboBoxSort();
    QTimer* updateTimer;
    ~SongsWindow();

private slots:
    void on_btnVote_clicked();
    void on_update_songs();
    void on_btnAddSong_clicked();
    void on_btnSortList_clicked();

private:
    Ui::SongsWindow *ui;
};

#endif // SONGSWINDOW_H
