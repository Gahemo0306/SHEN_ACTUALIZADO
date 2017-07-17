#ifndef PLOTTERDN_H
#define PLOTTERDN_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class plotterDN;
}

class plotterDN : public QWidget
{
    Q_OBJECT

public:
    explicit plotterDN(QWidget *parent = 0);
    ~plotterDN();
private slots:
    void titleDoubleClick(QMouseEvent* event);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void AlgoritmoCorrientes(QVector<double> TS, QVector<double> TE, QVector<double> WCP, QVector<double> H,
                             bool uniforme, bool diverso, double DTmin, double k);
    void AlgoritmoCoorrGrafi(QVector<QVector<double>> VectorCalientesMATRIZ,QVector<QVector<double>> VectorFriasMATRIZ ,
                             QVector<double> PuntodePliegue);
    void on_Legend_clicked();
    void AuxiliaryService();
    void conectStream();

    void hideItemText();
    void conectdefault();
    void plotsubstream(QVector<double> Caliente, QVector<double> Fria);
    //PRIMERA OPCION - IZQUIERAA
    void IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(double DTmin,double Caliente,double Fria ,double espaciadoruniones);
    void IZQ_PrimeraOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    bool controlintercambioIZQ(double DTmin, double dif1,double dif2, double X1,double X2,double TobjC,double TobjF);
    // PRIMERA OPCION - DERECHA
    void DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(double DTmin,double Caliente,double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    bool controlintercambioDER(double DTmin, double dif1,double dif2, double X1,double X2,double TobjC,double TobjF);
    //DETECTOR DE QUE PASO LA MITAD DE LA LINEA DE CORRIENTE
    bool ifpasomitadIZQ(double posicionCal, double posicionFri);
    bool ifpasomitadDER(double posicionCal, double posicionFri);
    //SEPARAR CORRIENTES
    void splitStream();
    void SeparacionesCalIZQ(double Corriente, double Divisiones);
    void SeparacionesFriIZQ(double Corriente, double Divisiones);
    void SeparacionesCalDER(double Corriente, double Divisiones);
    void SeparacionesFriDER(double Corriente, double Divisiones);
    bool comparadortemperaturas(double A,double B,double C);
    bool algoritmoCorrienteCaliente(double Corriente);
    bool ifissplittedCAL(double Corriente);
    bool ifissplittedFRI(double Corriente);
    void on_pushButton_clicked();

private:
    Ui::plotterDN *ui;
    QStringList titulos;
    QVector<double> TS,TE,WCP,H,Enfriamento,Calentamiento;
    QVector<QVector<double>> LadoCaliente, LadoFrio, Uniones , Servicios, SeparacionesCalientes, SeparacionesFrias;
    int NumUniones,contadorPlots,contadorUniones,CTo,CCo;
    double contadorSERCAL,contadorSERFRI,contadorSER,contadorSEPARACIONES, DTmin, K , espaciadoruniones;
    double contadorLADOIZQUIERDO,contadorLADODERECHO,contadorTOTALIZQYDER;
    double contadorDivisionesCalientes, contadorDivisionesFrias, contadorDivisionesTotal;
    bool uniforme,diverso,SI,SIS,control;
};

#endif // PLOTTERDN_H
