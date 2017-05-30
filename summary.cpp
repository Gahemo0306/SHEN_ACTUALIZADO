#include "summary.h"
#include "ui_summary.h"
#include <QMessageBox>
#include "duvaloraction.h"
#include "graficos.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include "valoraction.h"

Summary::Summary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Summary)
{
    ui->setupUi(this);
    QFile FileMadre(TAB_A_P);
        if (!FileMadre.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out26(&FileMadre);
    out26.setVersion(QDataStream::Qt_5_4);
    int TabAnali = -1;
    int Ventanamadre = -1;
    TabAnalisis TA(TabAnali,Ventanamadre);
    in26 << TA;
    if(FileMadre == 1){ // VENTANA ANALISIS PERSONALIZADO
        if(TabAnali == 0){ // uniforme
            QFile F(INFO_A_P_UNIFORME);
                if (!F.open(QIODevice::WriteOnly)){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in25(&F);
            in25.setVersion(QDataStream::Qt_5_4);
            QVector<double> Checkboxes1;
            Checkboxes1.resize(8);
            double Min1 = 0; Max1 = 0; Inc1 = 0;
            bool estatico1 = false, incremento1 = false;
            infoapuniforme info(Checkboxes1,Min1,Max1,Inc1,estatico1,incremento1);
            in25 >> info;
            QVector<double> Checkboxes = info.getCheckboxes();
            double Min = info.getMin();
            double Max = info.getMax();
            double Inc = info.getInc();
            bool Estatico = info.getestatico();
            bool Incremento = info.getincremento();
            //TAMAÑO DE 8 EL VECTOR
            ui->treeWidget->setColumnCount(1);
            ui->treeWidget->setHeaderLabel("Information avaiable:");
            QString LISTA1;
            QStringList LISTA2;
            if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                LISTA1 = "Problem table";
                LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location fo the pinch point";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                LISTA1 = "Area prediction";
                LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                LISTA1 = "Cost prediction";
                LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[3] == 1){ // CURVA CALIENTE
                LISTA1 = "Hot compositive curve";
                LISTA2 << "Data points";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[4] == 1){ // CURVA FRIA
                LISTA1 = "Cold compositive curve";
                LISTA2 << "Data points";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                LISTA1 = "Compositive curves";
                LISTA2 << "Data points";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                LISTA1 = "Compositive curves adjusted";
                LISTA2 << "Data points";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }else if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                LISTA1 = "Grand compositive curve";
                LISTA2 << "Data points";
                AddRoots(LISTA1,LISTA2);
                LISTA2.clear();
            }
            AddRoots("Report...",LISTA1);
            F.flush();
            F.close();
        }
    }
    FileMadre.flush();
    FileMadre.close();
}

Summary::~Summary()
{
    delete ui;
}

void Summary::AddRoots(QString name, QStringList listas)
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

void Summary::AddChild(QTreeWidgetItem *parent, QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,name);
    QString RutaIcono = ":/resources/Resources/241143.png";
    item->setIcon(0,QIcon(RutaIcono));
    parent->addChild(item);
}
