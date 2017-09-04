#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardItem>
#include <QtCore>
#include <QtGui>
#include <QMdiSubWindow>
#include <QFile>
#include <QDataStream>
#include "duvaloraction.h"
#include "valoraction.h"
#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"
#include "unidadesentrada.h"
#include "tabladatos.h"
#include "analisispersonalizado.h"
#include "plots.h"
#include "summary.h"
#include "problemtable.h"
#include "help.h"
#include "areatargeting.h"
#include "costtargeting.h"
#include "contenido_plots.h"
#include "contenido_plots_area.h"
#include "contenido_plots_cc.h"
#include "networkdesign.h"
#include "contenido_plots_costos.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    setWindowTitle("Software Heat Exchanger Network ");
    loadSubWindowPrincipal(new UnidadesEntrada(this));
    QFile FileMadre(TAB_A_P);
        if (!FileMadre.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out26(&FileMadre);
    out26.setVersion(QDataStream::Qt_5_4);
    int mTabAnali = 0;
    int mVentanamadre = 0;
    TabAnalisis TA(mTabAnali,mVentanamadre);
    out26 << TA;
    FileMadre.flush();
    FileMadre.close();
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::loadSubWindow(QWidget *widget,int ValorACTION)
{
    QString Titulo,RutaIcono;
    if(ValorACTION == 1){ //NEW
        Titulo = "WorkSpace";
        RutaIcono = ":/resources/Resources/workspace.png";
    }else if (ValorACTION == 2){ //OPEN
        Titulo = "WorkSpace";
        RutaIcono = ":/resources/Resources/workspace.png";
    }else if (ValorACTION ==3){ //CUSTOM ANALISYS
        Titulo = "Custom analisys";
        RutaIcono = "";
    }else if (ValorACTION == 4){ //GRAFICOS
        Titulo = "Plots";
        RutaIcono = ":/resources/Resources/plots.png";
    }else if (ValorACTION == 5){ //SUMMARY
        Titulo = "Report Options";
        RutaIcono = "";
    }else if (ValorACTION == 6){
        Titulo = "Problem Table";
        RutaIcono = ":/resources/Resources/table-icon-png-2.png";
    }else if( ValorACTION == 7){
        Titulo = "Help";
        RutaIcono = ":/resources/Resources/ayuda.png";
    }else if(ValorACTION == 8){
        Titulo = "Area targeting";
        RutaIcono = ":/resources/Resources/intercambiador.png";
    }else if(ValorACTION == 9){
        Titulo = "Super Targeting";
        RutaIcono = ":/resources/Resources/arrow.png";
    }else if(ValorACTION == 10){
        Titulo = "Network Design";
        RutaIcono = "";
    }
    auto window = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle(Titulo);
    window->setWindowIcon(QIcon(RutaIcono));
    window->show();
}

void VentanaPrincipal::loadSubWindowPrincipal(QWidget *widget)
{
    int x, y , w ,h;
    x= geometry().x()/2;
    y= geometry().y()/2;
    w= widget->sizeHint().width() +30;
    h= widget->sizeHint().height() +30;
    QString Titulo,RutaIcono;
    Titulo = "Welcome";
    RutaIcono = ":/resources/Resources/Bienvenida.png";
    auto window = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle(Titulo);
    window->setWindowIcon(QIcon(RutaIcono));
    window->setGeometry(x,y,w,h);
    window->show();
}

void VentanaPrincipal::on_actionNew_triggered() //NEW
{   //se tiene que abrir de TablaDatos
    int ValorACTION = 1;
    QFile F(VALORACTION_FILENAME);
    if (!F.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,"Error",F.errorString());
        return;
    }
    QDataStream out1(&F);
    out1.setVersion(QDataStream::Qt_5_4);
    Duvaloraction valoraction(ValorACTION);
    out1 << valoraction;
    F.flush();
    F.close();
    TablaDatos* _tabladatos = new TablaDatos(this);
    connect(this, &VentanaPrincipal::actionSave,_tabladatos, &TablaDatos::SaveToogle);
    connect(this, &VentanaPrincipal::actionSaveas,_tabladatos, &TablaDatos::SaveAsToogle);
    loadSubWindow(_tabladatos,ValorACTION);
}

