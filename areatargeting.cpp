#include "areatargeting.h"
#include "ui_areatargeting.h"

areatargeting::areatargeting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::areatargeting)
{
    ui->setupUi(this);
}

areatargeting::~areatargeting()
{
    delete ui;
}
