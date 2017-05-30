#include "contenido_plots_costos.h"
#include "ui_contenido_plots_costos.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include "duvaloraction.h"
#include "valoraction.h"
#include "graficos.h"

contenido_plots_costos::contenido_plots_costos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contenido_plots_costos)
{
    ui->setupUi(this);
    ui->groupBox2->setVisible(false);
    ui->labelk->setVisible(false);
    ui->labelmin->setVisible(false);
    ui->labelmax->setVisible(false);
    ui->labelinc->setVisible(false);
    ui->doubleSpinBoxK->setVisible(false);
    ui->doubleSpinBoxMin->setVisible(false);
    ui->doubleSpinBoxMax->setVisible(false);
    ui->doubleSpinBoxInc->setVisible(false);
    ui->groupBox3->setVisible(false);
    ui->labelType->setVisible(false);
    ui->TypeDesign->setVisible(false);
    ui->labelunits->setVisible(false);
    ui->labelfilm->setVisible(false);
    ui->labeltemp->setVisible(false);
    ui->comboBoxFilm->setVisible(false);
    ui->comboBoxTemp->setVisible(false);
    ui->labelAux->setVisible(false);
    ui->Services->setVisible(false);
    ui->groupBox4->setVisible(false);
    ui->labelcapital->setVisible(false);
    ui->comboBoxCapital->setVisible(false);
    ui->capitalcost->setVisible(false);
    ui->labelopera->setVisible(false);
    ui->operationcost->setVisible(false);
    ui->help->setVisible(false);
    ui->label->setVisible(false);
    ui->comboBoxUOP->setVisible(false);
    ui->label_WARNIG1->setVisible(false);
}

contenido_plots_costos::~contenido_plots_costos()
{
    delete ui;
}

void contenido_plots_costos::on_uniform_clicked()
{
    RADIOBUTTONS();
}

void contenido_plots_costos::on_diverse_clicked()
{
    RADIOBUTTONS();
}

void contenido_plots_costos::on_doubleSpinBox_valueChanged()
{
    accionguardar();
}

void contenido_plots_costos::on_doubleSpinBox_2_valueChanged()
{
    accionguardar();
}

void contenido_plots_costos::on_doubleSpinBox_3_valueChanged()
{
    accionguardar();
}

void contenido_plots_costos::on_doubleSpinBox_4_valueChanged()
{
    accionguardar();
}

