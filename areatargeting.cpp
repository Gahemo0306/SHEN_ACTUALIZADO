#include <algorithm>
#include <numeric>
#include "duvaloraction.h"
#include "valoraction.h"
#include "areatargeting.h"
#include "ui_areatargeting.h"
#include <QMessageBox>
#include <QFile>
#include <QDataStream>

areatargeting::areatargeting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::areatargeting)
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

areatargeting::~areatargeting()
{
    delete ui;
}

void areatargeting::on_uniform_clicked()
{
    RADIOBUTTONS();
}

void areatargeting::on_diverse_clicked()
{
    RADIOBUTTONS();
}

void areatargeting::on_TypeDesign_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->Services->setEnabled(true);
        ui->Services->setColumnCount(3);
        ui->Services->setRowCount(2);
        titulos.clear();
        titulos << "Tsupply" << "Ttarget" << "h";
        ui->Services->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->Services->setVerticalHeaderLabels(titulos);
        int row = ui->Services->rowCount();
        int column = ui->Services->columnCount();
        for(int i = 0 ; i < row; i++ )
        {
            for( int c = 0; c < column; ++c )
            {
                ui->Services->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }

}

void areatargeting::on_Services_cellChanged(int row, int column)
{
    if(ui->Services->isEnabled() == false){
        return;
    }else if(ui->Services->isEnabled() == true){
        row = ui->Services->rowCount();
        column = ui->Services->columnCount();
        for(int i = 0; i < row ; i++){
            for(int j = 0; j < column ; j++){
                if (!ui->Services->item(i,j)){
                   return ;
                }else if (ui->Services->item(i,j)->text() == "Empty"){
                   return ;
                }else if (ui->Services->item(i,j)->text() == "0"){
                   return ;
                }
            }
        }
        accionguardar();
    }
}

void areatargeting::accionguardar()
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
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
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

void areatargeting::RADIOBUTTONS()
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
        ui->TypeDesign->setCurrentIndex(0);
        ui->labelAux->setVisible(false);
        ui->Services->setVisible(false);
        ui->Services->setColumnCount(0);
        ui->Services->setRowCount(0);
        ui->help->setVisible(false);
        bool uniforme = ui->uniform->isChecked();
        bool diverso = ui->diverse->isChecked();
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
            ui->Services->setEnabled(true);
            ui->Services->setColumnCount(3);
            ui->Services->setRowCount(2);
            titulos.clear();
            titulos << "Tsupply" << "Ttarget" << "h";
            ui->Services->setHorizontalHeaderLabels(titulos);
            titulos.clear();
            titulos << "Heating service" << "Cooling service";
            ui->Services->setVerticalHeaderLabels(titulos);
            int row = ui->Services->rowCount();
            int column = ui->Services->columnCount();
            for(int i = 0 ; i < row; i++ )
            {
                for( int c = 0; c < column; ++c )
                {
                    ui->Services->setItem(i, c, new QTableWidgetItem("Empty"));
                }
            }
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

void areatargeting::on_SBk_valueChanged()
{
    if(ui->Services->isEnabled() == true){
        return;
    }else{
        accionguardar();
    }
}

void areatargeting::on_SBmin_valueChanged()
{
    if(ui->Services->isEnabled() == false){
        return;
    }else{
        accionguardar();
    }
}

void areatargeting::on_SBmax_valueChanged()
{
    if(ui->Services->isEnabled() == false){
        return;
    }else{
        accionguardar();
    }
}

void areatargeting::on_SBinc_valueChanged()
{
    if(ui->Services->isEnabled() == false){
        return;
    }else{
        accionguardar();
    }
}

