#include "../Model/ResourceManager.h"

ResourceManager::ResourceManager()
{
    undo = new QUndoStack;
    undo->setUndoLimit(500);

    modifyTiles = new ModifyTilesCommand;

    currentLayerID = 0;
    clipboard = new Clipboard;

    defaultLayer.SetName("Default");
    layerMap[defaultLayer.GetID()] = &defaultLayer;
    emit ResourceAdded(defaultLayer.GetID());
}

ResourceManager::~ResourceManager()
{
    if(modifyTiles)
        delete modifyTiles;

    delete clipboard;
}

int ResourceManager::GetTileWidth()
{
    return levelProperties.GetTileWidth();
}

int ResourceManager::GetTileHeight()
{
    return levelProperties.GetTileHeight();
}

int ResourceManager::GetCurrentLayerWidth()
{
    TileLayer *tempLayer = layerMap.value(currentLayerID);

    if(tempLayer)
        return tempLayer->GetWidth();

    return 0;
}

int ResourceManager::GetCurrentLayerHeight()
{
    TileLayer *tempLayer = layerMap.value(currentLayerID);

    if(tempLayer)
        return tempLayer->GetHeight();

    return 0;
}

int ResourceManager::AddImage(Image *newImage)
{
    if(newImage)
    {
        AddResourceCommand *add = new AddResourceCommand(newImage, &imageMap);
        undo->push(add);

        emit ResourceAdded(newImage->GetID());
    }

    return 0;
}

bool ResourceManager::DeleteImage(int ID)
{
    if(imageMap.value(ID))
    {
        DeleteResourceCommand *del = new DeleteResourceCommand(imageMap[ID], &imageMap);
        undo->push(del);

        emit ResourceDeleted(ID);

        return true;
    }

    return false;
}

QImage *ResourceManager::GetTileset()
{
    if(levelProperties.GetTilesetID() == 0)
        return NULL;

    return GetImage(levelProperties.GetTilesetID())->GetImage();
}

QPixmap ResourceManager::GetTilePixmap(TileCoord coord)
{
    if(pixmapCache.contains(coord))
        return pixmapCache[coord];

    QImage *tempTileset = GetTileset();
    QImage tempImage = *tempTileset;

    if(tempImage.isNull())
        return QPixmap();

    tempImage = tempImage.copy(levelProperties.GetTileWidth() * coord.first, levelProperties.GetTileHeight() * coord.second, levelProperties.GetTileWidth(), levelProperties.GetTileHeight());

    pixmapCache[coord] = QPixmap::fromImage(tempImage);
    return pixmapCache[coord];
}

void ResourceManager::AddTileLayer(TileLayer *newLayer)
{
    if(newLayer)
    {
        layerMap[newLayer->GetID()] = newLayer;

        emit ResourceAdded(newLayer->GetID());
        emit LayerAdded(newLayer->GetID());
    }
}

void ResourceManager::DeleteTileLayer(int ID)
{
    if(layerMap.value(ID))
    {
        delete layerMap[ID];
        layerMap[ID] = NULL;
        layerMap.remove(ID);

        emit ResourceDeleted(ID);
        emit LayerRemoved(ID);
    }
}

TileLayer *ResourceManager::GetTileLayer(int ID)
{
    if(layerMap.value(ID))
        return layerMap[ID];

    return NULL;
}

int ResourceManager::GetLayerOpacity(int layerID)
{
    TileLayer *tempLayer = GetTileLayer(layerID);

    if(tempLayer)
        return tempLayer->GetOpacity();

    return 100;
}

void ResourceManager::SetLayerVisibility(int layerID, bool visible)
{
    TileLayer *tempLayer = layerMap.value(currentLayerID);

    if(tempLayer)
    {
        tempLayer->SetVisibility(visible);
        emit LayerVisibilityUpdated(layerID, visible);
    }
}

void ResourceManager::ModifyTile(int x, int y, TileCoord origin)
{
    TileLayer *tempLayer = layerMap.value(currentLayerID);

    if(tempLayer)
    {
        //make sure the point will fit in the layer
        if(!tempLayer->ResizeToIncludePoint(x, y))
            return;

        emit LayerSizeUpdated(tempLayer->GetWidth(), tempLayer->GetHeight());

        modifyTiles->AddModification(tempLayer, x, y, origin, tempLayer->GetTileOrigin(x, y));

        //notify the view that this tile should be updated
        emit TileUpdated(currentLayerID, x, y, origin);
    }
}

