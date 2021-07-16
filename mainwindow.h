#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <sstream>
#include <QtDebug>
#include <QProcess>
#include<QMessageBox>
#include "inputHandler.h"
#include "compressor.h"
#include <utility>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_OpenFile_clicked();

    void on_TOJson_clicked();

    void on_Save_clicked();

    void on_Check_clicked();

    void on_Correct_clicked();

    void on_Prettify_clicked();

    void on_Minify_clicked();

    void on_Compress_clicked();

    void on_decompress_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
