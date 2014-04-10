#include "MiniMap.h"

MiniMap::MiniMap(QWidget *parent) :
    QGraphicsView(parent)
{
}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void MiniMap::mousePressEvent(QMouseEvent *event)
{
}

void MiniMap::mouseReleaseEvent(QMouseEvent *event)
{
}

void MiniMap::wheelEvent(QWheelEvent *event)
{
    int steps = (event->delta() / 8) / 15;

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.15;

    //zoom in
    if(event->delta() > 0)
    {
        //if the zoom is less than 10x, zoom in
        scale(scaleFactor, scaleFactor);
    }
    else //zoom out
    {
        //if the size of the scene is smaller than the view
        if(sceneRect()->contains(scene()->sceneRect()))
        {
            //make the scene fit nicely in there
            fitInView(scene()->sceneRect());
        }
        else//otherwise
        {
            //zoom out
            scale(1/scaleFactor, 1/scaleFactor);
        }
    }

    event->accept();
}
