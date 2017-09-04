#include "help.h"
#include "ui_help.h"
#include <QtCore>
#include <QtGui>
#include <QDebug>
#include <QDir>

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    ui->scrollArea->setAutoFillBackground(true);
    QPalette pal3 =ui->scrollArea->palette();
    pal3.setColor(QPalette::Window, QColor(Qt::white));
    ui->scrollArea->setPalette(pal3);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("Information avaiable:");
    QStringList LISTA1,LISTA2,LISTA3,LISTA4,LISTA5,LISTA6,LISTA7;
    LISTA1 << "¿What's is SHEN?" << "Programming code" << "Design structure";
    LISTA2 << "Workspace" << "Table problem" << "Plots" << "Summary" ;
    LISTA3 << "Uniform pinch point" << "Diverse pinch point";
    LISTA4 << "Problem table" << "Area targeting" << "Cost targeting" << "Heat networks" << "Plots";
    LISTA5 << "Properties of streams" << "Parameter K" << "Cost Parameters" << "Units conversion" ;
    LISTA6 << "Export .csv files" << "Export pdf";
    LISTA7 << "¿What to do in case of bugs?" << "¿Is the license free?" << "¿How can you contribute?";
    AddRoots("Introduction",LISTA1);
    AddRoots("Interface",LISTA2);
    AddRoots("Methodologies",LISTA3);
    AddRoots("Operations",LISTA4);
    AddRoots("Declaring information",LISTA5);
    AddRoots("Report Options",LISTA6);
    AddRoots("More...",LISTA7);
}

Help::~Help()
{
    delete ui;
}

void Help::AddRoots(QString name, QStringList listas)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,name);
    QString RutaIcono = ":/resources/Resources/help_carpeta.png";
    item->setIcon(0,QIcon(RutaIcono));
    ui->treeWidget->addTopLevelItem(item);
    for(int i = 0 ; i < listas.size(); i++){
        QString texto = listas.at(i);
        AddChild(item,texto);
    }
}

void Help::AddChild(QTreeWidgetItem *parent, QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,name);
    QString RutaIcono = ":/resources/Resources/241143.png";
    item->setIcon(0,QIcon(RutaIcono));
    parent->addChild(item);
    ui->text->setText("<b>Hello</b>");
}

//void Help::HyperText(const QModelIndex &index)
//{
//}

//void Help::on_treeWidget_doubleClicked(const QModelIndex &index)
//{
//    qDebug() << index ;
//    HyperText(index);
//}

//void Help::on_treeWidget_clicked(const QModelIndex &index)
//{
//    qDebug() << ui->treeWidget->currentIndex();
//    qDebug() << ui->treeWidget->currentItem();
//}