void areatargeting::Info_Uniforme()
{
    QFile F(INFO_AT_UNIFORME);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out25(&F);
    out25.setVersion(QDataStream::Qt_5_4);

    QFile Fwork(WORKSPACE_FILENAME);
    if (!Fwork.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&Fwork);
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
    QVector<double> WCP(Matriz.size()),H(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        WCP[i] = Matriz[i][2];
        H[i] = Matriz[i][3];
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
    SI = units.getSI();
    SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    Fwork.flush();
    Fwork.close();
    Min = ui->SBmin->value();
    Max = ui->SBmax->value();
    Inc = ui->SBinc->value();
    Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
    Max = units.ConvertirMax(Max,SI,SIS,UTemp);
    Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);

    bool estatico = false;
    bool incremento = true;

    QVector<double> Checkboxes;
    Checkboxes.resize(10);
    Checkboxes[0] = 0;
    Checkboxes[1] = 1;
    Checkboxes[2] = 0;
    Checkboxes[3] = 0;
    Checkboxes[4] = 0;
    Checkboxes[5] = 0;
    Checkboxes[6] = 0;
    Checkboxes[7] = 0;
    Checkboxes[8] = 1;
    Checkboxes[9] = 0;
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
    if (ui->TypeDesign->currentIndex() == 1){
        CTo = 1;
        CCo = 0;
    }else if(ui->TypeDesign->currentIndex() == 2){
        CTo = 0;
        CCo = 1;
    }
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    CapitalCost.resize(1);
    for(int i = 0; i < 1 ;i++){
        CapitalCost[i].resize(2);
    }
    OperationCost.resize(2);
    infoapuniforme info(Checkboxes,estatico,incremento,SI,SIS,Min,Max,Inc,TS,TE,WCP,H,Enfriamento,
                        Calentamiento,CTo,CCo,CapitalCost,OperationCost);
    out25 << info;
    F.flush();
    F.close();
}

void areatargeting::Info_Diversa()
{
    QFile F(INFO_AT_DIVERSO);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out27(&F);
    out27.setVersion(QDataStream::Qt_5_4);
    QFile Fwork(WORKSPACE_FILENAME);
    if (!Fwork.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&Fwork);
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
    QVector<double> WCP(Matriz.size()),H(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        WCP[i] = Matriz[i][2];
        H[i] = Matriz[i][3];
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
    SI = units.getSI();
    SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    Fwork.flush();
    Fwork.close();
    K = ui->SBk->value();
    Min = ui->SBmin->value();
    Max = ui->SBmax->value();
    Inc = ui->SBinc->value();
    Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
    Max = units.ConvertirMax(Max,SI,SIS,UTemp);
    Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);

    bool estatico = false;
    bool incremento = true;

    QVector<double> Checkboxes;
    Checkboxes.resize(10);
    Checkboxes[0] = 0;
    Checkboxes[1] = 1;
    Checkboxes[2] = 0;
    Checkboxes[3] = 0;
    Checkboxes[4] = 0;
    Checkboxes[5] = 0;
    Checkboxes[6] = 0;
    Checkboxes[7] = 0;
    Checkboxes[8] = 1;
    Checkboxes[9] = 0;
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
    CTo = 0;
    CCo = 0;
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    CapitalCost.resize(1);
    for(int i = 0; i < 1 ;i++){
        CapitalCost[i].resize(2);
    }
    OperationCost.resize(2);

    infoapdiversa info(Checkboxes,estatico,incremento,SI,SIS,K,Min,Max,Inc,TS,TE,WCP,H,Enfriamento,
                       Calentamiento,CTo,CCo,CapitalCost,OperationCost);
    out27 << info;
    F.flush();
    F.close();
}

void areatargeting::on_OK_clicked()
{
    VENTANAMADRE = 2; //
    QFile Filetab(TAB_A_P);
        if (!Filetab.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out26(&Filetab);
    int TabAnali;
    out26.setVersion(QDataStream::Qt_5_4);
    if(ui->uniform->isChecked() == true){
        TabAnali = 0; // uniforme
    }else{
        TabAnali = 1; // diverso
    }
    TabAnalisis TA(TabAnali,VENTANAMADRE);
    out26 << TA;
    Filetab.flush();
    Filetab.close();
    if(TabAnali == 0){
        Info_Uniforme();
    }else if(TabAnali == 1){
        Info_Diversa();
    }

}
