#include <algorithm>
#include <numeric>
#include "duvaloraction.h"
#include "valoraction.h"
#include "graficos.h"
#include "qcustomplot.h"
#include "contenido_plots_area.h"
#include "ui_contenido_plots_area.h"

contenido_plots_area::contenido_plots_area(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contenido_plots_area)
{
    ui->setupUi(this);
    ui->groupBox_3->setVisible(false);
    ui->groupBox_4->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->comboBoxF->setVisible(false);
    ui->comboBoxT->setVisible(false);
    ui->labelK->setVisible(false);
    ui->labelMin->setVisible(false);
    ui->labelMax->setVisible(false);
    ui->labelInc->setVisible(false);
    ui->SBk->setVisible(false);
    ui->SBmin->setVisible(false);
    ui->SBmax->setVisible(false);
    ui->SBinc->setVisible(false);
    ui->labelType->setVisible(false);
    ui->TypeDesign->setVisible(false);
    ui->labelAux->setVisible(false);
    ui->Services->setVisible(false);
    ui->help->setVisible(false);
}

contenido_plots_area::~contenido_plots_area()
{
    delete ui;
}

void contenido_plots_area::on_Uniform_clicked()
{
    RADIOBUTTONS();
}

void contenido_plots_area::on_Diverse_clicked()
{
    RADIOBUTTONS();
}

void contenido_plots_area::RADIOBUTTONS()
{
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    FileUnidades.flush();
    FileUnidades.close();
    ui->groupBox_3->setVisible(false);
    ui->groupBox_4->setVisible(false);
    ui->label->setVisible(false);
    ui->label_2->setVisible(false);
    ui->label_3->setVisible(false);
    ui->comboBoxF->setVisible(false);
    ui->comboBoxT->setVisible(false);
    ui->labelK->setVisible(false);
    ui->labelMin->setVisible(false);
    ui->labelMax->setVisible(false);
    ui->labelInc->setVisible(false);
    ui->SBk->setVisible(false);
    ui->SBmin->setVisible(false);
    ui->SBmax->setVisible(false);
    ui->SBinc->setVisible(false);
    ui->labelType->setVisible(false);
    ui->TypeDesign->setVisible(false);
    ui->labelAux->setVisible(false);
    ui->Services->setVisible(false);
    ui->help->setVisible(false);
    bool uniforme = ui->Uniform->isChecked();
    bool diverso = ui->Diverse->isChecked();
    if(uniforme == true){
        ui->groupBox_3->setVisible(true);
        ui->groupBox_4->setVisible(true);
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->comboBoxF->setVisible(true);
        ui->comboBoxT->setVisible(true);
        ui->labelK->setVisible(false);
        ui->labelMin->setVisible(true);
        ui->labelMax->setVisible(true);
        ui->labelInc->setVisible(true);
        ui->labelMin->setText("Minimun value:");
        ui->SBk->setVisible(false);
        ui->SBmin->setVisible(true);
        ui->SBmax->setVisible(true);
        ui->SBinc->setVisible(true);
        ui->labelType->setVisible(true);
        ui->TypeDesign->setVisible(true);
        ui->labelAux->setVisible(true);
        ui->Services->setVisible(true);
        ui->help->setVisible(false);
        if(SI == true){
            QStringList Lista1,Lista2;
            ui->comboBoxT->clear();
            ui->comboBoxF->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
            ui->comboBoxT->addItems(Lista1);
            ui->comboBoxF->addItems(Lista2);
        }else if(SIS == true){
            QStringList Lista1,Lista2;
            ui->comboBoxT->clear();
            ui->comboBoxF->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
            ui->comboBoxT->addItems(Lista1);
            ui->comboBoxF->addItems(Lista2);
        }
    }else if(diverso == true){
        ui->groupBox_3->setVisible(true);
        ui->groupBox_4->setVisible(true);
        ui->label->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->comboBoxF->setVisible(true);
        ui->comboBoxT->setVisible(true);
        ui->labelK->setVisible(true);
        ui->labelMin->setVisible(true);
        ui->labelMax->setVisible(true);
        ui->labelInc->setVisible(true);
        ui->labelMin->setText("Minimun value:");
        ui->SBk->setVisible(true);
        ui->SBmin->setVisible(true);
        ui->SBmax->setVisible(true);
        ui->SBinc->setVisible(true);
        ui->labelType->setVisible(false);
        ui->TypeDesign->setVisible(false);
        ui->labelAux->setVisible(true);
        ui->Services->setVisible(true);
        ui->help->setVisible(true);
        if(SI == true){
            QStringList Lista1,Lista2;
            ui->comboBoxT->clear();
            ui->comboBoxF->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
            ui->comboBoxT->addItems(Lista1);
            ui->comboBoxF->addItems(Lista2);
        }else if(SIS == true){
            QStringList Lista1,Lista2;
            ui->comboBoxT->clear();
            ui->comboBoxF->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
            ui->comboBoxT->addItems(Lista1);
            ui->comboBoxF->addItems(Lista2);
        }
    }else if(uniforme == false || diverso == false){
        ui->labelK->setVisible(false);
        ui->labelMin->setVisible(false);
        ui->labelMax->setVisible(false);
        ui->labelInc->setVisible(false);
        ui->SBk->setVisible(false);
        ui->SBmin->setVisible(false);
        ui->SBmax->setVisible(false);
        ui->SBinc->setVisible(false);
    }
}

