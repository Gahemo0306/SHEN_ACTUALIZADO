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
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 <<"Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Data points";
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
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 <<"Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Data points";
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
                if(Checkboxes[8] == 1){ // Areas vs DTmin
                    LISTA1 = "Areas vs DTmin";
                    LISTA2 << "Data points" ;
                    AddRoots(LISTA1,LISTA2);
                    LISTA2.clear();
                }
                if(Checkboxes[9] == 1){ // Cost vs DTmin
                    LISTA1 = "Cost vs DTmin";
                    LISTA2 << "Data points";
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
    clicked = ui->treeWidget->currentItem();
    item_select = clicked->text(column);//ui->treeWidget->currentItem()->text(column);
    desplegar_info(item_select);
}

void Summary::desplegar_info(QString text)
{
    ui->qcustomplot->setVisible(false);
    ui->tablewidget->setVisible(false);
    ui->qcustomplot->clearGraphs();
    title = new QCPTextElement(ui->qcustomplot, "  ", QFont("sans", 12, QFont::Bold));
    if(Ventanamadre == 1){//ANALISIS PERSONALIZADO
        if(text == "Problem table" || text == "Minimal hot utility" || text == "Minimal cold utility" ||
                text == "Location of the pinch point" ){
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
        }else if(text == "Area prediction" || text == "DTm log" || text == "Areas" || text == "Enthalpy"){
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
        }else if(text == "Cost prediction" || text == "Agglomerates" || text == "Capital cost" ||
                 text == "Operational cost" || text =="Total cost"){
            if(TabAnali == 0){ // UNIFORME
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double DTmin = Min;
                    costos_estatico_uniforme(DTmin,text);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        costos_incremento_uniforme(DTmin,text);
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 1){ // DIVERSO
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    costos_estatico_diversa(DTmin,k,text,punto1,punto2);
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        costos_incremento_diverso(DTmin,k,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }else if(TabAnali == 2){ // BOTH
                if(estatico == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    costos_estatico_both(DTmin,k,text,punto1,punto2);
                    CONTADORROWS.clear();
                }else if(incremento == true){
                    ui->tablewidget->clear();
                    ui->tablewidget->setVisible(true);
                    QStringList Headers;
                    Headers << "Area Agglomerate" << "Energy Agglomerate" << "Capital cost" << "Operational cost" << "Total cost" ;
                    ui->tablewidget->setColumnCount(5);
                    ui->tablewidget->setHorizontalHeaderLabels(Headers);
                    double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
                    double Iteraciones =(Maximo-Minimo)/Incremento;
                    double DTmin = Minimo;
                    float punto1 = .5, punto2 = 10;
                    CONTADORFILAS = 0;
                    for(int i = 0; i < (Iteraciones+1) ; i++){
                        costos_incremento_both(DTmin,k,text,punto1,punto2);
                        punto1 = K1;
                        punto2 = 10;
                        DTmin = DTmin + Incremento;
                    }
                    CONTADORROWS.clear();
                }
            }
        }else if(text == "Hot compositive curve"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                if(estatico == true){
                    grafico_HCC_estatico_uniforme();
                }else if(incremento == true){
                    grafico_HCC_estatico_uniforme();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_HCC_estatico_diverso(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K , DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_HCC_estatico_diverso(DTmin,k,punto1,punto2);
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_HCC_estatico_both(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_HCC_estatico_both(DTmin,k,punto1,punto2);
                }
            }
        }else if(text == "Cold compositive curve"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->clearGraphs();
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                if(estatico == true){
                    grafico_CCC_estatico_uniforme();
                }else if(incremento == true){
                    grafico_CCC_estatico_uniforme();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CCC_estatico_diverso(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CCC_estatico_diverso(DTmin,k,punto1,punto2);
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CCC_estatico_both(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CCC_estatico_both(DTmin,k,punto1,punto2);
                }
            }
        }else if(text == "Compositive curves"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                if(estatico == true){
                    grafico_CurvasCompuestas_estatico_uniforme();
                }else if(incremento == true){
                    grafico_CurvasCompuestas_estatico_uniforme();
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestas_estatico_diverso(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestas_estatico_diverso(DTmin,k,punto1,punto2);
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestas_estatico_both(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestas_estatico_both(DTmin,k,punto1,punto2);
                }
            }
        }else if(text == "Compositive curves adjusted"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                if(estatico == true){
                    double DTmin = Min;
                    grafico_CurvasCompuestasAjustadas_estatico_uniforme(DTmin);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    grafico_CurvasCompuestasAjustadas_incremento_uniforme(Maximo,Minimo,Incremento);
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestasAjustadas_estatico_diverso(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestasAjustadas_incremento_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestasAjustadas_estatico_both(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_CurvasCompuestasAjustadaas_incremento_both(Minimo,Maximo,Incremento,k,punto1,punto2);
                }
            }
        }else if(text == "Grand compositive curve"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                if(estatico == true){
                    double DTmin = Min;
                    grafico_GranCurvaCompuesta_estatico_uniforme(DTmin);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    grafico_GranCurvaCompuesta_incremento_uniforme(Maximo,Minimo,Incremento);
                }
            }else if(TabAnali == 1){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_GranCurvaCompuesta_estatico_diverso(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_GranCurvaCompuestas_incremento_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
                }
            }else if(TabAnali == 2){
                if(estatico == true){
                    double k = K, DTmin = Min;
                    float punto1 = .5, punto2 = 10;
                    grafico_GranCurvaCompuesta_estatico_both(DTmin,k,punto1,punto2);
                }else if(incremento == true){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_GranCurvaCompuesta_incremento_both(Minimo,Maximo,Incremento,k,punto1,punto2);
                }
            }
        }else if(text == "Areas vs DTmin"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    grafico_Areas_uniforme(Minimo,Maximo,Incremento);
            }else if(TabAnali == 1){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_Areas_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
            }else if(TabAnali == 2){
                    double Minimo = Min;
                    double Maximo = Max;
                    double Incremento = Inc;
                    double k = K;
                    float punto1 = .5, punto2 = 10;
                    grafico_Areas_both(Minimo,Maximo,Incremento,k,punto1,punto2);
            }
        }else if(text == "Cost vs DTmin"){
            ui->qcustomplot->setVisible(true);
            ui->qcustomplot->legend->setVisible(true);
            ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
            ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
            if(TabAnali == 0 ){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                grafico_Costos_uniforme(Minimo,Maximo,Incremento);
            }else if(TabAnali == 1){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                grafico_Costos_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
            }else if(TabAnali == 2){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                grafico_Costos_both(Minimo,Maximo,Incremento,k,punto1,punto2);
            }
        }else if(text == "Data points"){
            int column = ui->treeWidget->currentColumn();
            item_select_datapoint = ui->treeWidget->itemAbove(clicked)->text(column);
            DataPoints(item_select_datapoint);
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
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    ui->tablewidget->setRowCount(AreaAglomerados.size());
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i = 0; i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_incremento_uniforme(double DTmin, QString text)
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
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_estatico_diversa(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Costos_vs_Min_Divera plot(TS,TE,WCP,H,Calentamiento,Enfriamento,CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
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
    double valuek = plot.getK();
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    ui->tablewidget->setRowCount(AreaAglomerados.size());
    int row = ui->tablewidget->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(valuek) ;
    }
    ui->tablewidget->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i =0; i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j=j+1;
    }
    if(text == "Agglomerates"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i =0; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_incremento_diverso(double DTmin, double k, QString text, float punto1, float punto2)
{
    Plot_Costos_vs_Min_Divera plot(TS,TE,WCP,H,Calentamiento,Enfriamento,CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
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
    double valuek = plot.getK();
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(valuek) ;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_estatico_both(double DTmin, double k, QString text, float punto1, float punto2)
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
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
    Plot_Costos_vs_Min_Divera plot1(TS,TE,WCP,H,Calentamiento,Enfriamento,CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
    MOpeCosC = plot1.getOpeCosC();
    MOpeCosH = plot1.getOpeCosH();
    MCapCos3 = plot1.getCapCos3();
    MCapCos2 = plot1.getCapCos2();
    MCapCos1 = plot1.getCapCos1();
    MAH = plot1.getAH();
    MQH = plot1.getQH();
    MAR = plot1.getAR();
    MQR = plot1.getQR();
    MAC = plot1.getAC();
    MQC = plot1.getQC();
    double valuek = plot1.getK();
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - AreaAglomerados.size() ; i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(valuek) ;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j=0;
    for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j=j+1;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS; i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::costos_incremento_both(double DTmin, double k, QString text, float punto1, float punto2)
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
    QVector<double> AreaAglomerados, EnergiaAglomerados,VecCapitalCost,VecOperationCost,TotalCost;
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5;
    int j=0;
    for(int i = CONTADORFILAS - AreaAglomerados.size(); i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
    Plot_Costos_vs_Min_Divera plot1(TS,TE,WCP,H,Calentamiento,Enfriamento,CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
    MOpeCosC = plot1.getOpeCosC();
    MOpeCosH = plot1.getOpeCosH();
    MCapCos3 = plot1.getCapCos3();
    MCapCos2 = plot1.getCapCos2();
    MCapCos1 = plot1.getCapCos1();
    MAH = plot1.getAH();
    MQH = plot1.getQH();
    MAR = plot1.getAR();
    MQR = plot1.getQR();
    MAC = plot1.getAC();
    MQC = plot1.getQC();
    double valuek = plot1.getK();
    AreaAglomerados.resize(3),EnergiaAglomerados.resize(3), VecCapitalCost.resize(3),VecOperationCost.resize(3),TotalCost.resize(3);
    AreaAglomerados[0] = MAC;
    AreaAglomerados[1] = MAR;
    AreaAglomerados[2] = MAH;
    EnergiaAglomerados[0] = MQC;
    EnergiaAglomerados[1] = MQR;
    EnergiaAglomerados[2] = MQH;
    VecCapitalCost[0] = MCapCos1;
    VecCapitalCost[1] = MCapCos2;
    VecCapitalCost[2] = MCapCos3;
    VecOperationCost[0] = MOpeCosC;
    VecOperationCost[1] = 0;
    VecOperationCost[2] = MOpeCosH;
    TotalCost[0] = VecCapitalCost[0] + VecOperationCost[0];
    TotalCost[1] = VecCapitalCost[1];
    TotalCost[2] = VecCapitalCost[2] + VecOperationCost[2];
    CONTADORFILAS = CONTADORFILAS + AreaAglomerados.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(valuek) ;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j=0;
    for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
        val1 = AreaAglomerados[j];
        val2 = EnergiaAglomerados[j];
        val3 = VecCapitalCost[j];
        val4 = VecOperationCost[j];
        val5 = TotalCost[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
    if(text == "Agglomerates"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,0)->setBackground(Qt::red); // areas
            ui->tablewidget->item(i,1)->setBackground(Qt::red); // areas
        }
    }else if(text == "Capital cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,2)->setBackground(Qt::blue); // areas
        }
    }else if(text == "Operational cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,3)->setBackground(Qt::yellow); // areas
        }
    }else if(text =="Total cost"){
        for(int i = CONTADORFILAS- AreaAglomerados.size(); i < row ; i++){
            ui->tablewidget->item(i,4)->setBackground(Qt::yellow); // areas
        }
    }
}

void Summary::grafico_HCC_estatico_uniforme()
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CCC_estatico_uniforme()
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform cold curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CFENTALPIA,CFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_HCC_estatico_diverso(double DTmin, double k , float punto1,float punto2)
{
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Diverse hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);;
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CCC_estatico_diverso(double DTmin, double k , float punto1,float punto2)
{
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CFENTALPIA = plot1.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Diverse cold curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CFENTALPIA,CFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_HCC_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> UCCENTALPIA = plot.getCCENTALPIA();
    QVector<double> UCCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(UCCENTALPIA,UCCTEMPERATURAS);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> DCCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> DCCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Diverse hot curve");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(1)->setData(DCCENTALPIA,DCCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CCC_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> UCFENTALPIA = plot.getCFENTALPIA();
    QVector<double> UCFTEMPERATURAS = plot.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform cold curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(UCFENTALPIA,UCFTEMPERATURAS);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> DCFENTALPIA = plot1.getCFENTALPIA();
    QVector<double> DCFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Diverse cold curve");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(1)->setData(DCFENTALPIA,DCFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestas_estatico_uniforme()
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Uniform cold curve");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setData(CFENTALPIA,CFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestas_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Diverse hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->replot();
    QVector<double> CFENTALPIA = plot1.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Diverse cold curve");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setData(CFENTALPIA,CFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestas_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Uniform cold curve");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setData(CFENTALPIA,CFTEMPERATURAS);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CCENTALPIA = plot1.getCCENTALPIA();
    CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(2)->setName("Diverse hot curve");
    ui->qcustomplot->graph(2)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(2)->setData(CCENTALPIA,CCTEMPERATURAS);
    CFENTALPIA = plot1.getCFENTALPIA();
    CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(3)->setName("Diverse cold curve");
    ui->qcustomplot->graph(3)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(3)->setData(CFENTALPIA,CFTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadas_estatico_uniforme(double DTmin)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setName("Uniform cold curve");
    ui->qcustomplot->graph(1)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
    ui->qcustomplot->graph(0)->rescaleAxes();
    ui->qcustomplot->graph(1)->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadas_incremento_uniforme(double MAX, double MIN, double INC)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int j=0;
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(j)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->graph(j)->rescaleAxes();
    ui->qcustomplot->graph(j)->setName("Uniform hot curve");
    for(int i = 0; i <= Iteraciones ; i++){
        j++;
        Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
        QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
        ui->qcustomplot->addGraph();
        if(j == 1){
            ui->qcustomplot->graph(j)->setName("Uniform cold curve");
        }else if(j >= 2){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
        DTmin = DTmin + INC;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadas_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot2.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot2.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Diverse hot curve");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->replot();
    QVector<double> CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setName("Diverse cold curve");
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadas_incremento_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int j=0;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<double> CCENTALPIA = plot2.getCCENTALPIA();
        QVector<double> CCTEMPERATURAS = plot2.getCCTEMPERATURAS();
        ui->qcustomplot->addGraph();
        if(i == 0){
            ui->qcustomplot->graph(j)->setName("Diverse hot curve");
        }else if( i >= 1){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        ui->qcustomplot->graph(j)->setData(CCENTALPIA,CCTEMPERATURAS);
        j++;
        QVector<double> CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
        ui->qcustomplot->addGraph();
        if(i == 0){
            ui->qcustomplot->graph(j)->setName("Diverse cold curve");
        }else if( i >= 1){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
        j++;
        punto1 = float (plot2.getK());
        punto2 = 10.0;
        DTmin = DTmin + INC;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadas_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setName("Uniform hot curve");
    ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
    QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setName("Uniform cold curve");
    ui->qcustomplot->graph(1)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
    Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CCENTALPIA = plot2.getCCENTALPIA();
    CCTEMPERATURAS = plot2.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(2)->setName("Diverse hot curve");
    ui->qcustomplot->graph(2)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(2)->setData(CCENTALPIA,CCTEMPERATURAS);
    CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
    CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(3)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(3)->setName("Diverse cold curve");
    ui->qcustomplot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(3)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_CurvasCompuestasAjustadaas_incremento_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int j=0;
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(j)->setData(CCENTALPIA,CCTEMPERATURAS);
    ui->qcustomplot->graph(j)->setName("Uniform hot curve");
    j++;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
        QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
        ui->qcustomplot->addGraph();
        if(j == 1){
            ui->qcustomplot->graph(j)->setName("Uniform cold curve");
        }else if(j >= 2){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
        j++;
        Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<double> DCCENTALPIA = plot2.getCCENTALPIA();
        QVector<double> DCCTEMPERATURAS = plot2.getCCTEMPERATURAS();
        ui->qcustomplot->addGraph();
        if(i == 0){
            ui->qcustomplot->graph(j)->setName("Diverse hot curve");
        }else if( i >= 1){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
        ui->qcustomplot->graph(j)->setData(DCCENTALPIA,DCCTEMPERATURAS);
        j++;
        CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
        CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
        ui->qcustomplot->addGraph();
        if(i == 0){
            ui->qcustomplot->graph(j)->setName("Diverse cold curve");
        }else if( i >= 1){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
        ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
        ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
        j++;
        punto1 = float (plot2.getK());
        punto2 = 10.0;
        DTmin = DTmin + INC;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuesta_estatico_uniforme(double DTmin)
{
    Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
    QVector<double> GCENTALPIA = plot.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Uniform grand composite curve");
    ui->qcustomplot->graph(0)->setData(GCENTALPIA,GCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->graph(0)->rescaleAxes();
    ui->qcustomplot->graph(0)->rescaleAxes(true);
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuesta_incremento_uniforme(double MAX, double MIN, double INC)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
         Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
         QVector<double> GCENTALPIA = plot.getGCENTALPIA();
         QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
         ui->qcustomplot->addGraph();
         if(i == 0){
             ui->qcustomplot->graph(i)->setName("Uniform grand composite curve");
         }else if( i >= 1){
             ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
         }
         ui->qcustomplot->graph(i)->setPen(QPen(Qt::blue));
         ui->qcustomplot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
         ui->qcustomplot->graph(i)->setLineStyle(QCPGraph::lsLine);
         ui->qcustomplot->graph(i)->setData(GCENTALPIA,GCTEMPERATURAS);
         DTmin = DTmin + INC;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuesta_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Diverse grand composite curve");
    ui->qcustomplot->graph(0)->setData(GCENTALPIA,GCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuestas_incremento_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
        QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
        ui->qcustomplot->addGraph();
        if(i == 0){
            ui->qcustomplot->graph(i)->setName("Diverse grand composite curve");
        }else if( i >= 1){
            ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
        }
        ui->qcustomplot->graph(i)->setPen(QPen(Qt::blue));
        ui->qcustomplot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        ui->qcustomplot->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->qcustomplot->graph(i)->setData(GCENTALPIA,GCTEMPERATURAS);
        DTmin = DTmin + INC;
        punto1 = float (plotGCCD.getK());
        punto2 = 10.0;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuesta_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
    QVector<double> GCENTALPIA = plot.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Uniform grand composite curve");
    ui->qcustomplot->graph(0)->setData(GCENTALPIA,GCTEMPERATURAS);
    PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    GCENTALPIA = plotGCCD.getGCENTALPIA();
    GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(1)->setName("Diverse grand composite curve");
    ui->qcustomplot->graph(1)->setData(GCENTALPIA,GCTEMPERATURAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_GranCurvaCompuesta_incremento_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int j = 0;
    for(int i = 0; i <= Iteraciones ; i++){
         Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
         QVector<double> GCENTALPIA = plot.getGCENTALPIA();
         QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
         ui->qcustomplot->addGraph();
         if(i == 0){
             ui->qcustomplot->graph(j)->setName("Uniform grand composite curve");
         }else if( i >= 1){
             ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
         }
         ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
         ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
         ui->qcustomplot->graph(j)->setLineStyle(QCPGraph::lsLine);
         ui->qcustomplot->graph(j)->setData(GCENTALPIA,GCTEMPERATURAS);
         j++;
         PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
         GCENTALPIA = plotGCCD.getGCENTALPIA();
         GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
         ui->qcustomplot->addGraph();
         if(i == 0){
             ui->qcustomplot->graph(j)->setName("Diverse grand composite curve");
         }else if( i >= 1){
             ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
         }
         ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
         ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
         ui->qcustomplot->graph(j)->setLineStyle(QCPGraph::lsLine);
         ui->qcustomplot->graph(j)->setData(GCENTALPIA,GCTEMPERATURAS);
         j++;
         punto1 = float (plotGCCD.getK());
         punto2 = 10.0;
         DTmin = DTmin + INC;
    }
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
        ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Areas_uniforme(double MIN, double MAX, double INC)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> AREAS,DTMIN;
    AREAS.resize(Iteraciones+1);
    DTMIN.resize(Iteraciones+1);
    int l = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
         Plot_Dtmin_vs_Areas plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
         double AREA = plot3.getAREAS();
         if(AREA < 0){
             continue;
         }else{
             AREAS[l] = AREA;
             DTMIN[l] = DTmin;
             l++;
         }
         DTmin = DTmin + INC;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Unifom total area");
    ui->qcustomplot->graph(0)->setData(DTMIN,AREAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("AREAS ft^2");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("AREAS mt^2");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Areas_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> AREAS,DTMIN;
    AREAS.resize(Iteraciones+1);
    DTMIN.resize(Iteraciones+1);
    int l = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Dtmin_vs_Areas_DIVERSO plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                         k,DTmin,punto1,punto2);
        double AREA = plot3.getAREAS();
        if(AREA < 0){
           continue;
        }else{
            AREAS[l] = AREA;
            DTMIN[l] = DTmin;
            l++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot3.getK());
        punto2 = 10.0;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Diverse total area");
    ui->qcustomplot->graph(0)->setData(DTMIN,AREAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("AREAS ft^2");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("AREAS mt^2");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Areas_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    QVector<double> UAREAS,DAREAS;
    QVector<double> UDTMIN,DDTMIN;
    UAREAS.resize(Iteraciones+1);
    DAREAS.resize(Iteraciones+1);
    UDTMIN.resize(Iteraciones+1);
    DDTMIN.resize(Iteraciones+1);
    double DTmin = MIN;
    int u = 0, d = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Dtmin_vs_Areas plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
        double UAREA = plot3.getAREAS();
        if(UAREA < 0){
            continue;
        }else{
            UAREAS[u] = UAREA;
            UDTMIN[u] = DTmin;
            u++;
        }
        Plot_Dtmin_vs_Areas_DIVERSO plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                         k,DTmin,punto1,punto2);
        double DAREA = plot4.getAREAS();
        if(DAREA < 0){
           continue;
        }else{
            DAREAS[d] = DAREA;
            DDTMIN[d] = DTmin;
            d++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot4.getK());
        punto2 = 10.0;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setName("Uniform total area");
    ui->qcustomplot->graph(0)->setData(UDTMIN,UAREAS);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(1)->setName("Diverse total area");
    ui->qcustomplot->graph(1)->setData(DDTMIN,DAREAS);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("AREAS ft^2");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("AREAS mt^2");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Costos_uniforme(double MIN, double MAX, double INC)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN;
    VECCOSTOTOTAL.resize(Iteraciones+1);
    VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    VECDTMIN.resize(Iteraciones+1);
    int l = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Costos_vs_Areas_Uniforme plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                            CapitalCost,OperationCost,DTmin,CTo,CCo,SI,SIS);
        double COSTOTOTAL = plot4.getCostoTotal();
        double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
        double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            VECCOSTOTOTAL[l] = COSTOTOTAL;
            VECCOSTOCAPITALTOTAL[l] = COSTOCAPITALTOTAL;
            VECCOSTOOPERACIONTOTAL[l] = COSTOOPERACIONTOTAL;
            VECDTMIN[l] = DTmin;
            l++;
        }
        DTmin = DTmin + INC;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Unifom total cost");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setData(VECDTMIN,VECCOSTOTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Uniform total operation cost");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(1)->setData(VECDTMIN,VECCOSTOOPERACIONTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(2)->setName("Uniform total capital cost");
    ui->qcustomplot->graph(2)->setPen(QPen(Qt::darkCyan));
    ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(2)->setData(VECDTMIN,VECCOSTOCAPITALTOTAL);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Costos_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN;
    VECCOSTOTOTAL.resize(Iteraciones+1);
    VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    VECDTMIN.resize(Iteraciones+1);
    int l=0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
       Plot_Costos_vs_Min_Divera plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                       CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
       double COSTOTOTAL = plot4.getCostoTotal();
       double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
       double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
       if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
           continue;
       }else{
           VECCOSTOTOTAL[l] = COSTOTOTAL;
           VECCOSTOCAPITALTOTAL[l] = COSTOCAPITALTOTAL;
           VECCOSTOOPERACIONTOTAL[l] = COSTOOPERACIONTOTAL;
           VECDTMIN[l] = DTmin;
       }
       DTmin = DTmin + INC;
       punto1 = float (plot4.getK());
       punto2 = 10.0;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Diverse total cost");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setData(VECDTMIN,VECCOSTOTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Diverse total operation cost");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(1)->setData(VECDTMIN,VECCOSTOOPERACIONTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(2)->setName("Diverse total capital cost");
    ui->qcustomplot->graph(2)->setPen(QPen(Qt::darkCyan));
    ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(2)->setData(VECDTMIN,VECCOSTOCAPITALTOTAL);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }
    ui->qcustomplot->replot();
}

void Summary::grafico_Costos_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> UVECCOSTOTOTAL,UVECCOSTOCAPITALTOTAL,UVECCOSTOOPERACIONTOTAL,UVECDTMIN;
    QVector<double> DVECCOSTOTOTAL,DVECCOSTOCAPITALTOTAL,DVECCOSTOOPERACIONTOTAL,DVECDTMIN;
    UVECCOSTOTOTAL.resize(Iteraciones+1);
    UVECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    UVECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    UVECDTMIN.resize(Iteraciones+1);
    DVECCOSTOTOTAL.resize(Iteraciones+1);
    DVECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    DVECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    DVECDTMIN.resize(Iteraciones+1);
    int U = 0,D = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Costos_vs_Areas_Uniforme plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                            CapitalCost,OperationCost,DTmin,CTo,CCo,SI,SIS);
        double COSTOTOTAL = plot4.getCostoTotal();
        double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
        double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            UVECCOSTOTOTAL[U] = COSTOTOTAL;
            UVECCOSTOCAPITALTOTAL[U] = COSTOCAPITALTOTAL;
            UVECCOSTOOPERACIONTOTAL[U] = COSTOOPERACIONTOTAL;
            UVECDTMIN[U] = DTmin;
            U++;
        }
        Plot_Costos_vs_Min_Divera plot2(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                        CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
        COSTOTOTAL = plot2.getCostoTotal();
        COSTOCAPITALTOTAL = plot2.getCostoCapitalTotal();
        COSTOOPERACIONTOTAL = plot2.getCostoOperacionTotal();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            DVECCOSTOTOTAL[D] = COSTOTOTAL;
            DVECCOSTOCAPITALTOTAL[D] = COSTOCAPITALTOTAL;
            DVECCOSTOOPERACIONTOTAL[D] = COSTOOPERACIONTOTAL;
            DVECDTMIN[D] = DTmin;
            D++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot2.getK());
        punto2 = 10.0;
    }
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName("Uniform total cost");
    ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(0)->setData(UVECDTMIN,UVECCOSTOTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName("Uniform total operation cost");
    ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(1)->setData(UVECDTMIN,UVECCOSTOOPERACIONTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(2)->setName("Uniform total capital cost");
    ui->qcustomplot->graph(2)->setPen(QPen(Qt::darkCyan));
    ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    ui->qcustomplot->graph(2)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(2)->setData(UVECDTMIN,UVECCOSTOCAPITALTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(3)->setName("Diverse total cost");
    ui->qcustomplot->graph(3)->setPen(QPen(Qt::red));
    ui->qcustomplot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(3)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(3)->setData(DVECDTMIN,DVECCOSTOTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(4)->setName("Diverse total operation cost");
    ui->qcustomplot->graph(4)->setPen(QPen(Qt::blue));
    ui->qcustomplot->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(4)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(4)->setData(DVECDTMIN,DVECCOSTOOPERACIONTOTAL);
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(5)->setName("Diverse total capital cost");
    ui->qcustomplot->graph(5)->setPen(QPen(Qt::darkCyan));
    ui->qcustomplot->graph(5)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond));
    ui->qcustomplot->graph(5)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->graph(5)->setData(DVECDTMIN,DVECCOSTOCAPITALTOTAL);
    ui->qcustomplot->rescaleAxes(true);
    if(SI == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }else if(SIS == true){
        ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
        ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
    }
    ui->qcustomplot->replot();

}

void Summary::datapoints_HCC_estatico_uniforme()
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    ui->tablewidget->setRowCount(CCENTALPIA.size());
    double val1, val2;
    int row = ui->tablewidget->rowCount();
    QStringList lista;
    for(int i =0; i < row ; i++){
        lista << "Uniform hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(lista);
    for(int i =0; i < row ; i++){
        val1 = CCENTALPIA[i];
        val2 = CCTEMPERATURAS[i];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
    }
}

void Summary::datapoints_CCC_estatico_uniforme()
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    ui->tablewidget->setRowCount(CFENTALPIA.size());
    double val1, val2;
    int row = ui->tablewidget->rowCount();
    QStringList lista;
    for(int i =0; i < row ; i++){
        lista << "Uniform cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(lista);
    for(int i =0; i < row ; i++){
        val1 = CFENTALPIA[i];
        val2 = CFTEMPERATURAS[i];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
    }
}

void Summary::datapoints_HCC_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    ui->tablewidget->setRowCount(CCENTALPIA.size());
    double val1, val2;
    int row = ui->tablewidget->rowCount();
    QStringList lista;
    for(int i =0; i < row ; i++){
        lista << "Diverse hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(lista);
    for(int i =0; i < row ; i++){
        val1 = CCENTALPIA[i];
        val2 = CCTEMPERATURAS[i];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
    }
}

void Summary::datapoints_CCC_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CFENTALPIA = plot1.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    ui->tablewidget->setRowCount(CFENTALPIA.size());
    double val1, val2;
    int row = ui->tablewidget->rowCount();
    QStringList lista;
    for(int i =0; i < row ; i++){
        lista << "Diverse cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(lista);
    for(int i =0; i < row ; i++){
        val1 = CFENTALPIA[i];
        val2 = CFTEMPERATURAS[i];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
    }
}

void Summary::datapoints_HCC_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2;
    int row = ui->tablewidget->rowCount();
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CCENTALPIA = plot1.getCCENTALPIA();
    CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse hot curve" ;
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    row = ui->tablewidget->rowCount();
    j= 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CCC_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    double val1, val2;
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int row = ui->tablewidget->rowCount();
    int j = 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size() ; i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CFENTALPIA = plot1.getCFENTALPIA();
    CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    row = ui->tablewidget->rowCount();
    j= 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestas_estatico_uniforme()
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size() ; i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestas_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIA = plot1.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size() ; i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestas_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestas plot(TS,TE,WCP);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIA = plot.getCFENTALPIA();
    QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size() ; i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    Plot_curvascompuestas_diversa plot1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CCENTALPIA = plot1.getCCENTALPIA();
    CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse hot curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    CFENTALPIA = plot1.getCFENTALPIA();
    CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cold curve";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIA.size() ; i < row ; i++){
        val1 = CFENTALPIA[j];
        val2 = CFTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadas_estatico_uniforme(double DTmin)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform hot curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cold curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
        val1 = CFENTALPIAAJUSTADAS[j];
        val2 = CFTEMPERATURASAJUSTADAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadas_incremento_uniforme(double MAX, double MIN, double INC)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
        QVector<double> CCENTALPIA = plot.getCCENTALPIA();
        QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
        CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
        double val1, val2;
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform hot curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
            val1 = CCENTALPIA[j];
            val2 = CCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
        CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform cold curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
            val1 = CFENTALPIAAJUSTADAS[j];
            val2 = CFTEMPERATURASAJUSTADAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin + INC;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadas_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> CCENTALPIA = plot.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    double valuek = plot.getK();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse hot curve with DTmin" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cold curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
        val1 = CFENTALPIAAJUSTADAS[j];
        val2 = CFTEMPERATURASAJUSTADAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadas_incremento_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<double> CCENTALPIA = plot.getCCENTALPIA();
        QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
        double valuek = plot.getK();
        CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
        double val1, val2;
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse hot curve with DTmin" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
            val1 = CCENTALPIA[j];
            val2 = CCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
        CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse cold curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
            val1 = CFENTALPIAAJUSTADAS[j];
            val2 = CFTEMPERATURASAJUSTADAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin + INC;
        punto1 = plot.getK();
        punto2 = 10;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadas_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_curvascompuestasajustadas plot1(TS,TE,WCP,DTmin);
    QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
    QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    double val1, val2;
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform hot curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    QVector<double> CFENTALPIAAJUSTADAS = plot1.getCFENTALPIAAJUSTADAS();
    QVector<double> CFTEMPERATURASAJUSTADAS = plot1.getCFTEMPERATURASAJUSTADAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cold curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
        val1 = CFENTALPIAAJUSTADAS[j];
        val2 = CFTEMPERATURASAJUSTADAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    CCENTALPIA = plot.getCCENTALPIA();
    CCTEMPERATURAS = plot.getCCTEMPERATURAS();
    double valuek = plot.getK();
    CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse hot curve with DTmin" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
        val1 = CCENTALPIA[j];
        val2 = CCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
    CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
    CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cold curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
        val1 = CFENTALPIAAJUSTADAS[j];
        val2 = CFTEMPERATURASAJUSTADAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_CurvasCompuestasAjustadaas_incremento_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_curvascompuestasajustadas plot1(TS,TE,WCP,DTmin);
        QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
        QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
        CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
        double val1, val2;
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform hot curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
            val1 = CCENTALPIA[j];
            val2 = CCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        QVector<double> CFENTALPIAAJUSTADAS = plot1.getCFENTALPIAAJUSTADAS();
        QVector<double> CFTEMPERATURASAJUSTADAS = plot1.getCFTEMPERATURASAJUSTADAS();
        CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform cold curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
            val1 = CFENTALPIAAJUSTADAS[j];
            val2 = CFTEMPERATURASAJUSTADAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        CCENTALPIA = plot.getCCENTALPIA();
        CCTEMPERATURAS = plot.getCCTEMPERATURAS();
        double valuek = plot.getK();
        CONTADORFILAS = CONTADORFILAS + CCENTALPIA.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse hot curve with DTmin" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - CCENTALPIA.size() ; i < row ; i++){
            val1 = CCENTALPIA[j];
            val2 = CCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
        CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
        CONTADORFILAS = CONTADORFILAS + CFENTALPIAAJUSTADAS.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse cold curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - CFENTALPIAAJUSTADAS.size() ; i < row ; i++){
            val1 = CFENTALPIAAJUSTADAS[j];
            val2 = CFTEMPERATURASAJUSTADAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin + INC;
        punto1 = plot.getK();
        punto2 = 10;
    }
}

void Summary::datapoints_GranCurvaCompuesta_estatico_uniforme(double DTmin)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
    QVector<double> GCENTALPIA = plot.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform grand compositive curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    double val1,val2;
    for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
        val1 = GCENTALPIA[j];
        val2 = GCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_GranCurvaCompuesta_incremento_uniforme(double MAX, double MIN, double INC)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
        QVector<double> GCENTALPIA = plot.getGCENTALPIA();
        QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
        CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform grand compositive curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        double val1,val2;
        for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
            val1 = GCENTALPIA[j];
            val2 = GCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin  + INC;
    }
}

void Summary::datapoints_GranCurvaCompuesta_estatico_diverso(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
    double valuek = plotGCCD.getK();
    CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse grand compositive curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    double val1,val2;
    for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
        val1 = GCENTALPIA[j];
        val2 = GCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_GranCurvaCompuestas_incremento_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        PlotGCC_DIVERSA plotGCCD(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
        QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
        double valuek = plotGCCD.getK();
        CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse grand compositive curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        double val1,val2;
        for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
            val1 = GCENTALPIA[j];
            val2 = GCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin + INC;
        punto1 = plotGCCD.getK();
        punto2 = 10;
    }
}

void Summary::datapoints_GranCurvaCompuesta_estatico_both(double DTmin, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
    QVector<double> GCENTALPIA = plot.getGCENTALPIA();
    QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
    CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    int row = ui->tablewidget->rowCount();
    double val1,val2;
    for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform grand compositive curve with DTmin:" + QString::number(DTmin);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
        val1 = GCENTALPIA[j];
        val2 = GCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
    PlotGCC_DIVERSA plotGCCD1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    GCENTALPIA = plotGCCD1.getGCENTALPIA();
    GCTEMPERATURAS = plotGCCD1.getGCTEMPERATURAS();
    double valuek = plotGCCD1.getK();
    CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    row = ui->tablewidget->rowCount();
    for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse grand compositive curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    j = 0;
    for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
        val1 = GCENTALPIA[j];
        val2 = GCTEMPERATURAS[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_GranCurvaCompuesta_incremento_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Enthalpy" << "Temperatures"  ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    for(int i = 0; i <= Iteraciones ; i++){
        Plot_grancurvacompuesta plot(TS,TE,WCP,DTmin);
        QVector<double> GCENTALPIA = plot.getGCENTALPIA();
        QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
        CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        int row = ui->tablewidget->rowCount();
        double val1,val2;
        for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Uniform grand compositive curve with DTmin:" + QString::number(DTmin);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        int j = 0;
        for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
            val1 = GCENTALPIA[j];
            val2 = GCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        PlotGCC_DIVERSA plotGCCD1(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        GCENTALPIA = plotGCCD1.getGCENTALPIA();
        GCTEMPERATURAS = plotGCCD1.getGCTEMPERATURAS();
        double valuek = plotGCCD1.getK();
        CONTADORFILAS = CONTADORFILAS + GCENTALPIA.size();
        ui->tablewidget->setRowCount(CONTADORFILAS);
        row = ui->tablewidget->rowCount();
        for(int i = CONTADORFILAS - GCENTALPIA.size(); i < CONTADORFILAS; i++){
            CONTADORROWS << "Diverse gran compositive curve with DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
        }
        ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
        j = 0;
        for(int i = CONTADORFILAS - GCENTALPIA.size() ; i < row ; i++){
            val1 = GCENTALPIA[j];
            val2 = GCTEMPERATURAS[j];
            ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
            ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
            j++;
        }
        DTmin = DTmin + INC;
        punto1 = plotGCCD1.getK();
        punto2 = 10;
    }
}

void Summary::datapoints_Areas_uniforme(double MIN, double MAX, double INC)
{
    QStringList Headers;
    Headers << "Uniform total area" << "DTmin" ;
    ui->tablewidget->setColumnCount(2);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> AREAS,DTMIN;
    AREAS.resize(Iteraciones+1);
    DTMIN.resize(Iteraciones+1);
    int l = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Dtmin_vs_Areas plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
        double AREA = plot3.getAREAS();
        if(AREA < 0){
            continue;
        }else{
            AREAS[l] = AREA;
            DTMIN[l] = DTmin;
            l++;
        }
        DTmin = DTmin + INC;
    }
    CONTADORFILAS = CONTADORFILAS +  AREAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - AREAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform area vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2;
    for(int i = CONTADORFILAS - AREAS.size() ; i < row ; i++){
        val1 = AREAS[j];
        val2 = DTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        j++;
    }
}

void Summary::datapoints_Areas_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Diverse total area" << "K" << "DTmin" ;
    ui->tablewidget->setColumnCount(3);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    QVector<double> AREAS,DTMIN,DVECK;
    AREAS.resize(Iteraciones+1);
    DTMIN.resize(Iteraciones+1);
    DVECK.resize(Iteraciones+1);
    int l = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Dtmin_vs_Areas_DIVERSO plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                         k,DTmin,punto1,punto2);
        double AREA = plot3.getAREAS();
        if(AREA < 0){
           continue;
        }else{
            AREAS[l] = AREA;
            DTMIN[l] = DTmin;
            DVECK[l] = plot3.getK();
            l++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot3.getK());
        punto2 = 10.0;
    }
    CONTADORFILAS = CONTADORFILAS +  AREAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - AREAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse area vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2,val3;
    for(int i = CONTADORFILAS - AREAS.size() ; i < row ; i++){
        val1 = AREAS[j];
        val2 = DVECK[j];
        val3 = DTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        j++;
    }
}

void Summary::datapoints_Areas_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Uniform total area" << "Diverse total area" << "K" << "DTmin" ;
    ui->tablewidget->setColumnCount(4);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    QVector<double> UAREAS,DAREAS;
    QVector<double> UDTMIN,DDTMIN,DVECK;
    UAREAS.resize(Iteraciones+1);
    DAREAS.resize(Iteraciones+1);
    UDTMIN.resize(Iteraciones+1);
    DDTMIN.resize(Iteraciones+1);
    DVECK.resize(Iteraciones+1);
    double DTmin = MIN;
    int u = 0 , d = 0;
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Dtmin_vs_Areas plot3(TS,TE,WCP,H,Calentamiento,Enfriamento,DTmin,CTo,CCo);
        double UAREA = plot3.getAREAS();
        if(UAREA < 0){
            continue;
        }else{
            UAREAS[u] = UAREA;
            UDTMIN[u] = DTmin;
            u++;
        }
        Plot_Dtmin_vs_Areas_DIVERSO plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                         k,DTmin,punto1,punto2);
        double DAREA = plot4.getAREAS();
        if(DAREA < 0){
           continue;
        }else{
            DAREAS[d] = DAREA;
            DDTMIN[d] = DTmin;
            DVECK[d] = plot4.getK();
            d++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot4.getK());
        punto2 = 10.0;
    }
    CONTADORFILAS = CONTADORFILAS +  UAREAS.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - UAREAS.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform and diverse area vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2,val3,val4;
    for(int i = CONTADORFILAS - UAREAS.size() ; i < row ; i++){
        val1 = UAREAS[j];
        val2 = DAREAS[j];
        val3 = DVECK[j];
        val4 = UDTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        j++;
    }
}

void Summary::datapoints_Costos_uniforme(double MIN, double MAX, double INC)
{
    QStringList Headers;
    Headers << "Unifom total capital cost" << "Unifom total operation cost" << "Unifom total cost " << "DTmin" ;
    ui->tablewidget->setColumnCount(4);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int l = 0;
    QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN;
    VECCOSTOTOTAL.resize(Iteraciones+1);
    VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    VECDTMIN.resize(Iteraciones+1);
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Costos_vs_Areas_Uniforme plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                            CapitalCost,OperationCost,DTmin,CTo,CCo,SI,SIS);
        double COSTOTOTAL = plot4.getCostoTotal();
        double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
        double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            VECCOSTOTOTAL[l] = COSTOTOTAL;
            VECCOSTOCAPITALTOTAL[l] = COSTOCAPITALTOTAL;
            VECCOSTOOPERACIONTOTAL[l] = COSTOOPERACIONTOTAL;
            VECDTMIN[l] = DTmin;
            l++;
        }
        DTmin = DTmin + INC;
    }
    CONTADORFILAS = CONTADORFILAS +  VECCOSTOTOTAL.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - VECCOSTOTOTAL.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform cost vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2,val3,val4;
    for(int i = CONTADORFILAS - VECCOSTOTOTAL.size(); i < row ; i++){
        val1 = VECCOSTOCAPITALTOTAL[j];
        val2 = VECCOSTOOPERACIONTOTAL[j];
        val3 = VECCOSTOTOTAL[j];
        val4 = VECDTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        j++;
    }
}

