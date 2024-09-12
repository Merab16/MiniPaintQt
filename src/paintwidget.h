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
    Q_OBJECT
private:
    //QPainter* painter_;
    QPoint firstPoint_;
    QPoint secondPoint_;

    bool isDrawing_ = false;
    bool needUpdate_ = false;

    GeometricPrimitives::GEOMETRY_OBJ currentObj_ =
        GeometricPrimitives::GEOMETRY_OBJ::NONE;

    std::vector<GeometricPrimitives::Base*> objects_;
private:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;


    void DrawNewObject(QPainter& painter);


public:
    explicit PaintWidget(size_t height, QWidget *parent = nullptr);
    ~PaintWidget();


    QWidget* GetWidget() {return this; }

    void paintEvent(QPaintEvent *event) override;
    void CancelDrawing() {isDrawing_= false; }


    // getters
    bool GetIsDrawing() const {return isDrawing_; }

    // setters
    void SetCurrentObject(GeometricPrimitives::GEOMETRY_OBJ type)
        {currentObj_ = type; }



signals:
};

#endif // PAINTWIDGET_H