void ResourceManager::PreviewModifyTile(int x, int y, TileCoord origin)
{
    if(origin != TileCoord(-1, -1))
    {
        Tile tempTile;
        tempTile.origin = origin;
        tempTile.pos = TileCoord(x, y);
        previewTiles[TileCoord(x, y)] = tempTile;
    }
    else
    {
        previewTiles.remove(TileCoord(x, y));
    }

    emit PreviewTileUpdated(x, y, origin);
}

void ResourceManager::ClearPreview()
{
    QList<Tile> previewList = previewTiles.values();
    for(int i = 0; i < previewList.count(); i++)
    {
        PreviewModifyTile(previewList[i].pos.first, previewList[i].pos.second, TileCoord(-1, -1));
    }

    //previewTiles.clear();
}

TileCoord ResourceManager::GetTileOrigin(int x, int y)
{
    //see if there is an upcoming modification to this tile
    TileCoord modOrigin = modifyTiles->GetTileOrigin(x, y);
    if(modOrigin != TileCoord(-1, -1))
        return modOrigin;

    //otherwise, check if the tile exists in the normal model
    if(layerMap.value(currentLayerID))
    {
        TileLayer *tempLayer = layerMap.value(currentLayerID);

        if(tempLayer)
            return tempLayer->GetTileOrigin(x, y);
    }

    return TileCoord(-1, -1);
}

void ResourceManager::SelectTilesInArea(QRect area)
{
    if(area.left() > area.right())
    {
        int temp = area.left();
        area.setLeft(area.right());
        area.setRight(temp);
    }

    if(area.top() > area.bottom())
    {
        int temp = area.top();
        area.setTop(area.bottom());
        area.setBottom(temp);
    }

    selectionArea = area.normalized();

    emit SelectionGeometryUpdated(area);
}

QList<Tile> ResourceManager::GetSelectedTiles()
{
    QList<Tile> selectedTiles;

    for(int i = selectionArea.left(); i <= selectionArea.right(); i++)
    {
        for(int j = selectionArea.top(); j <= selectionArea.bottom(); j++)
        {
            TileCoord origin = GetTileOrigin(i, j);

            if(origin != TileCoord(-1, -1))
            {
                Tile tempTile;
                tempTile.pos.first = i;
                tempTile.pos.second = j;
                tempTile.origin = origin;
                selectedTiles.append(tempTile);
            }
        }
    }

    return selectedTiles;
}

void ResourceManager::ClearSelection()
{

}

Image *ResourceManager::GetImage(int imageID)
{
    if(imageMap.value(imageID))
        return imageMap[imageID];

    return NULL;
}

void ResourceManager::DestroyAllResources()
{
    delete undo;

    //destroy the images
    QList<Image*> imageList = imageMap.values();
    for(int i = 0; i < imageList.count(); i++)
    {
        delete imageList[i];
        imageList[i] = NULL;
    }
    imageMap.clear();

    QList<TileLayer*> layerList = layerMap.values();
    for(int i = 0; i < layerList.count(); i++)
    {
        if(layerList[i] != &defaultLayer)
        {
            delete layerList[i];
            layerList[i] = NULL;
        }
    }
    layerMap.clear();
}

void ResourceManager::Undo()
{
    undo->undo();
}

void ResourceManager::Redo()
{
    undo->redo();
}

ResourceNode *ResourceManager::GetObject(int ID)
{
    if(ID == levelProperties.GetID())
        return &levelProperties;

    QHash<int,Image*>::iterator imageIter;
    for(imageIter = imageMap.begin(); imageIter != imageMap.end(); ++imageIter)
    {
        if(imageIter.key() == ID)
            return imageIter.value();
    }

    QHash<int,TileLayer*>::iterator layerIter;
    for(layerIter = layerMap.begin(); layerIter != layerMap.end(); ++layerIter)
    {
        if(layerIter.key() == ID)
            return layerIter.value();
    }

    return NULL;
}

void ResourceManager::EndPaintOperation() 
{ 
    //if there are no changes that occured, no undo operation needs to be processed
    if(modifyTiles->GetModificationCount() == 0)
        return;

    //push the current bundle of modifications into the undo stack
    undo->push(modifyTiles); 

    //create a fresh bundle for the next operation
    modifyTiles = new ModifyTilesCommand;
}


