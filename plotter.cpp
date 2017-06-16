#include "plotter.h"
#include "ui_plotter.h"
#include "duvaloraction.h"
#include "valoraction.h"
#include "graficos.h"
#include "QDebug"

plotter::plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotter)
{
    ui->setupUi(this);
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
    if(ventanaplot == 0){
        QFile FileVec(VECPLOTCURVASCOMPUESTAS_FILENAME);
        if (!FileVec.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in6(&FileVec);
        bool Divers = false;
        bool Uniform = false;
        bool Estatic = false;
        bool Increment = false;
        double Mi = 0;
        double Ma = 0;
        double In = 0;
        double K = 0;
        QVector<double> Ts,Te,WCp,h;
        Ts.resize(10);
        Te.resize(10);
        WCp.resize(10);
        h.resize(10);
        in6.setVersion(QDataStream::Qt_5_4);
        VecPlot_CurvasCompuestas Vec(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,h,Mi,Ma,In,K);
        in6 >> Vec;
        bool diverso2 = Vec.getDiverso();
        bool uniforme2 = Vec.getUniforme();
        bool estatico2 = Vec.getEstatico();
        bool incremento2 = Vec.getIncremmento();
        double Min2 = Vec.getMin();
        double Max2 = Vec.getMax();
        double Inc2 = Vec.getInc();
        double K2 = Vec.getK();
        QVector<double> TS2 = Vec.getTS();
        QVector<double> TE2 = Vec.getTE();
        QVector<double> Wcp2 = Vec.getWCP();
        QVector<double> H2 = Vec.geth();
        QVector<double> Calentamiento2,Enfriamento2;
        int CTo2 = 0, CCo2 = 0;
        Calentamiento2.resize(10);
        Enfriamento2.resize(10);
        QVector<double> OperationCost2;
        OperationCost2.resize(10);
        QVector<QVector<double>> CapitalCost2;
        CapitalCost2.resize(10);
        for(int i = 0; i < CapitalCost2.size(); i++){
            CapitalCost2[i].resize(4);
        }
        plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
             CapitalCost2,OperationCost2);
        FileVec.flush();
        FileVec.close();
    }else if(ventanaplot==1){ // CURVAS COMPUESTAS AJSUTADAS OK
        QFile FileBools(VECPLOTCURVASCOMPUESTASAJUSTADAS_BOOL_FILENAME);
        if (!FileBools.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in9(&FileBools);
        in9.setVersion(QDataStream::Qt_5_4);
        bool uniforme = false;
        bool diverso = false;
        bool estatico = false;
        bool incremento = false;
        VecPlot_CurvasCompuestasAjustadasbool VecC(uniforme,diverso,estatico,incremento);
        in9 >> VecC;
        bool uni = VecC.getUniforme();
        bool div = VecC.getDiverso();
        bool est = VecC.getEstatico();
        bool inc = VecC.getIncremmento();
        if(uni == true && est == true){
            QFile FileVec(VECPLOTCURVASCOMPUESTASAJUSTADASESTATICO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in7(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Dtmin = 0;
            QVector<double> Ts,Te,WCp,H2;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            H2.resize(10);
            in7.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasEstatico VecCC(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,Dtmin);
            in7 >> VecCC;
            bool diverso2 =  VecCC.getDiverso();
            bool uniforme2 =  VecCC.getUniforme();
            bool estatico2 =  VecCC.getEstatico();
            bool incremento2 =  VecCC.getIncremmento();
            double Min2 =  VecCC.getDTmin();
            double Max2 = 0;
            double Inc2 = 0;
            double K2 = 0;
            QVector<double> TS2 =  VecCC.getTS();
            QVector<double> TE2 = VecCC.getTE();
            QVector<double> Wcp2 =  VecCC.getWCP();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(uni == true && inc == true){
            QFile FileVec(VECPLOTCURVASCOMPUESTASAJUSTADASDINAMICO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in8(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Min1 = 0;
            double Max1 = 0;
            double Inc1 = 0;
            QVector<double> Ts,Te,WCp,H2;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            H2.resize(10);
            in8.setVersion(QDataStream::Qt_5_4);
            VecPlot_CurvasCompuestasAjustadasIncremento VecCCAI(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,Min1,Max1,Inc1);
            in8 >> VecCCAI;
            bool diverso2 = VecCCAI.getDiverso();
            bool uniforme2 = VecCCAI.getUniforme();
            bool estatico2 = VecCCAI.getEstatico();
            bool incremento2 = VecCCAI.getIncremmento();
            double Min2 = VecCCAI.getMin();
            double Max2 = VecCCAI.getMax();
            double Inc2 = VecCCAI.getInc();
            double K2 = 0;
            QVector<double> TS2 = VecCCAI.getTS();
            QVector<double> TE2 = VecCCAI.getTE();
            QVector<double> Wcp2 = VecCCAI.getWCP();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(div == true && est == true ){
            QFile FileVec(VECPLOTCCAESTATICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in13(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Dtmin = 0;
            double K = 0;
            QVector<double> Ts,Te,WCp,h;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            h.resize(10);
            in13.setVersion(QDataStream::Qt_5_4);
            VecPlot_CCAjustadasEst_Diversa VecCCAED(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,h,Dtmin,K);
            in13 >> VecCCAED;
            bool diverso2 =  VecCCAED.getDiverso();
            bool uniforme2 =  VecCCAED.getUniforme();
            bool estatico2 =  VecCCAED.getEstatico();
            bool incremento2 =  VecCCAED.getIncremmento();
            double Min2 =  VecCCAED.getDTmin();
            double Max2 = 0;
            double Inc2 = 0;
            double K2 = VecCCAED.getK();
            QVector<double> TS2 =  VecCCAED.getTS();
            QVector<double> TE2 = VecCCAED.getTE();
            QVector<double> Wcp2 =  VecCCAED.getWCP();
            QVector<double> H2 = VecCCAED.geth();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(div == true && inc == true ){
            QFile FileVec(VECPLOTCCADINAMICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in14(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Min = 0;
            double Max = 0;
            double Inc = 0;
            double K = 0;
            QVector<double> Ts,Te,WCp,h;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            h.resize(10);
            in14.setVersion(QDataStream::Qt_5_4);
            VecPlot_CCAjustadasInc_Diversa VecCCAID(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,h,Min,Max,Inc,K);
            in14 >> VecCCAID;
            bool diverso2 =  VecCCAID.getDiverso();
            bool uniforme2 =  VecCCAID.getUniforme();
            bool estatico2 =  VecCCAID.getEstatico();
            bool incremento2 =  VecCCAID.getIncremmento();
            double Min2 =  VecCCAID.getMin();
            double Max2 = VecCCAID.getMax();
            double Inc2 = VecCCAID.getInc();
            double K2 = VecCCAID.getK();
            QVector<double> TS2 =  VecCCAID.getTS();
            QVector<double> TE2 = VecCCAID.getTE();
            QVector<double> Wcp2 =  VecCCAID.getWCP();
            QVector<double> H2 = VecCCAID.geth();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }
        FileBools.flush();
        FileBools.close();
    }else if(ventanaplot==2){ // GRAN CURVA COMPUESTA
        QFile FileBools(VECPLOTGCC_BOOL_FILENAME);
        if (!FileBools.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in10(&FileBools);
        in10.setVersion(QDataStream::Qt_5_4);
        bool uniforme = false;
        bool diverso = false;
        bool estatico = false;
        bool incremento = false;
        VecPlot_CurvasCompuestasAjustadasbool VecC(uniforme,diverso,estatico,incremento);
        in10 >> VecC;
        bool uni = VecC.getUniforme();
        bool div = VecC.getDiverso();
        bool est = VecC.getEstatico();
        bool inc = VecC.getIncremmento();
        if(uni == true && est == true){
            QFile FileVec(VECPLOTGCCESTATICO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in11(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Dtmin = 0;
            QVector<double> Ts,Te,WCp,H2;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            H2.resize(10);
            in11.setVersion(QDataStream::Qt_5_4);
            VecGCCestatico VecGCest(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,Dtmin);
            in11 >> VecGCest;
            bool diverso2 =  VecGCest.getDiverso();
            bool uniforme2 =  VecGCest.getUniforme();
            bool estatico2 =  VecGCest.getEstatico();
            bool incremento2 =  VecGCest.getIncremmento();
            double Min2 =  VecGCest.getDTmin();
            double Max2 = 0;
            double Inc2 = 0;
            double K2 = 0;
            QVector<double> TS2 =  VecGCest.getTS();
            QVector<double> TE2 = VecGCest.getTE();
            QVector<double> Wcp2 =  VecGCest.getWCP();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(uni == true && inc == true){
            QFile FileVec(VECPLOTGCCDINAMICO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in12(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Min = 0;
            double Max = 0;
            double Inc = 0;
            QVector<double> Ts,Te,WCp,H2;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            H2.resize(10);
            in12.setVersion(QDataStream::Qt_5_4);
            VecGCCdinamico VecGCdin(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,Min,Max,Inc);
            in12 >> VecGCdin;
            bool diverso2 =  VecGCdin.getDiverso();
            bool uniforme2 =  VecGCdin.getUniforme();
            bool estatico2 =  VecGCdin.getEstatico();
            bool incremento2 =  VecGCdin.getIncremmento();
            double Min2 = VecGCdin.getMin();
            double Max2 = VecGCdin.getMax();
            double Inc2 = VecGCdin.getInc();
            double K2 = 0;
            QVector<double> TS2 =  VecGCdin.getTS();
            QVector<double> TE2 = VecGCdin.getTE();
            QVector<double> Wcp2 =  VecGCdin.getWCP();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(div == true && est == true){
            QFile FileVec(VECPLOTGCCESTATICO_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in15(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double DTmin = 0;
            double K = 0;
            QVector<double> Ts,Te,WCp,h1;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            h1.resize(10);
            in15.setVersion(QDataStream::Qt_5_4);
            VecGCCED GCCED(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,h1,DTmin,K);
            in15 >> GCCED;
            bool diverso2 =  GCCED.getDiverso();
            bool uniforme2 =  GCCED.getUniforme();
            bool estatico2 =  GCCED.getEstatico();
            bool incremento2 =  GCCED.getIncremmento();
            double Min2 = GCCED.getDTmin();
            double Max2 = 0;
            double Inc2 = 0;
            double K2 = GCCED.getK();
            QVector<double> TS2 =  GCCED.getTS();
            QVector<double> TE2 = GCCED.getTE();
            QVector<double> Wcp2 =  GCCED.getWCP();
            QVector<double> H2 = GCCED.geth();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }else if(div == true && inc == true){
            QFile FileVec(VECPLOTGCCDINAMICA_DIVERSO_FILENAME);
            if (!FileVec.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in16(&FileVec);
            bool Divers = false;
            bool Uniform = false;
            bool Estatic = false;
            bool Increment = false;
            double Min = 0;
            double Max = 0;
            double Inc = 0;
            double K = 0;
            QVector<double> Ts,Te,WCp,h1;
            Ts.resize(10);
            Te.resize(10);
            WCp.resize(10);
            h1.resize(10);
            in16.setVersion(QDataStream::Qt_5_4);
            VecGCCDD GCCDD(Uniform,Divers,Estatic,Increment,Ts,Te,WCp,h1,Min,Max,Inc,K);
            in16 >> GCCDD;
            bool diverso2 =  GCCDD.getDiverso();
            bool uniforme2 =  GCCDD.getUniforme();
            bool estatico2 =  GCCDD.getEstatico();
            bool incremento2 =  GCCDD.getIncremmento();
            double Min2 = GCCDD.getMin();
            double Max2 = GCCDD.getMax();
            double Inc2 = GCCDD.getInc();
            double K2 = GCCDD.getK();
            QVector<double> TS2 =  GCCDD.getTS();
            QVector<double> TE2 = GCCDD.getTE();
            QVector<double> Wcp2 =  GCCDD.getWCP();
            QVector<double> H2 = GCCDD.geth();
            QVector<double> Calentamiento2,Enfriamento2;
            int CTo2 = 0, CCo2 = 0;
            Calentamiento2.resize(10);
            Enfriamento2.resize(10);
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileVec.flush();
            FileVec.close();
        }
    }else if(ventanaplot==3){ // FALTA PARA AREAS
        QFile FileBools(VECPLOTAREAS_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in18(&FileBools);
        in18.setVersion(QDataStream::Qt_5_4);
        bool uniforme1 = false;
        bool diverso1 = false;
        VecAreasUniformebool VecAUbool(uniforme1,diverso1);
        in18 >> VecAUbool;
        bool uniforme = VecAUbool.getUniforme();
        bool diverso = VecAUbool.getDiverso();
        if(uniforme == true){
            QFile FileAreas(VECPLOTAREAS_UNIFORME_FILENAME);
            if (!FileAreas.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in17(&FileAreas);
            in17.setVersion(QDataStream::Qt_5_4);
            QVector<double> TS,TE,Wcp,h,Calentamiento,Enfriamento;
            double Min,Max,Inc,Cto,CCo;
            TS.resize(10);
            TE.resize(10);
            Wcp.resize(10);
            h.resize(10);
            Calentamiento.resize(10);
            Enfriamento.resize(10);
            Min = 0,Max = 0,Inc = 0,Cto = 0,CCo = 0;
            VecAreasUniforme VAU(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,Enfriamento,Min,Max,Inc,Cto,CCo);
            in17 >> VAU;
            bool uniforme2 = VAU.getUniforme();
            bool diverso2 = VAU.getDiverso();
            bool estatico2 = false;
            bool incremento2 = false;
            QVector<double> TS2 = VAU.getTS();
            QVector<double> TE2 = VAU.getTE();
            QVector<double> Wcp2 = VAU.getWCP();
            QVector<double> H2 = VAU.geth();
            QVector<double> Calentamiento2 = VAU.getCalentamiento();
            QVector<double> Enfriamento2 = VAU.getEnfriamento();
            double Min2 = VAU.getMin();
            double Max2 = VAU.getMax();
            double Inc2 = VAU.getInc();
            double K2 = 0;
            int CTo2 = VAU.getCTo();
            int CCo2 = VAU.getCCo();
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileAreas.flush();
            FileAreas.close();
        }else if(diverso == true){
            QFile FileAreas(VECPLOTAREAS_DIVERSO_FILENAME);
            if (!FileAreas.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in19(&FileAreas);
            in19.setVersion(QDataStream::Qt_5_4);
            QVector<double> TS,TE,Wcp,h,Calentamiento,Enfriamento;
            double Min,Max,Inc,K,CTo,CCo;
            TS.resize(10);
            TE.resize(10);
            Wcp.resize(10);
            h.resize(10);
            Calentamiento.resize(10);
            Enfriamento.resize(10);
            Min = 0,Max = 0,Inc = 0,CTo = 0,CCo = 0, K = 0;
            VecAreasDiverso VAD(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,Enfriamento,Min,Max,Inc,K,CTo,CCo);
            in19 >> VAD;
            bool uniforme2 = VAD.getUniforme();
            bool diverso2 = VAD.getDiverso();
            bool estatico2 = false;
            bool incremento2 = false;
            QVector<double> TS2 = VAD.getTS();
            QVector<double> TE2 = VAD.getTE();
            QVector<double> Wcp2 = VAD.getWCP();
            QVector<double> H2 = VAD.geth();
            QVector<double> Calentamiento2 = VAD.getCalentamiento();
            QVector<double> Enfriamento2 = VAD.getEnfriamento();
            double Min2 = VAD.getMin();
            double Max2 = VAD.getMax();
            double Inc2 = VAD.getInc();
            double K2 = VAD.getK();
            int CTo2 = VAD.getCTo();
            int CCo2 = VAD.getCCo();
            QVector<double> OperationCost2;
            OperationCost2.resize(10);
            QVector<QVector<double>> CapitalCost2;
            CapitalCost2.resize(10);
            for(int i = 0; i < CapitalCost2.size(); i++){
                CapitalCost2[i].resize(4);
            }
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileAreas.flush();
            FileAreas.close();
        }
        FileBools.flush();
        FileBools.close();
    }else if(ventanaplot == 4){// Costos
        QFile FileBools(VECPLOTCOSTOS_BOOL_FILENAME);
            if (!FileBools.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in20(&FileBools);
        in20.setVersion(QDataStream::Qt_5_4);
        bool uniforme1 = false;
        bool diverso1 = false;
        VecCostUniformebool VCUB(uniforme1,diverso1);
        in20 >> VCUB;
        bool uniforme = VCUB.getUniforme();
        bool diverso = VCUB.getDiverso();
        if(uniforme == true){
            QFile FileCostos(VECPLOTCOSTOS_UNIFORME_FILENAME);
            if (!FileCostos.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in21(&FileCostos);
            in21.setVersion(QDataStream::Qt_5_4);
            QVector<double> TS,TE,Wcp,h,Calentamiento,Enfriamento,OperationCost;
            //bool uniforme, diverso;
            double Min,Max,Inc,Cto,CCo;
            TS.resize(10);
            TE.resize(10);
            Wcp.resize(10);
            h.resize(10);
            Calentamiento.resize(10);
            Enfriamento.resize(10);
            OperationCost.resize(10);
            QVector<QVector<double>> CapitalCost;
            CapitalCost.resize(10);
            for(int i = 0; i < CapitalCost.size(); i++){
                CapitalCost[i].resize(10);
            }
            Min = 0,Max = 0,Inc = 0,Cto = 0,CCo = 0;
            VecCostUniforme VCU(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,
                                Enfriamento,CapitalCost,OperationCost,Min,
                                Max,Inc,Cto,CCo);
            in21 >> VCU;
            bool uniforme2 = VCU.getUniforme();
            bool diverso2 = VCU.getDiverso();
            bool estatico2 = false;
            bool incremento2 = false;
            QVector<double> TS2 = VCU.getTS();
            QVector<double> TE2 = VCU.getTE();
            QVector<double> Wcp2 = VCU.getWCP();
            QVector<double> H2 = VCU.geth();
            QVector<double> Calentamiento2 = VCU.getCalentamiento();
            QVector<double> Enfriamento2 = VCU.getEnfriamento();
            QVector<QVector<double>> CapitalCost2 = VCU.getCapitalCost();
            QVector<double> OperationCost2 = VCU.getOperationCost();
            double Min2 = VCU.getMin();
            double Max2 = VCU.getMax();
            double Inc2 = VCU.getInc();
            double K2 = 0;
            int CTo2 = VCU.getCTo();
            int CCo2 = VCU.getCCo();
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileCostos.flush();
            FileCostos.close();
        }else if(diverso == true){
            QFile FileCostos(VECPLOTCOSTOS_DIVERSO_FILENAME);
            if (!FileCostos.open(QIODevice::ReadOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream in22(&FileCostos);
            in22.setVersion(QDataStream::Qt_5_4);
            QVector<double> TS,TE,Wcp,h,Calentamiento,Enfriamento,OperationCost;
            double Min,Max,Inc,K;
            TS.resize(10);
            TE.resize(10);
            Wcp.resize(10);
            h.resize(10);
            Calentamiento.resize(10);
            Enfriamento.resize(10);
            OperationCost.resize(10);
            QVector<QVector<double>> CapitalCost;
            CapitalCost.resize(10);
            for(int i = 0; i < CapitalCost.size(); i++){
                CapitalCost[i].resize(4);
            }
            Min = 0,Max = 0,Inc = 0, K=0;
            VecCostDiverso VCD(uniforme,diverso,TS,TE,Wcp,h,Calentamiento,
                               Enfriamento,CapitalCost,OperationCost,Min,
                               Max,Inc,K);
            in22 >> VCD;
            bool uniforme2 = VCD.getUniforme();
            bool diverso2 = VCD.getDiverso();
            bool estatico2 = false;
            bool incremento2 = false;
            QVector<double> TS2 = VCD.getTS();
            QVector<double> TE2 = VCD.getTE();
            QVector<double> Wcp2 = VCD.getWCP();
            QVector<double> H2 = VCD.geth();
            QVector<double> Calentamiento2 = VCD.getCalentamiento();
            QVector<double> Enfriamento2 = VCD.getEnfriamento();
            QVector<QVector<double>> CapitalCost2 = VCD.getCapitalCost();
            QVector<double> OperationCost2 = VCD.getOperationCost();
            double Min2 = VCD.getMin();
            double Max2 = VCD.getMax();
            double Inc2 = VCD.getInc();
            double K2 = VCD.getK();
            int CTo2 = 0;
            int CCo2 = 0;
            plot(ventanaplot,uniforme2,diverso2,estatico2,incremento2,TS2,TE2,Wcp2,H2,Calentamiento2,Enfriamento2,CTo2,CCo2,Min2,Max2,Inc2,K2,
                 CapitalCost2,OperationCost2);
            FileCostos.flush();
            FileCostos.close();
        }
        FileBools.flush();
        FileBools.close();
    }
    Fil.flush();
    Fil.close();
}

plotter::~plotter()
{
    delete ui;
}

void plotter::plot(int ventanaplot, bool uniforme, bool diverso, bool estatico, bool incremento,
                   QVector<double> TS, QVector<double> TE, QVector<double> Wcp, QVector<double> h,
                   QVector<double> Calentamiento, QVector<double> Enfriamento,int CTo,int CCo,
                   double Min, double Max, double Inc, double K,QVector<QVector<double>> CapitalCost,
                   QVector<double> OperaetionCost2)
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
    if(ventanaplot == 0){ // es la de las curvas compuestas OK
        ui->qcustomplot->setVisible(true);
        ui->qcustomplot->setEnabled(true);
        ui->qcustomplot->clearGraphs();
        ui->qcustomplot->legend->setVisible(true);
        ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
        ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        if(uniforme == true){
            Plot_curvascompuestas plot(TS,TE,Wcp);
            QVector<double> CCENTALPIA = plot.getCCENTALPIA();
            QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setName("Hot curve");
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
            ui->qcustomplot->replot();
            QVector<double> CFENTALPIA = plot.getCFENTALPIA();
            QVector<double> CFTEMPERATURAS = plot.getCFTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setName("Cold curve");
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setData(CFENTALPIA,CFTEMPERATURAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Uniform composite curves", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true){
            double DTmin = Min;
            float punto1 = 0.05;
            float punto2 = 10.0;
            Plot_curvascompuestas_diversa plot1(TS,TE,Wcp,h,DTmin,K,punto1,punto2);
            QVector<double> CCENTALPIA = plot1.getCCENTALPIA();
            QVector<double> CCTEMPERATURAS = plot1.getCCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setName("Hot curve");
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
            ui->qcustomplot->replot();
            QVector<double> CFENTALPIA = plot1.getCFENTALPIA();
            QVector<double> CFTEMPERATURAS = plot1.getCFTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setName("Cold curve");
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setData(CFENTALPIA,CFTEMPERATURAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Diverse composite curves", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }
    }else if(ventanaplot == 1){// es la de las curvas compuestas ajustadas OK
        ui->qcustomplot->setVisible(true);
        ui->qcustomplot->setEnabled(true);
        ui->qcustomplot->clearGraphs();
        ui->qcustomplot->legend->setVisible(true);
        ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
        ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        if(uniforme == true && estatico == true){
            double DTmin = Min;
            Plot_curvascompuestasajustadas plot(TS,TE,Wcp,DTmin);
            QVector<double> CCENTALPIA = plot.getCCENTALPIA();
            QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setName("Hot curve");
            ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
            ui->qcustomplot->replot();
            QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
            QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setName("Cold curve");
            ui->qcustomplot->graph(1)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Uniform composite curves adjusted", QFont("sans", 12, QFont::Bold)));
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->replot();
        }else if(uniforme == true && incremento == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Minimo;
            int j=0;
            Plot_curvascompuestasajustadas plot(TS,TE,Wcp,DTmin);
            QVector<double> CCENTALPIA = plot.getCCENTALPIA();
            QVector<double> CCTEMPERATURAS = plot.getCCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(j)->setData(CCENTALPIA,CCTEMPERATURAS);
            ui->qcustomplot->graph(j)->rescaleAxes();
            ui->qcustomplot->graph(j)->setName("Hot curve");
            for(int i = 0; i <= Iteraciones ; i++){
                j++;
                Plot_curvascompuestasajustadas plot(TS,TE,Wcp,DTmin);
                QVector<double> CFENTALPIAAJUSTADAS = plot.getCFENTALPIAAJUSTADAS();
                QVector<double> CFTEMPERATURASAJUSTADAS = plot.getCFTEMPERATURASAJUSTADAS();
                ui->qcustomplot->addGraph();
                if(j == 1){
                    ui->qcustomplot->graph(j)->setName("Cold curve");
                }else if(j >= 2){
                    ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
                }
                ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
                ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
                ui->qcustomplot->graph(j)->rescaleAxes(true);
                DTmin = DTmin + Incremento;
            }
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Uniform composite curves adjusted", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true && estatico == true){
            double DTmin = Min;
            float punto1 = 0.05;
            float punto2 = 10.0;
            Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,Wcp,h,DTmin,K,punto1,punto2);
            QVector<double> CCENTALPIA = plot2.getCCENTALPIA();
            QVector<double> CCTEMPERATURAS = plot2.getCCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setName("Hot curve");
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setData(CCENTALPIA,CCTEMPERATURAS);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->replot();
            QVector<double> CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
            QVector<double> CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setName("Cold curve");
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Diverse composite curves adjusted", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true && incremento == true){
                double Minimo = Min;
                double Maximo = Max;
                double Incremento = Inc;
                double Iteraciones =(Maximo-Minimo)/Incremento;
                double DTmin = Minimo;
                int j=0;
                float punto1 = 0.05;
                float punto2 = 10.0;
                for(int i = 0; i <= Iteraciones ; i++){
                    Plot_CCAJUSTADA_DIVERSA plot2(TS,TE,Wcp,h,DTmin,K,punto1,punto2);
                    QVector<double> CCENTALPIA = plot2.getCCENTALPIA();
                    QVector<double> CCTEMPERATURAS = plot2.getCCTEMPERATURAS();
                    ui->qcustomplot->addGraph();
                    if(i == 0){
                        ui->qcustomplot->graph(j)->setName("Hot curve");
                    }else if( i >= 1){
                        ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
                    }
                    ui->qcustomplot->graph(j)->setPen(QPen(Qt::red));
                    ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(j)->setData(CCENTALPIA,CCTEMPERATURAS);
                    ui->qcustomplot->graph(j)->rescaleAxes();
                    j++;
                    QVector<double> CFENTALPIAAJUSTADAS = plot2.getCFENTALPIAAJUSTADAS();
                    QVector<double> CFTEMPERATURASAJUSTADAS = plot2.getCFTEMPERATURASAJUSTADAS();
                    ui->qcustomplot->addGraph();
                    if(i == 0){
                        ui->qcustomplot->graph(j)->setName("Cold curve");
                    }else if( i >= 1){
                        ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
                    }
                    ui->qcustomplot->graph(j)->setPen(QPen(Qt::blue));
                    ui->qcustomplot->graph(j)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(j)->setData(CFENTALPIAAJUSTADAS,CFTEMPERATURASAJUSTADAS);
                    ui->qcustomplot->graph(j)->rescaleAxes(true);
                    j++;
                    punto1 = float (plot2.getK());
                    punto2 = 10.0;
                    DTmin = DTmin + Incremento;
                }
                if(SI == true){
                    ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                    ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
                }else if(SIS == true){
                    ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                    ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
                }
                ui->qcustomplot->plotLayout()->insertRow(0);
                ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Diverse composite curves adjusted", QFont("sans", 12, QFont::Bold)));
                ui->qcustomplot->replot();
            }
    }else if(ventanaplot == 2){ // GRAN CURVA COMPUESTA
        ui->qcustomplot->setVisible(true);
        ui->qcustomplot->setEnabled(true);
        ui->qcustomplot->clearGraphs();
        ui->qcustomplot->legend->setVisible(true);
        ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
        ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        if(uniforme == true && estatico == true){
            double DTmin = Min;
            Plot_grancurvacompuesta plot(TS,TE,Wcp,DTmin);
            QVector<double> GCENTALPIA = plot.getGCENTALPIA();
            QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setName("Grand composite curve");
            ui->qcustomplot->graph(0)->setData(GCENTALPIA,GCTEMPERATURAS);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Uniform grand compositive curve", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(uniforme == true && incremento == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Minimo;
            for(int i = 0; i <= Iteraciones ; i++){
                 Plot_grancurvacompuesta plot(TS,TE,Wcp,DTmin);
                 QVector<double> GCENTALPIA = plot.getGCENTALPIA();
                 QVector<double> GCTEMPERATURAS = plot.getGCTEMPERATURAS();
                 ui->qcustomplot->addGraph();
                 if(i == 0){
                     ui->qcustomplot->graph(i)->setName("Grand composite curve");
                 }else if( i >= 1){
                     ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
                 }
                 ui->qcustomplot->graph(i)->setPen(QPen(Qt::blue));
                 ui->qcustomplot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                 ui->qcustomplot->graph(i)->setLineStyle(QCPGraph::lsLine);
                 ui->qcustomplot->graph(i)->setData(GCENTALPIA,GCTEMPERATURAS);
                 ui->qcustomplot->graph(i)->rescaleAxes();
                 ui->qcustomplot->graph(i)->rescaleAxes(true);
                 DTmin = DTmin + Incremento;
            }
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Uniform grand compositive curve", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true && estatico == true){
            double DTMin = Min;
            float punto1 = 0.05;
            float punto2 = 10.0;
            PlotGCC_DIVERSA plotGCCD(TS,TE,Wcp,h,DTMin,K,punto1,punto2);
            QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
            QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setName("Grand composite curve");
            ui->qcustomplot->graph(0)->setData(GCENTALPIA,GCTEMPERATURAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Diverse grand compositive curve", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true && incremento == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Minimo;
            float punto1 = 0.05;
            float punto2 = 10.0;

            for( int i = 0; i <= Iteraciones ; i++){
                 PlotGCC_DIVERSA plotGCCD(TS,TE,Wcp,h,DTmin,K,punto1,punto2);
                 QVector<double> GCENTALPIA = plotGCCD.getGCENTALPIA();
                 QVector<double> GCTEMPERATURAS = plotGCCD.getGCTEMPERATURAS();
                 ui->qcustomplot->addGraph();
                 if(i == 0){
                     ui->qcustomplot->graph(i)->setName("Grand composite curve");
                 }else if( i >= 1){
                     ui->qcustomplot->legend->removeItem(ui->qcustomplot->legend->itemCount()-1);
                 }
                 ui->qcustomplot->graph(i)->setPen(QPen(Qt::blue));
                 ui->qcustomplot->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                 ui->qcustomplot->graph(i)->setLineStyle(QCPGraph::lsLine);
                 ui->qcustomplot->graph(i)->setData(GCENTALPIA,GCTEMPERATURAS);
                 ui->qcustomplot->graph(i)->rescaleAxes();
                 ui->qcustomplot->graph(i)->rescaleAxes(true);
                 DTmin = DTmin + Incremento;
                 punto1 = float (plotGCCD.getK());
                 punto2 = 10.0;
            }
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY BTU/HR");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE ªF");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("ENTHALPY W");
                ui->qcustomplot->yAxis->setLabel("TEMPERATURE  ªC");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Diverse grand compositive curve", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }
    }else if(ventanaplot==3){ //areas
        ui->qcustomplot->setVisible(true);
        ui->qcustomplot->setEnabled(true);
        ui->qcustomplot->clearGraphs();
        ui->qcustomplot->legend->setVisible(true);
        ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
        ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        if(uniforme == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Minimo;
            QVector<double> AREAS,DTMIN;
            AREAS.resize(Iteraciones+1);
            DTMIN.resize(Iteraciones+1);
            int l = 0;
            for(int i = 0; i < (Iteraciones+1) ; i++){
                 Plot_Dtmin_vs_Areas plot3(TS,TE,Wcp,h,Calentamiento,Enfriamento,DTmin,CTo,CCo);
                 double AREA = plot3.getAREAS();
                 if(AREA < 0){
                     continue;
                 }else{
                     AREAS[l] = AREA;
                     DTMIN[l] = DTmin;
                     l++;
                 }
                 DTmin = DTmin + Incremento;
            }
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
                ui->qcustomplot->yAxis->setLabel("AREAS ft^2");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
                ui->qcustomplot->yAxis->setLabel("AREAS mt^2");
            }
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setName("Total area");
            ui->qcustomplot->graph(0)->setData(DTMIN,AREAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Area vs Delta T min", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Min;
            QVector<double> AREAS;
            QVector<double> DTMIN;
            AREAS.resize(Iteraciones+1);
            DTMIN.resize(Iteraciones+1);
            float punto1 = 0.05;
            float punto2 = 10.0;
            int l = 0;
            for(int i = 0; i < (Iteraciones+1) ; i++){
                 Plot_Dtmin_vs_Areas_DIVERSO plot3(TS,TE,Wcp,h,Calentamiento,Enfriamento,
                                                   K,DTmin,punto1,punto2);
                 double AREA = plot3.getAREAS();
                 if(AREA < 0){
                    continue;
                 }else{
                     AREAS[l] = AREA;
                     DTMIN[l] = DTmin;
                     l++;
                 }
                 DTmin = DTmin + Incremento;
                 punto1 = float (plot3.getK());
                 punto2 = 10.0;
            }
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
                ui->qcustomplot->yAxis->setLabel("AREAS ft^2");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
                ui->qcustomplot->yAxis->setLabel("AREAS mt^2");
            }
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setName("Total area");
            ui->qcustomplot->graph(0)->setData(DTMIN,AREAS);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Area vs Delta T min", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }
    }else if(ventanaplot==4){//costos
        ui->qcustomplot->setVisible(true);
        ui->qcustomplot->setEnabled(true);
        ui->qcustomplot->clearGraphs();
        ui->qcustomplot->legend->setVisible(true);
        ui->qcustomplot->legend->setFont(QFont("Helvetica",9));
        ui->qcustomplot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
        if(uniforme == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Minimo;
            QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN;
            VECCOSTOTOTAL.resize(Iteraciones+1);
            VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
            VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
            VECDTMIN.resize(Iteraciones+1);
            int l = 0;
            for(int i = 0; i < (Iteraciones+1) ; i++){
                Plot_Costos_vs_Areas_Uniforme plot4(TS,TE,Wcp,h,Calentamiento,Enfriamento,
                                                    CapitalCost,OperaetionCost2,DTmin,CTo,CCo,SI,SIS);
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
                DTmin = DTmin + Incremento;
            }
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setName("Total cost");
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setData(VECDTMIN,VECCOSTOTOTAL);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setName("Total operation cost");
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(1)->setData(VECDTMIN,VECCOSTOOPERACIONTOTAL);
            ui->qcustomplot->graph(1)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(2)->setName("Total capital cost");
            ui->qcustomplot->graph(2)->setPen(QPen(Qt::darkCyan));
            ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(2)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(2)->setData(VECDTMIN,VECCOSTOCAPITALTOTAL);
            ui->qcustomplot->graph(2)->rescaleAxes();
            ui->qcustomplot->graph(2)->rescaleAxes(true);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
                ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
                ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Cost vs Delta T min", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }else if(diverso == true){
            double Minimo = Min;
            double Maximo = Max;
            double Incremento = Inc;
            double Iteraciones =(Maximo-Minimo)/Incremento;
            double DTmin = Min;
            QVector<double> VECCOSTOTOTAL,VECCOSTOCAPITALTOTAL,VECCOSTOOPERACIONTOTAL,VECDTMIN;
            VECCOSTOTOTAL.resize(Iteraciones+1);
            VECCOSTOCAPITALTOTAL.resize(Iteraciones+1);
            VECCOSTOOPERACIONTOTAL.resize(Iteraciones+1);
            VECDTMIN.resize(Iteraciones+1);
            float punto1 = 0.05;
            float punto2 = 10.0;
            int j = 0;
            for(int i = 0; i < (Iteraciones+1) ; i++){
                Plot_Costos_vs_Min_Divera plot4(TS,TE,Wcp,h,Calentamiento,Enfriamento,
                                                CapitalCost,OperaetionCost2,K,DTmin,punto1,punto2,SI,SIS);
                double COSTOTOTAL = plot4.getCostoTotal();
                double COSTOCAPITALTOTAL = plot4.getCostoCapitalTotal();
                double COSTOOPERACIONTOTAL = plot4.getCostoOperacionTotal();
                if(COSTOTOTAL < 0 || COSTOCAPITALTOTAL < 0 || COSTOOPERACIONTOTAL < 0 ){
                    continue;
                }else{
                    VECCOSTOTOTAL[j] = COSTOTOTAL;
                    VECCOSTOCAPITALTOTAL[j] = COSTOCAPITALTOTAL;
                    VECCOSTOOPERACIONTOTAL[j] = COSTOOPERACIONTOTAL;
                    VECDTMIN[j] = DTmin;
                }
                DTmin = DTmin + Incremento;
                punto1 = float (plot4.getK());
                punto2 = 10.0;
            }
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(0)->setName("Total cost");
            ui->qcustomplot->graph(0)->setPen(QPen(Qt::red));
            ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(0)->setData(VECDTMIN,VECCOSTOTOTAL);
            ui->qcustomplot->graph(0)->rescaleAxes();
            ui->qcustomplot->graph(0)->rescaleAxes(true);
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(1)->setName("Total operation cost");
            ui->qcustomplot->graph(1)->setPen(QPen(Qt::blue));
            ui->qcustomplot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(1)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(1)->setData(VECDTMIN,VECCOSTOOPERACIONTOTAL);
            ui->qcustomplot->graph(1)->rescaleAxes();
            ui->qcustomplot->graph(1)->rescaleAxes(true);
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(2)->setName("Total capital cost");
            ui->qcustomplot->graph(2)->setPen(QPen(Qt::darkCyan));
            ui->qcustomplot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
            ui->qcustomplot->graph(2)->setLineStyle(QCPGraph::lsLine);
            ui->qcustomplot->graph(2)->setData(VECDTMIN,VECCOSTOCAPITALTOTAL);
            ui->qcustomplot->graph(2)->rescaleAxes();
            ui->qcustomplot->graph(2)->rescaleAxes(true);
            if(SI == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªF");
                ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
            }else if(SIS == true){
                ui->qcustomplot->xAxis->setLabel("DTMIN ªC");
                ui->qcustomplot->yAxis->setLabel("COSTOS US $ / YEAR");
            }
            ui->qcustomplot->plotLayout()->insertRow(0);
            ui->qcustomplot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->qcustomplot, "Cost vs Delta T min", QFont("sans", 12, QFont::Bold)));
            ui->qcustomplot->replot();
        }

    }
    FileUnidades.flush();
    FileUnidades.close();
}

void plotter::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
    QString extension = ui->comboBox->currentText();
    QString URL = file_name + extension;
    QFile file(URL);
    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
         {
             QMessageBox::warning(0,"Could not create Project File",QObject::tr( "\n Could not create Project File on disk"));
         }
   ui->qcustomplot->saveJpg(URL,  0, 0, 1.0, -1  );
}


void plotter::on_pushButton_2_clicked()
{
    QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
    QString URL = file_name + ".pdf";
    QFile file(URL);
    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
         {
             QMessageBox::warning(0,"Could not create Project File",QObject::tr( "\n Could not create Project File on disk"));
         }
    ui->qcustomplot->savePdf(URL,0,0,QCP::ExportPen(),"","");
}
