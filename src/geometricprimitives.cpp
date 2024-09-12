#include <QDebug>

#include <cmath>

#include "geometricprimitives.h"


namespace GeometricPrimitives {
//================Base================//
Base::Base(const QPoint& start,
           const QPoint& finish)
    : startPos_(start)
    , finishPos_(finish)
{
    qDebug() << "Base created";
}

Base::~Base() {
    qDebug() << "Base deleted";
}


//================Rectangle================//
Rectangle::Rectangle(const QPoint& start,
                     const QPoint& finish)
    : Base(start, finish)
{
    qDebug() << "Rectangle created";


}

Rectangle::~Rectangle() {
    qDebug() << "Rectangle deleted";
}

// public
void Rectangle::Draw(QPainter& painter) const {
    painter.drawLine(startPos_.x(), startPos_.y(),
                     finishPos_.x(), startPos_.y());
    painter.drawLine(finishPos_.x(), startPos_.y(),
                     finishPos_.x(), finishPos_.y());
    painter.drawLine(finishPos_.x(), finishPos_.y(),
                     startPos_.x(), finishPos_.y());
    painter.drawLine(startPos_.x(), finishPos_.y(),
                     startPos_.x(), startPos_.y());
}


//================Triangle================//
Triangle::Triangle(const QPoint& start,
                     const QPoint& finish)
    : Base(start, finish)
{
    qDebug() << "Triangle created";
}

Triangle::~Triangle() {
    qDebug() << "Triangle deleted";
}

// public
void Triangle::Draw(QPainter& painter) const {
    painter.drawLine(startPos_.x(), finishPos_.y(),
                     startPos_.x() + (finishPos_.x() - startPos_.x()) / 2, startPos_.y());
    painter.drawLine(startPos_.x() + (finishPos_.x() - startPos_.x()) / 2, startPos_.y(),
                     finishPos_.x(), finishPos_.y());
    painter.drawLine(finishPos_.x(), finishPos_.y(),
                     startPos_.x(), finishPos_.y());
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


// public
void Ellipse::Draw(QPainter& painter) const {
    for (int i = 1; i < points_.size(); ++i) {
        painter.drawLine(points_[i], points_[i - 1]);
    }
    painter.drawLine(points_.back(), points_[0]);
}






} // namespace




