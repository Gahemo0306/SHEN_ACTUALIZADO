#ifndef GRAFICOS_H
#define GRAFICOS_H
#include <iostream>
#include <QVector>
#include <QDebug>
#include <QtMath>
#include <QtAlgorithms>
#include <algorithm>
#include <iterator>
#include <vector>


class Graficos
{
public:
    Graficos();
};

class Plot_curvascompuestas
{
public:
    Plot_curvascompuestas(QVector<double> Tsupply, QVector<double> Ttarget,
                          QVector<double> Cp);
    QVector<double> getCCENTALPIA();
    QVector<double> getCCTEMPERATURAS();
    QVector<double> getCFENTALPIA();
    QVector<double> getCFTEMPERATURAS();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIA,MCFTEMPERATURAS;
};

class Plot_curvascompuestasajustadas
{
public:
    Plot_curvascompuestasajustadas(QVector<double> Tsupply, QVector<double> Ttarget,
                                   QVector<double> Cp,double DTmin);
    QVector<double> getCCENTALPIA();
    QVector<double> getCCTEMPERATURAS();
    QVector<double> getCFENTALPIAAJUSTADAS();
    QVector<double> getCFTEMPERATURASAJUSTADAS();
    QVector<QVector<double>> getVectorCorrientesTotal();
    QVector<QVector<double>> getVecHeatFlow();
    QVector<QVector<double>> getVecAdjHeatFlow();
    QVector<double> getDeficit();
    QVector<QVector<double>> getVectorCalientesMATRIZ();
    QVector<QVector<double>> getVectorFriasMATRIZ();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIAAJUSTADA,MCFTEMPERATURASAJUSTADA;
    QVector<QVector<double>> MVectorCorrientesTotal ,MVectorCalientesMATRIZ,MVectorFriasMATRIZ;
    QVector<QVector<double>> MVecHeatFlow,MVecAdjHeatFlow;
    QVector<double> MDeficit;
};

class Plot_grancurvacompuesta
{
public:
    Plot_grancurvacompuesta(QVector<double> Tsupply, QVector<double> Ttarget,
                            QVector<double> Cp, double DTmin);
    QVector<double> getGCTEMPERATURAS();
    QVector<double> getGCENTALPIA();
private:
    QVector<double> MGCTEMPERATURAS,MGCENTALPIA;
};

class Plot_Dtmin_vs_Areas
{
public:
    Plot_Dtmin_vs_Areas(QVector<double> Tsupply, QVector<double> Ttarget,
                        QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento,
                        QVector<double> Enfriamento,double DTmin,int CTo,int CCo);
    double getAREAS();
    QVector<double> getDTmed();
    QVector<double> getDelta2();
    QVector<double> getDelta1();
    QVector<double> getVectorAreas();
    QVector<double> getVectorEntalpia();
    QVector<QVector<double>> getINTERVALOS_AGRUPADOS();
private:
    double MAREAS;
    QVector<double> MDTmed,MDelta2,MDelta1,MVectorAreas,MVectorEntalpia;
    QVector<QVector<double>> MINTERVALOS_AGRUPADOS;
};

class Plot_curvascompuestas_diversa
{
public:
    Plot_curvascompuestas_diversa(QVector<double> Tsupply, QVector<double> Ttarget,
                          QVector<double> Cp, QVector<double> h, double DTmin, double K, float punto1, float punto2);
    QVector<double> getCCENTALPIA();
    QVector<double> getCCTEMPERATURAS();
    QVector<double> getCFENTALPIA();
    QVector<double> getCFTEMPERATURAS();
    double getK();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIA,MCFTEMPERATURAS;
    double MK;
};

class Plot_CCAJUSTADA_DIVERSA
{
public:
    Plot_CCAJUSTADA_DIVERSA(QVector<double> Tsupply, QVector<double> Ttarget,
                            QVector<double> Cp, QVector<double> h, double DTmin, double K,
                            float punto1,float punto2);
    QVector<double> getCCENTALPIA();
    QVector<double> getCCTEMPERATURAS();
    QVector<double> getCFENTALPIAAJUSTADAS();
    QVector<double> getCFTEMPERATURASAJUSTADAS();
    QVector<QVector<double>> getVectorCorrientesTotal();
    QVector<QVector<double>> getVecHeatFlow();
    QVector<QVector<double>> getVecAdjHeatFlow();
    QVector<double> getDeficit();
    double getK();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIAAJUSTADA,MCFTEMPERATURASAJUSTADA;
    QVector<QVector<double>> MVectorCorrientesTotal;
    QVector<QVector<double>> MVecHeatFlow,MVecAdjHeatFlow;
    QVector<double> MDeficit;
    double MK;
};

