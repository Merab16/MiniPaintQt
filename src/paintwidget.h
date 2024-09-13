#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <vector>

#include "geometricprimitives.h"



class PaintWidget : public QWidget
{
    struct Link {

        Link() = default;
        Link(QPoint* point1, QPoint* point2) {
            p1 = point1;
            p2 = point2;
            sHalf = true;
        }

        Link(const Link&) = delete;
        Link& operator=(const Link&) = delete;

        Link(Link&& rhs) {
            p1 = rhs.p1;
            p2 = rhs.p2;
            sHalf = rhs.sHalf;

            rhs.Reset();
        }
        Link& operator=(Link&& rhs) {
            // delete p1;
            // delete p2;

            p1 = rhs.p1;
            p2 = rhs.p2;
            sHalf = rhs.sHalf;

            rhs.Reset();
            return *this;
        }


        void Draw(QPainter& painter) const {
            if (p1 && p2)
                painter.drawLine(*p1, *p2);
        }

        void Reset() {p1 = nullptr; p2 = nullptr; sHalf = false;}

        QPoint* p1 = nullptr;
        QPoint* p2 = nullptr;
        bool sHalf = false;
    };

    Q_OBJECT
private:
    QPoint firstPoint_;
    QPoint secondPoint_;

    bool isDrawing_ = false;
    bool needUpdate_ = false;
    bool link_ = false;

    QPoint current_mouse_pos{0, 0};

    GeometricPrimitives::GEOMETRY_OBJ currentObj_ =
        GeometricPrimitives::GEOMETRY_OBJ::NONE;

    std::vector<GeometricPrimitives::Base*> objects_;
    Link potential_link_;
    std::vector<Link> links_;
private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void DrawNewObject(QPainter& painter);
    void CheckIntersection();

public:
    explicit PaintWidget(size_t height, QWidget *parent = nullptr);
    ~PaintWidget();


    QWidget* GetWidget() {return this; }

    void paintEvent(QPaintEvent *event) override;




    void DeleteObject(const QPoint& point);
    //void Link();


    // getters
    bool GetIsDrawing() const {return isDrawing_; }

    // setters
    void CancelDrawing() { isDrawing_= false; }
    void SetCurrentObject(GeometricPrimitives::GEOMETRY_OBJ type)
        {currentObj_ = type; }
    void LinkObjects() { link_ = true; }
    void CancelLink();



signals:
};

#endif // PAINTWIDGET_H
