#include "plots.h"
#include "ui_plots.h"
#include "contenido_plots.h"
#include "contenido_plots_area.h"
#include "duvaloraction.h"
#include "valoraction.h"
#include "contenido_plots_cc.h"
#include "QFile"
#include "QDataStream"
#include "QMessageBox"
#include "contenido_plots_costos.h"
#include "plotter.h"

plots::plots(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plots)
{
    ui->setupUi(this);
    QFile F(TYPEOPERATION_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
        return;
    }
    QDataStream in5(&F);
    in5.setVersion(QDataStream::Qt_5_4);
    Valordeoperacion valor;
    while(!in5.atEnd()){
        in5 >> valor;
        if(valor.getvalor() == 0){  //NEW
            // aqui no se hace nada no se tiene nada de nada
        }else if(valor.getvalor() == 1){
            ui->tabWidget->addTab(new contenido_plots_cc, QString("Compositives curves " ));
            ui->tabWidget->addTab(new Contenido_PLOTS(), QString("Compositives curves adjusted "));
            ui->tabWidget->addTab(new Contenido_PLOTS(), QString("Grand compositive curve "));
        }else{
            ui->tabWidget->addTab(new contenido_plots_cc, QString("Compositives curves " ));
            ui->tabWidget->addTab(new Contenido_PLOTS(), QString("Compositives curves adjusted "));
            ui->tabWidget->addTab(new Contenido_PLOTS(), QString("Grand compositive curve "));
            ui->tabWidget->addTab(new contenido_plots_area(), QString("Area curve "));
            ui->tabWidget->addTab(new contenido_plots_costos(),QString("Cost curve "));
        }
    }
    F.close();
    //
    QFile Fil(TABPLOT_FILENAME);
    if (!Fil.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
        return;
    }
    QDataStream out4(&Fil);
    out4.setVersion(QDataStream::Qt_5_4);
    Tabplot tabvalue(0);
    tabvalue.settabvalue(0);
    out4 << tabvalue;
    Fil.flush();
    Fil.close();
    K=0;
}

plots::~plots()
{
    delete ui;
}



void plots::on_tabWidget_currentChanged(int index)
{
    QFile F(TABPLOT_FILENAME);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out4(&F);
    out4.setVersion(QDataStream::Qt_5_4);
    Tabplot tabvalue(index);
    out4 << tabvalue;
    F.flush();
    F.close();
}

void plots::on_pushButton_clicked()
{
    K = K + 1 ;
    ui->tabWidget->addTab(new plotter(), QString("Plot %0").arg(K));
}

void plots::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
