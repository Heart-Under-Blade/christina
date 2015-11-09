#ifndef SYMBOLRECOGNIZER_H
#define SYMBOLRECOGNIZER_H

#include <QObject>
#include <QWidget>
#include <QImage>
#include <QVector>
#include <QMatrix>
#include <QStack>
#include <QFile>
#include <QXmlStreamReader>

#include "Sign.h"

enum Direction {
    Left,
    Right,
    Up,
    Down
};

enum Extremum {
    Min,
    Max
};

enum BinColor {
    Black = 0,
    White = 1
};

class WordRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit WordRecognizer(const QString& filename, QObject *parent = 0);

    void setWordImage(const QImage& img);
    QString recognize();

    void setDefaultState();
    void splitSignConnection();
    void recoverElement();
    void addElementAsSign();
public slots:

signals:
    void binarized(QImage);
    void error(QString);

private:
    QImage wordImg;
    QVector<SignTemplate> signTemplates;
    bool **binImgMap;
    QVector<int> signPoints; // сюда запишем игрековые значения найденных экстремумов

    QVector<Sign> word;
    Sign element;

    QPoint startPoint;
    Direction moveDirection;
    Extremum extremumType;

    int coordY;
    int coordX;

    QString recognizeElement();
    bool isEqval(SignTemplate &symbol);

    void detachElement();
    QPoint getStartPoint();
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    void loadSignTemplates(const QString& filename);
    void parseTemplateFile(QFile &fxml);
    void readSignModel(QXmlStreamReader &parser, SignTemplate& temp);
    void readSignTemplate(QXmlStreamReader& parser);

    void binarizeImage();
    double getImageGrayMean();
};

#endif // SYMBOLRECOGNIZER_H
