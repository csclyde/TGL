#ifndef LEVELLAYER_H
#define LEVELLAYER_H

#include <QGraphicsItemGroup>

#include <Model/ObjectPrototype.h>

class LevelLayer : public QGraphicsItemGroup
{
public:
    LevelLayer();

    void AddObjectItem(QPoint pos, ObjectPrototype *proto);
    void ModifyTile(int tileX, int tileY, int originX, int originY);
};

#endif // LEVELLAYER_H
