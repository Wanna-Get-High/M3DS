#ifndef RAYTRACEVIEW_H
#define RAYTRACEVIEW_H

class Raytrace;

#include <QWidget>

class RaytraceView : public QWidget
{
    Q_OBJECT
public:
    explicit RaytraceView(QWidget *parent = 0);
    virtual ~RaytraceView();

    void raytrace(Raytrace *r) {_raytrace=r;}

signals:
    
public slots:
    

protected:
    void paintEvent(QPaintEvent *);

private:
    Raytrace *_raytrace;
};

#endif // RAYTRACEVIEW_H

