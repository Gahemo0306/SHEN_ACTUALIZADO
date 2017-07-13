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
    void splitStream();
    void hideItemText();
    void conectdefault();
    void plotsubstream(QVector<double> Caliente, QVector<double> Fria);
    bool comparadortemperaturas(double A,double B,double C);
    bool controlintercambio(double DTmin, double dif1,double dif2, double X1,double X2,double TobjC,double TobjF);
    bool algoritmoCorrienteCaliente(double Corriente);
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
