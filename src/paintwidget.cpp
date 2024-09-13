#include "paintwidget.h"

#include <string>


using namespace GeometricPrimitives;

PaintWidget::PaintWidget(size_t height, QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);

    // ??? Стиль не применяется ???
    // std::string styleSheet =
    //     "background-color: gray;";
    // setStyleSheet(styleSheet.c_str());

    setMinimumHeight(height * 0.9);

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

        if (link_)
            StartLink();
        if (move_)
            StartMove();
        if (delete_)
            StartDelete();

        break;

    case Qt::RightButton:
        isDrawing_= false;
        CancelLink();
        CancelMove();
        SetCurrentObject(GEOMETRY_OBJ::NONE);
        break;
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* event) {
    switch (event->button()) {
    case Qt::LeftButton:
        if (isDrawing_) {
            // drawing
            secondPoint_ = QPoint{event->pos()};
            isDrawing_ = false;

            // move
            move_ = false;
            start_move_pos_ = {0, 0};
            current_move_obj_ = nullptr;
            setCursor(QCursor(Qt::ArrowCursor));

            needUpdate_ = true;
            update();
        }
        break;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent* event)  {
    current_mouse_pos = event->pos();

    if (move_ && current_move_obj_) {
        QPoint delta = current_mouse_pos - start_move_pos_;
        current_move_obj_->Move(delta);
        start_move_pos_ = current_mouse_pos;
    }

    update();
}

void PaintWidget::DrawNewObject(QPainter& painter) {
    Base* obj;
    switch(currentObj_) {
    case GEOMETRY_OBJ::RECTANGLE:
        obj = new Rectangle(firstPoint_, secondPoint_);
        break;

    case GEOMETRY_OBJ::TRIANGLE:
        obj = new Triangle(firstPoint_, secondPoint_);
        break;

    case GEOMETRY_OBJ::ELLIPSE:
        obj = new Ellipse(firstPoint_, secondPoint_);
        break;
    }

    if (currentObj_ != GEOMETRY_OBJ::NONE) {
        obj->Draw(painter);
        objects_.emplace(obj);
    }

    needUpdate_ = false;
    currentObj_ = GEOMETRY_OBJ::NONE;
}

Base* PaintWidget::CheckIntersection() {
    for (const auto& obj: objects_) {
        if (obj->IsPointInside(firstPoint_)) {
            return obj;
        }
    }
    return nullptr;
}

void PaintWidget::StartLink() {

    move_ = false;
    delete_ = false;


    bool inters = false; // Проверка принадлежит ли вторая точка какому-либо объекту
    auto obj = CheckIntersection();
    if (!obj) return;
    if (!potential_link_.sHalf){
        potential_link_ = Link(&obj->GetCentre(), &current_mouse_pos);
        potential_link_obj_ = obj;
    }

    else {
        if (potential_link_.p1 == &obj->GetCentre()) {
            CancelLink();
            return;
        }
        potential_link_.p2 = &obj->GetCentre();
        links_.push_back(std::move(potential_link_));
        link_ = false;
    }

    inters = true;
    if (!inters) {
        CancelLink();
    }
}

void PaintWidget::StartMove() {
    link_ = false;
    delete_ = false;

    auto obj = CheckIntersection();
    if (!obj) return;

    setCursor(QCursor(Qt::ClosedHandCursor));
    current_move_obj_ = obj;
    start_move_pos_ = current_mouse_pos;


}

void PaintWidget::StartDelete() {

    move_ = false;
    link_ = false;

    auto obj = CheckIntersection();
    if (!obj) return;

    DeleteLinks(obj);
    delete obj;
    objects_.erase(obj);

    delete_ = false;
}

void PaintWidget::DeleteLinks(Base* obj) {
    std::vector<size_t> indexs;
    for (size_t i = 0; i < links_.size(); ++i) {
        if (links_[i].p1 == &obj->GetCentre() ||
            links_[i].p2 == &obj->GetCentre())
            indexs.push_back(i);
    }

    for (const auto& index: indexs) {
        links_.erase(links_.begin() + index);
    }
}


// public
void PaintWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);


    potential_link_.Draw(painter);


    // drawing links_
    for (const auto& l: links_) {
        l.Draw(painter);
    }

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



// setters
void PaintWidget::CancelLink() {
    if (!link_) return;
    qDebug() << "Cancel link";
    link_ = false;
    potential_link_.Reset();
    update();
}

void PaintWidget::CancelMove() {
    if (!move_) return;
    qDebug() << "Cancel move";
    move_ = false;
    start_move_pos_ = {0, 0};
    current_move_obj_ = nullptr;
    setCursor(QCursor(Qt::ArrowCursor));
}



