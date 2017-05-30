#include "problemtable.h"
#include "ui_problemtable.h"
#include "duvaloraction.h"
#include "valoraction.h"
#include "graficos.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

problemtable::problemtable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::problemtable)
{
    ui->setupUi(this);
    ui->groupBox_3->setVisible(false);
    ui->label1->setVisible(false);
    ui->label2->setVisible(false);
    ui->K->setVisible(false);
    ui->Min->setVisible(false);
}

problemtable::~problemtable()
{
    delete ui;
}

void problemtable::on_uniform_clicked()
{
    radiobuttons();
}

void problemtable::on_diverse_clicked()
{
    radiobuttons();
}

void problemtable::radiobuttons()
{
    ui->groupBox_3->setVisible(false);
    ui->label1->setVisible(false);
    ui->label2->setVisible(false);
    ui->K->setVisible(false);
    ui->Min->setVisible(false);
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
    if(uniforme == true){
        ui->groupBox_3->setVisible(true);
        ui->label2->setVisible(true);
        ui->Min->setVisible(true);
    }else if(diverso == true){
        ui->groupBox_3->setVisible(true);
        ui->label1->setVisible(true);
        ui->label2->setVisible(true);
        ui->K->setVisible(true);
        ui->Min->setVisible(true);
    }
}

void problemtable::on_pushButton_clicked()
{
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
    QFile F(WORKSPACE_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
        return;
    }
    QDataStream in2(&F);
    in2.setVersion(QDataStream::Qt_5_4);
    QVector<QVector<double>> prueba;
    int filas = 10; // default
    int columnas = 10; //defalt
    prueba.resize(filas);
    for(int i = 0; i < filas; i++)
    {
        prueba[i].resize(columnas);
    }
    Workspace MATRIZ(prueba);
    in2 >> MATRIZ;
    QVector<QVector<double>> Matriz = MATRIZ.getMatriz();
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> Wcp(Matriz.size()),h(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        Wcp[i] = Matriz[i][2];
        h[i] = Matriz[i][3];
    }
    F.flush();
    F.close();
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    int UTemp = units.getUTemp();
    int UWcp = units.getUWcp();;
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,Wcp,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    Wcp = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    double DTmin = ui->Min->value();
    double K = ui->K->value();
    if(uniforme==true){
        Plot_curvascompuestasajustadas plot(TS,TE,Wcp,DTmin);
        QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
        QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
        QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
        QVector<double> Deficit = plot.getDeficit();
        ui->Tabla->setRowCount(VecCorrientesTotal.size()-1);
        int row = ui->Tabla->rowCount();
        double val1, val2, val3,val4,val5,val6,val7;
        int k=1,j=0;
        for(int i =0; i < row ; i++){
            val1 = VecCorrientesTotal[k][0];
            val2 = VecCorrientesTotal[k][1];
            val3 = Deficit[j];
            val4 = VecHeatFlow[j][0];
            val5 = VecHeatFlow[j][1];
            val6 = VecAdjHeatFlow[j][0];
            val7 = VecAdjHeatFlow[j][1];
            ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
            ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
            ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
            ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
            ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
            k=k+1;
            j=j+1;
        }
    }else if(diverso==true){
        float punto1 = 0.05;
        float punto2 = 10.0;
        Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,Wcp,h,DTmin,K,punto1,punto2);
        QVector<QVector<double>> VecCorrientesTotal = plot2.getVectorCorrientesTotal();
        QVector<QVector<double>> VecHeatFlow = plot2.getVecHeatFlow();
        QVector<QVector<double>> VecAdjHeatFlow = plot2.getVecAdjHeatFlow();
        QVector<double> Deficit = plot2.getDeficit();
        ui->Tabla->setRowCount(VecCorrientesTotal.size()-1);
        int row = ui->Tabla->rowCount();
        double val1, val2, val3,val4,val5,val6,val7;
        int k=1,j=0;
        for(int i =0; i < row ; i++){
            val1 = VecCorrientesTotal[k][0];
            val2 = VecCorrientesTotal[k][1];
            val3 = Deficit[j];
            val4 = VecHeatFlow[j][0];
            val5 = VecHeatFlow[j][1];
            val6 = VecAdjHeatFlow[j][0];
            val7 = VecAdjHeatFlow[j][1];
            ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
            ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
            ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
            ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
            ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
            k=k+1;
            j=j+1;
        }
    }
}
