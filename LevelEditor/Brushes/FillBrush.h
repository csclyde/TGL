#ifndef FILLBRUSH_H
#define FILLBRUSH_H

#include "TileBrush.h"

class FillBrush : public TileBrush
{
public:
    FillBrush();

    void Move(int x, int y, TileLayerView *layer);

    void Paint(int x, int y, TileLayerView *layer, bool preview = false);
    void Fill(int tileX, int tileY, TileCoord newOrigin, TileCoord oldOrigin, TileLayerView *newLayer);

    QString GetType() { return "fill"; }

};

#endif // FILLBRUSH_H
