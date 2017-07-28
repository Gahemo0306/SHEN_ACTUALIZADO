#ifndef DUVALORACTION_H
#define DUVALORACTION_H
#include <QDataStream>
#include <QVector>
#include <QDebug>

//FUNCIONA
class Duvaloraction
{
public:
    Duvaloraction(int valact = -1);
    void setvalact(int valact);
    int getvalact() const;

    friend QDataStream &operator<<(QDataStream &out1,const Duvaloraction &valoraction);
    friend QDataStream &operator>>(QDataStream &in1, Duvaloraction &valoraction);

private:
    int Mvalact;
};
QDataStream &operator<<(QDataStream &out1, const Duvaloraction &valor); //salida
QDataStream &operator>>(QDataStream &in1, Duvaloraction &valoraction); //entrada

//FUNCIONA

class Workspace
{
public:
    Workspace(QVector<QVector<double> > &matriz);
    QVector<QVector<double>> getMatriz() const;
    friend QDataStream &operator<<(QDataStream &out2, const Workspace &MATRIZ );
    friend QDataStream &operator>>(QDataStream &in2, Workspace &MATRIZ);
private:
    QVector<QVector<double>> MMatriz;
    QVector<QVector<double>> matriz;
};
QDataStream &operator<<(QDataStream &out2, const Workspace &MATRIZ);
QDataStream &operator>>(QDataStream &in2, Workspace &MATRIZ);


//FUNCIONA

class Unidades
{
public:
    Unidades(bool SI = false, bool SIS = false, int UTemp = -1,
             int UWcp= -1, int Uh =-1 );
    bool getSI();
    bool getSIS();
    int getUTemp();
    int getUWcp();
    int getUh();
    void ConvertirUnidades(QVector<double> ST, QVector<double> TT,
                           QVector<double> Cp, bool SI, bool SIN,
                           int A, int B);
    double ConvertirDTmin(double DTmin,bool SI,bool SIN,int A);
    double ConvertirMax(double Max,bool SI,bool SIN,int A);
    double ConvertirInc(double Inc,bool SI,bool SIN,int A);
    QVector<double> getST();
    QVector<double> getTT();
    QVector<double> getCp();
    //VOID CONVERTIR UNIDAES
    friend QDataStream &operator<<(QDataStream &out3, const Unidades &units);
    friend QDataStream &operator>>(QDataStream &in3, Unidades &units);

private:
    QVector<double> MST,MTT,MCp,Mh;
    bool MSI,MSIS;
    int  MUTemp,MUWcp,MUh, MA, MB ,MC;
};
QDataStream &operator<<(QDataStream &out3, const Unidades &units);
QDataStream &operator>>(QDataStream &in3, Unidades &units);

//FUNCIONA

class Tabplot
{
public:
    Tabplot(int tabvalue = -1);
    void settabvalue(int tabvalue);
    int gettabvalue();
    friend QDataStream &operator<<(QDataStream &out4, const Tabplot &tabvalue);
    friend QDataStream &operator>>(QDataStream &in4, Tabplot &tabvalue);
private:
    int Mtabvalue;
};
QDataStream &operator<<(QDataStream &out4, const Tabplot &tabvalue);
QDataStream &operator>>(QDataStream &in4, Tabplot &tabvalue);

//FUNCIONA

class Valordeoperacion
{
public:
    Valordeoperacion(int valor = -1);
    void setvalor(int valor);
    int getvalor() const;
    friend QDataStream &operator<<(QDataStream &out5, const Valordeoperacion &valor);
    friend QDataStream &operator>>(QDataStream &in5, Valordeoperacion &valor);
private:
    int Mvalor;
};
QDataStream &operator<<(QDataStream &out5, const Valordeoperacion &valor);
QDataStream &operator>>(QDataStream &in5, Valordeoperacion &valor);

