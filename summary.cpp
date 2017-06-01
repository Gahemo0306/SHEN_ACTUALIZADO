#include "summary.h"
#include "ui_summary.h"
#include <QMessageBox>
#include "duvaloraction.h"
#include "graficos.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include "valoraction.h"
#include <QDebug>

Summary::Summary(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Summary)
{
    ui->setupUi(this);
    ui->qcustomplot->setVisible(false);
    ui->tablewidget->setVisible(false);
    QFile FileMadre(TAB_A_P);
        if (!FileMadre.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in26(&FileMadre);
    in26.setVersion(QDataStream::Qt_5_4);
    TabAnali = -1;
    Ventanamadre = -1;
    TabAnalisis TA(TabAnali,Ventanamadre);
    in26 >> TA;
    Ventanamadre = TA.getVentanamadre();
    TabAnali = TA.getTabAnalisis();
    if(Ventanamadre == 1){ // VENTANA ANALISIS PERSONALIZADO
        if(TabAnali == 0){ // uniforme
            QFile F(INFO_A_P_UNIFORME);
            if (!F.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in25(&F);
            in25.setVersion(QDataStream::Qt_5_4);
            QVector<double> Checkboxes1,TS1,TE1,WCP1,H1,Enfriamento1,Calentamiento1,OperationCost1;
            QVector<QVector<double>> CapitalCost1;
            Checkboxes1.resize(8),TS1.resize(1),TE1.resize(1),WCP1.resize(1),H1.resize(1);
            Enfriamento1.resize(1),Calentamiento1.resize(1),OperationCost1.resize(1);
            CapitalCost1.resize(1);
            for(int i = 0; i < CapitalCost1.size(); i++){
                CapitalCost1[i].resize(2);
            }
            double Min1 = 0, Max1 = 0, Inc1 = 0;
            int CTo1 = 0, CCo1 = 0;
            bool estatico1 = false, incremento1 = false,SI1 = false,SIS1 = false;
            infoapuniforme info(Checkboxes1,estatico1,incremento1,SI1,SIS1,Min1,Max1,Inc1,TS1,TE1,WCP1,H1,Enfriamento1,
                                Calentamiento1,CTo1,CCo1,CapitalCost1,OperationCost1);
            in25 >> info;
            QVector<double> Checkboxes = info.getCheckboxes();
            Min = info.getMin();
            Max = info.getMax();
            Inc = info.getInc();
            estatico = info.getestatico();
            incremento = info.getincremento();
            SI = info.getSI();
            SIS = info.getSIS();
            TS = info.getTS();
            TE = info.getTE();
            WCP = info.getWCP();
            H = info.getH();
            Enfriamento = info.getEnfriamento();
            Calentamiento = info.getCalentamiento();
            CTo = info.getCTo();
            CCo = info.getCCo();
            CapitalCost = info.getCapitalCost();
            OperationCost = info.getOperationCost();
            ui->treeWidget->setColumnCount(1);
            ui->treeWidget->setHeaderLabel("Information avaiable:");
            QString LISTA1;
            QStringList LISTA2;
            if(Checkboxes.size() == 8){ //
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }else if(Checkboxes.size() == 10){
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 << "Total areas" <<"Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Total cost" << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }
            F.flush();
            F.close();
        }else if(TabAnali == 1){ // metodologia diversa
            QFile F(INFO_A_P_DIVERSO);
            if (!F.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in27(&F);
            in27.setVersion(QDataStream::Qt_5_4);
            QVector<double> Checkboxes1,TS1,TE1,WCP1,H1,Enfriamento1,Calentamiento1,OperationCost1;
            QVector<QVector<double>> CapitalCost1;
            Checkboxes1.resize(8),TS1.resize(1),TE1.resize(1),WCP1.resize(1),H1.resize(1);
            Enfriamento1.resize(1),Calentamiento1.resize(1),OperationCost1.resize(1);
            CapitalCost1.resize(1);
            for(int i = 0; i < CapitalCost1.size(); i++){
                CapitalCost1[i].resize(2);
            }
            double Min1 = 0, Max1 = 0, Inc1 = 0, K1 = 0;
            int CTo1 = 0, CCo1 = 0;
            bool estatico1 = false, incremento1 = false, SI1 = false, SIS1 = false;
            infoapdiversa info(Checkboxes1,estatico1,incremento1,SI1,SIS1,K1,Min1,Max1,Inc1,TS1,TE1,WCP1,H1,Enfriamento1,
                               Calentamiento1,CTo1,CCo1,CapitalCost1,OperationCost1);
            in27 >> info;
            QVector<double> Checkboxes = info.getCheckboxes();
            K  = info.getK();
            Min = info.getMin();
            Max = info.getMax();
            Inc = info.getInc();
            estatico = info.getestatico();
            incremento = info.getincremento();
            SI = info.getSI();
            SIS = info.getSIS();
            TS = info.getTS();
            TE = info.getTE();
            WCP = info.getWCP();
            H = info.getH();
            Enfriamento = info.getEnfriamento();
            Calentamiento = info.getCalentamiento();
            CTo = info.getCTo();
            CCo = info.getCCo();
            CapitalCost = info.getCapitalCost();
            OperationCost = info.getOperationCost();
            ui->treeWidget->setColumnCount(1);
            ui->treeWidget->setHeaderLabel("Information avaiable:");
            QString LISTA1;
            QStringList LISTA2;
            if(Checkboxes.size() == 8){ //
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }else if(Checkboxes.size() == 10){
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 << "Total areas" <<"Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Total cost" << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }
            F.flush();
            F.close();
        }else if(TabAnali == 2){
            QFile F(INFO_A_P_BOTH);
            if (!F.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in28(&F);
            in28.setVersion(QDataStream::Qt_5_4);
            QVector<double> Checkboxes1,TS1,TE1,WCP1,H1,Enfriamento1,Calentamiento1,OperationCost1;
            QVector<QVector<double>> CapitalCost1;
            Checkboxes1.resize(8),TS1.resize(1),TE1.resize(1),WCP1.resize(1),H1.resize(1);
            Enfriamento1.resize(1),Calentamiento1.resize(1),OperationCost1.resize(1);
            CapitalCost1.resize(1);
            for(int i = 0; i < CapitalCost1.size(); i++){
                CapitalCost1[i].resize(2);
            }
            double Min1 = 0, Max1 = 0, Inc1 = 0, K1 = 0;
            int CTo1 = 0, CCo1 = 0;
            bool estatico1 = false, incremento1 = false, SI1 = false, SIS1 = false;
            infoapboth info(Checkboxes1,estatico1,incremento1,SI1,SIS1,K1,Min1,Max1,Inc1,TS1,TE1,WCP1,H1,Enfriamento1,
                               Calentamiento1,CTo1,CCo1,CapitalCost1,OperationCost1);
            in28 >> info;
            QVector<double> Checkboxes = info.getCheckboxes();
            K  = info.getK();
            Min = info.getMin();
            Max = info.getMax();
            Inc = info.getInc();
            estatico = info.getestatico();
            incremento = info.getincremento();
            SI = info.getSI();
            SIS = info.getSIS();
            TS = info.getTS();
            TE = info.getTE();
            WCP = info.getWCP();
            H = info.getH();
            Enfriamento = info.getEnfriamento();
            Calentamiento = info.getCalentamiento();
            CTo = info.getCTo();
            CCo = info.getCCo();
            CapitalCost = info.getCapitalCost();
            OperationCost = info.getOperationCost();
            ui->treeWidget->setColumnCount(1);
            ui->treeWidget->setHeaderLabel("Information avaiable:");
            QString LISTA1;
            QStringList LISTA2;
            if(Checkboxes.size() == 8){ //
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }else if(Checkboxes.size() == 10){
                if(Checkboxes[0] == 1){ //UNIFORME TABLA PROBLEMA
                    LISTA1 = "Problem table";
                    LISTA2 << "Minimal hot utility" << "Minimal cold utility" << "Location of the pinch point";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[1] == 1){ // UNIFORME AREA PROBLEMA
                    LISTA1 = "Area prediction";
                    LISTA2 << "Deltas T min" << "Areas" << "Enthalpy";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[3] == 1){ // CURVA CALIENTE
                    LISTA1 = "Hot compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[4] == 1){ // CURVA FRIA
                    LISTA1 = "Cold compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[5] == 1){ // CURVAS COMPUESTAS
                    LISTA1 = "Compositive curves";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[6] == 1){ // CURVAS COMPUESTAS AJUSTADAS
                    LISTA1 = "Compositive curves adjusted";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[7] == 1){ // GRAN CURVAS COMPUESTA
                    LISTA1 = "Grand compositive curve";
                    LISTA2 << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 << "Total areas" <<"Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Total cost" << "Data points";
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
            }
            F.flush();
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

void Summary::on_treeWidget_doubleClicked()
{
    int column = ui->treeWidget->currentColumn();
    item_select = ui->treeWidget->currentItem()->text(column);
    desplegar_info(item_select);
}

void Summary::desplegar_info(QString text)
{
    if(Ventanamadre == 1){//ANALISIS PERSONALIZADO
        if(text == "Problem table"){
            ui->qcustomplot->setVisible(false);
            ui->tablewidget->setVisible(false);
            if(TabAnali == 0){ // METODO UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){ //METODOLGIA DIVERSA
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_diverso(DTmin,K,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_diverso(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){ // AMBAS METODOLOGIAS
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS  = 0;
                    tablaproblema_estatico_both(DTmin,K,text,punto1,punto2);
                }else if(incremento == true){

                }
            }
        }else if(text == "Minimal hot utility"){
            ui->qcustomplot->setVisible(false);
            ui->tablewidget->setVisible(false);
            if(TabAnali == 0){ // METODO UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){ //METODOLGIA DIVERSA
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    qDebug() << "Entro";
                    tablaproblema_estatico_diverso(DTmin,K,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_diverso(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Minimal cold utility"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){ //METODOLGIA DIVERSA
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_diverso(DTmin,K,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_diverso(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Location of the pinch point"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    double k  = K;
                    qDebug() << "Entro";
                    tablaproblema_estatico_diverso(DTmin,k,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature " << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_incremento_diverso(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }
    }//VENTANA MADRE ELSE IF
}

void Summary::tablaproblema_estatico_uniforme(double DTmin, QString text)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    ui->tablewidget->setRowCount(VecCorrientesTotal.size()-1);
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7;
    int k=1,j=0;
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    for(int i =0; i < row ; i++){
        val1 = VecCorrientesTotal[k][0];
        val2 = VecCorrientesTotal[k][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        k=k+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(0, 5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(VecAdjHeatFlow.size()-1,6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(min_pos,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(0, 5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(VecAdjHeatFlow.size()-1,6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(min_pos,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

void Summary::tablaproblema_incremento_uniforme(double DTmin, QString text)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    double val1, val2, val3,val4,val5,val6,val7;
    int k=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[k][0];
        val2 = VecCorrientesTotal[k][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        k=k+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

void Summary::tablaproblema_estatico_diverso(double DTmin, double k, QString text)
{
    float punto1 = .5, punto2 = 10;
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    double valuek = plot.getK();
    ui->tablewidget->setRowCount(VecCorrientesTotal.size()-1);
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7;
    int kcontador=1,j=0;
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    for(int i =0; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(0, 5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(VecAdjHeatFlow.size()-1,6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(min_pos,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(0, 5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(VecAdjHeatFlow.size()-1,6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(min_pos,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

void Summary::tablaproblema_incremento_diverso(double DTmin, double k, QString text,float punto1, float punto2)
{
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    K1 = plot.getK();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    double val1, val2, val3,val4,val5,val6,val7;
    int kcontador=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

void Summary::tablaproblema_estatico_both(double DTmin, double k, QString text,float punto1,float punto2)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    double val1, val2, val3,val4,val5,val6,val7;
    int kcontador=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
    Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    VecCorrientesTotal = plot2.getVectorCorrientesTotal();
    VecHeatFlow = plot2.getVecHeatFlow();
    VecAdjHeatFlow = plot2.getVecAdjHeatFlow();
    Deficit = plot2.getDeficit();
    K1 = plot2.getK();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    VAL2.clear();
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
    kcontador=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
    if(text == "Problem table"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }else if(text == "Minimal hot utility"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

