#ifndef COSTTARGETING_H
#define COSTTARGETING_H

#include <QWidget>

namespace Ui {
class Costtargeting;
}

class Costtargeting : public QWidget
{
    Q_OBJECT

public:
    explicit Costtargeting(QWidget *parent = 0);
    ~Costtargeting();

private:
    Ui::Costtargeting *ui;
};

#endif // COSTTARGETING_H
