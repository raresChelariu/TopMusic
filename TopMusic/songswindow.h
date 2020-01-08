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
    ~SongsWindow();

private:
    Ui::SongsWindow *ui;
};

#endif // SONGSWINDOW_H
