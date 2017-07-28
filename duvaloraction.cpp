#include "duvaloraction.h"
#include <QVector>
#include <qalgorithms.h>
#include <iostream>
#include <qalgorithms.h>
#include <QtMath>
#include <QDebug>
#include <QtAlgorithms>

Duvaloraction::Duvaloraction(int valact)
{
    Mvalact = valact;
}

int Duvaloraction::getvalact() const //GETTER
{
    return Mvalact;
}

void Duvaloraction::setvalact(int valact) //SETTER
{
    Mvalact = valact;
}

Workspace::Workspace(QVector<QVector<double>>&matriz)
{
//    qDebug() << "Guardando";
    int filas = matriz.size();
    int columnas = matriz.at(0).size();
    MMatriz.resize(filas);
    for(int i = 0; i < filas; i++)
    {
        MMatriz[i].resize(columnas);
    }
    for(int i = 0; i < matriz.size(); i++){
        for(int j = 0 ; j < matriz.at(i).size() ;j++){
        MMatriz[i][j] = matriz[i][j];
//        qDebug() << matriz[i][j];
        }
    }
//    qDebug() << "Guardado";
}

QVector<QVector<double> > Workspace::getMatriz() const
{
    return MMatriz;
}

Valordeoperacion::Valordeoperacion(int valor)
{
    Mvalor =  valor;
}

void Valordeoperacion::setvalor(int valor)
{
    Mvalor = valor;
}

int Valordeoperacion::getvalor() const
{
    return Mvalor;
}

Unidades::Unidades(bool SI, bool SIS, int UTemp, int UWcp, int Uh)
{
    MSI = SI;
    MSIS = SIS;
    MUTemp = UTemp;
    MUWcp = UWcp;
    MUh = Uh;
}

bool Unidades::getSI()
{
    return MSI;
}

bool Unidades::getSIS()
{
    return MSIS;
}

int Unidades::getUTemp()
{
    return MUTemp;
}

int Unidades::getUWcp()
{
    return MUWcp;
}

int Unidades::getUh()
{
    return MUh;
}


void Unidades::ConvertirUnidades(QVector<double> ST, QVector<double> TT, QVector<double> Cp, bool SI, bool SIN, int A, int B)
{
    MST.resize(ST.size());
    MTT.resize(TT.size());
    MCp.resize(Cp.size());
    if(SI == true){
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �F
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Farenheit
            for (int i = 0; i < ST.size(); i++){
                MST[i] = ST[i] * 1.8 - 459.67;
                MTT[i] = TT[i] * 1.8 - 459.67;
            }
        } else if (A == 2) {
            // Conversi�n de Farenheit a Farenheit
            for (int i = 0; i < ST.size(); i++){
                MST[i] = ST[i];
                MTT[i] = TT[i];
            }
        } else if (A == 3) {
            // Conversi�n de Celsius a Farenheit
            for (int i =0; i < ST.size(); i++){
                MST[i] = ST[i] * 1.8 + 32.0;
                MTT[i] = TT[i] * 1.8 + 32.0;
            }
        } else if (A == 4) {
            // Conversi�n de Ranking a Farenheit
            for (int i =0; i < ST.size(); i++){
                MST[i] = ST[i] - 459.67;
                MTT[i] = TT[i] - 459.67;
            }
        }
        //  CONVERCI�N DE WCP
        if (B == 1) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i];
            }
        } else if (B == 2) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 60.0;
            }
        } else if (B == 3) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 3600.0;
            }
        } else if (B == 4) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 0.55555555556;
            }
        } else if (B == 5) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 0.55555555556 * 60.0;
            }
        } else if (B == 6.) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 0.55555555556 * 3600.0;
            }
        } else if (B == 7) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i];
            }
        } else if (B == 8) {
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 60.0;
            }
        } else if (B == 9){
            for (int i =0; i < Cp.size(); i++){
                MCp[i] = Cp[i] * 3600.0;
            }
        }
    } else {
        if (SIN == 1) {
            // SISTEMA INTERNACIONAL
            //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �C
            //  WCP A UNIDADES DE BTU/HR�F
            //  H SIGUE CONSTANTE BTU/HR FT^2 �F
            // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
            if (A == 1) {
                // Conversi�n de Kelvin a Celsius
                for (int i =0; i < ST.size(); i++){
                    MST[i] = ST[i] + 273.15;
                    MTT[i] = TT[i] + 273.15;
                }
            } else if (A == 2) {
                // Conversi�n de Farenheit a Celsius
                for (int i =0; i < ST.size(); i++){
                    MST[i] = ST[i] * 1.8 + 32.0;
                    MTT[i] = TT[i] * 1.8 + 32.0;
                }
            } else if (A == 3) {
                // Conversi�n de Celsius a Celsius
                for (int i =0; i < ST.size(); i++){
                    MST[i] = ST[i];
                    MTT[i] = TT[i];
                }
            } else if (A == 4){
                // Conversi�n de Ranking a Celsius
                for (int i =0; i < ST.size(); i++){
                    MST[i] = (MST[i] + 273.15) * 1.8;
                    MTT[i] = (MTT[i] + 273.15) * 1.8;
                }
            }
            //  CONVERCI�N DE WCP
            if (B == 1) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] / 3600.0;
                }
            } else if (B == 2) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] / 60.0;
                }
            } else if (B == 3) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i];
                }
            } else if (B == 4) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] / 3600.0;
                }
            } else if (B == 5) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] / 60.0;
                }
            } else if (B == 6) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i];
                }
            } else if (B == 7) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 1000.0 / 3600.0;
                }
            } else if (B == 8) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 1000.0 / 60.0;
                }
            } else if (B == 9) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 1000.0;
                }
            } else if (B == 10) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 1000.0 / 3600.0;
                }
            } else if (B == 11) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 1000.0 / 60.0;
                }
            } else if (B == 12) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i]* 1000.0;
                }
            } else if (B == 13) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 4186.8 / 3600.0;
                }
            } else if (B == 14) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i] * 4186.8 / 60.0;
                }
            } else if (B == 15) {
                for (int i =0; i < Cp.size(); i++){
                    MCp[i] = Cp[i]* 4186.8;
                }
            }
        }
    }
}

double Unidades::ConvertirDTmin(double DTmin, bool SI, bool SIN, int A)
{
    if(SI == true){
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �F
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Farenheit
            DTmin = DTmin * 1.8 - 459.67;;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Farenheit
            DTmin = DTmin;
        } else if (A == 3) {
            // Conversi�n de Celsius a Farenheit
            DTmin = DTmin * 1.8 + 32.0;
        } else if (A == 4) {
            // Conversi�n de Ranking a Farenheit
            DTmin = DTmin - 459.67;
        }
    } else if (SIN == 1) {
        // SISTEMA INTERNACIONAL
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �C
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Celsius
            DTmin = DTmin + 273.15;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Celsius
            DTmin = DTmin * 1.8 + 32.0;
        } else if (A == 3) {
            // Conversi�n de Celsius a Celsius
            DTmin = DTmin;
        } else if (A == 4){
            // Conversi�n de Ranking a Celsius
            DTmin = (DTmin + 273.15) * 1.8;
        }
    }
    return DTmin;
}

double Unidades::ConvertirMax(double Max, bool SI, bool SIN, int A)
{
    if(SI == true){
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �F
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Farenheit
            Max = Max * 1.8 - 459.67;;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Farenheit
            Max = Max;
        } else if (A == 3) {
            // Conversi�n de Celsius a Farenheit
            Max = Max * 1.8 + 32.0;
        } else if (A == 4) {
            // Conversi�n de Ranking a Farenheit
            Max = Max - 459.67;
        }
    } else if (SIN == 1) {
        // SISTEMA INTERNACIONAL
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �C
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Celsius
            Max = Max + 273.15;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Celsius
            Max = Max * 1.8 + 32.0;
        } else if (A == 3) {
            // Conversi�n de Celsius a Celsius
            Max = Max;
        } else if (A == 4){
            // Conversi�n de Ranking a Celsius
            Max = (Max + 273.15) * 1.8;
        }
    }
    return Max;
}