void Summary::datapoints_Costos_diverso(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Diverse total capital cost" << "Diverse total operation cost" << "Diverse total cost "<< "K" << "DTmin" ;
    ui->tablewidget->setColumnCount(5);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int l = 0;
    QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN,VECK;
    VECCOSTOTOTAL.resize(Iteraciones+1);
    VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    VECDTMIN.resize(Iteraciones+1);
    VECK.resize(Iteraciones+1);
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Costos_vs_Min_Divera plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                        CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
        double COSTOTOTAL = plot4.getCostoTotal();
        double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
        double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
        double k = plot4.getK();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            VECCOSTOTOTAL[l] = COSTOTOTAL;
            VECCOSTOCAPITALTOTAL[l] = COSTOCAPITALTOTAL;
            VECCOSTOOPERACIONTOTAL[l] = COSTOOPERACIONTOTAL;
            VECDTMIN[l] = DTmin;
            VECK[l] = k;
            l++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot4.getK());
        punto2 = 10.0;
    }
    CONTADORFILAS = CONTADORFILAS +  VECCOSTOTOTAL.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - VECCOSTOTOTAL.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Diverse cost vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2,val3,val4,val5;
    for(int i = CONTADORFILAS - VECCOSTOTOTAL.size(); i < row ; i++){
        val1 = VECCOSTOCAPITALTOTAL[j];
        val2 = VECCOSTOOPERACIONTOTAL[j];
        val3 = VECCOSTOTOTAL[j];
        val4 = VECK[j];
        val5 = VECDTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        j++;
    }
}

