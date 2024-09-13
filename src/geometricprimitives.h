#ifndef GEOMETRICPRIMITIVES_H
#define GEOMETRICPRIMITIVES_H

#include <QPoint>
#include <QPainter>

#include <set>
#include <fstream>
#include <map>


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
    static size_t INDEX;
    GEOMETRY_OBJ type_;
    size_t index_;
    std::vector<QPoint> points_;
    QPoint startPos_;
    QPoint finishPos_;
    QPoint centre_;


    std::set<Base*> links_;
    std::map<Base*, size_t> objToIndex_;

private:
    bool Cross(const QPoint& p1, const QPoint& p2,
               const QPoint& p) const;


public:
    Base(const QPoint& start,
         const QPoint& finish,
         GEOMETRY_OBJ type);
    virtual ~Base();


    void Draw(QPainter& painter) const;
    bool IsPointInside(const QPoint& point) const;
    void Move(const QPoint& move_offset);
    void AddLink(Base* obj);

    void Save(std::ofstream& fout) const;
    void Load();


    // getters
    QPoint& GetCentre() { return centre_; }
    const std::set<Base*>& GetLinks() const {return links_;}
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
            size_t pointCount = 50);
    virtual ~Ellipse();

};

}

#endif // GEOMETRICPRIMITIVES_H