void VentanaPrincipal::on_actionOpen_triggered()
{
    int ValorACTION = 2;
    QFile F(VALORACTION_FILENAME);
    if (!F.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this,"error",F.errorString());
        return;
    }
    QDataStream out1(&F);
    out1.setVersion(QDataStream::Qt_5_4);
    Duvaloraction valoraction(ValorACTION);
    out1 << valoraction;
    F.flush();
    F.close();
    TablaDatos* _tabladatos = new TablaDatos(this);
    connect(this, &VentanaPrincipal::actionSave,_tabladatos, &TablaDatos::SaveToogle);
    connect(this, &VentanaPrincipal::actionSaveas,_tabladatos, &TablaDatos::SaveAsToogle);
    loadSubWindow(_tabladatos,ValorACTION);
}

void VentanaPrincipal::on_actionClose_triggered()
{
    close();
}

void VentanaPrincipal::on_actionCustom_Analysis_triggered()
{
    int ValorACTION = 3;
    AnalisisPersonalizado* _AP = new AnalisisPersonalizado(this);
    emit actionHelp(true);
    connect(_AP,&AnalisisPersonalizado::helpsender,this,&VentanaPrincipal::on_actionHelp_triggered);
    connect(this, &VentanaPrincipal::actionHelp,_AP,&AnalisisPersonalizado::helprecibidor);
    loadSubWindow(_AP,ValorACTION);
}

void VentanaPrincipal::on_actionProblem_Table_triggered()
{
    int ValorACTION = 6;
    problemtable* _problemtable = new problemtable(this);
    emit actionHelp(true);
    connect(_problemtable,&problemtable::helpsender,this,&VentanaPrincipal::on_actionHelp_triggered);
    connect(this, &VentanaPrincipal::actionHelp,_problemtable,&problemtable::helprecibidor);
    loadSubWindow(_problemtable,ValorACTION);
}

void VentanaPrincipal::on_actionPlots_triggered()
{
    int ValorACTION = 4;
    contenido_plots_cc* _contenido_plots_cc = new contenido_plots_cc(this);
//    Contenido_PLOTS* _contenido_plots = new Contenido_PLOTS(this);
//    contenido_plots_area* _contenido_plots_area = new contenido_plots_area(this);
//    contenido_plots_costos* contendio_plots_costos = new contenido_plots_costos(this);
    connect(_contenido_plots_cc,&contenido_plots_cc::helpsender,this,&VentanaPrincipal::on_actionHelp_triggered);
    connect(this, &VentanaPrincipal::actionHelp,_contenido_plots_cc,&contenido_plots_cc::helprecibidor);
    loadSubWindow(new plots(this),ValorACTION);
}

void VentanaPrincipal::on_actionHelp_triggered()
{
    int ValorACTION = 7;
    Help* _help = new Help(this);
    //connect(this, &VentanaPrincipal::actionHelp,_help,&);
    loadSubWindow(_help,ValorACTION);
}

void VentanaPrincipal::on_actionArea_Targets_triggered()
{
    int ValorACTION = 8;
    areatargeting* _areatargeting = new areatargeting(this);
//    connect(this, &VentanaPrincipal::);
//    connect(this, &VentanaPrincipal::actionSave,_tabladatos, &TablaDatos::SaveToogle);
//    connect(this, &VentanaPrincipal::actionSaveas,_tabladatos, &TablaDatos::SaveAsToogle);
    loadSubWindow(_areatargeting,ValorACTION);
}

void VentanaPrincipal::on_actionSuper_Targeting_triggered()
{
    int ValorACTION = 9 ;
    Costtargeting* _costtargeting = new Costtargeting(this);
    loadSubWindow(_costtargeting,ValorACTION);
}

void VentanaPrincipal::on_actionNetwork_Design_triggered()
{
    int ValorACTION = 10 ;
    NetworkDesign* _networkdesign = new NetworkDesign(this);
    loadSubWindow(_networkdesign,ValorACTION);
}

void VentanaPrincipal::on_actionSummary_triggered()
{
    int ValorACTION = 5;
    Summary* _summary = new Summary(this);
    loadSubWindow(_summary,ValorACTION);
}

void VentanaPrincipal::on_actionSave_triggered()
{
    bool checked = true;
    emit actionSave(checked);
}

void VentanaPrincipal::on_actionSave_as_triggered()
{
    bool checked = true;
    emit actionSaveas(checked);
}

//void VentanaPrincipal::on_actionHelp_Topics_triggered()
//{

//}

//void VentanaPrincipal::on_actionAbour_SHEN_triggered()
//{

//}

//void VentanaPrincipal::on_actionContact_triggered()
//{

//}