void contenido_plots_costos::RADIOBUTTONS()
{
    disconnect(ui->Services, SIGNAL(cellChanged(int,int)), 0, 0);
    disconnect(ui->operationcost, SIGNAL(cellChanged(int,int)), 0, 0);
    disconnect(ui->capitalcost, SIGNAL(cellChanged(int,int)), 0, 0);
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
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
    ui->groupBox2->setVisible(false);
    ui->labelk->setVisible(false);
    ui->labelmin->setVisible(false);
    ui->labelmax->setVisible(false);
    ui->labelinc->setVisible(false);
    ui->doubleSpinBoxK->setVisible(false);
    ui->doubleSpinBoxMin->setVisible(false);
    ui->doubleSpinBoxMax->setVisible(false);
    ui->doubleSpinBoxInc->setVisible(false);
    ui->groupBox3->setVisible(false);
    ui->labelType->setVisible(false);
    ui->TypeDesign->setVisible(false);
    ui->labelunits->setVisible(false);
    ui->labelfilm->setVisible(false);
    ui->labeltemp->setVisible(false);
    ui->comboBoxFilm->setVisible(false);
    ui->comboBoxTemp->setVisible(false);
    ui->labelAux->setVisible(false);
    ui->Services->setVisible(false);
    ui->groupBox4->setVisible(false);
    ui->labelcapital->setVisible(false);
    ui->comboBoxCapital->setVisible(false);
    ui->capitalcost->setVisible(false);
    ui->labelopera->setVisible(false);
    ui->operationcost->setVisible(false);
    ui->help->setVisible(false);
    ui->label->setVisible(false);
    ui->comboBoxUOP->setVisible(false);
    if(uniforme == true){
        ui->groupBox2->setVisible(true);
        ui->labelk->setVisible(false);
        ui->labelmin->setVisible(true);
        ui->labelmax->setVisible(true);
        ui->labelinc->setVisible(true);
        ui->doubleSpinBoxK->setVisible(false);
        ui->doubleSpinBoxMin->setVisible(true);
        ui->doubleSpinBoxMax->setVisible(true);
        ui->doubleSpinBoxInc->setVisible(true);
        ui->groupBox3->setVisible(true);
        ui->labelType->setVisible(true);
        ui->TypeDesign->setVisible(true);
        ui->labelunits->setVisible(true);
        ui->labelfilm->setVisible(true);
        ui->labeltemp->setVisible(true);
        ui->comboBoxFilm->setVisible(true);
        ui->comboBoxTemp->setVisible(true);
        ui->labelAux->setVisible(true);
        ui->Services->setVisible(true);
        ui->groupBox4->setVisible(true);
        ui->labelcapital->setVisible(true);
        ui->comboBoxCapital->setVisible(true);
        ui->capitalcost->setVisible(true);
        ui->labelopera->setVisible(true);
        ui->operationcost->setVisible(true);
        ui->help->setVisible(false);
        ui->label->setVisible(true);
        ui->comboBoxUOP->setVisible(true);
        if(SI == true){
            QStringList Lista1,Lista2,Lista3;
            ui->comboBoxTemp->clear();
            ui->comboBoxFilm->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
            Lista3 << "Select one..." << "US $ / Btu" << "US $ / CHU";
            ui->comboBoxTemp->addItems(Lista1);
            ui->comboBoxFilm->addItems(Lista2);
            ui->comboBoxUOP->addItems(Lista3);
        }else if(SIS == true){
            QStringList Lista1,Lista2,Lista3;
            ui->comboBoxTemp->clear();
            ui->comboBoxFilm->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
            Lista3 << "Select one..." << "US $ / KW" << "US $ / W";
            ui->comboBoxTemp->addItems(Lista1);
            ui->comboBoxFilm->addItems(Lista2);
            ui->comboBoxUOP->addItems(Lista3);
        }
        accionguardar();
    }else if(diverso == true){
        ui->groupBox2->setVisible(true);
        ui->labelk->setVisible(true);
        ui->labelmin->setVisible(true);
        ui->labelmax->setVisible(true);
        ui->labelinc->setVisible(true);
        ui->doubleSpinBoxK->setVisible(true);
        ui->doubleSpinBoxMin->setVisible(true);
        ui->doubleSpinBoxMax->setVisible(true);
        ui->doubleSpinBoxInc->setVisible(true);
        ui->groupBox3->setVisible(true);
        ui->labelType->setVisible(false);
        ui->TypeDesign->setVisible(false);
        ui->labelunits->setVisible(true);
        ui->labelfilm->setVisible(true);
        ui->labeltemp->setVisible(true);
        ui->comboBoxFilm->setVisible(true);
        ui->comboBoxTemp->setVisible(true);
        ui->labelAux->setVisible(true);
        ui->Services->setVisible(true);
        ui->groupBox4->setVisible(true);
        ui->labelcapital->setVisible(true);
        ui->comboBoxCapital->setVisible(true);
        ui->capitalcost->setVisible(true);
        ui->labelopera->setVisible(true);
        ui->operationcost->setVisible(true);
        ui->help->setVisible(true);
        ui->label->setVisible(true);
        ui->comboBoxUOP->setVisible(true);
        if(SI == true){
            QStringList Lista1,Lista2,Lista3;
            ui->comboBoxTemp->clear();
            ui->comboBoxFilm->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
            Lista3 << "Select one..." << "US $ / Btu" << "US $ / CHU";
            ui->comboBoxTemp->addItems(Lista1);
            ui->comboBoxFilm->addItems(Lista2);
            ui->comboBoxUOP->addItems(Lista3);
        }else if(SIS == true){
            QStringList Lista1,Lista2,Lista3;
            ui->comboBoxTemp->clear();
            ui->comboBoxFilm->clear();
            Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
            Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
            Lista3 << "Select one..." << "US $ / KW" << "US $ / ";
            ui->comboBoxTemp->addItems(Lista1);
            ui->comboBoxFilm->addItems(Lista2);
            ui->comboBoxUOP->addItems(Lista3);
        }
        accionguardar();
    }
    connect(ui->Services, &QTableWidget::cellChanged,this, &contenido_plots_costos::on_Services_cellChanged);
    connect(ui->operationcost, &QTableWidget::cellChanged,this, &contenido_plots_costos::on_operationcost_cellChanged);
    connect(ui->capitalcost, &QTableWidget::cellChanged,this, &contenido_plots_costos::on_capitalcost_cellChanged);
}

