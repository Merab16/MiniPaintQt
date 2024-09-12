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
protected:
    std::vector<QPoint> points_;
    QPoint startPos_;
    QPoint finishPos_;


private:
    bool Cross(const QPoint& p1, const QPoint& p2, const QPoint& p) const;




public:
    Base(const QPoint& start,
         const QPoint& finish);
    virtual ~Base();


    virtual void Draw(QPainter& painter) const;
    bool IsPointInside(const QPoint& point) const;

};


//================Rectangle================//
class Rectangle: public Base {
public:
    Rectangle(const QPoint& start,
              const QPoint& finish);
    virtual ~Rectangle();


};


//================Triangle================//
class Triangle: public Base {
public:
    Triangle(const QPoint& start,
              const QPoint& finish);
    virtual ~Triangle();



};



//================Ellipse================//
class Ellipse: public Base {
private:
    size_t pointCount_;

private:
    float AngleToRadian(float angle) const;

public:
    Ellipse(const QPoint& start,
            const QPoint& finish,
            size_t pointCount = 100);
    virtual ~Ellipse();



};

}

#endif // GEOMETRICPRIMITIVES_H
