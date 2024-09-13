#ifndef GEOMETRICPRIMITIVES_H
#define GEOMETRICPRIMITIVES_H

#include <QPoint>
#include <QPainter>

#include <vector>


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
    QPoint centre_;

private:
    bool Cross(const QPoint& p1, const QPoint& p2,
               const QPoint& p) const;


public:
    Base(const QPoint& start,
         const QPoint& finish);
    ~Base();


    void Draw(QPainter& painter) const;
    bool IsPointInside(const QPoint& point) const;
    void Move(const QPoint& move_offset);

    // getters
    QPoint& GetCentre() { return centre_; }
};


//================Rectangle================//
class Rectangle: public Base {
public:
    Rectangle(const QPoint& start,
              const QPoint& finish);
    ~Rectangle();


};


//================Triangle================//
class Triangle: public Base {
public:
    Triangle(const QPoint& start,
              const QPoint& finish);
    ~Triangle();

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
            size_t pointCount = 50);
    ~Ellipse();

};

}

#endif // GEOMETRICPRIMITIVES_H
