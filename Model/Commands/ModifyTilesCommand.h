#ifndef MODIFYTILESCOMMAND_H
#define MODIFYTILESCOMMAND_H

#include <QUndoCommand>
#include <QList>

#include "../Model/TileLayer.h"

//this class should collect modifications with am AddModification function.
//Then, the redo function will actually implement all those changes.
//The modifications themselves will indicate whether they added a tile, or
//changed an existing one. The undo function will trawl backwards through
//the list, doing the inverse operations on each modification

struct TileModification
{
	TileLayer *layer;
    int x, y;
    TileCoord newOrigin;
    TileCoord oldOrigin;
};

class ModifyTilesCommand : public QUndoCommand
{
public:
    ModifyTilesCommand();

    void AddModification(TileLayer *layer, int newX, int newY, TileCoord newOrgn, TileCoord oldOrgn);

    TileCoord GetTileOrigin(int layerID, int x, int y);

    void undo();
    void redo();

private:

    QList<TileModification> mods;
};

#endif // MODIFYTILESCOMMAND_H