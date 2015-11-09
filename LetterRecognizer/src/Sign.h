#ifndef SYMBOL_H
#define SYMBOL_H

#include <QObject>
#include <QVector>
#include <QPoint>

enum Position {
    Above,
    Near,
    Below
};

// считываемый символ
class Sign : public QVector<int>
{
public:
    explicit Sign();
    void setDefaultState();

    void addCoord(int coord);
    void removeTopCoords();

    QString getName() const;
    void setName(const QString &value);

    QPoint getSplitPoint() const;
    void setSplitPoint(const QPoint &value);

    int getWidth() const;
    void incWidth();

public slots:

private:
    QString name;
    int width;

    QPoint splitPoint;
    int splitCoordIndex;
};

// шаблон символа для сверки
class SignTemplate : public QVector<Position>
{
public:
    explicit SignTemplate();
    SignTemplate(const QString& _name, int _pointCount);

    int getPointCount() const;
    void setPointCount(int value);

    QString getName() const;
    void setName(const QString &value);

public slots:

private:
    QString name;
    int pointCount;
};

#endif // SYMBOL_H
