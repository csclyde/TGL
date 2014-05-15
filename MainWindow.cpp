#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create a resource manager
    resources               = new ResourceController;
    tileController          = new TileController;
    levelPropertiesWindow   = new LevelPropertiesDialog;
    tileSelector            = new TileSelectorScene;
    layers                  = new TileScene;

    //register the resource manager with the various modules. They will
    //keep themselves in sync with the resource manager
    tileController->        RegisterResourceController(resources);
    levelPropertiesWindow-> RegisterResourceController(resources);
    tileSelector->          RegisterResourceController(resources);
    ui->resourceView->      RegisterResourceController(resources);
    ui->propertyBrowser->   RegisterResourceController(resources);
    ui->layerList->         RegisterResourceController(resources);

    ui->brushManager->      RegisterTileController(tileController);
    layers->                RegisterTileController(tileController);

    layers->                RegisterBrushManager(ui->brushManager);
    ui->brushManager->      RegisterTileSelector(tileSelector);

    connect(ui->actionAdd_Layer,        SIGNAL(triggered()),                resources,                  SLOT(AddTileLayer()));
    connect(layers,                     SIGNAL(SelectNewTile(TileCoord)),   tileSelector,               SLOT(SelectNewTile(TileCoord)));
    connect(ui->layerList,              SIGNAL(LayerSelectionChanged(int)), tileController,             SLOT(SetLayerSelection(int)));
    connect(ui->layerList,              SIGNAL(LayerSelectionChanged(int)), layers,                     SLOT(SetLayerSelection(int)));
    connect(ui->gridToggle,             SIGNAL(toggled(bool)),              layers,                     SLOT(ToggleGrid(bool)));
    connect(ui->actionSelect_Tileset,   SIGNAL(triggered()),                tileSelector,               SLOT(SelectTileset()));
    connect(ui->selectionTool,          SIGNAL(toggled(bool)),              layers,                     SLOT(ToggleSelectionMode(bool)));
    connect(ui->levelView,              SIGNAL(TraverseTileHistory(bool)),  tileSelector,               SLOT(TraverseTileHistory(bool)));
    connect(resources->GetClipboard(),  SIGNAL(PasteTiles(QList<Tile>)),    ui->brushManager,           SLOT(PasteTiles(QList<Tile>)));
    connect(ui->brushManager,           SIGNAL(SelectionCut(QList<Tile>)),  resources->GetClipboard(),  SLOT(Copy(QList<Tile>)));


    connect(ui->layerList,              SIGNAL(LayerVisibilityChanged(int,bool)), layers,               SLOT(UpdateLayerVisibility(int,bool)));

    connect(resources,                  SIGNAL(LayerAdded(int)),            layers,                     SLOT(AddLayer(int)));
    connect(resources,                  SIGNAL(LayerAdded(int)),            ui->layerList,              SLOT(AddLayer(int)));

    ui->levelView->setScene(layers);
    ui->levelView->setMouseTracking(true);

    ui->miniMap->setScene(layers);
    ui->tileSelectorView->setScene(tileSelector);
    ui->resourceView->RepopulateEverything();

    //assign IDs to each child in the toolgroup
    ui->toolGroup->setId(ui->pencilTool,        0);
    ui->toolGroup->setId(ui->eraserTool,        1);
    ui->toolGroup->setId(ui->bucketTool,        2);
    ui->toolGroup->setId(ui->lineTool,          3);
    ui->toolGroup->setId(ui->stampTool,         4);
    ui->toolGroup->setId(ui->eyedropperTool,    5);
    ui->toolGroup->setId(ui->selectionTool,     6);
    ui->toolGroup->setId(ui->scatterTool,       7);
    ui->toolGroup->setId(ui->scatterFillTool,   8);
    ui->toolGroup->setId(ui->replacerTool,      9);
    ui->toolGroup->setId(ui->matrixBrushTool,   10);
    ui->toolGroup->setId(ui->smartBrushTool,    11);

    ui->brushManager->SetCurrentBrush(0);

    statusBar()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;

    resources->DestroyAllResources();
    
    delete resources;
    //call a cleanup function?
    delete layers;
    delete tileSelector;
    delete levelPropertiesWindow;
}

void MainWindow::on_actionProperties_triggered()
{
    levelPropertiesWindow->LoadValues();
    levelPropertiesWindow->exec();
}

void MainWindow::on_actionUndo_triggered()
{
    if(resources)
    {
        resources->Undo();
        RepopulateEverything();
    }
}

void MainWindow::on_actionRedo_triggered()
{
    if(resources)
    {
        resources->Redo();
        RepopulateEverything();
    }
}

void MainWindow::RepopulateEverything()
{
    tileSelector->RepopulateTileSelector();

    ui->resourceView->RepopulateEverything();
}

void MainWindow::on_actionAdd_Image_triggered()
{
    //ask the user where to load the file from.
    QString filename = QFileDialog::getOpenFileName(this, "Add Image", ".", "Portable Network Graphics (*.png)");

    //if the dialog succeeds
    if(!filename.isEmpty())
    {
        Image *tempImage = new Image;

        //load the file
        tempImage->SetImageFromFile(filename);

        //add the image to the resource manager
        resources->AddImage(tempImage);
    }
}

void MainWindow::CenterMinimapOnLevel()
{
    QPointF topLeft     = ui->levelView->mapToScene( 0, 0 );
    QPointF bottomRight = ui->levelView->mapToScene( ui->levelView->viewport()->width() - 1, ui->levelView->viewport()->height() - 1 );
    QRectF rect(topLeft, bottomRight);

    int centerX = rect.left() + (rect.width() / 2);
    int centerY = rect.top() + (rect.height() / 2);

    ui->miniMap->centerOn(centerX, centerY);
}

void MainWindow::SetToolSelection(QCursor newCursor, int newSelection)
{
    ui->toolGroup->button(newSelection)->setChecked(true);
    ui->levelView->SetCursor(newCursor);
}


void MainWindow::on_actionCut_triggered()
{
    ui->brushManager->CutTiles();
    tileController->ClearPreview();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->brushManager->CopyTiles();
}

void MainWindow::on_actionPaste_triggered()
{
    resources->GetClipboard()->Paste();
}

void MainWindow::on_toolBox_currentChanged(int index)
{
    switch(index)
    {
    case 0: //resources
        ui->resourceView->RefreshNames();
        break;

    case 1: //tile painting
        ui->layerList->RefreshNames();
        break;

    default:
        break;
    }
}
