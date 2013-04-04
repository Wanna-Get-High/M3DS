#include "RaytraceView.h"
#include <QPainter>

#include "Raytrace.h"

/**
@file
@author Fabrice Aubert
*/

RaytraceView::RaytraceView(QWidget *parent) :
    QWidget(parent) {
  _raytrace=NULL;
}

RaytraceView::~RaytraceView() {

}

void RaytraceView::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(QPoint(0,0),*(_raytrace->image()));
}