double Unidades::ConvertirInc(double Inc, bool SI, bool SIN, int A)
{
    if(SI == true){
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �F
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Farenheit
            Inc = Inc * 1.8 - 459.67;;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Farenheit
            Inc = Inc;
        } else if (A == 3) {
            // Conversi�n de Celsius a Farenheit
            Inc = Inc * 1.8 + 32.0;
        } else if (A == 4) {
            // Conversi�n de Ranking a Farenheit
            Inc = Inc - 459.67;
        }
    } else if (SIN == 1) {
        // SISTEMA INTERNACIONAL
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �C
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Celsius
            Inc = Inc + 273.15;
        } else if (A == 2) {
            // Conversi�n de Farenheit a Celsius
            Inc = Inc * 1.8 + 32.0;
        } else if (A == 3) {
            // Conversi�n de Celsius a Celsius
            Inc = Inc;
        } else if (A == 4){
            // Conversi�n de Ranking a Celsius
            Inc = (Inc + 273.15) * 1.8;
        }
    }
    return Inc;
}

QVector<double> Unidades::getST()
{
    return MST;
}

QVector<double> Unidades::getTT()
{
    return MTT;
}

QVector<double> Unidades::getCp()
{
    return MCp;
}

Tabplot::Tabplot(int tabvalue)
{
    Mtabvalue = tabvalue;
}

void Tabplot::settabvalue(int tabvalue)
{
    Mtabvalue = tabvalue;
}

int Tabplot::gettabvalue()
{
    return Mtabvalue;
}


QDataStream &operator<<(QDataStream &out1, const Duvaloraction &valoraction)
{
    return out1 << valoraction.Mvalact;
}

QDataStream &operator>>(QDataStream &in1, Duvaloraction &valoraction)
{
    return in1 >> valoraction.Mvalact;
}

QDataStream &operator<<(QDataStream &out2, const Workspace &MATRIZ)
{
//        qDebug() << "out";
    return out2 << MATRIZ.MMatriz;
}

QDataStream &operator>>(QDataStream &in2, Workspace &MATRIZ)
{
//        qDebug() << "in";
    return in2  >> MATRIZ.MMatriz;
}


QDataStream &operator<<(QDataStream &out3, const Unidades &units)
{
    return out3 << units.MSI << units.MSIS << units.MUTemp << units.MUWcp << units.MUh;
}

QDataStream &operator>>(QDataStream &in3, Unidades &units)
{
    return in3 >> units.MSI >> units.MSIS >> units.MUTemp >> units.MUWcp >> units.MUh;
}

QDataStream &operator<<(QDataStream &out4, const Tabplot &tabvalue)
{
    return out4 << tabvalue.Mtabvalue;
}

QDataStream &operator>>(QDataStream &in4, Tabplot &tabvalue)
{
    return in4 >> tabvalue.Mtabvalue;
}

QDataStream &operator<<(QDataStream &out5, const Valordeoperacion &valor)
{
    return out5 << valor.Mvalor;
}

QDataStream &operator>>(QDataStream &in5, Valordeoperacion &valor)
{
    return in5 >> valor.Mvalor;
}


VecPlot_CurvasCompuestas::VecPlot_CurvasCompuestas(bool uniforme, bool diverso, bool estatico, bool incremento,
                                                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                                                   QVector<double> &h,
                                                   double Min, double Max, double Inc, double K)
{
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        Mh[i] = h[i];
    }
    MK = K;
}

bool VecPlot_CurvasCompuestas::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CurvasCompuestas::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CurvasCompuestas::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CurvasCompuestas::getIncremmento() const
{
    return Mincremento;
}

double VecPlot_CurvasCompuestas::getMin() const
{
    return MMin;
}

double VecPlot_CurvasCompuestas::getMax() const
{
    return MMax;
}

double VecPlot_CurvasCompuestas::getInc() const
{
    return MInc;
}

double VecPlot_CurvasCompuestas::getK() const
{
    return MK;
}

QVector<double> VecPlot_CurvasCompuestas::getTS() const
{
    return MTS;
}

QVector<double> VecPlot_CurvasCompuestas::getTE() const
{
    return MTE;
}

QVector<double> VecPlot_CurvasCompuestas::getWCP() const
{
    return MWCP;
}

QVector<double> VecPlot_CurvasCompuestas::geth() const
{
    return Mh;
}

QDataStream &operator<<(QDataStream &out6, const VecPlot_CurvasCompuestas &VecCC)
{
    return out6 << VecCC.Muniforme << VecCC.Mdiverso << VecCC.Mestatico << VecCC.Mincremento
                << VecCC.MTS << VecCC.MTE << VecCC.MWCP << VecCC.Mh << VecCC.MMin << VecCC.MMax << VecCC.MInc << VecCC.MK;
}

QDataStream &operator>>(QDataStream &in6, VecPlot_CurvasCompuestas &VecCC)
{
    return in6 >> VecCC.Muniforme >> VecCC.Mdiverso >> VecCC.Mestatico >> VecCC.Mincremento
               >> VecCC.MTS >> VecCC.MTE >> VecCC.MWCP >>  VecCC.Mh >> VecCC.MMin >> VecCC.MMax >> VecCC.MInc >> VecCC.MK;
}

VecPlot_CurvasCompuestasAjustadasEstatico::VecPlot_CurvasCompuestasAjustadasEstatico(bool uniforme, bool diverso, bool estatico, bool incremento,
                                                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                                                   double DTMin)
{
    MDTmin = DTMin;
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
}

bool VecPlot_CurvasCompuestasAjustadasEstatico::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CurvasCompuestasAjustadasEstatico::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CurvasCompuestasAjustadasEstatico::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CurvasCompuestasAjustadasEstatico::getIncremmento() const
{
    return Mincremento;
}

