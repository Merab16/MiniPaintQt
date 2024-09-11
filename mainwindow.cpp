#include "mainwindow.h"

#include <QFrame>
#include <string>

MainWindow::MainWindow(size_t width, size_t height,
                       QWidget *parent)
    : QMainWindow(parent)
    , window_(new QWidget)
    , vBox_(new QVBoxLayout)
    , navPanel_(new QHBoxLayout)
    , PaintArea_(new QHBoxLayout)
{
    // QMainWindow
    resize(width, height);
    setWindowTitle("MiniPaint");
    setCentralWidget(window_);

    // window_
    window_->setLayout(vBox_);

    // vBox_
    vBox_->addLayout(navPanel_);


    // navPanel_
    navPanel_->setAlignment(Qt::AlignTop);



    NavigationInitialization();




}

MainWindow::~MainWindow() {}


// private
void MainWindow::NavigationInitialization() {
    std::vector<std::string> buttons_names{
        "Прямоугольник", "Треугольник", "Эллипс", "|",
        "Связь", "|",
        "Переместить", "Удалить", "|",
        "Загрузить", "Сохранить"
    };

    navButtons_.reserve(8);

    for (const auto& name: buttons_names) {
        if (name == "|") {
            QFrame *vline = new QFrame();
            vline->setFrameShape(QFrame::VLine);
            navPanel_->addWidget(vline);
        }
        else {
            QPushButton* btn = new QPushButton(name.c_str());
            navButtons_.push_back(std::move(btn));
            navPanel_->addWidget(navButtons_.back());
        }
    }
}



// public
