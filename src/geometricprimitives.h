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


//================Rectangle================//
class Rectangle: public Base {
public:
    Rectangle(const QPoint& start,
              const QPoint& finish);
    virtual ~Rectangle();

    void Draw(QPainter& painter) const override;


};


//================Triangle================//
class Triangle: public Base {
public:
    Triangle(const QPoint& start,
              const QPoint& finish);
    virtual ~Triangle();

    void Draw(QPainter& painter) const override;

};



//================Ellipse================//
class Ellipse: public Base {
private:
    std::vector<QPoint> points_;
    size_t pointCount_;

private:
    float AngleToRadian(float angle) const;

public:
    Ellipse(const QPoint& start,
            const QPoint& finish,
            size_t pointCount = 100);
    virtual ~Ellipse();

    void Draw(QPainter& painter) const override;

};

}

#endif // GEOMETRICPRIMITIVES_H
