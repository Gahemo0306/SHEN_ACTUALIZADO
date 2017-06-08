#include "duvaloraction.h"
#include "valoraction.h"
#include "graficos.h"
#include "qcustomplot.h"
#include "contenido_plots.h"
#include "ui_contenido_plots.h"

Contenido_PLOTS::Contenido_PLOTS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Contenido_PLOTS)
{
    ui->setupUi(this);
    ui->groupBox_3->setVisible(false);
    ui->Utext1->setVisible(false);
    ui->Utext2->setVisible(false);
    ui->Utext3->setVisible(false);
    ui->Utext4->setVisible(false);
    ui->Minimun->setVisible(false);
    ui->Maximun->setVisible(false);
    ui->Increment->setVisible(false);
    ui->k->setVisible(false);
    ui->help->setVisible(false);
}

Contenido_PLOTS::~Contenido_PLOTS()
{
    delete ui;
}

void Contenido_PLOTS::on_Uniforme_clicked()
{
    RADIOBUTTONS();
}

void Contenido_PLOTS::on_Diverso_clicked()
{
    RADIOBUTTONS();
}

void Contenido_PLOTS::on_StaticradioButton_clicked()
{
    RADIOBUTTONS();
}

void Contenido_PLOTS::on_IncrementradioButton_clicked()
{
    RADIOBUTTONS();
}

void Contenido_PLOTS::RADIOBUTTONS()
{
    ui->groupBox_3->setVisible(false);
    ui->Utext1->setVisible(false);
    ui->Utext2->setVisible(false);
    ui->Utext3->setVisible(false);
    ui->Utext4->setVisible(false);
    ui->Minimun->setVisible(false);
    ui->Maximun->setVisible(false);
    ui->Increment->setVisible(false);
    ui->k->setVisible(false);
    ui->help->setVisible(false);
    bool uniforme = ui->Uniforme->isChecked();
    bool diverso = ui->Diverso->isChecked();
    bool estatico = ui->StaticradioButton->isChecked();
    bool incremento = ui->IncrementradioButton->isChecked();
    if(uniforme == true && estatico == true){
        ui->groupBox_3->setVisible(true);
        ui->Utext4->setVisible(false); //k
        ui->Utext1->setVisible(true);  //minimo
        ui->Utext2->setVisible(false); //maximo
        ui->Utext3->setVisible(false); //incremento
        ui->Utext1->setText("Value:");
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(false);
        ui->Increment->setVisible(false);
        ui->k->setVisible(false);
        ui->help->setVisible(false);
        accionguardar();
    }else if(uniforme == true && incremento == true){
        ui->groupBox_3->setVisible(true);
        ui->Utext1->setVisible(true);  //minmo
        ui->Utext2->setVisible(true); //maximo
        ui->Utext3->setVisible(true); //incremento
        ui->Utext4->setVisible(false); //k
        ui->Utext1->setText("Minimun value:");
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(true);
        ui->Increment->setVisible(true);
        ui->help->setVisible(false);
        ui->k->setVisible(false);
        accionguardar();
    }else if(diverso == true && estatico == true){
        ui->groupBox_3->setVisible(true);
        ui->Utext1->setVisible(true);  //minimo
        ui->Utext2->setVisible(false); //maximo
        ui->Utext3->setVisible(false); //incremento
        ui->Utext4->setVisible(true); //k
        ui->Utext1->setText("Value:");
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(false);
        ui->Increment->setVisible(false);
        ui->k->setVisible(true);
        ui->help->setVisible(true);
        accionguardar();
    }else if(diverso == true && incremento == true){
        ui->groupBox_3->setVisible(true);
        ui->Utext1->setVisible(true);//minimo
        ui->Utext2->setVisible(true);//maximo
        ui->Utext3->setVisible(true);//incremento
        ui->Utext4->setVisible(true);//k
        ui->Utext1->setText("Minimun value:");
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(true);
        ui->Increment->setVisible(true);
        ui->k->setVisible(true);
        ui->help->setVisible(true);
        accionguardar();
    }else if(uniforme == false || diverso == false){
        ui->Utext1->setVisible(false);
        ui->Utext2->setVisible(false);
        ui->Utext3->setVisible(false);
        ui->Utext4->setVisible(false);
        ui->Minimun->setVisible(false);
        ui->Maximun->setVisible(false);
        ui->Increment->setVisible(false);
        ui->k->setVisible(false);
        ui->help->setVisible(false);
    }
}


void Contenido_PLOTS::on_k_valueChanged()
{
    accionguardar();
}

void Contenido_PLOTS::on_Minimun_valueChanged()
{
    accionguardar();
}

void Contenido_PLOTS::on_Maximun_valueChanged()
{
    accionguardar();
}

void Contenido_PLOTS::on_Increment_valueChanged()
{
    accionguardar();
}