class VecPlot_CurvasCompuestas
{
public:
    VecPlot_CurvasCompuestas(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             QVector<double> &h,
                             double Min, double Max,
                             double Inc, double K);
    //,uniforme,diverso,estatico,incremento,TS,TE,Wcp
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    double getK() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    friend QDataStream &operator<<(QDataStream &out6, const VecPlot_CurvasCompuestas &VecCC);
    friend QDataStream &operator>>(QDataStream &in6, VecPlot_CurvasCompuestas &VecCC);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MMin,MMax,MInc,MK;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
    QVector<double> Mh;
};
QDataStream &operator<<(QDataStream &out6, const VecPlot_CurvasCompuestas &VecCC);
QDataStream &operator>>(QDataStream &in6, VecPlot_CurvasCompuestas &VecCC);

class VecPlot_CurvasCompuestasAjustadasEstatico
{
public:
    VecPlot_CurvasCompuestasAjustadasEstatico(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             double DTmin);
    //,uniforme,diverso,estatico,incremento,TS,TE,Wcp
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getDTmin() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    friend QDataStream &operator<<(QDataStream &out7, const VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE);
    friend QDataStream &operator>>(QDataStream &in7, VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MDTmin;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
};
QDataStream &operator<<(QDataStream &out7, const VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE);
QDataStream &operator>>(QDataStream &in7, VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE);

class VecPlot_CurvasCompuestasAjustadasIncremento
{
public:
    VecPlot_CurvasCompuestasAjustadasIncremento(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             double Min, double Max, double Inc);
    //,uniforme,diverso,estatico,incremento,TS,TE,Wcp
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    friend QDataStream &operator<<(QDataStream &out8, const VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI);
    friend QDataStream &operator>>(QDataStream &in8, VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MMin,MMax,MInc;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
};
QDataStream &operator<<(QDataStream &out8, const VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI);
QDataStream &operator>>(QDataStream &in8, VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI);

class VecPlot_CurvasCompuestasAjustadasbool
{
public:
    VecPlot_CurvasCompuestasAjustadasbool(bool uniforme, bool diverso, bool estatico, bool incremento);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    friend QDataStream &operator<<(QDataStream &out9, const VecPlot_CurvasCompuestasAjustadasbool &VecCCAB);
    friend QDataStream &operator>>(QDataStream &in9, VecPlot_CurvasCompuestasAjustadasbool &VecCCAB);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
};
QDataStream &operator<<(QDataStream &out9, const VecPlot_CurvasCompuestasAjustadasbool &VecCCAB);
QDataStream &operator>>(QDataStream &in9, VecPlot_CurvasCompuestasAjustadasbool &VecCCAB);

class VecGCCbool
{
public:
    VecGCCbool(bool uniforme, bool diverso, bool estatico, bool incremento);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    friend QDataStream &operator<<(QDataStream &out10, const VecGCCbool &VecGCCbools);
    friend QDataStream &operator>>(QDataStream &in10, VecGCCbool &VecGCCbools);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
};
QDataStream &operator<<(QDataStream &out10, const VecGCCbool &VecGCCbools);
QDataStream &operator>>(QDataStream &in10, VecGCCbool &VecGCCbools);

class VecGCCestatico
{
public:
    VecGCCestatico(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             double DTmin);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getDTmin() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    friend QDataStream &operator<<(QDataStream &out11, const VecGCCestatico &VecGCest);
    friend QDataStream &operator>>(QDataStream &in11, VecGCCestatico &VecGCest);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MDTmin;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
};
QDataStream &operator<<(QDataStream &out11, const VecGCCestatico &VecGCest);
QDataStream &operator>>(QDataStream &in11, VecGCCestatico &VecGCest);

class VecGCCdinamico
{
public:
    VecGCCdinamico(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             double Min, double Max,double Inc);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    friend QDataStream &operator<<(QDataStream &out12, const VecGCCdinamico &VecGCdin);
    friend QDataStream &operator>>(QDataStream &in12, VecGCCdinamico &VecGCdin);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MMin,MMax,MInc;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
};
QDataStream &operator<<(QDataStream &out12, const VecGCCdinamico &VecGCdin);
QDataStream &operator>>(QDataStream &in12, VecGCCdinamico &VecGCdin);

class VecPlot_CCAjustadasEst_Diversa
{
public:
    VecPlot_CCAjustadasEst_Diversa(bool uniforme, bool diverso, bool estatico, bool incremento,
                                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                                   QVector<double> &h, double DTmin,double K);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getDTmin() const;
    double getK() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    friend QDataStream &operator<<(QDataStream &out13, const VecPlot_CCAjustadasEst_Diversa &VecCCAED);
    friend QDataStream &operator>>(QDataStream &in13, VecPlot_CCAjustadasEst_Diversa &VecCCAED);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MDTmin;
    double MK;
    QVector<double> MTS;
    QVector<double> MTE;
    QVector<double> MWCP;
    QVector<double> Mh;
};
QDataStream &operator<<(QDataStream &out13, const VecPlot_CCAjustadasEst_Diversa &VecCCAED);
QDataStream &operator>>(QDataStream &in13, VecPlot_CCAjustadasEst_Diversa &VecCCAED);


class VecPlot_CCAjustadasInc_Diversa
{
public:
    VecPlot_CCAjustadasInc_Diversa(bool uniforme, bool diverso, bool estatico, bool incremento,
                                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                                   QVector<double> &h, double min, double max, double inc ,double K);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    double getK() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    friend QDataStream &operator<<(QDataStream &out14, const VecPlot_CCAjustadasInc_Diversa &VecCCAID);
    friend QDataStream &operator>>(QDataStream &in14, VecPlot_CCAjustadasInc_Diversa &VecCCAID);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MMin,MMax,MInc,MK;
    QVector<double> MTS,MTE,MWCP,Mh;
};
QDataStream &operator<<(QDataStream &out14, const VecPlot_CCAjustadasInc_Diversa &VecCCAID);
QDataStream &operator>>(QDataStream &in14, VecPlot_CCAjustadasInc_Diversa &VecCCAID);

class VecGCCED
{
public:
    VecGCCED(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             QVector<double> &h ,double DTmin,double K);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getDTmin() const;
    double getK() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    friend QDataStream &operator<<(QDataStream &out15, const VecGCCED &GCCED);
    friend QDataStream &operator>>(QDataStream &in15, VecGCCED &GCCED);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MDTmin, MK;
    QVector<double> MTS, MTE, MWCP, Mh;
};
QDataStream &operator<<(QDataStream &out15, const VecGCCED &GCCED);
QDataStream &operator>>(QDataStream &in15, VecGCCED &GCCED);

class VecGCCDD
{
public:
    VecGCCDD(bool uniforme, bool diverso, bool estatico, bool incremento,
                             QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                             QVector<double> &h ,double Min,double Max, double Inc,double K);
    bool getUniforme() const;
    bool getDiverso() const;
    bool getEstatico() const;
    bool getIncremmento() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    double getK() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    friend QDataStream &operator<<(QDataStream &out16, const VecGCCDD &GCCDD);
    friend QDataStream &operator>>(QDataStream &in16, VecGCCDD &GCCDD);
private:
    bool Muniforme,Mdiverso,Mestatico,Mincremento;
    double MMin,MMax,MInc,MK;
    QVector<double> MTS, MTE, MWCP, Mh;
};
QDataStream &operator<<(QDataStream &out16, const VecGCCDD &GCCDD);
QDataStream &operator>>(QDataStream &in16, VecGCCDD &GCCDD);

class VecAreasUniforme
{
public:
    VecAreasUniforme(bool uniforme, bool diverso,
                     QVector<double> &TS, QVector<double> &TE,
                     QVector<double> &Wcp,QVector<double> &h ,
                     QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                     double Min,double Max, double Inc, int CTo,int CCo);
    bool getUniforme() const;
    bool getDiverso() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    int getCTo() const;
    int getCCo() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    friend QDataStream &operator<<(QDataStream &out17, const VecAreasUniforme &VAU);
    friend QDataStream &operator>>(QDataStream &in17, VecAreasUniforme &VAU);
private:
    bool Muniforme,Mdiverso;
    double MMin, MMax, MInc, MCTo, MCCo;
    QVector<double> MTS, MTE, MWCP, Mh , MCalentamiento, MEnfriamento;
};
QDataStream &operator<<(QDataStream &out17, const VecAreasUniforme &VAU);
QDataStream &operator>>(QDataStream &in17, VecAreasUniforme &VAU);

class VecAreasUniformebool
{
public:
    VecAreasUniformebool(bool uniforme, bool diverso);
    bool getUniforme() const;
    bool getDiverso() const;
    friend QDataStream &operator<<(QDataStream &out18, const VecAreasUniformebool &VecAUB);
    friend QDataStream &operator>>(QDataStream &in18, VecAreasUniformebool &VecAUB);
private:
    bool Muniforme,Mdiverso;
};
QDataStream &operator<<(QDataStream &out18, const VecAreasUniformebool &VecAUB);
QDataStream &operator>>(QDataStream &in18, VecAreasUniformebool &VecAUB);

class VecAreasDiverso
{
public:
    VecAreasDiverso(bool uniforme, bool diverso,
                     QVector<double> &TS, QVector<double> &TE,
                     QVector<double> &Wcp,QVector<double> &h ,
                     QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                     double Min,double Max, double Inc, double K, int CTo,int CCo);
    bool getUniforme() const;
    bool getDiverso() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    double getK() const;
    int getCTo() const;
    int getCCo() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    friend QDataStream &operator<<(QDataStream &out19, const VecAreasDiverso &VAD);
    friend QDataStream &operator>>(QDataStream &in19, VecAreasDiverso &VAD);
private:
    bool Muniforme,Mdiverso;
    double MMin, MMax, MInc, MCTo, MCCo,MK;
    QVector<double> MTS, MTE, MWCP, Mh , MCalentamiento, MEnfriamento;
};
QDataStream &operator<<(QDataStream &out19, const VecAreasDiverso &VAD);
QDataStream &operator>>(QDataStream &in19, VecAreasDiverso &VAD);

class VecCostUniformebool
{
public:
    VecCostUniformebool(bool uniforme,bool diverso);
    bool getUniforme() const;
    bool getDiverso() const;
    friend QDataStream &operator<<(QDataStream &out20, const VecCostUniformebool &VecCUB);
    friend QDataStream &operator>>(QDataStream &in20, VecCostUniformebool &VecCUB);
private:
    bool Muniforme,Mdiverso;

};
QDataStream &operator<<(QDataStream &out20, const VecCostUniformebool &VecCUB);
QDataStream &operator>>(QDataStream &in20, VecCostUniformebool &VecCUB);

class VecCostUniforme
{
public:
    VecCostUniforme(bool uniforme, bool diverso,
                    QVector<double> &TS, QVector<double> &TE,
                    QVector<double> &Wcp, QVector<double> &h ,
                    QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                    QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost,
                    double Min, double Max, double Inc, int CTo, int CCo);
    bool getUniforme() const;
    bool getDiverso() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    int getCTo() const;
    int getCCo() const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    QVector<QVector<double>> getCapitalCost() const;
    QVector<double> getOperationCost() const;
    friend QDataStream &operator<<(QDataStream &out21, const VecCostUniforme &VCU);
    friend QDataStream &operator>>(QDataStream &in21, VecCostUniforme &VCU);
private:
    bool Muniforme,Mdiverso;
    double MMin, MMax, MInc, MCTo, MCCo;
    QVector<double> MTS, MTE, MWCP, Mh , MCalentamiento;
    QVector<double> MEnfriamento,MOperationCost;
    QVector<QVector<double>> MCapitalCost; 
};
QDataStream &operator<<(QDataStream &out21, const VecCostUniforme &VCU);
QDataStream &operator>>(QDataStream &in21, VecCostUniforme &VCU);

class VecCostDiverso
{
public:
    VecCostDiverso(bool uniforme, bool diverso,
                   QVector<double> &TS, QVector<double> &TE,
                   QVector<double> &Wcp,QVector<double> &h ,
                   QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                   QVector<QVector<double>> &CapitalCost, QVector<double> &OperationCost,
                   double Min,double Max, double Inc, double K);
    bool getUniforme() const;
    bool getDiverso() const;
    double getMin() const;
    double getMax() const;
    double getInc() const;
    double getK()const;
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> geth() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    QVector<QVector<double>> getCapitalCost() const;
    QVector<double> getOperationCost() const;
    friend QDataStream &operator<<(QDataStream &out22, const VecCostDiverso &VCD);
    friend QDataStream &operator>>(QDataStream &in22, VecCostDiverso &VCD);
private:
    bool Muniforme,Mdiverso;
    double MMin, MMax, MInc,MK;
    QVector<double> MTS, MTE, MWCP, Mh , MCalentamiento;
    QVector<double> MEnfriamento,MOperationCost;
    QVector<QVector<double>> MCapitalCost;
};
QDataStream &operator<<(QDataStream &out22, const VecCostDiverso &VCD);
QDataStream &operator>>(QDataStream &in22, VecCostDiverso &VCD);

class UnidadesSerAux
{
public:
    UnidadesSerAux(bool SI = false, bool SIS = false, int UTemp = -1);
    bool getSI();
    bool getSIS();
    int getUTemp();
    void ConvertirUnidades(QVector<double> Calentamiento, QVector<double> Enfriamento, bool SI,
                           bool SIN, int A);
    QVector<double> getCalentamiento();
    QVector<double> getEnfriamento();
    //VOID CONVERTIR UNIDAES
    friend QDataStream &operator<<(QDataStream &out23, const UnidadesSerAux &units);
    friend QDataStream &operator>>(QDataStream &in23, UnidadesSerAux &units);

private:
    QVector<double> MCalentamiento,MEnfriamento;
    bool MSI,MSIS;
    int  MA, MUTemp;
};
QDataStream &operator<<(QDataStream &out23, const UnidadesSerAux &units);
QDataStream &operator>>(QDataStream &in23, UnidadesSerAux &units);

class CostosCapitalCost{
public:
    CostosCapitalCost(int CapCost);
    int getCapCost();
    friend QDataStream &operator<<(QDataStream &out24, const CostosCapitalCost &units);
    friend QDataStream &operator>>(QDataStream &in24, CostosCapitalCost &units);
private:
    int MCapCost;
};
QDataStream &operator<<(QDataStream &out24, const CostosCapitalCost &units);
QDataStream &operator>>(QDataStream &in24, CostosCapitalCost &units);

class infoapuniforme{
public:
    infoapuniforme(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS
                   , double Min, double Max, double Inc, QVector<double> &TS, QVector<double> &TE,
                   QVector<double> &WCP, QVector<double> &H, QVector<double> &Enfriamento, QVector<double> &Calentamiento,
                   int CTo, int CCo,
                   QVector<QVector<double>> &CapitalCost, QVector<double> &OperationCost);
    double getMin();
    double getMax();
    double getInc();
    int getCTo();
    int getCCo();
    bool getestatico();
    bool getincremento();
    bool getSI();
    bool getSIS();
    QVector<double> getCheckboxes();
    QVector<double> getTS();
    QVector<double> getTE();
    QVector<double> getWCP();
    QVector<double> getH();
    QVector<double> getEnfriamento();
    QVector<double> getCalentamiento();
    QVector<double> getOperationCost();
    QVector<QVector<double>> getCapitalCost();
    friend QDataStream &operator<<(QDataStream &out25, const infoapuniforme &checkboxes);
    friend QDataStream &operator>>(QDataStream &in25, infoapuniforme &checkboxes);
private:
    QVector<double> MCheckboxes, MTS, MTE,MWCP,MH,MEnfriamento,MOperationCost,MCalentamiento;
    QVector<QVector<double>> MCapitalCost;
    double MMin,MMax,MInc;
    int MCTo,MCCo;
    bool Mestatico,Mincremento,MSI,MSIS;
};
QDataStream &operator<<(QDataStream &out25, const infoapuniforme &checkboxes);
QDataStream &operator>>(QDataStream &in25, infoapuniforme &checkboxes);

class TabAnalisis
{
public:
    TabAnalisis(int TabAnalisis = -1, int Ventanamadre = -1);
    int getTabAnalisis();
    int getVentanamadre();
    friend QDataStream &operator<<(QDataStream &out26, const TabAnalisis &TA);
    friend QDataStream &operator>>(QDataStream &in26, TabAnalisis &TA);
private:
    int MTabAnalisis, MVentanamadre;
};
QDataStream &operator<<(QDataStream &out26, const TabAnalisis &TA);
QDataStream &operator>>(QDataStream &in26, TabAnalisis &TA);

class infoapdiversa{
public:
    infoapdiversa(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS
                   ,double K, double Min, double Max, double Inc, QVector<double> &TS, QVector<double> &TE,
                   QVector<double> &WCP, QVector<double> &H, QVector<double> &Enfriamento, QVector<double> &Calentamiento,
                   int CTo, int CCo,
                   QVector<QVector<double>> &CapitalCost, QVector<double> &OperationCost);
    double getMin();
    double getMax();
    double getInc();
    double getK();
    int getCTo();
    int getCCo();
    bool getestatico();
    bool getincremento();
    bool getSI();
    bool getSIS();
    QVector<double> getCheckboxes();
    QVector<double> getTS();
    QVector<double> getTE();
    QVector<double> getWCP();
    QVector<double> getH();
    QVector<double> getEnfriamento();
    QVector<double> getCalentamiento();
    QVector<double> getOperationCost();
    QVector<QVector<double>> getCapitalCost();
    friend QDataStream &operator<<(QDataStream &out27, const infoapdiversa &apdiversa);
    friend QDataStream &operator>>(QDataStream &in27, infoapdiversa &apdiversa);
private:
    QVector<double> MCheckboxes, MTS, MTE,MWCP,MH,MEnfriamento,MOperationCost,MCalentamiento;
    QVector<QVector<double>> MCapitalCost;
    double MMin,MMax,MInc,MK;
    int MCTo,MCCo;
    bool Mestatico,Mincremento,MSI,MSIS;
};
QDataStream &operator<<(QDataStream &out27, const infoapdiversa &apdiversa);
QDataStream &operator>>(QDataStream &in27, infoapdiversa &apdiversa);

class infoapboth{
public:
    infoapboth(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS
                   ,double K, double Min, double Max, double Inc, QVector<double> &TS, QVector<double> &TE,
                   QVector<double> &WCP, QVector<double> &H, QVector<double> &Enfriamento, QVector<double> &Calentamiento,
                   int CTo, int CCo,
                   QVector<QVector<double>> &CapitalCost, QVector<double> &OperationCost);
    double getMin();
    double getMax();
    double getInc();
    double getK();
    int getCTo();
    int getCCo();
    bool getestatico();
    bool getincremento();
    bool getSI();
    bool getSIS();
    QVector<double> getCheckboxes();
    QVector<double> getTS();
    QVector<double> getTE();
    QVector<double> getWCP();
    QVector<double> getH();
    QVector<double> getEnfriamento();
    QVector<double> getCalentamiento();
    QVector<double> getOperationCost();
    QVector<QVector<double>> getCapitalCost();
    friend QDataStream &operator<<(QDataStream &out28, const infoapboth &apboth);
    friend QDataStream &operator>>(QDataStream &in28, infoapboth &apboth);
private:
    QVector<double> MCheckboxes, MTS, MTE,MWCP,MH,MEnfriamento,MOperationCost,MCalentamiento;
    QVector<QVector<double>> MCapitalCost;
    double MMin,MMax,MInc,MK;
    int MCTo,MCCo;
    bool Mestatico,Mincremento,MSI,MSIS;
};
QDataStream &operator<<(QDataStream &out28, const infoapboth &apboth);
QDataStream &operator>>(QDataStream &in28, infoapboth &apboth);

class savehelp{
public:
    savehelp(int Help);
    int gethelp();
    friend QDataStream &operator<<(QDataStream &out29, const savehelp &savehel);
    friend QDataStream &operator>>(QDataStream &in29, savehelp &savehel);
private:
    int MHelp;
};
QDataStream &operator<<(QDataStream &out29, const savehelp &savehel);
QDataStream &operator>>(QDataStream &in29, savehelp &savehel);

class VecCostUniDesGri
{
public:
    VecCostUniDesGri(bool uniforme, bool diverso,
                     QVector<double> &TS, QVector<double> &TE, QVector<double> &WCP,
                     QVector<double> &H,
                     QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                     QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost,
                    double DTmin, int CTo, int CCo);
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> getH() const;
    bool getUniforme() const;
    bool getDiverso() const;
    double getDTmin() const;
    int getCTo() const;
    int getCCo() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    QVector<QVector<double>> getCapitalCost() const;
    QVector<double> getOperationCost() const;
    friend QDataStream &operator<<(QDataStream &out30, const VecCostUniDesGri &VCUD);
    friend QDataStream &operator>>(QDataStream &in30, VecCostUniDesGri &VCUD);
private:
    bool Muniforme,Mdiverso;
    double MDTmin, MCTo, MCCo;
    QVector<double> MCalentamiento,MTS,MTE,MWCP,MH;
    QVector<double> MEnfriamento,MOperationCost;
    QVector<QVector<double>> MCapitalCost;
};
QDataStream &operator<<(QDataStream &out30, const VecCostUniDesGri &VCUD);
QDataStream &operator>>(QDataStream &in30, VecCostUniDesGri &VCUD);


class VecCostDivDesGri
{
public:
    VecCostDivDesGri(bool uniforme, bool diverso,
                     QVector<double> TS, QVector<double> TE, QVector<double> WCP,
                     QVector<double> H,
                     QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                     QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost,
                    double DTmin, double k);
    QVector<double> getTS() const;
    QVector<double> getTE() const;
    QVector<double> getWCP() const;
    QVector<double> getH() const;
    bool getUniforme() const;
    bool getDiverso() const;
    double getDTmin() const;
    double getk() const;
    QVector<double> getCalentamiento() const;
    QVector<double> getEnfriamento() const;
    QVector<QVector<double>> getCapitalCost() const;
    QVector<double> getOperationCost() const;
    friend QDataStream &operator<<(QDataStream &out31, const VecCostDivDesGri &VCDD);
    friend QDataStream &operator>>(QDataStream &in31, VecCostDivDesGri &VCDD);
private:
    bool Muniforme,Mdiverso;
    double MDTmin, Mk;
    QVector<double> MCalentamiento,MTS,MTE,MWCP,MH;
    QVector<double> MEnfriamento,MOperationCost;
    QVector<QVector<double>> MCapitalCost;
};
QDataStream &operator<<(QDataStream &out31, const VecCostDivDesGri &VCDD);
QDataStream &operator>>(QDataStream &in31, VecCostDivDesGri &VCDD);

class VecDesigBool
{
public:
    VecDesigBool(bool uniforme,bool diverso);
    bool getUniforme() const;
    bool getDiverso() const;
    friend QDataStream &operator<<(QDataStream &out32, const VecDesigBool &VecDB);
    friend QDataStream &operator>>(QDataStream &in32,VecDesigBool &VecDB);
private:
    bool Muniforme,Mdiverso;

};
QDataStream &operator<<(QDataStream &out32, const VecDesigBool &VecDB);
QDataStream &operator>>(QDataStream &in32, VecDesigBool &VecDB);

class Grid{
public:
    Grid(QVector<QVector<double>> Uniones, QVector<QVector<double>> Servicios,
         QVector<double> EnergiaServicios, QVector<double> EnergiaUniones);
    QVector<QVector<double>> getUniones();
    QVector<QVector<double>> getServicios();
    QVector<double> getEnergiaServicios();
    QVector<double> getEnergiaUniones();
    friend QDataStream &operator<<(QDataStream &out33, const Grid &VecGrid);
    friend QDataStream &operator>>(QDataStream &in33,Grid &VecGrid);
private:
    QVector<QVector<double>> MUniones,MServicios;
    QVector<double> MEnergiaServicios,MEnergiaUniones;
};
QDataStream &operator<<(QDataStream &out33, const Grid &VecGrid);
QDataStream &operator>>(QDataStream &in33, Grid &VecGrid);


#endif // DUVALORACTION_H