double VecPlot_CurvasCompuestasAjustadasEstatico::getDTmin() const
{
    return MDTmin;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasEstatico::getTS() const
{
    return MTS;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasEstatico::getTE() const
{
    return MTE;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasEstatico::getWCP() const
{
    return MWCP;
}

QDataStream &operator<<(QDataStream &out7, const VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE)
{
    return out7 << VecCCAE.Muniforme << VecCCAE.Mdiverso << VecCCAE.Mestatico << VecCCAE.Mincremento << VecCCAE.MTS
                << VecCCAE.MTE << VecCCAE.MWCP << VecCCAE.MDTmin ;
}

QDataStream &operator>>(QDataStream &in7, VecPlot_CurvasCompuestasAjustadasEstatico &VecCCAE)
{
    return in7 >> VecCCAE.Muniforme >> VecCCAE.Mdiverso >> VecCCAE.Mestatico >> VecCCAE.Mincremento >> VecCCAE.MTS
               >> VecCCAE.MTE >> VecCCAE.MWCP >> VecCCAE.MDTmin ;
}

VecPlot_CurvasCompuestasAjustadasIncremento::VecPlot_CurvasCompuestasAjustadasIncremento(bool uniforme, bool diverso, bool estatico, bool incremento,
                                                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                                                   double Min, double Max, double Inc)
{
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
}

bool VecPlot_CurvasCompuestasAjustadasIncremento::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CurvasCompuestasAjustadasIncremento::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CurvasCompuestasAjustadasIncremento::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CurvasCompuestasAjustadasIncremento::getIncremmento() const
{
    return Mincremento;
}

double VecPlot_CurvasCompuestasAjustadasIncremento::getMin() const
{
    return MMin;
}

double VecPlot_CurvasCompuestasAjustadasIncremento::getMax() const
{
    return MMax;
}

double VecPlot_CurvasCompuestasAjustadasIncremento::getInc() const
{
    return MInc;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasIncremento::getTS() const
{
    return MTS;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasIncremento::getTE() const
{
    return MTE;
}

QVector<double> VecPlot_CurvasCompuestasAjustadasIncremento::getWCP() const
{
    return MWCP;
}

QDataStream &operator<<(QDataStream &out8, const VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI)
{
    return out8 << VecCCAI.Muniforme << VecCCAI.Mdiverso << VecCCAI.Mestatico << VecCCAI.Mincremento
                << VecCCAI.MTS << VecCCAI.MTE << VecCCAI.MWCP << VecCCAI.MMin << VecCCAI.MMax << VecCCAI.MInc ;
}

QDataStream &operator>>(QDataStream &in8, VecPlot_CurvasCompuestasAjustadasIncremento &VecCCAI)
{
    return in8 >> VecCCAI.Muniforme >> VecCCAI.Mdiverso >> VecCCAI.Mestatico >> VecCCAI.Mincremento
               >> VecCCAI.MTS >> VecCCAI.MTE >> VecCCAI.MWCP >> VecCCAI.MMin >> VecCCAI.MMax >> VecCCAI.MInc ;
}

VecPlot_CurvasCompuestasAjustadasbool::VecPlot_CurvasCompuestasAjustadasbool(bool uniforme, bool diverso,
                                                                             bool estatico, bool incremento)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
}

bool VecPlot_CurvasCompuestasAjustadasbool::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CurvasCompuestasAjustadasbool::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CurvasCompuestasAjustadasbool::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CurvasCompuestasAjustadasbool::getIncremmento() const
{
    return Mincremento;
}


QDataStream &operator<<(QDataStream &out9, const VecPlot_CurvasCompuestasAjustadasbool &VecCCAB)
{
     return out9 << VecCCAB.Muniforme << VecCCAB.Mdiverso << VecCCAB.Mestatico << VecCCAB.Mincremento ;
}

QDataStream &operator>>(QDataStream &in9, VecPlot_CurvasCompuestasAjustadasbool &VecCCAB)
{
    return in9 >> VecCCAB.Muniforme >> VecCCAB.Mdiverso >> VecCCAB.Mestatico >> VecCCAB.Mincremento ;
}



VecGCCbool::VecGCCbool(bool uniforme, bool diverso, bool estatico, bool incremento)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
}

bool VecGCCbool::getUniforme() const
{
    return Muniforme;
}

bool VecGCCbool::getDiverso() const
{
    return Mdiverso;
}

bool VecGCCbool::getEstatico() const
{
    return Mestatico;
}

bool VecGCCbool::getIncremmento() const
{
    return Mincremento;
}

QDataStream &operator<<(QDataStream &out10, const VecGCCbool &VecGCCbools)
{
    return out10 << VecGCCbools.Muniforme << VecGCCbools.Mdiverso << VecGCCbools.Mestatico << VecGCCbools.Mincremento ;
}

QDataStream &operator>>(QDataStream &in10, VecGCCbool &VecGCCbools)
{
    return in10 >> VecGCCbools.Muniforme >> VecGCCbools.Mdiverso >> VecGCCbools.Mestatico >> VecGCCbools.Mincremento ;
}


VecGCCestatico::VecGCCestatico(bool uniforme, bool diverso, bool estatico, bool incremento,
                               QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp, double DTmin)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MDTmin = DTmin;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
}

bool VecGCCestatico::getUniforme() const
{
    return Muniforme;
}

bool VecGCCestatico::getDiverso() const
{
    return Mdiverso;
}

bool VecGCCestatico::getEstatico() const
{
    return Mestatico;
}

bool VecGCCestatico::getIncremmento() const
{
    return Mincremento;
}

double VecGCCestatico::getDTmin() const
{
    return MDTmin;
}

QVector<double> VecGCCestatico::getTS() const
{
    return MTS;
}

QVector<double> VecGCCestatico::getTE() const
{
    return MTE;
}

QVector<double> VecGCCestatico::getWCP() const
{
    return MWCP;
}

QDataStream &operator<<(QDataStream &out11, const VecGCCestatico &VecGCest)
{
    return out11 << VecGCest.Muniforme << VecGCest.Mdiverso << VecGCest.Mestatico << VecGCest.Mincremento
                 << VecGCest.MDTmin << VecGCest.MTS << VecGCest.MTE << VecGCest.MWCP;
}

QDataStream &operator>>(QDataStream &in11, VecGCCestatico &VecGCest)
{
    return in11 >> VecGCest.Muniforme >> VecGCest.Mdiverso >> VecGCest.Mestatico >> VecGCest.Mincremento
                >> VecGCest.MDTmin >> VecGCest.MTS >> VecGCest.MTE >> VecGCest.MWCP;
}


VecGCCdinamico::VecGCCdinamico(bool uniforme, bool diverso, bool estatico, bool incremento,
                               QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                               double Min, double Max, double Inc)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
}

bool VecGCCdinamico::getUniforme() const
{
    return Muniforme;
}

bool VecGCCdinamico::getDiverso() const
{
    return Mdiverso;
}

bool VecGCCdinamico::getEstatico() const
{
    return Mestatico;
}

bool VecGCCdinamico::getIncremmento() const
{
    return Mincremento;
}

double VecGCCdinamico::getMin() const
{
    return MMin;
}

double VecGCCdinamico::getMax() const
{
    return MMax;
}

double VecGCCdinamico::getInc() const
{
    return MInc;
}

QVector<double> VecGCCdinamico::getTS() const
{
    return MTS;
}

QVector<double> VecGCCdinamico::getTE() const
{
    return MTE;
}

QVector<double> VecGCCdinamico::getWCP() const
{
    return MWCP;
}

QDataStream &operator<<(QDataStream &out12, const VecGCCdinamico &VecGCdin)
{
    return out12 << VecGCdin.Muniforme << VecGCdin.Mdiverso << VecGCdin.Mestatico << VecGCdin.Mincremento
                 << VecGCdin.MMin << VecGCdin.MMax << VecGCdin.MInc << VecGCdin.MTS << VecGCdin.MTE << VecGCdin.MWCP;
}

QDataStream &operator>>(QDataStream &in12, VecGCCdinamico &VecGCdin)
{
    return in12 >> VecGCdin.Muniforme >> VecGCdin.Mdiverso >> VecGCdin.Mestatico >> VecGCdin.Mincremento
                >> VecGCdin.MMin >> VecGCdin.MMax >> VecGCdin.MInc >> VecGCdin.MTS >> VecGCdin.MTE >> VecGCdin.MWCP;
}

VecPlot_CCAjustadasEst_Diversa::VecPlot_CCAjustadasEst_Diversa(bool uniforme, bool diverso, bool estatico, bool incremento,
                                                                         QVector<double> &TS, QVector<double> &TE,
                                                                         QVector<double> &Wcp, QVector<double> &h,
                                                                         double DTmin,double K)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MDTmin = DTmin;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        Mh[i] = h[i];
    }
    MK = K;
}

bool VecPlot_CCAjustadasEst_Diversa::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CCAjustadasEst_Diversa::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CCAjustadasEst_Diversa::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CCAjustadasEst_Diversa::getIncremmento() const
{
    return Mincremento;
}

double VecPlot_CCAjustadasEst_Diversa::getDTmin() const
{
    return MDTmin;
}

double VecPlot_CCAjustadasEst_Diversa::getK() const
{
    return MK;
}

QVector<double> VecPlot_CCAjustadasEst_Diversa::getTS() const
{
    return MTS;
}

QVector<double> VecPlot_CCAjustadasEst_Diversa::getTE() const
{
    return MTE;
}

QVector<double> VecPlot_CCAjustadasEst_Diversa::getWCP() const
{
    return MWCP;
}

QVector<double> VecPlot_CCAjustadasEst_Diversa::geth() const
{
    return Mh;
}

QDataStream &operator<<(QDataStream &out13, const VecPlot_CCAjustadasEst_Diversa &VecCCAED)
{
    return out13 << VecCCAED.Muniforme << VecCCAED.Mdiverso << VecCCAED.Mestatico << VecCCAED.Mincremento
                 << VecCCAED.MDTmin << VecCCAED.MK << VecCCAED.MTS << VecCCAED.MTE << VecCCAED.MWCP << VecCCAED.Mh;
}

QDataStream &operator>>(QDataStream &in13, VecPlot_CCAjustadasEst_Diversa &VecCCAED)
{
    return in13 >> VecCCAED.Muniforme >> VecCCAED.Mdiverso >> VecCCAED.Mestatico >> VecCCAED.Mincremento
                >> VecCCAED.MDTmin >> VecCCAED.MK >> VecCCAED.MTS >> VecCCAED.MTE >> VecCCAED.MWCP >> VecCCAED.Mh;
}

VecPlot_CCAjustadasInc_Diversa::VecPlot_CCAjustadasInc_Diversa(bool uniforme, bool diverso,
                                                               bool estatico, bool incremento,
                                                               QVector<double> &TS, QVector<double> &TE,
                                                               QVector<double> &Wcp, QVector<double> &h,
                                                               double min, double max, double inc, double K)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MMin = min;
    MMax = max;
    MInc = inc;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        Mh[i] = h[i];
    }
    MK = K;
}

bool VecPlot_CCAjustadasInc_Diversa::getUniforme() const
{
    return Muniforme;
}

bool VecPlot_CCAjustadasInc_Diversa::getDiverso() const
{
    return Mdiverso;
}

bool VecPlot_CCAjustadasInc_Diversa::getEstatico() const
{
    return Mestatico;
}

bool VecPlot_CCAjustadasInc_Diversa::getIncremmento() const
{
    return Mincremento;
}

double VecPlot_CCAjustadasInc_Diversa::getMin() const
{
    return MMin;
}

double VecPlot_CCAjustadasInc_Diversa::getMax() const
{
    return MMax;
}

double VecPlot_CCAjustadasInc_Diversa::getInc() const
{
    return MInc;
}

