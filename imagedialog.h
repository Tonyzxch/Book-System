#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

class ImageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImageDialog(const QPixmap &pixmap, QWidget *parent = nullptr);

private:
    QLabel *imageLabel;
};

#endif // IMAGEDIALOG_H
