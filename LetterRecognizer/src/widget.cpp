#include <QFileDialog>

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);

    recognizer = new WordRecognizer(":/template/template/sign_templates.xml");
    connect(recognizer, SIGNAL(binarized(QImage)), this, SLOT(showImage(QImage)));

    // fast start
    on_pushButton_clicked();
//    on_pushButton_2_clicked();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                 "Images (*.png *.pgm *.jpg)");
    QImage letterImg(fname);
    recognizer->setWordImage(letterImg);

    showImage(letterImg);
}

void Widget::on_pushButton_2_clicked()
{
    ui->textBrowser->setText(recognizer->recognize());
}

void Widget::showImage(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img));
}

