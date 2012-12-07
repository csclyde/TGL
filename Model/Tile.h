#ifndef TILE_H
#define TILE_H

#include "Savable.h"

class Tile : public Savable
{
public:
    bool SaveToFile(Exporter *exporter);
    bool LoadFromFile(Exporter *exporter);
    QString GetType() { return "TILE"; }

    Tile();

    void SetPos(int newX, int newY) { x = newX; y = newY; }
    void SetX(int newX) { x = newX; }
    void SetY(int newY) { y = newY; }
    int GetX() { return x; }
    int GetY() { return y; }

    void SetCollision(int newType) { collision = newType; }
    int GetCollision();

    void SetOrigin(int newX, int newY) { originX = newX; originY = newY; }
    void SetXOrigin(int newX) { originX = newX; }
    void SetYOrigin(int newY) { originY = newY; }
    int GetXOrigin() { return originX; }
    int GetYOrigin() { return originY; }

private:
    int originX, originY;
    int collision;

    int x, y;
};

#endif // TILE_H
