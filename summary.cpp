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
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>

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
            qDebug() << Enfriamento << Calentamiento;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[2] == 1){ // UNIFORME COSTO PROBLEMA
                    LISTA1 = "Cost prediction";
                    LISTA2 << "Agglomerates" << "Capital cost" << "Operational cost" << "Total cost" ;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
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
                    LISTA2 << "DTm log" << "Enthalpy" << "Areas" ;
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat_flow_in"
                            << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_diverso(DTmin,K,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS  = 0;
                    tablaproblema_estatico_both(DTmin,K,text,punto1,punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){ //FALTA
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_estatico_both(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
            }else if(TabAnali == 2){ //METODOLODIA BOTH
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS  = 0;
                    tablaproblema_estatico_both(DTmin,K,text,punto1,punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){ //FALTA
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_estatico_both(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Minimal cold utility"){
            if(TabAnali == 0){ // METODO UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_diverso(DTmin,K,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
            }else if(TabAnali == 2){ //METODOLODIA BOTH
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature"<< "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS  = 0;
                    tablaproblema_estatico_both(DTmin,K,text,punto1,punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){ //FALTA
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_estatico_both(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Location of the pinch point"){
            if(TabAnali == 0){ // METODO UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    tablaproblema_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    double k  = K;
                    tablaproblema_estatico_diverso(DTmin,k,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
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
            }else if(TabAnali == 2){ //METODOLODIA BOTH
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS  = 0;
                    tablaproblema_estatico_both(DTmin,K,text,punto1,punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){ //FALTA
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Temperature" << "Temperature" << "Deficit" << "Heat flow in"
                            << "Heat flow out" << "Heat flow in adjusted" << "Heat flow out adjusted";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        tablaproblema_estatico_both(DTmin, k ,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Area prediction"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    areas_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    areas_estatico_diverso(DTmin,k,text,punto1,punto2);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_diverso(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min, k = K;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    areas_estatico_both( DTmin,k, text,  punto1,  punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_both(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "DTm log"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    areas_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    areas_estatico_diverso(DTmin,k,text,punto1,punto2);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_diverso(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min, k = K;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    areas_estatico_both( DTmin,k, text,  punto1,  punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_both(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Areas"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    areas_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    areas_estatico_diverso(DTmin,k,text,punto1,punto2);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_diverso(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min, k = K;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    areas_estatico_both( DTmin,k, text,  punto1,  punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_both(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Enthalpy"){
            if(TabAnali == 0){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    areas_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    areas_estatico_diverso(DTmin,k,text,punto1,punto2);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4"  << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(7);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_diverso(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min, k = K;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    areas_estatico_both( DTmin,k, text,  punto1,  punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        areas_incremento_both(DTmin, k, text, punto1, punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Cost prediction"){
            if(TabAnali == 0){ // UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;

                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "INT1" << "INT2" << "INT3" << "INT4" << "DeltaT1" << "DeltaT2"
                            << "DTmlog" << "Enthalpy" << "Areas";
                    ui->tablewidget->setColumnCount(9);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        //areas_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){ // DIVERSO
                if(estatico == true){

                }else if(incremento == true){

                }
            }else if(TabAnali == 2){ // BOTH
                if(estatico == true){

                }else if(incremento == true){

                }
            }
        }else if(text == "Agglomerates"){

        }else if(text == "Capital cost"){

        }else if(text == "Operational cost"){

        }else if(text == "Total cost"){

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
    if(text == "Minimal hot utility"){
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
    if(text == "Minimal hot utility"){
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
    if(text == "Minimal hot utility"){
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
    if(text == "Minimal hot utility"){
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
    if(text == "Minimal hot utility"){
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
    if(text == "Minimal hot utility"){
        ui->tablewidget->item(CONTADORFILAS - VecAdjHeatFlow.size(),5)->setBackground(Qt::red); //hot utility
    }else if(text == "Minimal cold utility"){
        ui->tablewidget->item(CONTADORFILAS - 1 , 6)->setBackground(Qt::blue); // coldutility
    }else if(text == "Location of the pinch point"){
        for(int i = 0; i < ui->tablewidget->columnCount(); i++){
            ui->tablewidget->item(CONTADORFILAS - min_pos -1,i)->setBackground(Qt::darkYellow); //pinchpoint
        }
    }
}

void Summary::areas_estatico_uniforme(double DTmin, QString text)
{
    Plot_Dtmin_vs_Areas plot(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDTmed();
    QVector<double> Delta1 = plot.getDelta1();
    QVector<double> Delta2 = plot.getDelta2();
    ui->tablewidget->setRowCount(Areas.size());
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7,val8,val9;
    int j=0;
    for(int i =0; i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = Delta1[j];
        val6 = Delta2[j];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::areas_incremento_uniforme(double DTmin, QString text)
{
    Plot_Dtmin_vs_Areas plot(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDTmed();
    QVector<double> Delta1 = plot.getDelta1();
    QVector<double> Delta2 = plot.getDelta2();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7,val8,val9;
    int j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = Delta1[j];
        val6 = Delta2[j];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::areas_estatico_diverso(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Dtmin_vs_Areas_DIVERSO plot(TS,TE,WCP,H,Calentamiento,Enfriamento,k,DTmin,punto1,punto2);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDeltaTLM();
    double valuek = plot.getK();
    ui->tablewidget->setRowCount(Areas.size());
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7;
    int j=0;
    for(int i = 0; i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = DTmedlog[j];
        val6 = Entalpia[j];
        val7 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = 0; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = 0; i < row ; i++){
            ui->tablewidget->item(i,5)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = 0; i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::areas_incremento_diverso(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Dtmin_vs_Areas_DIVERSO plot(TS,TE,WCP,H,Calentamiento,Enfriamento,k,DTmin,punto1,punto2);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDeltaTLM();
    K1 = plot.getK();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7;
    int j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = DTmedlog[j];
        val6 = Entalpia[j];
        val7 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,5)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::yellow); // areas
        }
    }

}

void Summary::areas_estatico_both(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Dtmin_vs_Areas plot(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDTmed();
    QVector<double> Delta1 = plot.getDelta1();
    QVector<double> Delta2 = plot.getDelta2();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7,val8,val9;
    int j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = Delta1[j];
        val6 = Delta2[j];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
    Plot_Dtmin_vs_Areas_DIVERSO plot2(TS,TE,WCP,H,Calentamiento,Enfriamento,k,DTmin,punto1,punto2);
    Intervalos = plot2.getINTERVALOS_AGRUPADOS();
    Areas = plot2.getVectorAreas();
    Entalpia = plot2.getVectorEntalpia();
    DTmedlog = plot2.getDeltaTLM();
    K1 = plot2.getK();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem("------"));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem("------"));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::areas_incremento_both(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Dtmin_vs_Areas plot(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
    QVector<QVector<double>> Intervalos = plot.getINTERVALOS_AGRUPADOS();
    QVector<double> Areas = plot.getVectorAreas();
    QVector<double> Entalpia = plot.getVectorEntalpia();
    QVector<double> DTmedlog = plot.getDTmed();
    QVector<double> Delta1 = plot.getDelta1();
    QVector<double> Delta2 = plot.getDelta2();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7,val8,val9;
    int j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val5 = Delta1[j];
        val6 = Delta2[j];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
    Plot_Dtmin_vs_Areas_DIVERSO plot2(TS,TE,WCP,H,Calentamiento,Enfriamento,k,DTmin,punto1,punto2);
    Intervalos = plot2.getINTERVALOS_AGRUPADOS();
    Areas = plot2.getVectorAreas();
    Entalpia = plot2.getVectorEntalpia();
    DTmedlog = plot2.getDeltaTLM();
    K1 = plot2.getK();
    CONTADORFILAS = CONTADORFILAS + Areas.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - Areas.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j=0;
    for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
        val1 = Intervalos[j][0];
        val2 = Intervalos[j][1];
        val3 = Intervalos[j][2];
        val4 = Intervalos[j][3];
        val7 = DTmedlog[j];
        val8 = Entalpia[j];
        val9 = Areas[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem("------"));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem("------"));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
        j=j+1;
    }
    if(text == "DTm log"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
        }
    }else if(text == "Enthalpy"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Areas"){
        for(int i = CONTADORFILAS - Areas.size(); i < row ; i++){
            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_estatico_uniforme(double DTmin, QString text)
{
    Plot_Costos_vs_Areas_Uniforme plot(TS,TE,WCP,H,Calentamiento,Enfriamento,CapitalCost,OperationCost,DTmin,CTo,CCo,SI,SIS);
    double MOpeCosC = plot.getOpeCosC();
    double MOpeCosH = plot.getOpeCosH();
    double MCapCos3 = plot.getCapCos3();
    double MCapCos2 = plot.getCapCos2();
    double MCapCos1 = plot.getCapCos1();
    double MAH = plot.getAH();
    double MQH = plot.getQH();
    double MAR = plot.getAR();
    double MQR = plot.getQR();
    double MAC = plot.getAC();
    double MQC = plot.getQC();
//    ui->tablewidget->setRowCount(Areas.size());
//    int row = ui->tablewidget->rowCount();
//    QStringList Rows;
//    for(int i = 0; i < row; i++){
//        Rows << "DTmin:  " + QString::number(DTmin);
//    }
//    ui->tablewidget->setVerticalHeaderLabels(Rows);
//    double val1, val2, val3,val4,val5,val6,val7,val8,val9;
//    int j=0;
//    for(int i =0; i < row ; i++){
//        val1 = Intervalos[j][0];
//        val2 = Intervalos[j][1];
//        val3 = Intervalos[j][2];
//        val4 = Intervalos[j][3];
//        val5 = Delta1[j];
//        val6 = Delta2[j];
//        val7 = DTmedlog[j];
//        val8 = Entalpia[j];
//        val9 = Areas[j];
//        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
//        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
//        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
//        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
//        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
//        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
//        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
//        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
//        ui->tablewidget->setItem(i,8, new QTableWidgetItem(QString::number(val9)));
//        j=j+1;
//    }
//    if(text == "DTm log"){
//        for(int i =0; i < row ; i++){
//            ui->tablewidget->item(i,6)->setBackground(Qt::red); // areas
//        }
//    }else if(text == "Enthalpy"){
//        for(int i =0; i < row ; i++){
//            ui->tablewidget->item(i,7)->setBackground(Qt::blue); // areas
//        }
//    }else if(text == "Areas"){
//        for(int i =0; i < row ; i++){
//            ui->tablewidget->item(i,8)->setBackground(Qt::yellow); // areas
//        }
//    }
}

QString Summary::currDate()
{
    QDate date = QDate::currentDate();
    return date.toString("dd.MM.yyyy");
}

void Summary::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save the file");
    if(!filename.contains(".pdf")){
        filename = filename + ".pdf";
    }
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(filename);
    QTextDocument doc;
    QString citydate = "Report, ";
    citydate += currDate();
    if(estatico == true){
        QString style("<style>");
        style.append("body { color:black; } ");
        style.append("table { border-collapse: collapse;font-size:10px; }");
        style.append("table, th, td { border: 1px solid black; text-align: center; }");
        style.append("</style>");
        QString text("<html><body>");
        text.append("<h>").append("     " + citydate + "     ").append("</h>");
        text.append("<p>").append("     Program: SHEN (SOFTWARE HEAAT EXCHANGERS NETWORKS     ").append("</p>");
        text.append("<p>").append("     Intellectual Property: Technological Institute of Celaya     ").append("</p>");
        text.append("<p>").append("     Project directed by: Dr. Arturo Jimenez Gutierrez     ").append("</p>");
        text.append("<p>").append("     Contributors: Ing. Gabriel Hernàndez Morales     ").append("</p>");
        text.append("<p>").append("                                                      ").append("</p>");
        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
        QString  titulo = espaceadorcalculos(item_select,row);
        text.append("<p>").append( titulo ).append("</p>");
        text.append("<p>").append("                                                      ").append("</p>");
        text.append("<table><tbody>");
        text.append("<tr>"); //abre
        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
        }
        text.append("</tr>"); // ciertta
        for (int i = 0; i < ui->tablewidget->rowCount(); i++) {
            text.append("<tr>");
            for (int j = 0; j < ui->tablewidget->columnCount(); j++) {
                QTableWidgetItem *item = ui->tablewidget->item(i, j);
                if (!item || item->text().isEmpty()) {
                    ui->tablewidget->setItem(i, j, new QTableWidgetItem("0"));
                }
                text.append("<td>").append("        "+ui->tablewidget->item(i, j)->text()+"        ").append("</td>");
            }
            text.append("</tr>");
        }
        text.append("</tbody></table>");
        text.append("</body></html>");
        doc.setDefaultStyleSheet(style);
        doc.setHtml(text);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);
    }else if(incremento == true){
        int Iteraciones = (Max-Min)/Inc;
        QString style("<style>");
        style.append("body { color:black; } ");
        style.append("table { border-collapse: collapse;font-size:10px; }");
        style.append("table, th, td { border: 1px solid black; text-align: center; }");
        style.append("</style>");
        QString text("<html><body>");
        text.append("<h>").append("     " + citydate + "     ").append("</h>");
        text.append("<p>").append("     Program: SHEN (SOFTWARE HEAAT EXCHANGERS NETWORKS     ").append("</p>");
        text.append("<p>").append("     Intellectual Property: Technological Institute of Celaya     ").append("</p>");
        text.append("<p>").append("     Project directed by: Dr. Arturo Jimenez Gutierrez     ").append("</p>");
        text.append("<p>").append("     Contributors: Ing. Gabriel Hernàndez Morales     ").append("</p>");
        text.append("<p>").append("                                                      ").append("</p>");
        CONTADORFILAS = 0;
        QString row1,row2,row,titulo;
        for(int j = 0 ; j <  Iteraciones+1 ;j++){
            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
            titulo = espaceadorcalculos(item_select,row);
            text.append("<p>").append(titulo).append("</p>");
            text.append("<p>").append("                                                      ").append("</p>");
            text.append("<table><tbody>");
            text.append("<tr>"); //abre
            for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
            }
            text.append("</tr>"); // ciertta
            for (int i = CONTADORFILAS; i < ui->tablewidget->rowCount(); i++) {
                text.append("<tr>");
                CONTADORFILAS = CONTADORFILAS + 1;
                for (int j = 0; j < ui->tablewidget->columnCount(); j++) {
                    QTableWidgetItem *item = ui->tablewidget->item(i, j);
                    if (!item || item->text().isEmpty()) {
                        ui->tablewidget->setItem(i, j, new QTableWidgetItem("0"));
                    }
                    text.append("<td>").append("        "+ui->tablewidget->item(i, j)->text()+"        ").append("</td>");
                }
                text.append("</tr>");
                if(i < ui->tablewidget->rowCount() - 2){
                    row1 = ui->tablewidget->verticalHeaderItem(i)->text();
                    row2 = ui->tablewidget->verticalHeaderItem(i+1)->text();
                    if(compararrows(row1,row2) == true){ // son diferentes
                        break;
                    }
                }
            }
            text.append("</tbody></table>");
        }
        text.append("</body></html>");
        doc.setDefaultStyleSheet(style);
        doc.setHtml(text);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);
    }
}

QString Summary::espaceadorcalculos(QString text,QString row)
{
    QString espace;
    if(Ventanamadre == 1){ // VENTANA ANALISIS PERSONALIZADO
        if(text =="Problem table" || text == "Minimal hot utility"
                || text == "Minimal cold utility" || text == "Location of the pinch point"){
            if(TabAnali == 0){ // uniforme
                if(estatico == true  || incremento == true ){
                    QString inicio = "Table problem with";
                    QString final;
                    if(SI == true ){
                        final = " ªF";
                        espace = inicio+ row + final;
                    }else if(SIS == true){
                        final = " ªC";
                        espace = inicio+ row + final;
                    }
                }
            }
        }
    }
    return espace;
}

bool Summary::compararrows(QString row1, QString row2)
{
    if(row1 == row2){ // false soy iguales
        return false;
    }else{ // true son diferentes
        return true;
    }
}