void Contenido_PLOTS::accionguardar()
{
    bool uniforme = ui->Uniforme->isChecked();
    bool diverso = ui->Diverso->isChecked();
    bool estatico = ui->StaticradioButton->isChecked();
    bool incremento = ui->IncrementradioButton->isChecked();
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
        QMessageBox::warning(this,tr("Error"),tr("Error"));
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
    QFile Fil(TABPLOT_FILENAME);
    if (!Fil.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in4(&Fil);
    in4.setVersion(QDataStream::Qt_5_4);
    Tabplot tabvalue;
    in4 >> tabvalue;
    int ventanaplot = tabvalue.gettabvalue();
    Fil.flush();
    Fil.close();
    if(ventanaplot == 1) // curvas compuestas ajustadas)
    {
        if(uniforme == true && estatico == true){
            QFile FileBools(VECPLOTCURVASCOMPUESTASAJUSTADAS_BOOL_FILENAME);
                if (!FileBools.open(QIODevice::WriteOnly)){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTCURVASCOMPUESTASAJUSTADASESTATICO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out7(&FileVec);
            out7.setVersion(QDataStream::Qt_5_4);
            DTmin = ui->Minimun->value();
            DTmin = units.ConvertirDTmin(DTmin,SI,SIS,UTemp);
            VecPlot_CurvasCompuestasAjustadasEstatico VecCCAE(uniforme,diverso,estatico,incremento,TS,TE,Wcp,DTmin);
            out7 << VecCCAE;
            FileVec.flush();
            FileVec.close();
        }else if(uniforme == true && incremento == true){
            QFile FileBools(VECPLOTCURVASCOMPUESTASAJUSTADAS_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTCURVASCOMPUESTASAJUSTADASDINAMICO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out8(&FileVec);
            out8.setVersion(QDataStream::Qt_5_4);
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
            Max = units.ConvertirMax(Max,SI,SIS,UTemp);
            Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
            VecPlot_CurvasCompuestasAjustadasIncremento Vec(uniforme,diverso,estatico,incremento,TS,TE,Wcp,Min,Max,Inc);
            out8 << Vec;
            FileVec.flush();
            FileVec.close();
        }else if(diverso == true && estatico == true){
            QFile FileBools(VECPLOTCURVASCOMPUESTASAJUSTADAS_BOOL_FILENAME);
                if (!FileBools.open(QIODevice::WriteOnly)){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTCCAESTATICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out13(&FileVec);
            out13.setVersion(QDataStream::Qt_5_4);
            DTmin = ui->Minimun->value();
            DTmin = units.ConvertirDTmin(DTmin,SI,SIS,UTemp);
            K = ui->k->value();
            VecPlot_CCAjustadasEst_Diversa VecCCAED(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,DTmin,K);
            out13 << VecCCAED;
            FileVec.flush();
            FileVec.close();
        }else if(diverso == true && incremento == true){
            QFile FileBools(VECPLOTCURVASCOMPUESTASAJUSTADAS_BOOL_FILENAME);
                if (!FileBools.open(QIODevice::WriteOnly)){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTCCADINAMICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out14(&FileVec);
            out14.setVersion(QDataStream::Qt_5_4);
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
            Max = units.ConvertirMax(Max,SI,SIS,UTemp);
            Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
            K = ui->k->value();
            VecPlot_CCAjustadasInc_Diversa VecCCAID(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,Min,Max,Inc,K);
            out14 << VecCCAID;
            FileVec.flush();
            FileVec.close();
        }
    }else if(ventanaplot == 2){ //Gran curva compuesta
        if(uniforme == true && estatico == true){
            QFile FileBools(VECPLOTGCC_BOOL_FILENAME);
                if (!FileBools.open(QIODevice::WriteOnly)){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out10(&FileBools);
            out10.setVersion(QDataStream::Qt_5_4);
            VecGCCbool VecGCCbools(uniforme,diverso,estatico,incremento);
            out10 << VecGCCbools;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTGCCESTATICO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out11(&FileVec);
            out11.setVersion(QDataStream::Qt_5_4);
            DTmin = ui->Minimun->value();
            DTmin = units.ConvertirDTmin(DTmin,SI,SIS,UTemp);
            VecGCCestatico VecGCest(uniforme,diverso,estatico,incremento,TS,TE,Wcp,DTmin);
            out11 << VecGCest;
            FileVec.flush();
            FileVec.close();
        }else if(uniforme == true && incremento == true){
            QFile FileBools(VECPLOTGCC_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTGCCDINAMICO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out12(&FileVec);
            out12.setVersion(QDataStream::Qt_5_4);
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
            Max = units.ConvertirMax(Max,SI,SIS,UTemp);
            Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
            VecGCCdinamico VecGCdin(uniforme,diverso,estatico,incremento,TS,TE,Wcp,Min,Max,Inc);
            out12 << VecGCdin;
            FileVec.flush();
            FileVec.close();
        }else if(diverso == true && estatico == true){
            QFile FileBools(VECPLOTGCC_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTGCCESTATICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out15(&FileVec);
            out15.setVersion(QDataStream::Qt_5_4);
            DTmin = ui->Minimun->value();
            DTmin = units.ConvertirDTmin(DTmin,SI,SIS,UTemp);
            K = ui->k->value();
            VecGCCED GCCED(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,DTmin,K);
            out15 << GCCED;
            FileVec.flush();
            FileVec.close();
        }else if(diverso == true && incremento == true){
            QFile FileBools(VECPLOTGCC_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out9(&FileBools);
            out9.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasbool VecCCAB(uniforme,diverso,estatico,incremento);
            out9 << VecCCAB;
            FileBools.flush();
            FileBools.close();
            QFile FileVec(VECPLOTGCCDINAMICA_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out16(&FileVec);
            out16.setVersion(QDataStream::Qt_5_4);
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Min = units.ConvertirDTmin(Min,SI,SIS,UTemp);
            Max = units.ConvertirMax(Max,SI,SIS,UTemp);
            Inc = units.ConvertirInc(Inc,SI,SIS,UTemp);
            K = ui->k->value();
            VecGCCDD GCCDD(uniforme,diverso,estatico,incremento,TS,TE,Wcp,h,Min,Max,Inc,K);
            out16 << GCCDD;
            FileVec.flush();
            FileVec.close();
        }
    }
}


void Contenido_PLOTS::on_pushButton_clicked()
{

}