void Summary::datapoints_Costos_both(double MIN, double MAX, double INC, double k, float punto1, float punto2)
{
    QStringList Headers;
    Headers << "Unifom total capital cost" << "Unifom total operation cost" << "Unifom total cost "
            << "Diverse total capital cost" << "Diverse total operation cost" << "Diverse total cost " << " K "<< "DTmin" ;
    ui->tablewidget->setColumnCount(8);
    ui->tablewidget->setHorizontalHeaderLabels(Headers);
    double Iteraciones =(MAX-MIN)/INC;
    double DTmin = MIN;
    int u = 0 , d = 0;
    QVector<double> UVECCOSTOTOTAL,UVECCOSTOCAPITALTOTAL,UVECCOSTOOPERACIONTOTAL,UVECDTMIN;
    UVECCOSTOTOTAL.resize(Iteraciones+1);
    UVECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    UVECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    UVECDTMIN.resize(Iteraciones+1);
    QVector<double> DVECCOSTOTOTAL,DVECCOSTOCAPITALTOTAL,DVECCOSTOOPERACIONTOTAL,DVECDTMIN,DVECK;
    DVECCOSTOTOTAL.resize(Iteraciones+1);
    DVECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
    DVECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
    DVECDTMIN.resize(Iteraciones+1);
    DVECK.resize(Iteraciones+1);
    for(int i = 0; i < (Iteraciones+1) ; i++){
        Plot_Costos_vs_Areas_Uniforme plot4(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                            CapitalCost,OperationCost,DTmin,CTo,CCo,SI,SIS);
        double COSTOTOTAL = plot4.getCostoTotal();
        double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
        double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            UVECCOSTOTOTAL[u] = COSTOTOTAL;
            UVECCOSTOCAPITALTOTAL[u] = COSTOCAPITALTOTAL;
            UVECCOSTOOPERACIONTOTAL[u] = COSTOOPERACIONTOTAL;
            UVECDTMIN[u] = DTmin;
            u++;
        }
        Plot_Costos_vs_Min_Divera plot5(TS,TE,WCP,H,Calentamiento,Enfriamento,
                                        CapitalCost,OperationCost,k,DTmin,punto1,punto2,SI,SIS);
        COSTOTOTAL = plot5.getCostoTotal();
        COSTOCAPITALTOTAL = plot5.getCostoCapitalTotal();
        COSTOOPERACIONTOTAL = plot5.getCostoOperacionTotal();
        double VECK = plot5.getK();
        if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
            continue;
        }else{
            DVECCOSTOTOTAL[d] = COSTOTOTAL;
            DVECCOSTOCAPITALTOTAL[d] = COSTOCAPITALTOTAL;
            DVECCOSTOOPERACIONTOTAL[d] = COSTOOPERACIONTOTAL;
            DVECDTMIN[d] = DTmin;
            DVECK[d] = VECK;
            d++;
        }
        DTmin = DTmin + INC;
        punto1 = float (plot5.getK());
        punto2 = 10.0;
    }
    CONTADORFILAS = CONTADORFILAS +  UVECCOSTOTOTAL.size();
    ui->tablewidget->setRowCount(CONTADORFILAS);
    for(int i = CONTADORFILAS - UVECCOSTOTOTAL.size(); i < CONTADORFILAS; i++){
        CONTADORROWS << "Uniform and diverse cost vs DTmin";
    }
    ui->tablewidget->setVerticalHeaderLabels(CONTADORROWS);
    int j = 0;
    int row = ui->tablewidget->rowCount();
    double val1,val2,val3,val4,val5,val6,val7,val8;
    for(int i = CONTADORFILAS - UVECCOSTOTOTAL.size(); i < row ; i++){
        val1 = UVECCOSTOCAPITALTOTAL[j];
        val2 = UVECCOSTOOPERACIONTOTAL[j];
        val3 = UVECCOSTOTOTAL[j];
        val4 = DVECCOSTOCAPITALTOTAL[j];
        val5 = DVECCOSTOOPERACIONTOTAL[j];
        val6 = DVECCOSTOTOTAL[j];
        val7 = DVECK[j];
        val8 = UVECDTMIN[j];
        ui->tablewidget->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->tablewidget->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->tablewidget->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->tablewidget->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->tablewidget->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->tablewidget->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->tablewidget->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        ui->tablewidget->setItem(i,7, new QTableWidgetItem(QString::number(val8)));
        j++;
    }
}

