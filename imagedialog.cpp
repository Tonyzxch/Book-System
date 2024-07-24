#include "imagedialog.h"

ImageDialog::ImageDialog(const QPixmap &pixmap, QWidget *parent) :
    QDialog(parent),
    imageLabel(new QLabel(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    imageLabel->setPixmap(pixmap);
    layout->addWidget(imageLabel);
    setLayout(layout);
    setWindowTitle("Book Image");
    resize(pixmap.size());
}
