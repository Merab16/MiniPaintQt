#include "paintwidget.h"

#include <fstream>
#include <string>

using namespace GeometricPrimitives;

bool operator< (const QPoint& lhs, const QPoint& rhs) {
    if (lhs.x() == rhs.x())
        return lhs.y() < rhs.y();
    return lhs.x() < rhs.x();
}

struct LoadObj {
    bool operator< (const LoadObj& rhs) const {
        return index < rhs.index;
    }


    GEOMETRY_OBJ type;
    size_t index;
    QPoint startPos;
    QPoint finishPos;
    size_t links_size;
    std::vector<size_t> links;
};


PaintWidget::PaintWidget(size_t height, QWidget *parent)
    : QWidget{parent}
{
    setMouseTracking(true);

    // ??? Стиль не применяется ???
    // std::string styleSheet =
    //     "background-color: gray;";
    // setStyleSheet(styleSheet.c_str());

    setMinimumHeight(height * 0.9);

}

PaintWidget::~PaintWidget() {
    for (auto& obj: objects_) {
        delete obj;
    }
}


// private
void PaintWidget::mousePressEvent(QMouseEvent* event) {
    switch (event->button()) {
    case Qt::LeftButton:
        firstPoint_ = QPoint{event->pos()};
        isDrawing_ = true;

        if (link_)
            StartLink();
        if (move_)
            StartMove();
        if (delete_)
            StartDelete();

        break;

    case Qt::RightButton:
        isDrawing_= false;
        CancelLink();
        CancelMove();
        SetCurrentObject(GEOMETRY_OBJ::NONE);
        break;
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* event) {
    switch (event->button()) {
    case Qt::LeftButton:
        if (isDrawing_) {
            // drawing
            secondPoint_ = QPoint{event->pos()};
            isDrawing_ = false;


            // move

            move_ = false;
            current_move_pos_ = {0, 0};
            current_move_obj_ = nullptr;
            setCursor(QCursor(Qt::ArrowCursor));

            if (currentObj_ != GEOMETRY_OBJ::NONE)
                AddNewObject();

            update();
        }
        break;
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent* event)  {
    current_mouse_pos = event->pos();

    if (move_ && current_move_obj_) {
        QPoint delta = current_mouse_pos - current_move_pos_;
        current_move_obj_->Move(delta);
        current_move_pos_ = current_mouse_pos;
    }

    update();
}

void PaintWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    potential_link_.Draw(painter);

    // init exist links between two objs
    std::set<Link> links;
    for (const auto& obj: objects_) {
        for (const auto& other_obj: obj->GetLinks()) {
            links.emplace(&obj->GetCentre(), &other_obj->GetCentre());
        }
    }

    // drawing links
    for (const auto& l: links) {
        l.Draw(painter);
    }

    // drawing exist objects
    for (const auto& obj: objects_) {
        obj->Draw(painter);
    }

    painter.end();
}


void PaintWidget::AddNewObject() {
    Base* obj;
    switch(currentObj_) {
    case GEOMETRY_OBJ::RECTANGLE:
        obj = new Rectangle(firstPoint_, secondPoint_);
        break;

    case GEOMETRY_OBJ::TRIANGLE:
        obj = new Triangle(firstPoint_, secondPoint_);
        break;

    case GEOMETRY_OBJ::ELLIPSE:
        obj = new Ellipse(firstPoint_, secondPoint_);
        break;
    }

    objects_.emplace(obj);
    currentObj_ = GEOMETRY_OBJ::NONE;
}

Base* PaintWidget::CheckIntersection() {
    for (const auto& obj: objects_) {
        if (obj->IsPointInside(firstPoint_)) {
            return obj;
        }
    }
    return nullptr;
}

void PaintWidget::StartLink() {

    bool inters = false; // Проверка принадлежит ли вторая точка какому-либо объекту
    auto obj = CheckIntersection();
    if (!obj)  {
        CancelLink();
        return;
    }
    if (!potential_link_.sHalf){
        potential_link_ = Link(&obj->GetCentre(), &current_mouse_pos);
        potential_link_obj_ = obj;
    }
    else {
        if (potential_link_.p1 == &obj->GetCentre()) {
            CancelLink();
            return;
        }
        potential_link_.Reset();

        potential_link_obj_->AddLink(obj);
        obj->AddLink(potential_link_obj_);

        link_ = false;
    }

    inters = true;
    if (!inters) {
        CancelLink();
    }
}

void PaintWidget::StartMove() {

    auto obj = CheckIntersection();
    if (!obj) return;

    setCursor(QCursor(Qt::ClosedHandCursor));
    current_move_obj_ = obj;
    start_move_pos_ = current_mouse_pos;
    current_move_pos_ = current_mouse_pos;


}

void PaintWidget::StartDelete() {

    auto obj = CheckIntersection();
    if (!obj) return;

    objects_.erase(obj);
    delete obj;

    delete_ = false;
}

void PaintWidget::ConvertObj(const std::set<LoadObj>& vec) {
    // tmp_objs необходим, для хранение объектов без связей
    // связи будут инициализироваться после создания всех объектов
    std::vector<Base*> tmp_objs;
    tmp_objs.reserve(vec.size());
    for (const auto& el: vec) {
        Base* obj;
        if (el.type == GEOMETRY_OBJ::RECTANGLE) {
            obj = new Rectangle(el.startPos, el.finishPos);
        } else if (el.type == GEOMETRY_OBJ::TRIANGLE) {
            obj = new Triangle(el.startPos, el.finishPos);
        } else if (el.type == GEOMETRY_OBJ::ELLIPSE) {
            obj = new Ellipse(el.startPos, el.finishPos);
        }

        tmp_objs.push_back(obj);
    }

    for (size_t i = 0; i < tmp_objs.size(); ++i) {
        LoadObj lo;
        lo.index = i;
        auto it = vec.find(lo);
        for (const auto& el: it->links) {
            tmp_objs[i]->AddLink(tmp_objs[el]);
        }
    }

    for (auto& obj: tmp_objs) {
        objects_.emplace(std::move(obj));
    }
    update();
}



// public
void PaintWidget::SaveCanvas() const {
    std::ofstream fout("Canvas.txt");


    for (const auto& obj: objects_) {
        obj->Save(fout);
    }


    fout.close();
}


void PaintWidget::LoadCanvas() {
    std::ifstream fin("Canvas.txt");

    if (!fin.is_open()) {
        qDebug() << "Can't open file" << " Canvas.txt";
        return ;
    }

    if (fin.peek() == std::ifstream::traits_type::eof()) {
        qDebug() << "File empty";
        fin.close();
        return ;
    }


    size_t max_index = 0;
    int p1, p2;

    std::set<LoadObj> tmp_objs;
    LoadObj tmp;


    std::string str;
    while (!fin.eof()) {
        int itype;;
        fin >> itype;
        tmp.type = (GEOMETRY_OBJ)itype;

        fin >> tmp.index;
        max_index = std::max(max_index, tmp.index);

        fin >> p1 >> p2;
        tmp.startPos = QPoint{p1, p2};

        fin >> p1 >> p2;
        tmp.finishPos = QPoint{p1, p2};

        fin >> tmp.links_size;
        tmp.links.reserve(tmp.links_size);
        for (size_t i = 0; i < tmp.links_size; ++i) {
            fin >> p1;
            tmp.links.push_back(p1);
        }

        tmp_objs.emplace(tmp);
    }

    fin.close();

    ConvertObj(tmp_objs);

}


// setters
void PaintWidget::CancelLink() {
    if (!link_) return;
    qDebug() << "Cancel link";
    link_ = false;
    potential_link_.Reset();
    update();
}

void PaintWidget::CancelMove() {
    if (!move_) return;
    qDebug() << "Cancel move";
    qDebug() << start_move_pos_ - current_move_pos_;
    current_move_obj_->Move(start_move_pos_ - current_move_pos_);

    move_ = false;
    start_move_pos_ = {0, 0};
    current_move_pos_ = {0, 0};
    current_move_obj_ = nullptr;
    setCursor(QCursor(Qt::ArrowCursor));
    update();
}



