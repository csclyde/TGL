#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QUndoStack>

#include "Sprite.h"
#include "Image.h"
#include "TileLayer.h"
#include "Model/LevelProperties.h"

#include "../Commands/AddResourceCommand.h"
#include "../Commands/DeleteResourceCommand.h"
#include "../Commands/ModifyTilesCommand.h"

class ResourceManager : public QObject
{
    Q_OBJECT

    //ResourceManager(QObject *parent = 0);
public:
    ResourceManager();
    ~ResourceManager();

public slots:

    LevelProperties *GetLevelProperties() { return &levelProperties; }

    int AddSprite(Sprite *newSprite);
    bool DeleteSprite(int ID);
    Sprite *GetSprite(int ID);
    Sprite *GetSpriteByIndex(int index);
    int GetSpriteCount() { return spriteMap.count(); }

    int AddImage(Image *newImage);
    bool DeleteImage(int ID);
    Image *GetImage(int ID);
    Image *GetImageByIndex(int index);
    int GetImageCount() { return imageMap.count(); }

    QPixmap GetSpriteSymbol(int spriteID);
    QImage *GetTileset();
    QPixmap GetTilePixmap(TileCoord coord);

    void AddTileLayer(TileLayer *newLayer);
    void DeleteTileLayer(int ID);
    TileLayer *GetTileLayer(int ID);
    TileLayer *GetLayerByIndex(int index);
    int GetLayerCount() { return layerMap.count(); }

    int GetLayerOpacity(int layerID);
    void ModifyTile(int layerID, int x, int y, TileCoord origin, TileCoord oldOrigin);
    TileCoord GetTileOrigin(int layerID, int x, int y);
    int GetTileCount(int layerID);
    Tile *GetTileByIndex(int layerID, int i);

    void DestroyAllResources();

    void Undo();
    void Redo();

    void EndPaintOperation();
    
    bool IsUndoHappening() { return undoing; }

signals:
    void SpriteListModified(QStringList spriteNameList);
    void LayerListModified(int newID);
    void ImageListModified();

    void ObjectPrototypeListModified(QStringList objectPrototypeNameList, int type);

    void UndoRedoPerformed();

private:
    QMap<int, Sprite*> spriteMap;
    QMap<int, Image*> imageMap;
    QMap<int, TileLayer*> layerMap;

    LevelProperties levelProperties;

    QUndoStack *undo;
    ModifyTilesCommand *modifyTiles;
    
    bool undoing;
};

#endif // RESOURCEMANAGER_H
