#include <QColor>
#include <QtDebug>
#include <QTextStream>

#include "WordReader.h"

WordRecognizer::WordRecognizer(const QString& filename, QObject *parent) :
    QObject(parent)
{
    loadSignTemplates(filename);
}

void WordRecognizer::loadSignTemplates(const QString& filename)
{
    QFile fxml(filename);

    if (fxml.open(QIODevice::ReadOnly | QIODevice::Text))
        parseTemplateFile(fxml);
    else emit error(QString::fromUtf8("Невозможно открыть XML-файл"));
}

void WordRecognizer::parseTemplateFile(QFile& fxml)
{
    QXmlStreamReader parser(&fxml);
    QXmlStreamReader::TokenType token;

    while (!parser.atEnd() && !parser.hasError())
    {
        token = parser.readNext();

        if (token == QXmlStreamReader::StartElement
            && parser.name() == "sign")
        {
            readSignTemplate(parser);
        }
    }

    fxml.close();
}

void WordRecognizer::readSignTemplate(QXmlStreamReader& parser)
{
    QXmlStreamAttributes attrs = parser.attributes();
    SignTemplate temp(attrs.value("name").toString(),
                        attrs.value("pointCount").toString().toInt());
    parser.readNext();

    while (parser.tokenType() != QXmlStreamReader::EndElement
           || parser.name() != "sign")
    {
        if (parser.tokenType() == QXmlStreamReader::StartElement
            && parser.name() == "model")
        {
            readSignModel(parser, temp);
        }
        parser.readNext();
    }
}

void WordRecognizer::readSignModel(QXmlStreamReader& parser, SignTemplate &temp)
{
    QString model = parser.readElementText().simplified();
//    qDebug() << "str" << model.length();

    for (int i = 0; i < model.length(); ++i)
    {
        switch (model[i].toLatin1())
        {
        case 'A': temp << Above;
            break;
        case 'B': temp << Below;
            break;
        case 'N': temp << Near;
            break;
        default:
            break;
        }
    }
    signTemplates << temp;
    parser.readNext();
}

void WordRecognizer::setWordImage(const QImage& img)
{
    wordImg = img;
}

void WordRecognizer::setDefaultState()
{
    signPoints.clear();
    word.clear();
    element.clear();
}

QString WordRecognizer::recognize()
{
    setDefaultState();
    binarizeImage();

    startPoint = getStartPoint();

    while (...)
    {
        detachElement();
        bool success = recognizeElement();

        startPoint = element.getSplitPoint();

        if (success) addElementAsSign();
        else recoverElement();
    }
//  emit binarized(wordImg);
//  tr("<not recognized, extCount = %1>").arg(currSign.count());
}

void WordRecognizer::addElementAsSign()
{
    word << element;
    element.setDefaultState();
}

void WordRecognizer::recoverElement()
{
    element.removeTopCoords();

    // add skipped extremum
    int y = wordImg.height() - coordY;
    element.addCoord(y);
    coordY ++;
    //  wordImg.setPixel(cordX, cordY, qRgb(255,0,0));
}

QString WordRecognizer::recognizeElement()
{
    foreach (SignTemplate sign, signTemplates)
    {
        if (element.count() == sign.getPointCount())
        {
            if (isEqval(sign))
            {
                element.setName(sign.getName());
                return true;
            }
        }
    }

    return false;
}

bool WordRecognizer::isEqval(SignTemplate &sign)
{
    int i = 0;
    int j = i + 1;

    int index = 0;

    foreach (Position pos, sign)
    {
        bool isOccur = false;

        switch (pos) {
        case Above:
            isOccur = element[i] > element[j];
            break;
        case Below:
            isOccur = element[i] < element[j];
            break;
        case Near:
            isOccur = true;
            break;
        default:
            break;
        }

        if (isOccur) {
            j ++;

            if (j == element.count()) {
                i ++;
                j = i + 1;
            }
        }
        else {
            qDebug() << i-1 << element[i];
            qDebug() << j-1 << element[j];
            qDebug() << index-1 << sign[index];
            return false;
        }
        index ++;
    }

    return true;
}