void contenido_plots_costos::accionguardar()
{
    if(confirmartablas() == true){
        return;
    }
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
    int A = ui->comboBoxTemp->currentIndex(); //corregido
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
    K = ui->doubleSpinBoxK->value();
    Min = ui->doubleSpinBoxMin->value();
    Max = ui->doubleSpinBoxMax->value();
    Inc = ui->doubleSpinBoxInc->value();
    Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
    Max = units.ConvertirMax(Max,SI,SIS,UTemp);
    Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    int ccsize = ui->capitalcost->columnCount();
    int rcsize = ui->capitalcost->rowCount();
    CapitalCost.resize(rcsize);
    if(ccsize == 5){
        for(int i = 0; i < rcsize ;i++){
            CapitalCost[i].resize(ccsize);
        }
        for(int j = 0; j < rcsize ; j++){
            for(int i = 0; i < ccsize;i++){
                CapitalCost[j][i] =  ui->capitalcost->item(j,i)->text().toDouble();
            }
        }
    }else if (ccsize == 4){
        for(int i = 0; i < rcsize ;i++){
            CapitalCost[i].resize(ccsize);
        }
        for(int j = 0; j < rcsize ; j++){
            for(int i = 0; i < ccsize;i++){
                CapitalCost[j][i] =  ui->capitalcost->item(j,i)->text().toDouble();
            }
        }
    }
    int cbUOP = ui->comboBoxUOP->currentIndex();
    int opsize = ui->operationcost->rowCount();
    OperationCost.resize(opsize);
    for(int i = 0; i < opsize ; i++){
        OperationCost[i] = ui->operationcost->item(i,0)->text().toDouble();
    }
    OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
    if(uniforme == true){
        QFile FileBools(VECPLOTCOSTOS_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out20(&FileBools);
        out20.setVersion(QDataStream::Qt_5_4);
        VecCostUniformebool VCUB(uniforme,diverso);
        out20 << VCUB;
        FileBools.flush();
        FileBools.close();
        QFile FileCostos(VECPLOTCOSTOS_UNIFORME_FILENAME);
        if (!FileCostos.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out21(&FileCostos);
        out21.setVersion(QDataStream::Qt_5_4);
        VecCostUniforme VCU(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,
                            Enfriamento,CapitalCost,OperationCost,Min,
                            Max,Inc,CTo,CCo);
        out21 << VCU;
        FileCostos.flush();
        FileCostos.close();
    }else if(diverso == true){
        QFile FileBools(VECPLOTCOSTOS_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out20(&FileBools);
        out20.setVersion(QDataStream::Qt_5_4);
        VecCostUniformebool VCUB(uniforme,diverso);
        out20 << VCUB;
        FileBools.flush();
        FileBools.close();
        QFile FileCostos(VECPLOTCOSTOS_DIVERSO_FILENAME);
        if (!FileCostos.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out22(&FileCostos);
        out22.setVersion(QDataStream::Qt_5_4);
        VecCostDiverso VCD(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,
                           Enfriamento,CapitalCost,OperationCost,Min,
                           Max,Inc,K);
        out22 << VCD;
        FileCostos.flush();
        FileCostos.close();
    }
}

void contenido_plots_costos::on_comboBoxCapital_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else{
        QFile F(VECPLOTCOSTOS_CC_FILENAME);
        if (!F.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out24(&F);
        out24.setVersion(QDataStream::Qt_5_4);
        CostosCapitalCost CCC(index);
        out24 << CCC;
        F.flush();
        F.close();
        if(index == 1){ // Ley de intercambiadores
            ui->capitalcost->setEnabled(true);
            ui->label_WARNIG1->setVisible(true);
            int ColumAct = 5;
            ui->capitalcost->clear();
            ui->capitalcost->setColumnCount(ColumAct);
            titulos.clear();
            titulos << "a" << "b" <<"c"<<"i"<<"n";
            ui->capitalcost->setHorizontalHeaderLabels(titulos);
            ui->capitalcost->setRowCount(3);
            titulos.clear();
            titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
            ui->capitalcost->setVerticalHeaderLabels(titulos);
            for( int r = 0 ; r < ui->capitalcost->rowCount(); ++r )
            {
                for( int c = 0; c < ui->capitalcost->columnCount(); ++c )
                {
                    ui->capitalcost->setItem(r, c, new QTableWidgetItem("Empty"));
                }
            }
        }else if(index == 2){
            ui->capitalcost->setEnabled(true);
            ui->label_WARNIG1->setVisible(true);
            int ColumAct = 4;
            ui->capitalcost->clear();
            ui->capitalcost->setColumnCount(ColumAct);
            titulos.clear();
            titulos << "b" <<"c"<<"i"<<"n";
            ui->capitalcost->setHorizontalHeaderLabels(titulos);
            ui->capitalcost->setRowCount(3);
            titulos.clear();
            titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
            ui->capitalcost->setVerticalHeaderLabels(titulos);
            for( int r = 0 ; r < ui->capitalcost->rowCount(); ++r )
            {
                for( int c = 0; c < ui->capitalcost->columnCount(); ++c )
                {
                    ui->capitalcost->setItem(r, c, new QTableWidgetItem("Empty"));
                }
            }
        }
    }
}

void contenido_plots_costos::on_Services_cellChanged()
{
    if(confirmartablas()== true){
        return;
    }else{
        accionguardar();
    }
}

void contenido_plots_costos::on_capitalcost_cellChanged()
{
    if(confirmartablas()== true){
        return;
    }else{
        accionguardar();
    }
}

void contenido_plots_costos::on_operationcost_cellChanged()
{
    if(confirmartablas()== true){
        return;
    }else{
        accionguardar();
    }
}

bool contenido_plots_costos::confirmartablas()
{
    int row = ui->Services->rowCount();
    int column = ui->Services->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->Services->item(i,j)){
               return true;
            }else if (ui->Services->item(i,j)->text() == "Empty"){
               return true;
            }else if (ui->Services->item(i,j)->text() == "0"){
               return true;
            }
        }
    }
    row = ui->capitalcost->rowCount();
    column = ui->capitalcost->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->capitalcost->item(i,j)){
               return true;
            }else if (ui->capitalcost->item(i,j)->text() == "Empty"){
               return true;
            }else if (ui->capitalcost->item(i,j)->text() == "0"){
               return true;
            }
        }
    }
    row = ui->operationcost->rowCount();
    column = ui->operationcost->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->operationcost->item(i,j)){
               return true;
            }else if (ui->operationcost->item(i,j)->text() == "Empty"){
               return true;
            }else if (ui->operationcost->item(i,j)->text() == "0"){
               return true;
            }
        }
    }
    return false;
}

