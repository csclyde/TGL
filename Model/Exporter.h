#ifndef EXPORTER_H
#define EXPORTER_H

#include <QString>
#include <QPoint>
#include <QImage>
#include <QRect>

class Exporter
{
public:
    Exporter();

    //these functions help write out different types of data
    virtual bool WriteType(int ID, QString type) = 0;
    virtual bool WriteString(QString string) = 0;
    virtual bool WriteInt(int number) = 0;
    virtual bool WriteDouble(double number) = 0;
    virtual bool WriteBool(bool boolean) = 0;
    virtual bool WriteRect(QRect rect) = 0;
    virtual bool WritePoint(QPoint point) = 0;
    virtual bool WriteImage(QImage image) = 0;
    virtual bool WriteVariant(QVariant var) = 0;

    //these functions help read in different types of data
    virtual bool ReadType(int &ID, char type[4]) = 0;
    virtual bool ReadString(QString &string) = 0;
    virtual bool ReadInt(int &number) = 0;
    virtual bool ReadDouble(double &number) = 0;
    virtual bool ReadBool(bool &boolean) = 0;
    virtual bool ReadRect(QRect &rect) = 0;
    virtual bool ReadPoint(QPoint &point) = 0;
    virtual bool ReadImage(QImage &image) = 0;
    virtual bool ReadVariant(QVariant var) = 0;
};

#endif // EXPORTER_H
