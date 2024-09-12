#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>


#include <vector>


#include "paintwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget* window_;

    QVBoxLayout* vBox_;
    QHBoxLayout* navPanel_;
    QHBoxLayout* paintHBox_;
    PaintWidget* paintArea_;

    std::vector<QPushButton*> navButtons_;

private:
    void keyPressEvent(QKeyEvent* event) override;

    void NavigationInitialization();


public:
    MainWindow(size_t width, size_t height,
               QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
