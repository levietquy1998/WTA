#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_N_clicked();

    void on_Random_clicked();
    QString random();
    void Calculator();

    void on_Begin_clicked();

    void on_Begin_pressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