void contenido_plots_costos::on_comboBoxUOP_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->operationcost->setEnabled(true);
        ui->operationcost->setColumnCount(1);
        ui->operationcost->setRowCount(2);
        titulos.clear();
        titulos << "Value";
        ui->operationcost->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->operationcost->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->operationcost->rowCount(); i++ )
        {
            for( int c = 0; c < ui->operationcost->columnCount(); ++c )
            {
                ui->operationcost->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

QVector<double> contenido_plots_costos::ConvertirOperationCost(QVector<double> OperationCost, bool SI, bool SIS, int A)
{
    // "US $ / KW" << "US $ / W"; "US $ / Btu" << "US $ / CHU";
    if(SI ==true ){
        if(A == 1){ //US $ / Btu
            OperationCost[0]= OperationCost[0];
            OperationCost[1]= OperationCost[1];
        }else if(A == 1){ //US $ / CHU  ---> US $ / BTU
            OperationCost[0]= OperationCost[0]/1.80;
            OperationCost[1]= OperationCost[1]/1.80;
        }
    }else if(SIS == true){
        if(A == 1){ //US $ / KW
            OperationCost[0]= OperationCost[0];
            OperationCost[1]= OperationCost[1];
        }else if(A == 1){ //US $ / W  ---> US $ / KW
            OperationCost[0]= OperationCost[0]*1000;
            OperationCost[1]= OperationCost[1]*1000;
        }
    }
    return OperationCost;
}
