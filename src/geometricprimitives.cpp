#include <QDebug>

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







} // namespace




