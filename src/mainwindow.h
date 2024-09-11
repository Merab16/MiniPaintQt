#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget* window_;

    QVBoxLayout* vBox_;
    QHBoxLayout* navPanel_;
    QHBoxLayout* PaintArea_;

    std::vector<QPushButton*> navButtons_;


private:
    void NavigationInitialization();



public:
    MainWindow(size_t width, size_t height,
               QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
