#include "SmartBrush.h"

void SmartBrush::Paint(int x, int y, LayerGroup *layer, bool preview)
{
    //erase the previous preview, if we are in preview mode. Get ready for the next
    if(preview)
        layer->ClearPreview();

    //paint the middle tiles. These get painted over everything
    int radius = size;

    //y dimension
    for(signed int i = -radius + 1; i < radius; i++)
    {
        //x dimension
        for(signed int j = -radius + 1; j < radius; j++)
        {
            if(overwrite || layer->GetTileType(j + x, i + y) == 0)
            {
                if(preview)
                    layer->PreviewModifyTile(j + x, i + y, GetRandomTile(4));
                else
                    layer->ModifyTile(j + x, i + y, GetRandomTile(4));
            }
        }
    }

    //paint the surrounding tiles. These do NOT paint over the middle tiles

    //loop for the diameter
    for(int i = -radius + 1; i < radius; i++)
    {
        //midle left
        if(!ListContainsTile(4, layer->GetTileType(x - radius, y + i)))
        {
            if(overwrite || layer->GetTileType(x - radius, y + i) == 0)
            {
                ContextPaintTile(x - radius, y + i, layer, preview);
            }
        }

        //midle right
        if(!ListContainsTile(4, layer->GetTileType(x + radius, y + i)))
        {
            if(overwrite || layer->GetTileType(x + radius, y + i) == 0)
            {
                ContextPaintTile(x + radius, y + i, layer, preview);
            }
        }

        //top middle
        if(!ListContainsTile(4, layer->GetTileType(x + i, y - radius)))
        {
            if(overwrite || layer->GetTileType(x + i, y - radius) == 0)
            {
                ContextPaintTile(x + 1, y - radius, layer, preview);
            }
        }

        //bottom middle
        if(!ListContainsTile(4, layer->GetTileType(x + i, y + radius)))
        {
            if(overwrite || layer->GetTileType(x + i, y + radius) == 0)
            {
                ContextPaintTile(x + i, y + radius, layer, preview);
            }
        }
    }

    //top left
    if(!ListContainsTile(4, layer->GetTileType(x - radius, y - radius)))
    {
        if(overwrite || layer->GetTileType(x - radius, y - radius) == 0)
        {
            ContextPaintTile(x - radius, y - radius, layer, preview);
        }
    }

    //top right
    if(!ListContainsTile(4, layer->GetTileType(x + radius, y - radius)))
    {
        if(overwrite || layer->GetTileType(x + radius, y - radius) == 0)
        {
            ContextPaintTile(x + radius, y - radius, layer, preview);
        }
    }

    //bottom left
    if(!ListContainsTile(4, layer->GetTileType(x - radius, y + radius)))
    {
        if(overwrite || layer->GetTileType(x - radius, y + radius) == 0)
        {
             ContextPaintTile(x - radius, y + radius, layer, preview);
        }
    }

    //bottom right
    if(!ListContainsTile(4, layer->GetTileType(x + radius, y + radius)))
    {
        if(overwrite || layer->GetTileType(x + radius, y + radius) == 0)
        {
            ContextPaintTile(x + radius, y + radius, layer, preview);
        }
    }
}

void SmartBrush::ContextPaintTile(int x, int y, LayerGroup *layer, bool preview)
{
    //check for middle tiles that pinch this tile. That means paint a middle tile
    if(ListContainsTile(4, layer->GetTileType(x - 1, y)) && ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(4));
        else
            layer->ModifyTile(x, y, GetRandomTile(4));
    }

    if(ListContainsTile(4, layer->GetTileType(x, y - 1)) && ListContainsTile(4, layer->GetTileType(x, y + 1)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(4));
        else
            layer->ModifyTile(x, y, GetRandomTile(4));
    }

    if(ListContainsTile(4, layer->GetTileType(x - 1, y - 1)) && ListContainsTile(4, layer->GetTileType(x + 1, y + 1)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(4));
        else
            layer->ModifyTile(x, y, GetRandomTile(4));
    }

    if(ListContainsTile(4, layer->GetTileType(x - 1, y + 1)) && ListContainsTile(4, layer->GetTileType(x + 1, y - 1)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(4));
        else
            layer->ModifyTile(x, y, GetRandomTile(4));
    }

    //check for side tiles

    //left side
    if(ListContainsTile(4, layer->GetTileType(x + 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x - 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(3));
        else
            layer->ModifyTile(x, y, GetRandomTile(3));

        return;
    }

    //right side
    if(ListContainsTile(4, layer->GetTileType(x - 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(5));
        else
            layer->ModifyTile(x, y, GetRandomTile(5));

        return;
    }

    //top side
    if(ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x + 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x - 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x, y - 1)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(1));
        else
            layer->ModifyTile(x, y, GetRandomTile(1));

        return;
    }

    //bottom side
    if(ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x + 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x - 1, y)) &&
       !ListContainsTile(4, layer->GetTileType(x, y + 1)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(7));
        else
            layer->ModifyTile(x, y, GetRandomTile(7));

        return;
    }

    //top left
    if(ListContainsTile(4, layer->GetTileType(x + 1, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(0));
        else
            layer->ModifyTile(x, y, GetRandomTile(0));

        return;
    }

    //top right
    if(ListContainsTile(4, layer->GetTileType(x - 1, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       !ListContainsTile(4, layer->GetTileType(x - 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(2));
        else
            layer->ModifyTile(x, y, GetRandomTile(2));

        return;
    }

    //bottom left
    if(ListContainsTile(4, layer->GetTileType(x + 1, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(6));
        else
            layer->ModifyTile(x, y, GetRandomTile(6));

        return;
    }

    //bottom right
    if(ListContainsTile(4, layer->GetTileType(x - 1, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       !ListContainsTile(4, layer->GetTileType(x - 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(8));
        else
            layer->ModifyTile(x, y, GetRandomTile(8));

        return;
    }

    //top left corner
    if(ListContainsTile(4, layer->GetTileType(x + 1, y + 1)) &&
       ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(9));
        else
            layer->ModifyTile(x, y, GetRandomTile(9));

        return;
    }

    //top right
    if(ListContainsTile(4, layer->GetTileType(x - 1, y + 1)) &&
       ListContainsTile(4, layer->GetTileType(x, y + 1)) &&
       ListContainsTile(4, layer->GetTileType(x - 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(10));
        else
            layer->ModifyTile(x, y, GetRandomTile(10));

        return;
    }

    //bottom left
    if(ListContainsTile(4, layer->GetTileType(x + 1, y - 1)) &&
       ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       ListContainsTile(4, layer->GetTileType(x + 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(11));
        else
            layer->ModifyTile(x, y, GetRandomTile(11));

        return;
    }

    //bottom right
    if(ListContainsTile(4, layer->GetTileType(x - 1, y - 1)) &&
       ListContainsTile(4, layer->GetTileType(x, y - 1)) &&
       ListContainsTile(4, layer->GetTileType(x - 1, y)))
    {
        if(preview)
            layer->PreviewModifyTile(x, y, GetRandomTile(12));
        else
            layer->ModifyTile(x, y, GetRandomTile(12));

        return;
    }
}


SmartBrush::SmartBrush()
{
    AddList(13);
}
