#ifndef ADDSONGWINDOW_H
#define ADDSONGWINDOW_H

#include <QMainWindow>

namespace Ui {
class AddSongWindow;
}

class AddSongWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddSongWindow(QWidget *parent = 0);
    ~AddSongWindow();

private:
    Ui::AddSongWindow *ui;
};

#endif // ADDSONGWINDOW_H
