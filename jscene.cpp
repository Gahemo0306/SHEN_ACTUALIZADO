#include "jscene.h"
#include <QPen>
Jscene::Jscene(QGraphicsScene *parent)
    : QGraphicsScene(parent)
{
    int x1,y1,x2,y2;
    x1 = 0; y1 = 0; x2 = 0; y2 = 275;
    QPen  lGrayPen(Qt::gray);
    lGrayPen.setWidth(1);
    lGrayPen.setStyle(Qt::DashLine);
    for(int j = 0 ; j < 12; j++){
        x1 += 41;
        x2 += 41;
        addLine(x1,y1,x2,y2,lGrayPen);
    }
    addLine(0,-2,492,-2,lGrayPen);
    addLine(0,277,492,277,lGrayPen);
}
