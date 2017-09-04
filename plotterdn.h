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
    void hideItemText();
    void on_Legend_clicked();
    void AlgoritmoCorrientes(QVector<double> TS, QVector<double> TE, QVector<double> WCP, QVector<double> H,
                             bool uniforme, bool diverso, double DTmin, double k);
    void AlgoritmoCoorrGrafi(QVector<QVector<double>> VectorCalientesMATRIZ,QVector<QVector<double>> VectorFriasMATRIZ ,
                             QVector<double> PuntodePliegue);
    void AuxiliaryService();
    void conectStream();
    void conectdefault();
    //ESPACIADOR
    bool ifisinUnion(double Conexion);
    //bool ifisinUnionSub(double Corriente);
    //PRIMERA OPCION
    void PrimeraOPC_plotsubstream(QVector<double> Caliente, QVector<double> Fria);
    //SEGUNDA OPCION
    void SegundaOPC_plotsubstream(QVector<double> Caliente, QVector<double> Fria);
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
    //SEGUNDA OPCION - IZQUIERAA
    void IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_0(double DTmin,double Caliente,double Fria ,double espaciadoruniones);
    void IZQ_SegundaOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    //SEGUNDA OPCION - DERECHA
    void DER_SegundaOPC_Caliente_SI_0_FRIA_SI_0(double DTmin,double Caliente,double Fria, double espaciadoruniones);
    void DER_SegundaOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    void DER_SegundaOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones);
    //CONECTORES SUBSTREAM PRIMERA OPC - IZQUIERDA
    void IZQ_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin,double Caliente, double Fria, double espaciadorunionesSUB);
    void IZQ_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin,double Caliente, double Fria, double espaciadorunionesSUB);
    //CONECTORES SUBSTREAM PRIMERA OPC_ DERECHA
    void DER_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin,double Caliente,double Fria, double espaciadorunionesSUB);
    void DER_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin,double Caliente,double Fria, double espaciadorunionesSUB);
    //CONECTORES SUBSTREAAM PRIMERA OPC IZQ
    void IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    //void IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Sub(double DTmin, double Calientes, double subfria, double espaciadorunionesSUB );
    // CONECTORES SUBSTREAM PRIMERA OPC DER
    void DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    //CONECTORES SUBSTREAM PRIMERA OPC IZQ SUB A SUB
    void PrimeraOPC_Caliente_Subs_X_X_Fria_Subs_X_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB);
    //CONECTORES SUBSTREAM Segunda OPC - IZQUIERDA
    void IZQ_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin,double Caliente, double Fria, double espaciadorunionesSUB);
    void IZQ_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin,double Caliente, double Fria, double espaciadorunionesSUB);
    //CONECTORES SUBSTREAM Segunda OPC_ DERECHA
    void DER_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin,double Caliente,double Fria, double espaciadorunionesSUB);
    void DER_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin,double Caliente,double Fria, double espaciadorunionesSUB);
    //CONECTORES SUBSTREAAM Segunda OPC IZQ
    void IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    //void IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Sub(double DTmin, double Calientes, double subfria, double espaciadorunionesSUB );
    // CONECTORES SUBSTREAM Segunda OPC DER
    void DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    void DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB );
    // CONECTORES SUBSTREAM Segunda OPC IZQ SUB A SUB
    void SegundaOPC_Caliente_Subs_X_X_Fria_Subs_X_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB);
    //DETECTOR DE QUE PASO LA MITAD DE LA LINEA DE CORRIENTE
    bool ifpasomitadIZQ(double posicionCal, double posicionFri);
    bool ifpasomitadDER(double posicionCal, double posicionFri);
    //SEPARAR CORRIENTES
    void splitStream();
    void SeparacionesCalIZQ(double Corriente, double Divisiones);
    void SeparacionesFriIZQ(double Corriente, double Divisiones);
    void SeparacionesCalDER(double Corriente, double Divisiones);
    void SeparacionesFriDER(double Corriente, double Divisiones);
    //SERVICIONES AUXILIARES
    void IZQ_cooling_split_No(double Corriente);
    void DER_cooling_split_No(double Corriente);
    void IZQ_heating_split_No(double Corriente);
    void DER_heating_split_No(double Corriente);
    void IZQ_cooling_split_Si(double Corriente,double Temp);
    void DER_cooling_split_Si(double Corriente,double Temp);
    void IZQ_heating_split_Si(double Corriente,double Temp);
    void DER_heating_split_Si(double Corriente,double Temp);
    bool comparadortemperaturas(double A,double B,double C);
    bool ifissplittedCAL(double Corriente);
    bool ifissplittedFRI(double Corriente);
    //EXPORTADORES
    void on_PDF_clicked();
    void on_IMAGE_clicked();
    //GUARDADOR
    void on_SAVE_clicked();

private:
    Ui::plotterDN *ui;
    QStringList titulos;
    QVector<double> TS,TE,WCP,H,Enfriamento,Calentamiento,ENERGIA_UNIONES,ENERGIA_SERVICIOS;
    QVector<QVector<double>> LadoCaliente, LadoFrio, Uniones , Servicios, SeparacionesCalientes, SeparacionesFrias;
    QVector<QVector<double>> CoorSerIZQ,CoorSerDER;
    int NumUniones,contadorPlots,contadorUniones,CTo,CCo;
    double contadorSERCAL,contadorSERFRI,contadorSER,contadorSEPARACIONES, DTmin, K , espaciadoruniones , espaciadorunionesSUB;
    double contadorLADOIZQUIERDO,contadorLADODERECHO,contadorTOTALIZQYDER;
    double contadorDivisionesCalientes, contadorDivisionesFrias, contadorDivisionesTotal;
    bool uniforme,diverso,SI,SIS,control;
};

#endif // PLOTTERDN_H