void Summary::DataPoints(QString text)
{
    ui->tablewidget->setVisible(true);
    ui->tablewidget->clear();
    if(text == "Hot compositive curve"){
        CONTADORFILAS = 0 ;
        if(TabAnali == 0){
            if(estatico == true){
                datapoints_HCC_estatico_uniforme();
                CONTADORROWS.clear();
            }else if(incremento == true){
                datapoints_HCC_estatico_uniforme();
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 1){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_HCC_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_HCC_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 2){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_HCC_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_HCC_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }
    }else if(text == "Cold compositive curve"){
        CONTADORFILAS = 0 ;
        if(TabAnali == 0){
            if(estatico == true){
                datapoints_CCC_estatico_uniforme();
                CONTADORROWS.clear();
            }else if(incremento == true){
                datapoints_CCC_estatico_uniforme();
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 1){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_CCC_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_CCC_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 2){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_CCC_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                datapoints_CCC_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }
    }else if(text == "Compositive curves"){
        if(TabAnali == 0){
            if(estatico == true){
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_uniforme();
                CONTADORROWS.clear();
            }else if(incremento == true){
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_uniforme();
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 1){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 2){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestas_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }
    }else if(text == "Compositive curves adjusted"){
        if(TabAnali == 0){
            if(estatico == true){
                double DTmin = Min;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadas_estatico_uniforme(DTmin);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadas_incremento_uniforme(Maximo,Minimo,Incremento);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 1){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadas_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadas_incremento_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 2){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadas_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_CurvasCompuestasAjustadaas_incremento_both(Minimo,Maximo,Incremento,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }
    }else if(text == "Grand compositive curve"){
        if(TabAnali == 0){
            if(estatico == true){
                double DTmin = Min;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuesta_estatico_uniforme(DTmin);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuesta_incremento_uniforme(Maximo,Minimo,Incremento);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 1){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuesta_estatico_diverso(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuestas_incremento_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }else if(TabAnali == 2){
            if(estatico == true){
                double k = K , DTmin = Min;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuesta_estatico_both(DTmin,k,punto1,punto2);
                CONTADORROWS.clear();
            }else if(incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double k = K;
                float punto1 = .5, punto2 = 10;
                CONTADORFILAS = 0;
                datapoints_GranCurvaCompuesta_incremento_both(Minimo,Maximo,Incremento,k,punto1,punto2);
                CONTADORROWS.clear();
            }
        }
    }else if(text == "Areas vs DTmin"){
        if(TabAnali == 0){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            CONTADORFILAS = 0;
            datapoints_Areas_uniforme(Minimo,Maximo,Incremento);
            CONTADORROWS.clear();
        }else if(TabAnali == 1){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double k = K;
            float punto1 = .5, punto2 = 10;
            CONTADORFILAS = 0;
            datapoints_Areas_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
            CONTADORROWS.clear();
        }else if(TabAnali == 2){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double k = K;
            float punto1 = .5, punto2 = 10;
            CONTADORFILAS = 0;
            datapoints_Areas_both(Minimo,Maximo,Incremento,k,punto1,punto2);
            CONTADORROWS.clear();
        }
    }else if(text == "Cost vs DTmin"){
        if(TabAnali == 0){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            CONTADORFILAS = 0;
            datapoints_Costos_uniforme(Minimo,Maximo,Incremento);
            CONTADORROWS.clear();
        }else if(TabAnali == 1){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double k = K;
            float punto1 = .5, punto2 = 10;
            CONTADORFILAS = 0;
            datapoints_Costos_diverso(Minimo,Maximo,Incremento,k,punto1,punto2);
            CONTADORROWS.clear();
        }else if(TabAnali == 2){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double k = K;
            float punto1 = .5, punto2 = 10;
            CONTADORFILAS = 0;
            datapoints_Costos_both(Minimo,Maximo,Incremento,k,punto1,punto2);
            CONTADORROWS.clear();
        }
    }
}

QString Summary::currDate()
{
    QDate date = QDate::currentDate();
    return date.toString("dd.MM.yyyy");
}

void Summary::on_pushButton_clicked()
{
    if(ui->qcustomplot->isVisible() == true){
        QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
        QString URL = file_name + ".pdf";
        QFile file(URL);
        if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
             {
                 QMessageBox::warning(0,"Could not create Project File",QObject::tr( "\n Could not create Project File on disk"));
             }
        ui->qcustomplot->savePdf(URL,0,0,QCP::ExportPen(),"","");
    }else if(ui->tablewidget->isVisible() == true){
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
        QString style("<style>");
        style.append("body { color:black; } ");
        style.append("table { border-collapse: collapse;font-size:10px; }");
        style.append("table, th, td { border: 1px solid black; text-align: center; }");
        style.append("</style>");
        QString text("<html><body>");
        text.append("<h>").append("     " + citydate + "     ").append("</h>");
        text.append("<p>").append("     Program: SHEN (SOFTWARE HEAT EXCHANGERS NETWORKS)     ").append("</p>");
        text.append("<p>").append("     Intellectual Property: Technological Institute of Celaya     ").append("</p>");
        text.append("<p>").append("     Project directed by: Dr. Arturo Jimenez Gutierrez     ").append("</p>");
        text.append("<p>").append("     Contributors: Ing. Gabriel Hernàndez Morales     ").append("</p>");
        text.append("<p>").append("                                                      ").append("</p>");
        if(Ventanamadre == 1){
            if(item_select =="Problem table" || item_select == "Minimal hot utility"
                    || item_select == "Minimal cold utility" || item_select == "Location of the pinch point"){
                if(TabAnali == 0 || TabAnali == 1){ //uniforme y diversa
                    if(estatico == true){
                        CONTADORBOTH = 0;
                        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                        QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                        text.append("<p>").append( titulo ).append("</p>");
                        text.append("<p>").append(" ").append("</p>");
                        text.append("<table><tbody>");
                        text.append("<tr>");
                        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                        }
                        text.append("</tr>");
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
                        QStringList Unidades;
                        Unidades = unidadestablas(item_select);
                        text.append("<tr>");
                        for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                        }
                        text.append("</tr>");
                        text.append("</tbody></table>");
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = (Max-Min)/Inc;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones+1 ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(TabAnali == 2){ // both
                    if(estatico == true){
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = ( ( (Max-Min)/Inc)  + 1 )*2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }
            }else if(item_select == "Area prediction" || item_select == "DTm log" || item_select == "Areas" || item_select == "Enthalpy"){
                if(TabAnali == 0 || TabAnali == 1){ //uniforme y diversa
                    if(estatico == true){
                        CONTADORBOTH = 0;
                        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                        QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                        text.append("<p>").append( titulo ).append("</p>");
                        text.append("<p>").append(" ").append("</p>");
                        text.append("<table><tbody>");
                        text.append("<tr>");
                        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                        }
                        text.append("</tr>");
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
                        QStringList Unidades;
                        Unidades = unidadestablas(item_select);
                        text.append("<tr>");
                        for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                        }
                        text.append("</tr>");
                        text.append("</tbody></table>");
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = (Max-Min)/Inc;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones+1 ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(TabAnali == 2){ // both
                    if(estatico == true){
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = ( ( (Max-Min)/Inc)  + 1 )*2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }
            }else if(item_select == "Cost prediction" || item_select == "Agglomerates" || item_select == "Capital cost" || item_select == "Operational cost" || item_select =="Total cost"){
                if(TabAnali == 0 || TabAnali == 1){
                    if(estatico == true){
                        CONTADORBOTH = 0;
                        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                        QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                        text.append("<p>").append( titulo ).append("</p>");
                        text.append("<p>").append(" ").append("</p>");
                        text.append("<table><tbody>");
                        text.append("<tr>");
                        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                        }
                        text.append("</tr>");
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
                        QStringList Unidades;
                        Unidades = unidadestablas(item_select);
                        text.append("<tr>");
                        for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                        }
                        text.append("</tr>");
                        text.append("</tbody></table>");
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = (Max-Min)/Inc;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones+1 ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(TabAnali == 2){
                    if(estatico == true){
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(incremento == true){
                        int Iteraciones = (((Max-Min)/Inc)  + 1)*2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }
            }else if(item_select == "Data points"){
                if(item_select_datapoint == "Hot compositive curve"){
                    if(TabAnali == 0 || TabAnali == 1){ //SIEMPRE VA HABER 1 TABLA
                        CONTADORBOTH = 0;
                        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                        QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                        text.append("<p>").append( titulo ).append("</p>");
                        text.append("<p>").append(" ").append("</p>");
                        text.append("<table><tbody>");
                        text.append("<tr>");
                        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                        }
                        text.append("</tr>");
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
                        QStringList Unidades;
                        Unidades = unidadestablas(item_select);
                        text.append("<tr>");
                        for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                        }
                        text.append("</tr>");
                        text.append("</tbody></table>");
                        text.append("</body></html>");
                    }else if(TabAnali == 2){ //SIEMPRE VA HABER 2 TABLAS
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(item_select_datapoint == "Cold compositive curve"){
                    if(TabAnali == 0 || TabAnali == 1){ //SIEMPRE VA HABER 1 TABLA
                        CONTADORBOTH = 0;
                        QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                        QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                        text.append("<p>").append( titulo ).append("</p>");
                        text.append("<p>").append(" ").append("</p>");
                        text.append("<table><tbody>");
                        text.append("<tr>");
                        for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                            text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                        }
                        text.append("</tr>");
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
                        QStringList Unidades;
                        Unidades = unidadestablas(item_select);
                        text.append("<tr>");
                        for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                        }
                        text.append("</tr>");
                        text.append("</tbody></table>");
                        text.append("</body></html>");
                    }else if(TabAnali == 2){ //SIEMPRE VA HABER 2 TABLAS
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(item_select_datapoint == "Compositive curves"){
                    if(TabAnali == 0 || TabAnali == 1){ //SIEMPRE VA HABER 2 TABLA
                        int Iteraciones = 2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(TabAnali == 2){ //SIEMPRE VA HABER 4 TABLAS
                        int Iteraciones = 4;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(item_select_datapoint == "Compositive curves adjusted"){
                    if(TabAnali == 0 || TabAnali == 1){
                        if(estatico == true){ //son 2
                            int Iteraciones = 2;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones;j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }else if(incremento == true){ // arreglo
                            int Iteraciones = ( ( (Max-Min)/Inc)  + 1 )*2;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones; j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }
                    }else if(TabAnali == 2){
                        if(estatico == true){ // son 4
                            int Iteraciones = 4;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones;j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }else if(incremento == true){ // arreglo
                            int Iteraciones = ( ( (Max-Min)/Inc)  + 1 )*4;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones; j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }
                    }
                }else if(item_select_datapoint == "Grand compositive curve"){
                    if(TabAnali == 0 || TabAnali == 1){
                        if(estatico == true){ //solo es 1
                            CONTADORBOTH = 0;
                            QString  row    = ui->tablewidget->verticalHeaderItem(0)->text();
                            QString  titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append( titulo ).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
                            text.append("<table><tbody>");
                            text.append("<tr>");
                            for (int i = 0; i < ui->tablewidget->columnCount(); i++) {
                                text.append("<td>").append("        " + ui->tablewidget->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
                            }
                            text.append("</tr>");
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
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                            text.append("</body></html>");
                        }else if(incremento == true){ // arreglo
                            int Iteraciones = ( ( (Max-Min)/Inc)  + 1 );
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones; j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }
                    }else if(TabAnali == 2){
                        if(estatico == true){ // son 2
                            int Iteraciones = 2;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones;j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }else if(incremento == true){ // arreglo
                            int Iteraciones = ( ( (Max-Min)/Inc)  + 1 )*2;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones; j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }
                    }
                }else if(item_select_datapoint == "Areas vs DTmin"){
                    if(TabAnali == 0 || TabAnali == 1 ){
                        int Iteraciones = 1;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(TabAnali == 2){
                        int Iteraciones = 1 ; //( ( (Max-Min)/Inc)  + 1 )*2;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }
                }else if(item_select_datapoint == "Cost vs DTmin"){
                    if(TabAnali == 0 || TabAnali == 1 ){
                        int Iteraciones = 1;
                        CONTADORFILAS = 0;
                        CONTADORBOTH = 0;
                        QString row1,row2,row,titulo;
                        for(int j = 0 ; j <  Iteraciones ;j++){
                            row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                            titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                            text.append("<p>").append(titulo).append("</p>");
                            text.append("<p>").append(" ").append("</p>");
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
                                        CONTADORBOTH = CONTADORBOTH + 1;
                                        break;
                                    }
                                }
                            }
                            QStringList Unidades;
                            Unidades = unidadestablas(item_select);
                            text.append("<tr>");
                            for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                            }
                            text.append("</tr>");
                            text.append("</tbody></table>");
                        }
                        text.append("</body></html>");
                    }else if(TabAnali == 2){
                        if(estatico == true){
                            int Iteraciones = 1 ; //( ( (Max-Min)/Inc)  + 1 )*2;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones;j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }else if(incremento == true){
                            int Iteraciones = 1;
                            CONTADORFILAS = 0;
                            CONTADORBOTH = 0;
                            QString row1,row2,row,titulo;
                            for(int j = 0 ; j <  Iteraciones;j++){
                                row    = ui->tablewidget->verticalHeaderItem(CONTADORFILAS)->text();
                                titulo = espaceadorcalculos(item_select,row,CONTADORBOTH);
                                text.append("<p>").append(titulo).append("</p>");
                                text.append("<p>").append(" ").append("</p>");
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
                                            CONTADORBOTH = CONTADORBOTH + 1;
                                            break;
                                        }
                                    }
                                }
                                QStringList Unidades;
                                Unidades = unidadestablas(item_select);
                                text.append("<tr>");
                                for( int i = 0; i < ui->tablewidget->columnCount(); i++){
                                    text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
                                }
                                text.append("</tr>");
                                text.append("</tbody></table>");
                            }
                            text.append("</body></html>");
                        }
                    }
                }
            }
        }
        doc.setDefaultStyleSheet(style);
        doc.setHtml(text);
        doc.setPageSize(printer.pageRect().size());
        doc.print(&printer);
    }
}

QString Summary::espaceadorcalculos(QString text,QString row,int j )
{
    QString espace;
    if(Ventanamadre == 1){ // VENTANA ANALISIS PERSONALIZADO
        if(text =="Problem table" || text == "Minimal hot utility"
                || text == "Minimal cold utility" || text == "Location of the pinch point"){
            if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    QString inicio = "Table problem with ";
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
        }else if(text == "Area prediction" || text == "DTm log" || text == "Areas" || text == "Enthalpy"){
            if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    QString inicio = "Area prediction with ";
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
        }else if(text == "Cost prediction"|| text == "Agglomerates" || text == "Capital cost" || text == "Operational cost" || text =="Total cost"){
            if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    QString inicio = "Cost prediction with ";
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
        }else if(text == "Data points"){
            if(item_select_datapoint == "Hot compositive curve"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Cold compositive curve"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Compositive curves"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Compositive curves adjusted"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Grand compositive curve"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Areas vs DTmin"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }else if(item_select_datapoint == "Cost vs DTmin"){
                if(TabAnali == 0){
                    espace = row;
                }else if(TabAnali == 1){
                    espace = row;
                }else if(TabAnali == 2){
                    if(j == 0){
                        espace = row;
                    }else if(j == 1){
                        espace = row;
                        CONTADORBOTH = 0;
                    }
                }
            }
        }
    }
    return espace;
}

QStringList Summary::unidadestablas(QString item_select)
{
    QStringList lista;
    if(Ventanamadre == 1){ // VENTANA ANALISIS PERSONALIZADO
        if(item_select =="Problem table" || item_select == "Minimal hot utility"
                || item_select == "Minimal cold utility" || item_select == "Location of the pinch point"){
            if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    if(SI == true ){
                        lista << "ªF" << "ªF" << "BTU/HR" << "BTU/HR" << "BTU/HR" << "BTU/HR" << "BTU/HR";
                    }else if(SIS == true){
                        lista << "ªC" << "ªC" << "Watts" << "Watts" << "Watts" << "Watts" << "Watts";
                    }
                }
            }
        }else if(item_select == "Area prediction" || item_select == "DTm log" || item_select == "Areas" || item_select == "Enthalpy"){
            if(TabAnali == 0 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    if(SI == true ){
                        lista << "ªF" << "ªF" << "ªF" << "ªF" << "ªF" << "ªF" << "ªF" << "BTU/HR" << "ft^2" ;
                    }else if(SIS == true){
                        lista << "ªC" << "ªC" << "ªC" << "ªC" << "ªC" << "ªC" << "ªC" << "Watts" << "mt^2" ;
                    }
                }
            }else if(TabAnali == 1){
                if(estatico == true  || incremento == true ){
                    if(SI == true ){
                        lista << "ªF" << "ªF" << "ªF" << "ªF" << "ªF" << "BTU/HR" << "ft^2" ;
                    }else if(SIS == true){
                        lista << "ªC" << "ªC" << "ªC" << "ªC" << "ªC" << "Watts" << "mt^2" ;
                    }
                }
            }
        }else if(item_select == "Cost prediction" || item_select == "Agglomerates" || item_select == "Capital cost" || item_select == "Operational cost" || item_select =="Total cost"){
            if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){ // todas las metodologias
                if(estatico == true  || incremento == true ){
                    if(SI == true ){
                        lista << "ft^2" << "BTU/HR" << "US $ / YEAR" << "US $ / YEAR" << "US $ / YEAR" ;
                    }else if(SIS == true){
                        lista << "mt^2" << "Watts"  << "US $ / YEAR" << "US $ / YEAR" << "US $ / YEAR";
                    }
                }
            }
        }else if(item_select == "Data points"){
            if(item_select_datapoint == "Hot compositive curve"){
                if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){
                    if(SI == true ){
                        lista << "BTU/HR" << "ªF";
                    }else if(SIS == true){
                        lista << "Watts"  << "ªC";
                    }
                }
            }else if(item_select_datapoint == "Cold compositive curve"){
                if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){
                    if(SI == true ){
                        lista << "BTU/HR" << "ªF";
                    }else if(SIS == true){
                        lista << "Watts"  << "ªC";
                    }
                }
            }else if(item_select_datapoint == "Compositive curves"){
                if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){
                    if(SI == true ){
                        lista << "BTU/HR" << "ªF";
                    }else if(SIS == true){
                        lista << "Watts"  << "ªC";
                    }
                }
            }else if(item_select_datapoint == "Compositive curves adjusted"){
                if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){
                    if(SI == true ){
                        lista << "BTU/HR" << "ªF";
                    }else if(SIS == true){
                        lista << "Watts"  << "ªC";
                    }
                }
            }else if(item_select_datapoint == "Grand compositive curve"){
                if(TabAnali == 0 || TabAnali == 1 || TabAnali == 2){
                    if(SI == true ){
                        lista << "BTU/HR" << "ªF";
                    }else if(SIS == true){
                        lista << "Watts"  << "ªC";
                    }
                }
            }else if(item_select_datapoint == "Areas vs DTmin"){
                if(TabAnali == 0  ){
                    if(SI == true ){
                        lista <<  "ft^2" << "ªF";
                    }else if(SIS == true){
                        lista  << "mt^2" <<  "ªC";
                    }
                }else if(TabAnali == 1){
                    if(SI == true ){
                        lista <<  "ft^2" << " " <<  "ªF";
                    }else if(SIS == true){
                        lista  << "mt^2" << " " <<  "ªC";
                    }
                }else if(TabAnali == 2){
                    if(SI == true ){
                        lista <<  "ft^2" <<  "ft^2" << " " << "ªF";
                    }else if(SIS == true){
                        lista  << "mt^2" << "mt^2" << " " <<  "ªC";
                    }
                }
            }else if(item_select_datapoint == "Cost vs DTmin"){
                if(TabAnali == 0){
                    if(SI == true ){
                        lista <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << "ªF";
                    }else if(SIS == true){
                        lista <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << "ªC";
                    }
                }else if(TabAnali == 1){
                    if(SI == true ){
                        lista <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << " " << "ªF";
                    }else if(SIS == true){
                        lista <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << " " << "ªC";
                    }
                }else if(TabAnali == 2){
                    if(SI == true ){
                        lista  <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR"
                                <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << " " << "ªF";
                    }else if(SIS == true){
                        lista  <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR"
                                <<  "US $ / YEAR" <<  "US $ / YEAR" <<  "US $ / YEAR" << " " <<  "ªC";
                    }
                }
            }
        }
    }
    return lista;
}

bool Summary::compararrows(QString row1, QString row2)
{
    if(row1 == row2){ // false soy iguales
        return false;
    }else{ // true son diferentes
        return true;
    }
}

void Summary::on_pushButton_4_clicked()
{
    if(ui->qcustomplot->isVisible() == true){
        if(ui->comboBox->currentIndex()== 0){

        }else if(ui->comboBox->currentIndex() == 1){
            QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
            if(!file_name.contains(".jpg")){
                file_name = file_name + ".jpg";
            }
            QFile file(file_name);
            if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
                 {
                     QMessageBox::warning(0,"Could not create Project File",QObject::tr( "\n Could not create Project File on disk"));
                 }
           ui->qcustomplot->saveJpg(file_name,  0, 0, 1.0, -1  );
        }else if(ui->comboBox->currentIndex() == 2){

        }
    }
}
