#ifndef GEOMETRICPRIMITIVES_H
#define GEOMETRICPRIMITIVES_H

#include <QPoint>
#include <QPainter>



namespace GeometricPrimitives {
enum class GEOMETRY_OBJ {
    RECTANGLE,
    TRIANGLE,
    ELLIPSE,

    NONE
};


//================Base================//
class Base {
public:
    QPoint startPos_;
    QPoint finishPos_;


public:
    Base(const QPoint& start,
         const QPoint& finish);
    virtual ~Base();


    virtual void Draw(QPainter& painter) const = 0;


};


//================Triangle================//
class Triangle: public Base {


};

//================Rectangle================//
class Rectangle: public Base {
public:
    Rectangle(const QPoint& start,
              const QPoint& finish);
    virtual ~Rectangle();

    void Draw(QPainter& painter) const override;


};

//================Ellipse================//
class Ellipse: public Base {


};

}

#endif // GEOMETRICPRIMITIVES_H