void WordRecognizer::detachElement()
{
    coordX = startPoint.x();
    coordY = startPoint.y();

    moveDirection = Down;
    extremumType = Min;

    do {
        switch (moveDirection) {
        case Left:
            if (binImgMap[coordY+1][coordX]) {
                if (!binImgMap[coordY][coordX-1])
                    coordX --;
                else if (!binImgMap[coordY-1][coordX])
                    moveUp();
                else moveRight();
            }
            else moveDown();
            break;
        case Up:
            if (binImgMap[coordY][coordX-1]) {
                if (!binImgMap[coordY-1][coordX])
                    coordY --;
                else if (!binImgMap[coordY][coordX+1])
                    moveRight();
                else moveDown();
            }
            else moveLeft();
            break;
        case Right:
            if (binImgMap[coordY-1][coordX]) {
                if (!binImgMap[coordY][coordX+1])
                    coordX ++;
                else if (!binImgMap[coordY+1][coordX])
                    moveDown();
                else moveLeft();
            }
            else moveUp();
            break;
        case Down:
            if (binImgMap[coordY][coordX+1]) {
                if (!binImgMap[coordY+1][coordX])
                    coordY ++;
                else if (!binImgMap[coordY][coordX-1])
                    moveLeft();
                else moveUp();
            }
            else moveRight();
            break;
        default:
            break;
        }
        // получаем трек точек
//        symbolImg.setPixel(cordX, cordY, qRgb(255,0,0));
    }
    while (coordX != startPoint.x()
           || coordY != startPoint.y());

    moveDown();
}

void WordRecognizer::moveLeft()
{
    coordX --;
    moveDirection = Left;
}

void WordRecognizer::moveUp()
{
    coordY --;
    moveDirection = Up;

    if (extremumType == Min)
    {
        int y = wordImg.height() - coordY;
        element.addCoord(y);
        extremumType = Max;
//        wordImg.setPixel(cordX, cordY, qRgb(255,0,0));
    }
}

void WordRecognizer::moveRight()
{
    coordX ++;
    moveDirection = Right;
}

void WordRecognizer::moveDown()
{
    if (extremumType == Min){
        coordY ++;
        moveDirection = Down;
    }
    else if (!element.getIsTopOfSign())
        splitSignConnection();
}

void WordRecognizer::splitSignConnection()
{
    element.setSplitPoint(QPoint(coordX, coordY));
    element.incWidth();

    // отделяем букву (двигаемся вверх)
    while (!binImgMap[coordY--][coordX])
    {
        binImgMap[coordY][coordX] = White;
        coordY --;
    }

    moveDirection = Up;
}

void WordRecognizer::binarizeImage()
{
    double grayMean = getImageGrayMean();

    int height = wordImg.height();
    int width = wordImg.width();

//    QRgb black = qRgb(0,0,0);
//    QRgb white = qRgb(255,255,255);

    // инитим бинарную матрицу
    binImgMap = new bool* [height];
    for (int i = 0; i < height; i ++)
        binImgMap[i] = new bool [width];

    // заполняем
    for (int i = 0; i < height; i ++)
        for (int j = 0; j < width; j ++)
        {
            binImgMap[i][j] = (qRed(wordImg.pixel(j, i)) < grayMean) ? 0 : 1;
//            if (qRed(wordImg.pixel(j, i)) > grayMean)
//                wordImg.setPixel(j, i, white);
//            else wordImg.setPixel(j, i, black);
        }

//    emit binarized(wordImg);
}

QPoint WordRecognizer::getStartPoint()
{
    int height = wordImg.height();
    int width = wordImg.width();

    // ищем первую левую точку буквы
    for (int j = width-1; j >= 0; j --)
        for (int i = 0; i < height; i ++)
            if (!binImgMap[i][j])
                return QPoint(j, i);

    return QPoint();
}

double WordRecognizer::getImageGrayMean()
{
    double Gmean = 0;
    int height = wordImg.height();
    int width = wordImg.width();

    for (int i = 0; i < width; i ++)
        for (int j = 0; j < height; j ++)
            Gmean += qRed(wordImg.pixel(i, j));

    return Gmean / (width * height);;
}