double VecPlot_CCAjustadasInc_Diversa::getK() const
{
    return MK;
}

QVector<double> VecPlot_CCAjustadasInc_Diversa::getTS() const
{
    return MTS;
}

QVector<double> VecPlot_CCAjustadasInc_Diversa::getTE() const
{
    return MTE;
}

QVector<double> VecPlot_CCAjustadasInc_Diversa::getWCP() const
{
    return MWCP;
}

QVector<double> VecPlot_CCAjustadasInc_Diversa::geth() const
{
    return Mh;
}

QDataStream &operator<<(QDataStream &out14, const VecPlot_CCAjustadasInc_Diversa &VecCCAID)
{
    return out14 << VecCCAID.Muniforme << VecCCAID.Mdiverso << VecCCAID.Mestatico << VecCCAID.Mincremento
                 << VecCCAID.MMin << VecCCAID.MMax << VecCCAID.MInc << VecCCAID.MK << VecCCAID.MTS
                 << VecCCAID.MTE << VecCCAID.MWCP << VecCCAID.Mh;
}

QDataStream &operator>>(QDataStream &in14, VecPlot_CCAjustadasInc_Diversa &VecCCAID)
{
    return in14  >> VecCCAID.Muniforme >> VecCCAID.Mdiverso >> VecCCAID.Mestatico >> VecCCAID.Mincremento
                 >> VecCCAID.MMin >> VecCCAID.MMax >> VecCCAID.MInc >> VecCCAID.MK >> VecCCAID.MTS
                 >> VecCCAID.MTE >> VecCCAID.MWCP >> VecCCAID.Mh;
}

VecGCCED::VecGCCED(bool uniforme, bool diverso, bool estatico, bool incremento,
                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                   QVector<double> &h ,double DTmin,double K)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MDTmin = DTmin;
    MK = K;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
}

bool VecGCCED::getUniforme() const
{
    return Muniforme;
}

bool VecGCCED::getDiverso() const
{
    return Mdiverso;
}

bool VecGCCED::getEstatico() const
{
    return Mestatico;
}

bool VecGCCED::getIncremmento() const
{
    return Mincremento;
}

double VecGCCED::getDTmin() const
{
    return MDTmin;
}

double VecGCCED::getK() const
{
    return MK;
}

QVector<double> VecGCCED::getTS() const
{
    return MTS;
}

QVector<double> VecGCCED::getTE() const
{
    return MTE;
}

QVector<double> VecGCCED::getWCP() const
{
    return MWCP;
}

QVector<double> VecGCCED::geth() const
{
    return Mh;
}

QDataStream &operator<<(QDataStream &out15, const VecGCCED &GCCED)
{
    return out15 << GCCED.Muniforme << GCCED.Mdiverso << GCCED.Mestatico << GCCED.Mincremento
                 << GCCED.MDTmin << GCCED.MK << GCCED.MTS
                 << GCCED.MTE << GCCED.MWCP << GCCED.Mh;
}

QDataStream &operator>>(QDataStream &in15, VecGCCED &GCCED)
{
    return in15  >> GCCED.Muniforme >> GCCED.Mdiverso >> GCCED.Mestatico >> GCCED.Mincremento
                 >> GCCED.MDTmin >> GCCED.MK >> GCCED.MTS
                 >> GCCED.MTE >> GCCED.MWCP >> GCCED.Mh;
}

VecGCCDD::VecGCCDD(bool uniforme, bool diverso, bool estatico, bool incremento,
                   QVector<double> &TS, QVector<double> &TE, QVector<double> &Wcp,
                   QVector<double> &h, double Min, double Max, double Inc, double K)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    Mestatico = estatico;
    Mincremento = incremento;
    MMin= Min;
    MMax = Max;
    MInc = Inc;
    MK = K;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
}

bool VecGCCDD::getUniforme() const
{
    return Muniforme;
}

bool VecGCCDD::getDiverso() const
{
    return Mdiverso;
}

bool VecGCCDD::getEstatico() const
{
    return Mestatico;
}

bool VecGCCDD::getIncremmento() const
{
    return Mincremento;
}

double VecGCCDD::getMin() const
{
    return MMin;
}

double VecGCCDD::getMax() const
{
    return MMax;
}

double VecGCCDD::getInc() const
{
    return MInc;
}

double VecGCCDD::getK() const
{
    return MK;
}

QVector<double> VecGCCDD::getTS() const
{
    return MTS;
}

QVector<double> VecGCCDD::getTE() const
{
    return MTE;
}

QVector<double> VecGCCDD::getWCP() const
{
    return MWCP;
}

QVector<double> VecGCCDD::geth() const
{
    return Mh;
}

QDataStream &operator<<(QDataStream &out16, const VecGCCDD &GCCDD)
{
    return out16 << GCCDD.Muniforme << GCCDD.Mdiverso << GCCDD.Mestatico << GCCDD.Mincremento
                 << GCCDD.MMin << GCCDD.MMax << GCCDD.MInc << GCCDD.MK << GCCDD.MTS
                 << GCCDD.MTE << GCCDD.MWCP << GCCDD.Mh;
}

QDataStream &operator>>(QDataStream &in16, VecGCCDD &GCCDD)
{
    return in16  >> GCCDD.Muniforme >> GCCDD.Mdiverso >> GCCDD.Mestatico >> GCCDD.Mincremento
                 >> GCCDD.MMin >> GCCDD.MMax >> GCCDD.MInc >> GCCDD.MK >> GCCDD.MTS
                 >> GCCDD.MTE >> GCCDD.MWCP >> GCCDD.Mh;
}

VecAreasUniforme::VecAreasUniforme(bool uniforme, bool diverso,
                                   QVector<double> &TS, QVector<double> &TE,
                                   QVector<double> &Wcp, QVector<double> &h, QVector<double> &Calentamiento,
                                   QVector<double> &Enfriamento, double Min, double Max, double Inc, int CTo, int CCo)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MCTo = CTo;
    MCCo = CCo;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
}

bool VecAreasUniforme::getUniforme() const
{
    return Muniforme;
}

bool VecAreasUniforme::getDiverso() const
{
    return Mdiverso;
}


double VecAreasUniforme::getMax() const
{
    return MMax;
}

double VecAreasUniforme::getInc() const
{
    return MInc;
}

double VecAreasUniforme::getMin() const
{
    return MMin;
}

int VecAreasUniforme::getCTo() const
{
    return MCTo;
}

int VecAreasUniforme::getCCo() const
{
    return MCCo;
}

QVector<double> VecAreasUniforme::getTS() const
{
    return MTS;
}

QVector<double> VecAreasUniforme::getTE() const
{
    return MTE;
}

QVector<double> VecAreasUniforme::getWCP() const
{
    return MWCP;
}

QVector<double> VecAreasUniforme::geth() const
{
    return Mh;
}

QVector<double> VecAreasUniforme::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecAreasUniforme::getEnfriamento() const
{
    return MEnfriamento;
}

QDataStream &operator<<(QDataStream &out17, const VecAreasUniforme &VAU)
{
    return out17 << VAU.Muniforme << VAU.Mdiverso
                 << VAU.MMin << VAU.MMax << VAU.MInc
                 << VAU.MCTo << VAU.MCCo << VAU.MTS
                 << VAU.MTE << VAU.MWCP << VAU.Mh
                 << VAU.MCalentamiento << VAU.MEnfriamento;
}

QDataStream &operator>>(QDataStream &in17, VecAreasUniforme &VAU)
{
    return in17  >> VAU.Muniforme >> VAU.Mdiverso
                 >> VAU.MMin >> VAU.MMax >> VAU.MInc
                 >> VAU.MCTo >> VAU.MCCo >> VAU.MTS
                 >> VAU.MTE >> VAU.MWCP >> VAU.Mh
                 >> VAU.MCalentamiento >> VAU.MEnfriamento;
}

VecAreasUniformebool::VecAreasUniformebool(bool uniforme, bool diverso)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
}

bool VecAreasUniformebool::getUniforme() const
{
    return Muniforme;
}

bool VecAreasUniformebool::getDiverso() const
{
    return Mdiverso;
}

QDataStream &operator<<(QDataStream &out18, const VecAreasUniformebool &VecAUB)
{
    return out18 << VecAUB.Muniforme << VecAUB.Mdiverso ;
}

QDataStream &operator>>(QDataStream &in18, VecAreasUniformebool &VecAUB)
{
    return in18 >> VecAUB.Muniforme >> VecAUB.Mdiverso ;
}

