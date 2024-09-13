#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <vector>
#include <set>


#include "geometricprimitives.h"

bool operator< (const QPoint& lhs, const QPoint& rhs);

struct LoadObj;

class PaintWidget : public QWidget
{
    struct Link {
        bool operator< (const Link& rhs) const {
            if (*p1 == *rhs.p1)
                return *p2 < *rhs.p2;
            return *p1 < *rhs.p1;
        }


        Link() = default;
        Link(QPoint* point1, QPoint* point2) {
            p1 = point1;
            p2 = point2;
            sHalf = true;
        }

        ~Link() {}

        Link(const Link&) = delete;
        Link& operator=(const Link&) = delete;

        Link(Link&& rhs) {
            p1 = rhs.p1;
            p2 = rhs.p2;
            sHalf = rhs.sHalf;

            rhs.Reset();
        }
        Link& operator=(Link&& rhs) {
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

        void Reset() {
            p1 = nullptr;
            p2 = nullptr;
            sHalf = false;
        }

        QPoint* p1 = nullptr;
        QPoint* p2 = nullptr;
        bool sHalf = false;

    };


    Q_OBJECT
private:
    // general vars
    QPoint current_mouse_pos {0, 0};

    std::set<GeometricPrimitives::Base*> objects_;

    // bool vars
    bool isDrawing_ = false;
    bool needUpdate_ = false;
    bool link_ = false;
    bool move_ = false;
    bool delete_ = false;

    // vars of new obj
    QPoint firstPoint_;
    QPoint secondPoint_;
    GeometricPrimitives::GEOMETRY_OBJ currentObj_ =
        GeometricPrimitives::GEOMETRY_OBJ::NONE;

    // vars of linking obj
    Link potential_link_;
    GeometricPrimitives::Base* potential_link_obj_ = nullptr;

    // vars of moving obj
    QPoint start_move_pos_;
    GeometricPrimitives::Base* current_move_obj_ = nullptr;

private:
    // override events
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *event) override;


    void DrawNewObject(QPainter& painter);
    GeometricPrimitives::Base* CheckIntersection();


    void StartLink();
    void StartMove();
    void StartDelete();
    void ConvertObj(const std::set<LoadObj>& vec);

public:
    explicit PaintWidget(size_t height, QWidget *parent = nullptr);
    ~PaintWidget();

    void SaveCanvas() const;
    void LoadCanvas();

    // getters
    QWidget* GetWidget() {return this; }
    bool GetIsDrawing() const {return isDrawing_; }


    // setters
    void CancelDrawing() { isDrawing_= false; }
    void SetCurrentObject(GeometricPrimitives::GEOMETRY_OBJ type)
        { currentObj_ = type; link_ = move_ = delete_ = false; }
    void LinkObjects() { link_ = true; }
    void CancelLink();
    void MoveObjects() { move_ = true; }
    void CancelMove();
    void DeleteObject() { delete_ = true; }


signals:
};

#endif // PAINTWIDGET_H
