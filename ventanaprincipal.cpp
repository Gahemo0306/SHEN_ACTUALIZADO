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

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    setWindowTitle("Software Heat Exchanger Network ");
    loadSubWindowPrincipal(new UnidadesEntrada(this));
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
    }
    else if (ValorACTION == 2){ //OPEN
        Titulo = "WorkSpace";
        RutaIcono = ":/resources/Resources/workspace.png";
    }
    else if (ValorACTION ==3){ //CUSTOM ANALISYS
        Titulo = "Custom analisys";
        RutaIcono = "";
    }
    else if (ValorACTION == 4){ //GRAFICOS
        Titulo = "Plots";
        RutaIcono = ":/resources/Resources/plots.png";
    }
    else if (ValorACTION == 5){ //SUMMARY
        Titulo = "Report Options";
        RutaIcono = "";
    }else if (ValorACTION == 6){
        Titulo = "Problem Table";
        RutaIcono = ":/resources/Resources/table-icon-png-2.png";
    }else if( ValorACTION == 7){
        Titulo = "Help";
        RutaIcono = ":/resources/Resources/ayuda.png";
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
    RutaIcono = "";
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
    loadSubWindow(new AnalisisPersonalizado(this),ValorACTION);
}

void VentanaPrincipal::on_actionPlots_triggered()
{
    int ValorACTION = 4;
    loadSubWindow(new plots(this),ValorACTION);
}

void VentanaPrincipal::on_actionHelp_triggered()
{
    int ValorACTION = 7;
    loadSubWindow(new Help(this),ValorACTION);
}

void VentanaPrincipal::on_actionSummary_triggered()
{
    int ValorACTION = 5;
    loadSubWindow(new Summary(this),ValorACTION);
}

void VentanaPrincipal::on_actionProblem_Table_triggered()
{
    int ValorACTION = 6;;
    loadSubWindow(new problemtable(this),ValorACTION);
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
