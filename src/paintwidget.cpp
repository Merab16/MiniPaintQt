#include "paintwidget.h"

#include <string>


using namespace GeometricPrimitives;

PaintWidget::PaintWidget(size_t height, QWidget *parent)
    : QWidget{parent}
{

    std::string styleSheet =
        "background-color: red;";
    setStyleSheet(styleSheet.c_str());
    setMinimumHeight(height * 0.9);

    //connect(this, &QWidget::)

}

PaintWidget::~PaintWidget() {
    for (auto& obj: objects_) {
        delete obj;
    }
}


// private
void PaintWidget::mousePressEvent(QMouseEvent* event) {
    switch (event->button()) {
    case Qt::LeftButton:
        firstPoint_ = QPoint{event->pos()};
        isDrawing_ = true;

        for (const auto& obj: objects_) {
            if (obj->IsPointInside(firstPoint_)) {
                qDebug() << "Inside";
            }
        }


        break;

    case Qt::RightButton:
        isDrawing_= false;
        SetCurrentObject(GEOMETRY_OBJ::NONE);
        break;
    }

    //repaint();
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* event) {
    switch (event->button()) {
    case Qt::LeftButton:
        if (isDrawing_) {
            secondPoint_ = QPoint{event->pos()};
            isDrawing_ = false;
            needUpdate_ = true;
            update();
        }
        break;
    }
}

void PaintWidget::DrawNewObject(QPainter& painter) {
    Base* obj;
    switch(currentObj_) {
    case GEOMETRY_OBJ::RECTANGLE:
        obj = new Rectangle(firstPoint_, secondPoint_);
        obj->Draw(painter);
        objects_.push_back(obj);

        break;

    case GEOMETRY_OBJ::TRIANGLE:
        obj = new Triangle(firstPoint_, secondPoint_);
        obj->Draw(painter);
        objects_.push_back(obj);
        break;

    case GEOMETRY_OBJ::ELLIPSE:
        obj = new Ellipse(firstPoint_, secondPoint_);
        obj->Draw(painter);
        objects_.push_back(obj);
        break;
    }

    needUpdate_ = false;
    currentObj_ = GEOMETRY_OBJ::NONE;
}


// public
void PaintWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // drawing exist objects
    for (const auto& obj: objects_) {
        obj->Draw(painter);
    }

    // drawing new object
    if (needUpdate_) {
        DrawNewObject(painter);
    }
    painter.end();
}

void PaintWidget::DeleteObject(const QPoint& point) {
    size_t i = 0;
    for (; i < objects_.size(); ++i) {
        if (objects_[i]->IsPointInside(point)) {
            break;
        }
    }

    objects_.erase(objects_.begin() + i);

}