class PlotGCC_DIVERSA
{
public:
    PlotGCC_DIVERSA(QVector<double> Tsupply, QVector<double> Ttarget,
                    QVector<double> Cp, QVector<double> h , double DTmin,
                    double K,float punto1,float punto2);
    QVector<double> getGCTEMPERATURAS();
    QVector<double> getGCENTALPIA();
    double getK();
private:
    QVector<double> MGCTEMPERATURAS,MGCENTALPIA;
    double MK;
};

class Plot_Dtmin_vs_Areas_DIVERSO
{
public:
    Plot_Dtmin_vs_Areas_DIVERSO(QVector<double> Tsupply, QVector<double> Ttarget,
                        QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento,
                        QVector<double> Enfriamento, double K, double DTmin, float punto1, float punto2);
    double getAREAS();
    double getK();
    QVector<double> getVectorAreas();
    QVector<double> getVectorEntalpia();
    QVector<double> getDeltaTLM();
    QVector<QVector<double>> getINTERVALOS_AGRUPADOS();
private:
    QVector<double> MVectorAreas,MVectorEntalpia,MDeltaTLM;
    QVector<QVector<double>> MINTERVALOS_AGRUPADOS;
    double MAREAS;
    double MK;
};

class FindKvalue
{
public:
    FindKvalue(QVector<double> Tsupply, QVector<double> Ttarget,
               QVector<double> Cp, QVector<double> h , double K, double DTmin);
    double getK();
    float getUC();
    float getUF();
private:
    double MUTILIDADCALENTAMIENTO, MUTILIDADFRIA;
};

class Plot_Costos_vs_Areas_Uniforme
{
public:
    Plot_Costos_vs_Areas_Uniforme(QVector<double> Tsupply, QVector<double> Ttarget,
                                  QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento,
                                  QVector<double> Enfriamento, QVector<QVector<double>> CapitalCost,
                                  QVector<double> OperationCost, double DTmin, int CTo, int CCo, bool SI, bool SIS);
    double getCostoTotal();
    double getCostoOperacionTotal();
    double getCostoCapitalTotal();
    double getOpeCosC();
    double getOpeCosH();
    double getCapCos3();
    double getCapCos2();
    double getCapCos1();
    double getAH();
    double getAR();
    double getAC();
    double getQH();
    double getQR();
    double getQC();
private:
    double MCostoTotal, MCostoOperacionTotal, MCostoCapitalTotal;
    double MOpeCosC, MOpeCosH , MCapCos3 , MCapCos2 , MCapCos1, MAH, MQH, MAR, MQR, MAC, MQC;
};

class Plot_Costos_vs_Min_Divera
{
public:
    Plot_Costos_vs_Min_Divera(QVector<double> Tsupply, QVector<double> Ttarget,
                              QVector<double> Cp, QVector<double> h, QVector<double> Calentamiento,
                              QVector<double> Enfriamento, QVector<QVector<double>> CapitalCost,
                              QVector<double> OperationCost,double K, double DTmin, float punto1,
                              float punto2, bool SI, bool SIS);
    double getCostoTotal();
    double getCostoOperacionTotal();
    double getCostoCapitalTotal();
    double getK();
    double getOpeCosC();
    double getOpeCosH();
    double getCapCos3();
    double getCapCos2();
    double getCapCos1();
    double getAH();
    double getAR();
    double getAC();
    double getQH();
    double getQR();
    double getQC();
private:
    double MK;
    double MCostoTotal, MCostoOperacionTotal, MCostoCapitalTotal;
    double MOpeCosC, MOpeCosH , MCapCos3 , MCapCos2 , MCapCos1, MAH, MQH, MAR, MQR, MAC, MQC;
};

class Grid_Areaa_Uniforme
{
public:
    Grid_Areaa_Uniforme(QVector<double> Tsupply,QVector<double> Ttarget, QVector<double> Cp,
                        QVector<double> h, QVector<double> Calentamiento,
                        QVector<double> Enfriamento,QVector<QVector<double>> Uniones,
                        QVector<QVector<double>> Servicios, int CTo, int CCo);
//    QVector<QVector<double>> CapitalCost,
//    QVector<double> OperationCost, double DTmin , bool SI, bool SIS
    double getAREAS();
    QVector<double> getVectorAreas();
    QVector<double> getVectorEntalpia();
    QVector<double> getDTmed();
    QVector<double> getDT1();
    QVector<double> getDT2();
    QVector<QVector<double>> getINTERVALOS_AGRUPADOS();
private:
    QVector<QVector<double>> MINTERVALOS_AGRUPADOS;
    QVector<double> MDTmed,MDelta1,MDelta2,MVectorAreas,MVectorEntalpia;
    double MAREAS;
};

