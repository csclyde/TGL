#ifndef BrushManager_H
#define BrushManager_H

#include <QFrame>
#include <QTime>

#include "../../Dialogs/BrushPropertiesDialog.h"

#include "TileBrush.h"
#include "PencilBrush.h"
#include "EraserBrush.h"
#include "FillBrush.h"
#include "EyedropperBrush.h"
#include "LineBrush.h"
#include "SmartBrush.h"
#include "ScatterBrush.h"
#include "ReplacerBrush.h"
#include "MatrixBrush.h"
#include "StampBrush.h"
#include "SelectionBrush.h"

namespace Ui {
class BrushManager;
}

class BrushManager : public QFrame
{
    Q_OBJECT
    
public:
    explicit BrushManager(QWidget *parent = 0);
    ~BrushManager();

    TileBrush *GetCurrentBrush() { return currentBrush; }

    void RepopulateBrushLists();

    void RegisterTileSelector(QGraphicsScene *selector) { propertiesWindow.RegisterTileSelector(selector); }
    void RegisterResourceManager(ResourceManager *newRM) { propertiesWindow.RegisterResourceManager(newRM); }

    void DestroyBrushes();

    enum
    {
        Pencil = 0,
        Eraser,
        Bucket,
        Line,
        Stamp,
        Eyedropper,
        Selector,
        Scatter,
        ScatterFill,
        Replacer,
        Matrix,
        Smart
    };
    
signals:
    void BrushChanged(QCursor newCursor, int type);
    void SelectionCut(QList<TileData> tiles);

public slots:
    void SetCurrentBrush(int type);
    void RevertToPreviousSingleTileBrush();
    void SetSelectedTiles(TileList newList);

    void CutTiles();
    void CopyTiles();
    void PasteTiles(QList<TileData> pasteTiles);

    void on_overwriteCheckbox_toggled(bool checked);
    void on_brushSizeInput_valueChanged(int arg1);
    void on_addScatterBrush_clicked();
    void on_scatterBrushCombo_currentIndexChanged(int index);
    void on_editScatterBrush_clicked();
    void on_deleteScatterBrush_clicked();
    void on_smartBrushCombo_currentIndexChanged(int index);
    void on_addSmartBrush_clicked();
    void on_editSmartBrushes_clicked();
    void on_addReplacerBrush_clicked();
    void on_editReplacerBrush_clicked();
    void on_deleteReplacerBrush_clicked();
    void on_replacerBrushCombo_currentIndexChanged(int index);
    void on_addMatrixBrush_clicked();
    void on_editMatrixBrush_clicked();
    void on_deleteMatrixBrush_clicked();
    void on_matrixBrushCombo_currentIndexChanged(int index);

private:
    Ui::BrushManager *ui;
    BrushPropertiesDialog propertiesWindow;

    TileBrush *currentBrush;
    PencilBrush pencil;
    EraserBrush eraser;
    FillBrush bucket;
    EyedropperBrush eyedropper;
    LineBrush line;
    StampBrush stamp;
    SelectionBrush selector;

    QList<ScatterBrush*> scatter;
    int scatterBrushIndex;

    QList<SmartBrush*> smart;
    int smartBrushIndex;

    QList<ReplacerBrush*> replacer;
    int replacerBrushIndex;

    QList<MatrixBrush*> matrix;
    int matrixBrushIndex;

    int singleTileBrush;
};

#endif // BrushManager_H
