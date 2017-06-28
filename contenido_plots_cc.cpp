#include "contenido_plots_cc.h"
#include "ui_contenido_plots_cc.h"
#include "duvaloraction.h"
#include "graficos.h"
#include "valoraction.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

contenido_plots_cc::contenido_plots_cc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contenido_plots_cc)
{
    ui->setupUi(this);
    ui->groupBox_2->setVisible(false);
    ui->label1->setVisible(false);
    ui->label2->setVisible(false);
    ui->value->setVisible(false);
    ui->k->setVisible(false);
    ui->help->setVisible(false);
}

contenido_plots_cc::~contenido_plots_cc()
{
    delete ui;
}

void contenido_plots_cc::on_radioButton_clicked()
{
    RADIOBUTTONS();
}


void contenido_plots_cc::on_radioButton_2_clicked()
{
    RADIOBUTTONS();
}

void contenido_plots_cc::RADIOBUTTONS()
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
    TS.resize(Matriz.size());
    TE.resize(Matriz.size());
    Wcp.resize(Matriz.size());
    h.resize(Matriz.size());
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
    bool uniforme = ui->radioButton->isChecked();
    bool diverso = ui->radioButton_2->isChecked();
    if(uniforme == true ){
        ui->groupBox_2->setVisible(false);
        ui->label1->setVisible(false);
        ui->label2->setVisible(false);
        ui->value->setVisible(false);
        ui->k->setVisible(false);
        ui->help->setVisible(false);
        QFile FileVec(VECPLOTCURVASCOMPUESTAS_FILENAME);
        if (!FileVec.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out6(&FileVec);
        out6.setVersion(QDataStream::Qt_5_4);
        double Min = 0;
        double Max = 0;
        double Inc = 0;
        double K = 0;
        bool estatico = false;
        bool incremento = false;
        VecPlot_CurvasCompuestas VecCC(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,Min,Max,Inc,K);
        out6 << VecCC;
        FileVec.flush();
        FileVec.close();
    }else if(diverso == true){
        ui->groupBox_2->setVisible(true);
        ui->label1->setVisible(true);
        ui->label2->setVisible(true);
        ui->value->setVisible(true);
        ui->k->setVisible(true);
        ui->help->setVisible(true);
    }
}

void contenido_plots_cc::on_value_valueChanged()
{
    Guardar();
}

void contenido_plots_cc::on_k_valueChanged()
{
    Guardar();
}

void contenido_plots_cc::Guardar()
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
    int UTemp = units.getUTemp();
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    bool uniforme = ui->radioButton->isChecked();
    bool diverso = ui->radioButton_2->isChecked();
    if(uniforme == true ){
        QFile FileVec(VECPLOTCURVASCOMPUESTAS_FILENAME);
        if (!FileVec.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out6(&FileVec);
        out6.setVersion(QDataStream::Qt_5_4);
        double Min = 0;
        double Max = 0;
        double Inc = 0;
        double K = 0;
        bool estatico = false;
        bool incremento = false;
        VecPlot_CurvasCompuestas VecCC(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,Min,Max,Inc,K);
        out6 << VecCC;
        FileVec.flush();
        FileVec.close();
    }else if(diverso == true){
        QFile FileVec(VECPLOTCURVASCOMPUESTAS_FILENAME);
        if (!FileVec.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream out6(&FileVec);
        out6.setVersion(QDataStream::Qt_5_4);
        double Min = ui->value->value();
        double Max = 0;
        double Inc = 0;
        Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
        double K = ui->k->value();
        bool estatico = false;
        bool incremento = false;
        VecPlot_CurvasCompuestas VecCC(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,Min,Max,Inc,K);
        out6 << VecCC;
        FileVec.flush();
        FileVec.close();
    }
    FileUnidades.flush();
    FileUnidades.close();
}

void contenido_plots_cc::on_help_clicked()
{
    int Help = 1; //AYUDA SOBRE PARAMETRO K
    QFile F(AYUDA_FILENAME);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out29(&F);
    out29.setVersion(QDataStream::Qt_5_4);
    savehelp help(Help);
    out29 << help;
    F.flush();
    F.close();
    emit helpsender(true);
}

void contenido_plots_cc::helprecibidor(bool checked)
{
    if(checked == true){
        on_help_clicked();
    }else{
        return;
    }
}
