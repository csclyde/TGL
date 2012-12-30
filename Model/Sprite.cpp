#include "Sprite.h"

Sprite::Sprite()
{
    imageID = 0;
}

bool Sprite::Export(Exporter *exporter)
{
    exporter->WriteString(name);
    exporter->WriteInt(imageID);

    return true;
}

bool Sprite::Import(Exporter *exporter)
{
    exporter->ReadString(name);
    exporter->ReadInt(imageID);

    return true;
}

void Sprite::DestroyAllAnimations()
{
    //loop through and destroy all animations
    for(int i = 0; i < children.count(); i++)
    {
        static_cast<Animation*>(children[i])->DestroyAllFrames();
        delete children[i];
    }

    children.clear();
}

void Sprite::AddAnimation(Animation *newAnimation)
{
    AddChild(newAnimation);
}

void Sprite::DeleteAnimation(int ID)
{
    RemoveChild(ID);
}

void Sprite::SetImageID(int newImageID)
{
    imageID = newImageID;
}

Animation *Sprite::GetAnimationByIndex(int index)
{
    return static_cast<Animation*>(GetChildByIndex(index));
}
