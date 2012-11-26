#include "Image.h"

Image::Image()
{
    image = NULL;
}

Image::~Image()
{
    if(image)
        delete image;
}

bool Image::SaveToFile(QFile &file)
{
    return false;
}

bool Image::LoadFromFile(QFile &file)
{
    return false;
}

void Image::SetImageFromFile(QString filename)
{
    //check if the image already exists
    if(image != NULL)
    {
        //if so, delete it and nullify it
        delete image;
        image = NULL;
    }

    //create a new image
    image = new QImage;
    //and load it from the specified filename
    image->load(filename);    
}

QImage *Image::GetImage()
{
    return image;
}
