#include "costtargeting.h"
#include "ui_costtargeting.h"

Costtargeting::Costtargeting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Costtargeting)
{
    ui->setupUi(this);
}

Costtargeting::~Costtargeting()
{
    delete ui;
}
