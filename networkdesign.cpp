#include "networkdesign.h"
#include "ui_networkdesign.h"

NetworkDesign::NetworkDesign(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkDesign)
{
    ui->setupUi(this);
}

NetworkDesign::~NetworkDesign()
{
    delete ui;
}
