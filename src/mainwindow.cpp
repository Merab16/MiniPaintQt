#include "mainwindow.h"

#include <QFrame>
#include <string>

#include "geometricprimitives.h"


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

    connect(navButtons_[0], &QPushButton::clicked, [this](){
        qDebug() << "Clicked";
        paintArea_->SetCurrentObject(GEOMETRY_OBJ::RECTANGLE);
    });


}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch(event->key()) {
    case Qt::Key_Escape:
        if (paintArea_->GetIsDrawing())
            paintArea_->CancelDrawing();
        break;
    }
}



// public