void contenido_plots_area::accionguardar()
{
    QFile F(WORKSPACE_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
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
    in2>> MATRIZ;
    QVector<QVector<double>> Matriz = MATRIZ.getMatriz();
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> Wcp(Matriz.size()),h(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        Wcp[i] = Matriz[i][2];
        h[i] = Matriz[i][3];
    }
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    int UTemp = units.getUTemp();
    int UWcp = units.getUWcp();
    //int Uh = units.getUh();
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,Wcp,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    Wcp = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    int ncols,CTo,CCo;
    ncols = ui->Services->columnCount();
    QVector<double> Enfriamento,Calentamiento,Enf,Cal;
    Enfriamento.resize(ncols);
    Calentamiento.resize(ncols);
    Enf.resize(ncols);
    Cal.resize(ncols);
    for (int j = 0; j < ncols ; j++){
        Cal[j] = ui->Services->item(0,j)->text().toDouble();
        Enf[j] = ui->Services->item(1,j)->text().toDouble();
    }
    UnidadesSerAux units2(SI,SIS,0);
    int A = ui->comboBoxT->currentIndex();
    units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
    Enfriamento = units2.getEnfriamento();
    Calentamiento = units2.getCalentamiento();
    bool uniforme = ui->Uniform->isChecked();
    bool diverso = ui->Diverse->isChecked();
    if (ui->TypeDesign->currentIndex() == 1){
        CTo = 1;
        CCo = 0;
    }else if(ui->TypeDesign->currentIndex() == 2){
        CTo = 0;
        CCo = 1;
    }
    K = ui->SBk->value();
    Min = ui->SBmin->value();
    Max = ui->SBmax->value();
    Inc = ui->SBinc->value();
    Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
    Max = units.ConvertirMax(Max,SI,SIS,UTemp);
    Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
    if(uniforme == true){
        QFile FileBools(VECPLOTAREAS_BOOL_FILENAME);
        if (!FileBools.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out18(&FileBools);
        out18.setVersion(QDataStream::Qt_5_4);
        VecAreasUniformebool VecAUbool(uniforme,diverso);
        out18 << VecAUbool;
        FileBools.flush();
        FileBools.close();
        QFile FileAreas(VECPLOTAREAS_UNIFORME_FILENAME);
        if (!FileAreas.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out17(&FileAreas);
        out17.setVersion(QDataStream::Qt_5_4);
        VecAreasUniforme VAU(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,Enfriamento,Min,Max,Inc,CTo,CCo);
        out17 << VAU;
        FileAreas.flush();
        FileAreas.close();
    }else if(diverso == true){
        QFile FileBools(VECPLOTAREAS_BOOL_FILENAME);
        if (!FileBools.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out18(&FileBools);
        out18.setVersion(QDataStream::Qt_5_4);
        VecAreasUniformebool VecAUbool(uniforme,diverso);
        out18 << VecAUbool;
        FileBools.flush();
        FileBools.close();
        QFile FileAreas(VECPLOTAREAS_DIVERSO_FILENAME);
        if (!FileAreas.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out19(&FileAreas);
        out19.setVersion(QDataStream::Qt_5_4);
        VecAreasDiverso VAD(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,Enfriamento,Min,Max,Inc,K,CTo,CCo);
        out19 << VAD;
        FileAreas.flush();
        FileAreas.close();
    }

}


void contenido_plots_area::on_SBk_valueChanged()
{
    accionguardar();
}

void contenido_plots_area::on_SBmin_valueChanged()
{
    accionguardar();
}

void contenido_plots_area::on_SBmax_valueChanged()
{
    accionguardar();
}

void contenido_plots_area::on_SBinc_valueChanged()
{
    accionguardar();
}

void contenido_plots_area::on_Services_cellChanged()
{
    accionguardar();
}
