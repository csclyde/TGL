#include "TileSelectorScene.h"

TileSelectorScene::TileSelectorScene(QObject *parent)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(PackageAndEmitSelection()));

    selection = new QRubberBand(QRubberBand::Rectangle);

    QPalette palette;
    palette.setBrush(QPalette::Foreground, QBrush(Qt::green));
    palette.setBrush(QPalette::Base, QBrush(Qt::red));

    selection->setPalette(palette);

    selectionIndex = 0;
    selectionChangeFromHistory = false;
}

TileSelectorScene::~TileSelectorScene()
{
    delete selection;
}

void TileSelectorScene::RepopulateTileSelector()
{
    clear();

    if(!spritesheet)
        return;

    if(!spritesheet->isNull())
    {
        int tileW = resources->GetLevelProperties()->GetTileWidth();
        int tileH = resources->GetLevelProperties()->GetTileHeight();

        if(tileW && tileH)
        {
            int imageW = spritesheet->width() / tileW;
            int imageH = spritesheet->height() / tileH;

            TileWidgetItem *tempItem;

            //loop for the width of the spritesheet divided by the width of a tile
            for(int i = 0; i < imageH; i++)
            {
                //loop for the height of the spritesheet divided by the height of a tile
                for(int j = 0; j < imageW; j++)
                {
                    tempItem = new TileWidgetItem;

                    //copy the correct fragment of the image into a new TileItem
                    tempItem->SetTilePixmap(resources->GetTilePixmap(TileCoord(j, i)));

                    //create a tile with the correct origin points, and add it in.
                    tempItem->SetTileOrigin(TileCoord(j, i));

                    //add the tile item to the tile selector at (i * tilewidth) + i
                    addItem(tempItem);

                    tempItem->setPos((j * tileW) + j, (i * tileH) + i);
                    tempItem->setFlag(QGraphicsItem::ItemIsSelectable);
                }
            }
        }
    }
}

void TileSelectorScene::SelectNewTile(TileCoord origin)
{
    int tileW = resources->GetLevelProperties()->GetTileWidth();
    int tileH = resources->GetLevelProperties()->GetTileHeight();

    //find the x and y position of the tile
    int tileX = (tileW * origin.first) + tileW - 1;
    int tileY = (tileH * origin.second) + tileH - 1;

    clearSelection();

    //find that tile based on position
    QGraphicsItem *tempTileItem = itemAt(tileX, tileY);

    //select the new tile
    if(tempTileItem)
        tempTileItem->setSelected(true);
}

void TileSelectorScene::SelectTileset()
{
    //ask the user where to load the file from.
    QString filename = QFileDialog::getOpenFileName(NULL, "Add Image", ".", "Portable Network Graphics (*.png)");

    //if the dialog succeeds
    if(!filename.isEmpty())
    {
        Image *tempImage = new Image;

        //load the file
        tempImage->SetImageFromFile(filename);

        //add the image to the resource manager
        resources->AddImage(tempImage);

        //set the spritesheet as that image
        spritesheet = tempImage->GetImage();

        //store its ID as the image to be used as the tileset
        resources->GetLevelProperties()->SetTilesetID(tempImage->GetID());

        //and repopulate the tile selector
        RepopulateTileSelector();
    }
}

void TileSelectorScene::TraverseTileHistory(bool forward)
{
    //the selection history should be a list of TileLists. When the history is traversed,
    //each of those items should be selected. PackageAndEmitSelection will automatically
    //select the stamp brush is there are more than one tile. Don't forget to set the
    //selectionChangeFrameHistory for *each* iteration of the loop

    //if we are moving forward
    if(forward)
    {
        //and there is something to move to
        if(selectionIndex - 1 >= 0)
        {
            //set this flag so this change in selections isnt added to the history
            selectionChangeFromHistory = true;

            //clear the current selection, and select the next item
            clearSelection();
            selectionHistory[--selectionIndex]->setSelected(true);
        }
    }
    //if we are moving backwards
    else
    {
        //and there is something to move to
        if(selectionIndex + 1 < selectionHistory.count())
        {
            selectionChangeFromHistory = true;

            //clear the current selection, and select the previous item
            clearSelection();
            selectionHistory[++selectionIndex]->setSelected(true);
        }
    }

}

void TileSelectorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        clickSpot = event->scenePos().toPoint();

        //if there is no item here, select the eraser
       // if(!itemAt(event->scenePos().toPoint()))
            //emit SelectNewBrush(1);
    }
}

void TileSelectorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);

    if(event->buttons() & Qt::LeftButton)
    {
        selection->setGeometry(clickSpot.x(), clickSpot.y(),
                               event->scenePos().x() - clickSpot.x(), event->scenePos().y() - clickSpot.y());

        QPainterPath path;
        path.addRect(clickSpot.x(), clickSpot.y(),
                     event->scenePos().x() - clickSpot.x(), event->scenePos().y() - clickSpot.y());

        setSelectionArea(path);
    }
}

void TileSelectorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);

    if(event->button() == Qt::LeftButton)
    {
        selection->setGeometry(0, 0, 0, 0);
    }
}

TileList TileSelectorScene::GetSelectedTiles()
{
    TileList coordList;
    
    //pull out a list of all selected items
    QList<QGraphicsItem*> selectedTiles = selectedItems();
    
    //for every one
    for(int i = 0; i < selectedTiles.count(); i++)
    {
        //cast it into a tilewidget item
        TileWidgetItem *tempItem = dynamic_cast<TileWidgetItem*>(selectedTiles[i]);
        
        //and add its origin coordinates to the list
        if(tempItem)
            coordList.push_back(tempItem->GetTileOrigin());
    }

    return coordList;
}

//this is called when the selection in the scene changes. It will collect the list
//of selected tiles and emit them
void TileSelectorScene::PackageAndEmitSelection()
{
    //there needs to be a better way to store the tool selection. Having the pencil be
    //selected for single tile lists here is not tenable. Its annoying that it keeps
    //switching to the pencil when you select a tile. However, it should deselect the
    //stamp somehow, especially when traversing the selection history

    TileList selectedList = GetSelectedTiles();

    if(selectedList.empty())
        return;

    //if this selection change is not spawned by a history traversal
    if(!selectionChangeFromHistory)
    {
        //prune out any identical enteries before adding this new onw
        if(selectionHistory.contains(selectedItems()[0]))
            selectionHistory.removeAll(selectedItems()[0]);

        //add the top most selected item into the front of the history
        selectionHistory.push_front(selectedItems()[0]);

        //and reset the selection index
        selectionIndex = 0;

        //chop down the list to 10 items
        if(selectionHistory.count() > 10)
            selectionHistory.pop_back();
    }

    //reset the flag
    selectionChangeFromHistory = false;

    //send out the change so other things can know about the current selection
    emit SelectionChanged(selectedList);

    //if there are multiple items being selected, select the stamp brush
    if(selectedList.count() > 1)
        emit SelectNewBrush(4);
}
