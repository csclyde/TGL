#include "LevelProperties.h"

LevelProperties::LevelProperties()
{
    tileWidth = 16;
    tileHeight = 16;
    mapWidth  = 99;
    mapHeight = 99;
    tilesetID = 0;

    levelName = "";
}

bool LevelProperties::ArePropertiesSet()
{
    if(tileWidth && tileHeight && mapWidth && mapHeight)
        return true;

    return false;
}

void LevelProperties::SetLevelSize(int newW, int newH)
{ 
    if(newW != mapWidth || newH != mapHeight)
    {
        mapWidth = newW; 
        mapHeight = newH; 

        emit MapSizeChanged(newW, newH);
    }
}

bool LevelProperties::Export(Exporter *exporter)
{
    exporter->WriteInt(tileWidth);
    exporter->WriteInt(tileHeight);
    exporter->WriteInt(mapWidth);
    exporter->WriteInt(mapHeight);

    exporter->WriteString(levelName);

    exporter->WriteInt(tilesetID);

    return true;
}

bool LevelProperties::Import(Exporter *exporter)
{
    exporter->ReadInt(tileWidth);
    exporter->ReadInt(tileHeight);
    exporter->ReadInt(mapWidth);
    exporter->ReadInt(mapHeight);

    exporter->ReadString(levelName);

    exporter->ReadInt(tilesetID);

    return true;
}
