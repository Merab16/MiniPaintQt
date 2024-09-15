#include "mainwindow.h"

#include <QFrame>
#include <QDebug>
#include <string>


#include "geometricprimitives.h"


struct ButtonInfo {
    std::string name;
    std::function<void()> pfunc;
};

using namespace GeometricPrimitives;

MainWindow::MainWindow(size_t width, size_t height,
                       QWidget *parent)
    : QMainWindow(parent)
    , window_(new QWidget)
    , vBox_(new QVBoxLayout)
    , navPanel_(new QHBoxLayout)
    , paintHBox_(new QHBoxLayout)
    , paintArea_(new PaintWidget(height))
{
    // QMainWindow
    resize(width, height);
    setWindowTitle("MiniPaint");
    setCentralWidget(window_);

    // window_
    window_->setLayout(vBox_);


    // vBox_
    vBox_->addLayout(navPanel_);
    vBox_->addLayout(paintHBox_);


    // navPanel_
    navPanel_->setAlignment(Qt::AlignTop);

    // paintHBox_
    auto w = paintArea_->GetWidget();
    paintHBox_->addWidget(w);





    NavigationInitialization();



}

MainWindow::~MainWindow() {
    deleteLater();

}


// private
void MainWindow::NavigationInitialization() {
    std::vector<ButtonInfo> btns {
        {"Прямоугольник",   [this](){ paintArea_->SetCurrentObject(GEOMETRY_OBJ::RECTANGLE);}},
        {"Треугольник",     [this](){ paintArea_->SetCurrentObject(GEOMETRY_OBJ::TRIANGLE);}},
        {"Эллипс",          [this](){ paintArea_->SetCurrentObject(GEOMETRY_OBJ::ELLIPSE);}},
        {"|",               std::function<void()>()},
        {"Связь",           [this](){ paintArea_->LinkObjects(); }},
        {"|",               std::function<void()>()},
        {"Переместить",     [this](){ paintArea_->MoveObjects(); }},
        {"Удалить",         [this](){ paintArea_->DeleteObject(); }},
        {"|",               std::function<void()>()},
        {"Загрузить",       [this](){ paintArea_->LoadCanvas(); }},
        {"Сохранить",       [this](){ paintArea_->SaveCanvas(); }},
    };

    navButtons_.reserve(8);

    for (const auto& [name, func]: btns) {
        if (name == "|") {
            QFrame *vline = new QFrame();
            vline->setFrameShape(QFrame::VLine);
            navPanel_->addWidget(vline);
        }
        else {
            QPushButton* btn = new QPushButton(name.c_str());
            navButtons_.push_back(std::move(btn));
            navPanel_->addWidget(navButtons_.back());
            connect(btn, &QPushButton::clicked, func);
        }
    }

}


void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        paintArea_->CancelLink();
        paintArea_->CancelMove();
        break;
    }
}



// public
