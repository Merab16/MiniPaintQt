#include <QDebug>

#include <cmath>

#include "geometricprimitives.h"


namespace GeometricPrimitives {




//================Base================//
Base::Base(const QPoint& start,
           const QPoint& finish)
    : startPos_(start)
    , finishPos_(finish)
    , centre_(start.x() + (finish.x() - start.x()) / 2, start.y() + (finish.y() - start.y()) / 2)
{
    qDebug() << "Base created";
}

Base::~Base() {
    qDebug() << "Base deleted";
}

// private
bool Base::Cross(const QPoint& p1, const QPoint& p2, const QPoint& p) const {    
    if (p1.y() > p.y() && p2.y() > p.y()) return false;
    if (p1.y() < p.y() && p2.y() < p.y()) return false;
    if (p1.x() < p.x() && p2.x() < p.x()) return false;

    double intersectionX = p1.x() + (p.y() - p1.y()) * (p2.x() - p1.x()) / (p2.y() - p1.y());
    return intersectionX >= p.x();
}



// public
void Base::Draw(QPainter& painter) const {
    for (int i = 1; i < points_.size(); ++i) {
        painter.drawLine(points_[i], points_[i - 1]);
    }
    painter.drawLine(points_.back(), points_[0]);
}

bool Base::IsPointInside(const QPoint& point) const {
    // Алгоритм:
    // От указанной точки проводится горизонтальный луч право
    // подсчитывается кол-во пересечений с объектом
    // если intersections % 2 == 1: внутри
    // иначе: вне объекта


    size_t intersections = 0;
    for (size_t i = 0; i < points_.size(); i++) {
        QPoint p1 = points_[i];
        QPoint p2 = points_[(i + 1) % points_.size()];

        if (Cross(p1, p2, point)) {
            intersections++;
        }
    }
    return intersections % 2 == 1;


}

void Base::Move(const QPoint& move_offset) {
    centre_ += move_offset;
    startPos_ += move_offset;
    finishPos_ += move_offset;

    for (auto& point: points_) {
        point += move_offset;
    }

}



//================Rectangle================//
Rectangle::Rectangle(const QPoint& start,
                     const QPoint& finish)
    : Base(start, finish)
{
    qDebug() << "Rectangle created";
    points_.reserve(4);
    points_.push_back({startPos_.x(), startPos_.y()});
    points_.push_back({finishPos_.x(), startPos_.y()});
    points_.push_back({finishPos_.x(), finishPos_.y()});
    points_.push_back({startPos_.x(), finishPos_.y()});

}

Rectangle::~Rectangle() {
    qDebug() << "Rectangle deleted";
}




//================Triangle================//
Triangle::Triangle(const QPoint& start,
                     const QPoint& finish)
    : Base(start, finish)
{
    qDebug() << "Triangle created";
    points_.reserve(3);

    points_.push_back({startPos_.x(), finishPos_.y()});
    points_.push_back({finishPos_.x(), finishPos_.y()});
    points_.push_back({startPos_.x() + (finishPos_.x() - startPos_.x()) / 2, startPos_.y()});
}

Triangle::~Triangle() {
    qDebug() << "Triangle deleted";
}




//================Ellipse================//
Ellipse::Ellipse(const QPoint& start,
                 const QPoint& finish,
                 size_t pointCount)
    : Base(start, finish)
    , pointCount_(pointCount)
{
    qDebug() << "Ellipse created";
    points_.reserve(pointCount_);
    QPoint centre_offset = {
        startPos_.x() + (finishPos_.x() - startPos_.x()) / 2,
        startPos_.y() + (finishPos_.y() - startPos_.y()) / 2
    };

    float angle = 0;
    for (size_t i = 0; i < pointCount_; i++) {
        points_.push_back({
            QPoint(
            0.5 * (finishPos_.x() - startPos_.x()) * std::cos(AngleToRadian(angle)),
            0.5 * (finishPos_.y() - startPos_.y()) * std::sin(AngleToRadian(angle))
            ) + centre_offset
        });
        angle += 360.f / pointCount_;
    }
}

Ellipse::~Ellipse() {
    qDebug() << "Ellipse deleted";
}

// private
float Ellipse::AngleToRadian(float angle) const {
    return 3.141592 * angle / 180;
}


} // namespace