VecAreasDiverso::VecAreasDiverso(bool uniforme, bool diverso, QVector<double> &TS,
                                 QVector<double> &TE, QVector<double> &Wcp,
                                 QVector<double> &h, QVector<double> &Calentamiento,
                                 QVector<double> &Enfriamento, double Min, double Max,
                                 double Inc, double K, int CTo, int CCo)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MK = K;
    MCTo = CTo;
    MCCo = CCo;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
}

bool VecAreasDiverso::getUniforme() const
{
    return Muniforme;
}

bool VecAreasDiverso::getDiverso() const
{
    return Mdiverso;
}

double VecAreasDiverso::getMin() const
{
    return MMin;
}

double VecAreasDiverso::getMax() const
{
    return MMax;
}

double VecAreasDiverso::getInc() const
{
    return MInc;
}

double VecAreasDiverso::getK() const
{
    return MK;
}

int VecAreasDiverso::getCTo() const
{
    return MCTo;
}

int VecAreasDiverso::getCCo() const
{
    return MCCo;
}

QVector<double> VecAreasDiverso::getTS() const
{
    return MTS;
}

QVector<double> VecAreasDiverso::getTE() const
{
    return MTE;
}

QVector<double> VecAreasDiverso::getWCP() const
{
    return MWCP;
}

QVector<double> VecAreasDiverso::geth() const
{
    return Mh;
}

QVector<double> VecAreasDiverso::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecAreasDiverso::getEnfriamento() const
{
    return MEnfriamento;
}

QDataStream &operator<<(QDataStream &out19, const VecAreasDiverso &VAD)
{
    return out19 << VAD.Muniforme << VAD.Mdiverso
                 << VAD.MMin << VAD.MMax << VAD.MInc << VAD.MK
                 << VAD.MCTo << VAD.MCCo << VAD.MTS
                 << VAD.MTE << VAD.MWCP << VAD.Mh
                 << VAD.MCalentamiento << VAD.MEnfriamento;
}

QDataStream &operator>>(QDataStream &in19, VecAreasDiverso &VAD)
{
    return in19  >> VAD.Muniforme >> VAD.Mdiverso
                 >> VAD.MMin >> VAD.MMax >> VAD.MInc >> VAD.MK
                 >> VAD.MCTo >> VAD.MCCo >> VAD.MTS
                 >> VAD.MTE >> VAD.MWCP >> VAD.Mh
                 >> VAD.MCalentamiento >> VAD.MEnfriamento;
}

VecCostUniformebool::VecCostUniformebool(bool uniforme, bool diverso)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
}

bool VecCostUniformebool::getUniforme() const
{
    return Muniforme;
}

bool VecCostUniformebool::getDiverso() const
{
    return Mdiverso;
}

QDataStream &operator<<(QDataStream &out20, const VecCostUniformebool &VecCUB)
{
    return out20 << VecCUB.Muniforme << VecCUB.Mdiverso;
}

QDataStream &operator>>(QDataStream &in20, VecCostUniformebool &VecCUB)
{
    return in20  >> VecCUB.Muniforme >> VecCUB.Mdiverso;
}

VecCostUniforme::VecCostUniforme(bool uniforme, bool diverso, QVector<double> &TS, QVector<double> &TE,
                                 QVector<double> &Wcp, QVector<double> &h, QVector<double> &Calentamiento,
                                 QVector<double> &Enfriamento, QVector<QVector<double>> &CapitalCost,
                                 QVector<double> &OperationCost, double Min, double Max, double Inc,
                                 int CTo, int CCo)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MCTo = CTo;
    MCCo = CCo;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    int nfils = CapitalCost.size();
    MCapitalCost.resize(nfils);
    int columns = CapitalCost[0].size();
    for(int i = 0; i < CapitalCost.size(); i++){
        MCapitalCost[i].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < nfils; i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

bool VecCostUniforme::getUniforme() const
{
    return Muniforme;
}

bool VecCostUniforme::getDiverso() const
{
    return Mdiverso;
}

double VecCostUniforme::getMin() const
{
    return MMin;
}

double VecCostUniforme::getMax() const
{
    return MMax;
}

double VecCostUniforme::getInc() const
{
    return MInc;
}

int VecCostUniforme::getCTo() const
{
    return MCTo;
}

int VecCostUniforme::getCCo() const
{
    return MCCo;
}

QVector<double> VecCostUniforme::getTS() const
{
    return MTS;
}

QVector<double> VecCostUniforme::getTE() const
{
    return MTE;
}

QVector<double> VecCostUniforme::getWCP() const
{
    return MWCP;
}

QVector<double> VecCostUniforme::geth() const
{
    return Mh;
}

QVector<double> VecCostUniforme::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecCostUniforme::getEnfriamento() const
{
    return MEnfriamento;
}

QVector<QVector<double> > VecCostUniforme::getCapitalCost() const
{
    return MCapitalCost;
}

QVector<double> VecCostUniforme::getOperationCost() const
{
    return MOperationCost;
}

QDataStream &operator<<(QDataStream &out21, const VecCostUniforme &VCU)
{
    return out21 << VCU.Muniforme << VCU.Mdiverso
                 << VCU.MMin << VCU.MMax << VCU.MInc
                 << VCU.MCTo << VCU.MCCo << VCU.MTS
                 << VCU.MTE << VCU.MWCP << VCU.Mh
                 << VCU.MCalentamiento << VCU.MEnfriamento
                 << VCU.MCapitalCost << VCU.MOperationCost;
}

QDataStream &operator>>(QDataStream &in21, VecCostUniforme &VCU)
{
    return in21  >> VCU.Muniforme >> VCU.Mdiverso
                 >> VCU.MMin >> VCU.MMax >> VCU.MInc
                 >> VCU.MCTo >> VCU.MCCo >> VCU.MTS
                 >> VCU.MTE >> VCU.MWCP >> VCU.Mh
                 >> VCU.MCalentamiento >> VCU.MEnfriamento
                 >> VCU.MCapitalCost >> VCU.MOperationCost;
}

VecCostDiverso::VecCostDiverso(bool uniforme, bool diverso, QVector<double> &TS, QVector<double> &TE,
                               QVector<double> &Wcp, QVector<double> &h, QVector<double> &Calentamiento,
                               QVector<double> &Enfriamento, QVector<QVector<double> > &CapitalCost,
                               QVector<double> &OperationCost,
                               double Min, double Max, double Inc, double K)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MK = K;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(Wcp.size());
    Mh.resize(h.size());
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    MCapitalCost.resize(CapitalCost.size());
    double columns = CapitalCost[0].size();
    for(int j = 0; j < CapitalCost.size(); j++){
        MCapitalCost[j].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < Wcp.size(); i++){
        MWCP[i] = Wcp[i];
    }
    for(int i = 0; i < h.size(); i++){
        Mh[i] = h[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < CapitalCost.size(); i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

bool VecCostDiverso::getUniforme() const
{
    return Muniforme;
}

bool VecCostDiverso::getDiverso() const
{
    return Mdiverso;
}

double VecCostDiverso::getMin() const
{
    return MMin;
}

double VecCostDiverso::getMax() const
{
    return MMax;
}

double VecCostDiverso::getInc() const
{
    return MInc;
}

double VecCostDiverso::getK() const
{
    return MK;
}

QVector<double> VecCostDiverso::getTS() const
{
    return MTS;
}

QVector<double> VecCostDiverso::getTE() const
{
    return MTE;
}

QVector<double> VecCostDiverso::getWCP() const
{
    return MWCP;
}

QVector<double> VecCostDiverso::geth() const
{
    return Mh;
}

QVector<double> VecCostDiverso::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecCostDiverso::getEnfriamento() const
{
    return MEnfriamento;
}

QVector<QVector<double> > VecCostDiverso::getCapitalCost() const
{
    return MCapitalCost;
}

QVector<double> VecCostDiverso::getOperationCost() const
{
    return MOperationCost;
}

QDataStream &operator<<(QDataStream &out22, const VecCostDiverso &VCD)
{
    return out22 << VCD.Muniforme << VCD.Mdiverso << VCD.MMin << VCD.MMax << VCD.MInc << VCD.MK << VCD.MTS
                 << VCD.MTE << VCD.MWCP << VCD.Mh << VCD.MCalentamiento << VCD.MEnfriamento << VCD.MCapitalCost << VCD.MOperationCost;
}

QDataStream &operator>>(QDataStream &in22, VecCostDiverso &VCD)
{
    return in22  >> VCD.Muniforme >> VCD.Mdiverso >> VCD.MMin >> VCD.MMax >> VCD.MInc >> VCD.MK >> VCD.MTS
                 >> VCD.MTE >> VCD.MWCP >> VCD.Mh>> VCD.MCalentamiento >> VCD.MEnfriamento >> VCD.MCapitalCost >> VCD.MOperationCost;
}


bool UnidadesSerAux::getSI()
{
    return MSI;
}

bool UnidadesSerAux::getSIS()
{
    return MSIS;
}

void UnidadesSerAux::ConvertirUnidades(QVector<double> Calentamiento,
                                       QVector<double> Enfriamento,
                                       bool SI, bool SIN, int A)
{
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    if(SI == true){
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �F
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Farenheit
            for (int i = 0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i] * 1.8 - 459.67;
                MEnfriamento[i] = Enfriamento[i] * 1.8 - 459.67;
            }
        } else if (A == 2) {
            // Conversi�n de Farenheit a Farenheit
            for (int i = 0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i];
                MEnfriamento[i] = Enfriamento[i];
            }
        } else if (A == 3) {
            // Conversi�n de Celsius a Farenheit
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i] * 1.8 + 32.0;
                MEnfriamento[i] = Enfriamento[i] * 1.8 + 32.0;
            }
        } else if (A == 4) {
            // Conversi�n de Ranking a Farenheit
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i] - 459.67;
                MEnfriamento[i] = Enfriamento[i] - 459.67;
            }
        }
    } else if(SIN == 1) {
        // SISTEMA INTERNACIONAL
        //  CONVIERTE LAS TEMPERATURAS A UNIDADES DE �C
        //  WCP A UNIDADES DE BTU/HR�F
        //  H SIGUE CONSTANTE BTU/HR FT^2 �F
        // CONVERSION DE TEMPERATURA DE ENTRADA Y OBJETIVO
        if (A == 1) {
            // Conversi�n de Kelvin a Celsius
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i] + 273.15;
                MEnfriamento[i] = Enfriamento[i] + 273.15;
            }
        } else if (A == 2) {
            // Conversi�n de Farenheit a Celsius
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i] * 1.8 + 32.0;
                MEnfriamento[i] = Enfriamento[i] * 1.8 + 32.0;
            }
        } else if (A == 3) {
            // Conversi�n de Celsius a Celsius
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = Calentamiento[i];
                MEnfriamento[i] = Enfriamento[i];
            }
        } else if (A == 4){
            // Conversi�n de Ranking a Celsius
            for (int i =0; i < 2; i++){
                MCalentamiento[i] = (Calentamiento[i]+ 273.15) * 1.8;
                MEnfriamento[i] = (Enfriamento[i] + 273.15) * 1.8;
            }
        }
    }
    MCalentamiento[2] = Calentamiento[2];
    MEnfriamento[2] = Enfriamento[2];
}

