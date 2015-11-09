#include "Sign.h"

Sign::Sign() : QVector<int>()
{
    setDefaultState();
}

void Sign::setDefaultState()
{
    clear();
    width = 0;
    splitCoordIndex = 0;
}

void Sign::addCoord(int coord)
{
    append(coord);

    if (isTopOfSign)
        splitCoordIndex ++;
}

void Sign::removeTopCoords()
{
    remove(splitCoordIndex ,count()-1);
}

QPoint Sign::getSplitPoint() const
{
    return splitPoint;
}

void Sign::setSplitPoint(const QPoint &value)
{
    splitCoordIndex = count();
    splitPoint = value;
}

int Sign::getWidth() const
{
    return width;
}

void Sign::incWidth()
{
    width ++;
}

SignTemplate::SignTemplate() : QVector<Position>() {}

SignTemplate::SignTemplate(const QString &_name, int _pointCount) :
    QVector<Position>()
{
    name = _name;
    pointCount = _pointCount;
}

int SignTemplate::getPointCount() const
{
    return pointCount;
}

void SignTemplate::setPointCount(int value)
{
    pointCount = value;
}

QString SignTemplate::getName() const
{
    return name;
}

void SignTemplate::setName(const QString &value)
{
    name = value;
}

