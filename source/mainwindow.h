#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "d1mpq.h"
#include "d1ps1dirbin.h"

#define D1_ARCHIVE_TOOL_VERSION "0.1.0"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = 0 );
    ~MainWindow();

    QString getSizeString( quint64 );

    void showDirBinContent();

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSelect_All_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();

private:
    Ui::MainWindow *ui;

    QPointer<D1PS1DirBin> mpq;
    QPointer<D1PS1DirBin> dirbin;
};

#endif // MAINWINDOW_H