QVector<double> UnidadesSerAux::getCalentamiento()
{
    return MCalentamiento;
}

QVector<double> UnidadesSerAux::getEnfriamento()
{
    return MEnfriamento;
}

UnidadesSerAux::UnidadesSerAux(bool SI, bool SIS, int UTemp)
{
    MSI = SI;
    MSIS = SIS;
    MUTemp = UTemp;
}

CostosCapitalCost::CostosCapitalCost(int CapCost)
{
    MCapCost = CapCost;
}

int CostosCapitalCost::getCapCost()
{
    return MCapCost;
}

QDataStream &operator<<(QDataStream &out24, const CostosCapitalCost &units)
{
    return out24 << units.MCapCost;
}

QDataStream &operator>>(QDataStream &in24, CostosCapitalCost &units)
{
    return in24 >> units.MCapCost;
}

infoapuniforme::infoapuniforme(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS
                               , double Min, double Max, double Inc, QVector<double> &TS, QVector<double> &TE,
                               QVector<double> &WCP, QVector<double> &H, QVector<double> &Enfriamento,
                               QVector<double> &Calentamiento, int CTo, int CCo,
                               QVector<QVector<double>> &CapitalCost, QVector<double> &OperationCost)
{
    MCheckboxes.resize(Checkboxes.size());
    for(int i = 0; i < Checkboxes.size(); i++){
        MCheckboxes[i] = Checkboxes[i];
    }
    Mestatico = estatico;
    Mincremento = incremento;
    MSI = SI;
    MSIS = SIS;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(WCP.size());
    MH.resize(H.size());
    MCTo = CTo;
    MCCo = CCo;
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    MCapitalCost.resize(CapitalCost.size());
    double columns = CapitalCost[0].size();
    for(int j = 0; j < CapitalCost.size(); j++){
        MCapitalCost[j].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < WCP.size(); i++){
        MWCP[i] = WCP[i];
    }
    for(int i = 0; i < H.size(); i++){
        MH[i] = H[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < CapitalCost.size(); i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

double infoapuniforme::getMin()
{
    return MMin;
}

double infoapuniforme::getMax()
{
    return MMax;
}

double infoapuniforme::getInc()
{
    return MInc;
}

int infoapuniforme::getCTo()
{
    return MCTo;
}

int infoapuniforme::getCCo()
{
    return MCCo;
}

bool infoapuniforme::getestatico()
{
    return Mestatico;
}

bool infoapuniforme::getincremento()
{
    return Mincremento;
}

bool infoapuniforme::getSI()
{
    return MSI;
}

bool infoapuniforme::getSIS()
{
    return MSIS;
}

QVector<double> infoapuniforme::getCheckboxes()
{
    return MCheckboxes;
}

QVector<double> infoapuniforme::getTS()
{
    return MTS;
}

QVector<double> infoapuniforme::getTE()
{
    return MTE;
}

QVector<double> infoapuniforme::getWCP()
{
    return MWCP;
}

QVector<double> infoapuniforme::getH()
{
    return MH;
}

QVector<double> infoapuniforme::getEnfriamento()
{
    return MEnfriamento;
}

QVector<double> infoapuniforme::getCalentamiento()
{
    return MCalentamiento;
}

QVector<double> infoapuniforme::getOperationCost()
{
    return MOperationCost;
}

QVector<QVector<double> > infoapuniforme::getCapitalCost()
{
    return MCapitalCost;
}

QDataStream &operator<<(QDataStream &out25, const infoapuniforme &checkboxes)
{
    return out25 << checkboxes.MMin << checkboxes.MMax << checkboxes.MInc << checkboxes.Mestatico << checkboxes.Mincremento
                 << checkboxes.MSI << checkboxes.MSIS << checkboxes.MCheckboxes << checkboxes.MTS << checkboxes.MTE
                 << checkboxes.MWCP << checkboxes.MH << checkboxes.MEnfriamento << checkboxes.MCalentamiento << checkboxes.MOperationCost
                 << checkboxes.MCapitalCost << checkboxes.MCTo << checkboxes.MCCo;
}

QDataStream &operator>>(QDataStream &in25, infoapuniforme &checkboxes)
{
    return in25 >> checkboxes.MMin >> checkboxes.MMax >> checkboxes.MInc >> checkboxes.Mestatico >> checkboxes.Mincremento
                >> checkboxes.MSI >> checkboxes.MSIS >> checkboxes.MCheckboxes >> checkboxes.MTS >> checkboxes.MTE
                >> checkboxes.MWCP >> checkboxes.MH >> checkboxes.MEnfriamento >> checkboxes.MCalentamiento >> checkboxes.MOperationCost
                >> checkboxes.MCapitalCost >> checkboxes.MCTo >> checkboxes.MCCo;
}

TabAnalisis::TabAnalisis(int TabAnalisis, int Ventanamadre)
{
    MTabAnalisis = TabAnalisis;
    MVentanamadre = Ventanamadre;
}

int TabAnalisis::getTabAnalisis()
{
    return MTabAnalisis;
}

int TabAnalisis::getVentanamadre()
{
    return MVentanamadre;
}

QDataStream &operator<<(QDataStream &out26, const TabAnalisis &TA)
{
    return out26 << TA.MTabAnalisis << TA.MVentanamadre;
}

QDataStream &operator>>(QDataStream &in26, TabAnalisis &TA)
{
    return in26 >> TA.MTabAnalisis >> TA.MVentanamadre;
}

infoapdiversa::infoapdiversa(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS,
                             double K, double Min, double Max, double Inc, QVector<double> &TS, QVector<double> &TE,
                             QVector<double> &WCP, QVector<double> &H, QVector<double> &Enfriamento,
                             QVector<double> &Calentamiento, int CTo, int CCo, QVector<QVector<double> > &CapitalCost,
                             QVector<double> &OperationCost)
{
    MCheckboxes.resize(Checkboxes.size());
    for(int i = 0; i < Checkboxes.size(); i++){
        MCheckboxes[i] = Checkboxes[i];
    }
    Mestatico = estatico;
    Mincremento = incremento;
    MSI = SI;
    MSIS = SIS;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MK = K;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(WCP.size());
    MH.resize(H.size());
    MCTo = CTo;
    MCCo = CCo;
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    MCapitalCost.resize(CapitalCost.size());
    double columns = CapitalCost[0].size();
    for(int j = 0; j < CapitalCost.size(); j++){
        MCapitalCost[j].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < WCP.size(); i++){
        MWCP[i] = WCP[i];
    }
    for(int i = 0; i < H.size(); i++){
        MH[i] = H[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < CapitalCost.size(); i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

double infoapdiversa::getMin()
{
    return MMin;
}

double infoapdiversa::getMax()
{
    return MMax;
}

double infoapdiversa::getInc()
{
    return MInc;
}

double infoapdiversa::getK()
{
    return MK;
}

int infoapdiversa::getCTo()
{
    return MCTo;
}

int infoapdiversa::getCCo()
{
    return MCCo;
}

bool infoapdiversa::getestatico()
{
    return Mestatico;
}

bool infoapdiversa::getincremento()
{
    return Mincremento;
}

bool infoapdiversa::getSI()
{
    return MSI;
}

bool infoapdiversa::getSIS()
{
    return MSIS;
}

QVector<double> infoapdiversa::getCheckboxes()
{
    return MCheckboxes;
}

QVector<double> infoapdiversa::getTS()
{
    return MTS;
}

QVector<double> infoapdiversa::getTE()
{
    return MTE;
}

QVector<double> infoapdiversa::getWCP()
{
    return MWCP;
}

QVector<double> infoapdiversa::getH()
{
    return MH;
}

QVector<double> infoapdiversa::getEnfriamento()
{
    return MEnfriamento;
}

QVector<double> infoapdiversa::getCalentamiento()
{
    return MCalentamiento;
}

QVector<double> infoapdiversa::getOperationCost()
{
    return MOperationCost;
}

QVector<QVector<double> > infoapdiversa::getCapitalCost()
{
    return MCapitalCost;
}

QDataStream &operator<<(QDataStream &out27, const infoapdiversa &apdiversa)
{
    return out27 << apdiversa.MMin << apdiversa.MMax << apdiversa.MInc << apdiversa.MK << apdiversa.Mestatico << apdiversa.Mincremento
                 << apdiversa.MSI << apdiversa.MSIS << apdiversa.MCheckboxes << apdiversa.MTS << apdiversa.MTE
                 << apdiversa.MWCP << apdiversa.MH << apdiversa.MEnfriamento << apdiversa.MCalentamiento << apdiversa.MOperationCost
                 << apdiversa.MCapitalCost << apdiversa.MCTo << apdiversa.MCCo;
}

QDataStream &operator>>(QDataStream &in27, infoapdiversa &apdiversa)
{
    return in27 >> apdiversa.MMin >> apdiversa.MMax >> apdiversa.MInc >> apdiversa.MK >> apdiversa.Mestatico >> apdiversa.Mincremento
                >> apdiversa.MSI >> apdiversa.MSIS >> apdiversa.MCheckboxes >> apdiversa.MTS >> apdiversa.MTE
                >> apdiversa.MWCP >> apdiversa.MH >> apdiversa.MEnfriamento >> apdiversa.MCalentamiento >> apdiversa.MOperationCost
                >> apdiversa.MCapitalCost >> apdiversa.MCTo >> apdiversa.MCCo;
}

infoapboth::infoapboth(QVector<double> &Checkboxes, bool estatico, bool incremento, bool SI, bool SIS, double K, double Min,
                       double Max, double Inc, QVector<double> &TS, QVector<double> &TE, QVector<double> &WCP, QVector<double> &H,
                       QVector<double> &Enfriamento, QVector<double> &Calentamiento, int CTo, int CCo,
                       QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost)
{
    MCheckboxes.resize(Checkboxes.size());
    for(int i = 0; i < Checkboxes.size(); i++){
        MCheckboxes[i] = Checkboxes[i];
    }
    Mestatico = estatico;
    Mincremento = incremento;
    MSI = SI;
    MSIS = SIS;
    MMin = Min;
    MMax = Max;
    MInc = Inc;
    MK = K;
    MCTo = CTo;
    MCCo = CCo;
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    MCapitalCost.resize(CapitalCost.size());
    double columns = CapitalCost[0].size();
    for(int j = 0; j < CapitalCost.size(); j++){
        MCapitalCost[j].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(WCP.size());
    MH.resize(H.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < WCP.size(); i++){
        MWCP[i] = WCP[i];
    }
    for(int i = 0; i < H.size(); i++){
        MH[i] = H[i];
    }
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < CapitalCost.size(); i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

double infoapboth::getMin()
{
    return MMin;
}

double infoapboth::getMax()
{
    return MMax;
}

double infoapboth::getInc()
{
    return MInc;
}

double infoapboth::getK()
{
    return MK;
}

int infoapboth::getCTo()
{
    return MCTo;
}

int infoapboth::getCCo()
{
    return MCCo;
}

bool infoapboth::getestatico()
{
    return Mestatico;
}

bool infoapboth::getincremento()
{
    return Mincremento;
}

bool infoapboth::getSI()
{
    return MSI;
}

bool infoapboth::getSIS()
{
    return MSIS;
}

QVector<double> infoapboth::getCheckboxes()
{
    return MCheckboxes;
}

QVector<double> infoapboth::getTS()
{
    return MTS;
}

QVector<double> infoapboth::getTE()
{
    return MTE;
}

QVector<double> infoapboth::getWCP()
{
    return MWCP;
}

QVector<double> infoapboth::getH()
{
    return MH;
}

QVector<double> infoapboth::getEnfriamento()
{
    return MEnfriamento;
}

QVector<double> infoapboth::getCalentamiento()
{
    return MCalentamiento;
}

QVector<double> infoapboth::getOperationCost()
{
    return MOperationCost;
}

QVector<QVector<double> > infoapboth::getCapitalCost()
{
    return MCapitalCost;
}

QDataStream &operator<<(QDataStream &out28, const infoapboth &apboth)
{
    return out28 << apboth.MMin << apboth.MMax << apboth.MInc << apboth.MK << apboth.Mestatico << apboth.Mincremento
                 << apboth.MSI << apboth.MSIS << apboth.MCheckboxes << apboth.MTS << apboth.MTE
                 << apboth.MWCP << apboth.MH << apboth.MEnfriamento << apboth.MCalentamiento << apboth.MOperationCost
                 << apboth.MCapitalCost << apboth.MCTo << apboth.MCCo;
}

QDataStream &operator>>(QDataStream &in28, infoapboth &apboth)
{
    return in28 >> apboth.MMin >> apboth.MMax >> apboth.MInc >> apboth.MK >> apboth.Mestatico >> apboth.Mincremento
                >> apboth.MSI >> apboth.MSIS >> apboth.MCheckboxes >> apboth.MTS >> apboth.MTE
                >> apboth.MWCP >> apboth.MH >> apboth.MEnfriamento >> apboth.MCalentamiento >> apboth.MOperationCost
                >> apboth.MCapitalCost >> apboth.MCTo >> apboth.MCCo;
}

savehelp::savehelp(int Help)
{
    MHelp = Help;
}

int savehelp::gethelp()
{
    return MHelp;
}

QDataStream &operator<<(QDataStream &out29, const savehelp &savehel)
{
    return out29 << savehel.MHelp;
}

QDataStream &operator>>(QDataStream &in29, savehelp &savehel)
{
    return in29 >> savehel.MHelp;
}

VecCostUniDesGri::VecCostUniDesGri(bool uniforme, bool diverso, QVector<double> &TS, QVector<double> &TE, QVector<double> &WCP,
                                   QVector<double> &H, QVector<double> &Calentamiento, QVector<double> &Enfriamento, QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost, double DTmin, int CTo, int CCo)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(WCP.size());
    MH.resize(H.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < WCP.size(); i++){
        MWCP[i] = WCP[i];
    }
    for(int i = 0; i < H.size(); i++){
        MH[i] = H[i];
    }
    MDTmin = DTmin;
    MCTo = CTo;
    MCCo = CCo;
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    int nfils = CapitalCost.size();
    MCapitalCost.resize(nfils);
    int columns = CapitalCost[0].size();
    for(int i = 0; i < CapitalCost.size(); i++){
        MCapitalCost[i].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < nfils; i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

QVector<double> VecCostUniDesGri::getTS() const
{
    return MTS;
}

QVector<double> VecCostUniDesGri::getTE() const
{
    return MTE;
}

QVector<double> VecCostUniDesGri::getWCP() const
{
    return MWCP;
}

QVector<double> VecCostUniDesGri::getH() const
{
    return MH;
}

bool VecCostUniDesGri::getUniforme() const
{
    return Muniforme;
}

bool VecCostUniDesGri::getDiverso() const
{
    return Mdiverso;
}

double VecCostUniDesGri::getDTmin() const
{
    return MDTmin;
}

int VecCostUniDesGri::getCTo() const
{
    return MCTo;
}

int VecCostUniDesGri::getCCo() const
{
    return MCCo;
}

QVector<double> VecCostUniDesGri::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecCostUniDesGri::getEnfriamento() const
{
    return MEnfriamento;
}

QVector<QVector<double> > VecCostUniDesGri::getCapitalCost() const
{
    return MCapitalCost;
}

QVector<double> VecCostUniDesGri::getOperationCost() const
{
    return MOperationCost;
}

QDataStream &operator<<(QDataStream &out30, const VecCostUniDesGri &VCUD)
{
    return out30 << VCUD.MTS << VCUD.MTE << VCUD.MWCP << VCUD.MH << VCUD.MCalentamiento << VCUD.MCapitalCost << VCUD.MCCo << VCUD.MCTo << VCUD.Mdiverso << VCUD.MDTmin
                 << VCUD.MEnfriamento << VCUD.MOperationCost << VCUD.Muniforme;
}

QDataStream &operator>>(QDataStream &in30, VecCostUniDesGri &VCUD)
{
    return in30 >> VCUD.MTS >> VCUD.MTE >> VCUD.MWCP >> VCUD.MH >> VCUD.MCalentamiento >> VCUD.MCapitalCost >> VCUD.MCCo >> VCUD.MCTo >> VCUD.Mdiverso >> VCUD.MDTmin
                >> VCUD.MEnfriamento >> VCUD.MOperationCost >> VCUD.Muniforme;
}

VecCostDivDesGri::VecCostDivDesGri(bool uniforme, bool diverso, QVector<double> TS, QVector<double> TE, QVector<double> WCP,
                                   QVector<double> H, QVector<double> &Calentamiento, QVector<double> &Enfriamento,
                                   QVector<QVector<double> > &CapitalCost, QVector<double> &OperationCost, double DTmin, double k)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
    MTS.resize(TS.size());
    MTE.resize(TE.size());
    MWCP.resize(WCP.size());
    MH.resize(H.size());
    for(int i = 0; i < TS.size(); i++){
        MTS[i] = TS[i];
    }
    for(int i = 0; i < TE.size(); i++){
        MTE[i] = TE[i];
    }
    for(int i = 0; i < WCP.size(); i++){
        MWCP[i] = WCP[i];
    }
    for(int i = 0; i < H.size(); i++){
        MH[i] = H[i];
    }
    MDTmin = DTmin;
    Mk = k;
    MCalentamiento.resize(Calentamiento.size());
    MEnfriamento.resize(Enfriamento.size());
    int nfils = CapitalCost.size();
    MCapitalCost.resize(nfils);
    int columns = CapitalCost[0].size();
    for(int i = 0; i < CapitalCost.size(); i++){
        MCapitalCost[i].resize(columns);
    }
    MOperationCost.resize(OperationCost.size());
    for(int i = 0; i < Calentamiento.size(); i++){
        MCalentamiento[i] = Calentamiento[i];
    }
    for(int i = 0; i < Enfriamento.size(); i++){
        MEnfriamento[i] = Enfriamento[i];
    }
    for(int i = 0; i < nfils; i++){
        for(int j = 0; j < columns; j++){
            MCapitalCost[i][j] = CapitalCost[i][j];
        }
    }
    for(int i = 0; i < OperationCost.size(); i++){
        MOperationCost[i] = OperationCost[i];
    }
}

bool VecCostDivDesGri::getUniforme() const
{
    return Muniforme;
}

bool VecCostDivDesGri::getDiverso() const
{
    return Mdiverso;
}

QVector<double> VecCostDivDesGri::getTS() const
{
    return MTS;
}

QVector<double> VecCostDivDesGri::getTE() const
{
    return MTE;
}

QVector<double> VecCostDivDesGri::getWCP() const
{
    return MWCP;
}

QVector<double> VecCostDivDesGri::getH() const
{
    return MH;
}

double VecCostDivDesGri::getDTmin() const
{
    return MDTmin;
}

double VecCostDivDesGri::getk() const
{
    return Mk;
}

QVector<double> VecCostDivDesGri::getCalentamiento() const
{
    return MCalentamiento;
}

QVector<double> VecCostDivDesGri::getEnfriamento() const
{
    return MEnfriamento;
}

QVector<QVector<double> > VecCostDivDesGri::getCapitalCost() const
{
    return MCapitalCost;
}

QVector<double> VecCostDivDesGri::getOperationCost() const
{
    return MOperationCost;
}

QDataStream &operator<<(QDataStream &out31, const VecCostDivDesGri &VCDD)
{
    return out31 << VCDD.MTS << VCDD.MTE << VCDD.MWCP << VCDD.MH << VCDD.MCalentamiento << VCDD.MCapitalCost << VCDD.Mdiverso << VCDD.MDTmin << VCDD.Mk
                 << VCDD.MEnfriamento << VCDD.MOperationCost << VCDD.Muniforme;
}

QDataStream &operator>>(QDataStream &in31, VecCostDivDesGri &VCDD)
{
    return in31  >> VCDD.MTS >> VCDD.MTE >> VCDD.MWCP >> VCDD.MH >> VCDD.MCalentamiento >> VCDD.MCapitalCost >> VCDD.Mdiverso >> VCDD.MDTmin >> VCDD.Mk
                 >> VCDD.MEnfriamento >> VCDD.MOperationCost >> VCDD.Muniforme;
}

VecDesigBool::VecDesigBool(bool uniforme, bool diverso)
{
    Muniforme = uniforme;
    Mdiverso = diverso;
}

bool VecDesigBool::getUniforme() const
{
    return Muniforme;
}

bool VecDesigBool::getDiverso() const
{
    return Mdiverso;
}

QDataStream &operator<<(QDataStream &out32, const VecDesigBool &VecDB)
{
    return out32 << VecDB.Muniforme << VecDB.Mdiverso;
}

QDataStream &operator>>(QDataStream &in32, VecDesigBool &VecDB)
{
    return in32 >> VecDB.Muniforme >> VecDB.Mdiverso;
}

Grid::Grid(QVector<QVector<double> > Uniones, QVector<QVector<double> > Servicios, QVector<double> EnergiaServicios, QVector<double> EnergiaUniones)
{
    qDebug() << Uniones << Servicios;
    MUniones.resize(Uniones.size());
    MServicios.resize(Servicios.size());
    int columns = Uniones[0].size();
    for(int i = 0; i < Uniones.size(); i++){
        MUniones[i].resize(columns);
    }
    columns = Servicios[0].size();
    for(int i = 0; i < Servicios.size(); i++){
        MServicios[i].resize(columns);
    }
    columns = Uniones[0].size();
    for(int i = 0; i < Uniones.size(); i++){
        for(int j = 0; j < columns; j++){
            MUniones[i][j] = Uniones[i][j];
        }
    }
    columns = Servicios[0].size();
    for(int i = 0; i < Servicios.size(); i++){
        for(int j = 0; j < columns; j++){
            MServicios[i][j] = Servicios[i][j];
        }
    }
    MEnergiaServicios.resize(EnergiaServicios.size());
    MEnergiaUniones.resize(EnergiaUniones.size());
    qDebug() << MUniones << MServicios;
}

QVector<QVector<double> > Grid::getUniones()
{
    return MUniones;
}

QVector<QVector<double> > Grid::getServicios()
{
    return MServicios;
}

QVector<double> Grid::getEnergiaServicios()
{
    return MEnergiaServicios;
}

QVector<double> Grid::getEnergiaUniones()
{
    return MEnergiaUniones;
}

QDataStream &operator<<(QDataStream &out33, const Grid &VecGrid)
{
    return out33 << VecGrid.MEnergiaServicios << VecGrid.MEnergiaUniones << VecGrid.MServicios << VecGrid.MUniones;
}

QDataStream &operator>>(QDataStream &in33, Grid &VecGrid)
{
    return in33 >> VecGrid.MEnergiaServicios >> VecGrid.MEnergiaUniones >> VecGrid.MServicios >> VecGrid.MUniones;
}

