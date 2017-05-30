#ifndef NETWORKDESIGN_H
#define NETWORKDESIGN_H

#include <QWidget>

namespace Ui {
class NetworkDesign;
}

class NetworkDesign : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkDesign(QWidget *parent = 0);
    ~NetworkDesign();

private:
    Ui::NetworkDesign *ui;
};

#endif // NETWORKDESIGN_H
