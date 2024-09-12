#include "geometricprimitives.h"

namespace GeometricPrimitives {
//================Base================//
Base::Base(const QPoint& start,
           const QPoint& finish)
    : startPos_(start)
    , finishPos_(finish)
{

}

Base::~Base() {

}


//================Rectangle================//
Rectangle::Rectangle(const QPoint& start,
                     const QPoint& finish)
    : Base(start, finish)
{



}

Rectangle::~Rectangle() {

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


}

