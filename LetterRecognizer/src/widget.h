#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "WordReader.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void showImage(QImage img);

private:
    Ui::Widget *ui;
    WordRecognizer* recognizer;

};

#endif // WIDGET_H
