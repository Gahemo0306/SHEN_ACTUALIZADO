#ifndef AREATARGETING_H
#define AREATARGETING_H

#include <QWidget>

namespace Ui {
class areatargeting;
}

class areatargeting : public QWidget
{
    Q_OBJECT

public:
    explicit areatargeting(QWidget *parent = 0);
    ~areatargeting();

private:
    Ui::areatargeting *ui;
};

#endif // AREATARGETING_H
