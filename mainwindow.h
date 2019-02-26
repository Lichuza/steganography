#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "graphics.h"
#include <bitset>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    int i1;
    vector<int> key;
};

#endif // MAINWINDOW_H