class Grid_Areas_Diversa
{
public:
    Grid_Areas_Diversa(QVector<double> Tsupply,QVector<double> Ttarget, QVector<double> Cp,
                       QVector<double> h, QVector<double> Calentamiento,
                       QVector<double> Enfriamento,QVector<QVector<double>> Uniones,
                       QVector<QVector<double>> Servicios, double K, double DTmin, float punto1, float punto2);
    double getAREAS();
    double getK();
    QVector<double> getVectorAreas();
    QVector<double> getVectorEntalpia();
    QVector<double> getDeltaTLM();
    QVector<QVector<double>> getINTERVALOS_AGRUPADOS();
private:
    QVector<double> MVectorAreas,MVectorEntalpia,MDeltaTLM;
    QVector<QVector<double>> MINTERVALOS_AGRUPADOS;
    double MAREAS;
    double MK;
};

class Grid_Costos_Uniforme
{
public:
    Grid_Costos_Uniforme(QVector<double> Tsupply,QVector<double> Ttarget, QVector<double> Cp,
                         QVector<double> h, QVector<double> Calentamiento,
                         QVector<QVector<double>> CapitalCost,
                         QVector<double> OperationCost,
                         QVector<double> Enfriamento,QVector<QVector<double>> Uniones,
                         QVector<QVector<double>> Servicios, int CTo, int CCo,bool SI,bool SIS);
    double getCostoTotal();
    double getCostoOperacionTotal();
    double getCostoCapitalTotal();
    double getOpeCosC();
    double getOpeCosH();
    double getCapCos3();
    double getCapCos2();
    double getCapCos1();
    double getAH();
    double getAR();
    double getAC();
    double getQH();
    double getQR();
    double getQC();
private:
    double MCostoTotal, MCostoOperacionTotal, MCostoCapitalTotal;
    double MOpeCosC, MOpeCosH , MCapCos3 , MCapCos2 , MCapCos1, MAH, MQH, MAR, MQR, MAC, MQC;
};

class Grid_Costos_Diversa
{
public:
    Grid_Costos_Diversa(QVector<double> Tsupply,QVector<double> Ttarget, QVector<double> Cp,
                        QVector<double> h, QVector<double> Calentamiento,
                        QVector<QVector<double>> CapitalCost,
                        QVector<double> OperationCost,
                        QVector<double> Enfriamento,QVector<QVector<double>> Uniones,
                        QVector<QVector<double>> Servicios,bool SI,bool SIS,
                        double K, double DTmin, float punto1, float punto2);
    double getCostoTotal();
    double getCostoOperacionTotal();
    double getCostoCapitalTotal();
    double getOpeCosC();
    double getOpeCosH();
    double getCapCos3();
    double getCapCos2();
    double getCapCos1();
    double getAH();
    double getAR();
    double getAC();
    double getQH();
    double getQR();
    double getQC();
private:
    double MCostoTotal, MCostoOperacionTotal, MCostoCapitalTotal;
    double MOpeCosC, MOpeCosH , MCapCos3 , MCapCos2 , MCapCos1, MAH, MQH, MAR, MQR, MAC, MQC;
};

class Pre_Grid_UNIFORME
{
public:
    Pre_Grid_UNIFORME(QVector<double> Tsupply, QVector<double> Ttarget, QVector<double> Cp,
                      QVector<double> h, double DTmin);
    QVector<double> getCCENTALPIA();
    QVector<double> getCCTEMPERATURAS();
    QVector<double> getCFENTALPIAAJUSTADAS();
    QVector<double> getCFTEMPERATURASAJUSTADAS();
    QVector<QVector<double>> getVectorCorrientesTotal();
    QVector<QVector<double>> getVecHeatFlow();
    QVector<QVector<double>> getVecAdjHeatFlow();
    QVector<double> getDeficit();
    QVector<QVector<double>> getVectorCalientesMATRIZ();
    QVector<QVector<double>> getVectorFriasMATRIZ();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIAAJUSTADA,MCFTEMPERATURASAJUSTADA;
    QVector<QVector<double>> MVectorCorrientesTotal ,MVectorCalientesMATRIZ,MVectorFriasMATRIZ;
    QVector<QVector<double>> MVecHeatFlow,MVecAdjHeatFlow;
    QVector<double> MDeficit;
};

class Pre_Grid_DIVERSO
{
public:
    Pre_Grid_DIVERSO(QVector<double> Tsupply, QVector<double> Ttarget,
                     QVector<double> Cp, QVector<double> h, double DTmin, double K,
                     float punto1,float punto2);
    QVector<QVector<double>> getVectorCorrientesTotal();
    QVector<QVector<double>> getVecAdjHeatFlow();
    double getK();
    QVector<QVector<double>> getVectorCalientesMATRIZ();
    QVector<QVector<double>> getVectorFriasMATRIZ();
private:
    QVector<double> MCCENTALPIA,MCCTEMPERATURAS;
    QVector<double> MCFENTALPIAAJUSTADA,MCFTEMPERATURASAJUSTADA;
    QVector<QVector<double>> MVectorCorrientesTotal,MVectorCalientesMATRIZ,MVectorFriasMATRIZ;
    QVector<QVector<double>> MVecHeatFlow,MVecAdjHeatFlow;
    QVector<double> MDeficit;
    double MK;
};


#endif // GRAFICOS_H
