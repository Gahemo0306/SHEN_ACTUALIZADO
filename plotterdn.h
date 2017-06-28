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
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void AlgoritmoCorrientes(QVector<double> TS, QVector<double> TE, QVector<double> WCP, QVector<double> H,
                             bool uniforme, bool diverso, double DTmin, double k);
    void AlgoritmoCoorrGrafi(QVector<QVector<double>> VectorCalientesMATRIZ,QVector<QVector<double>> VectorFriasMATRIZ ,
                             QVector<double> PuntodePliegue);
    void on_Legend_clicked();
    bool comparadortemperaturas(double A,double B,double C);
    void heatingService();
    void coolingService();
    void conectStream();
private:
    Ui::plotterDN *ui;
    QStringList titulos;
    QVector<double> TS,TE,WCP,H,Enfriamento,Calentamiento;
    QVector<QVector<double>> LadoCaliente, LadoFrio, Uniones;
    int CTo,CCo;
    double DTmin, K;
    bool uniforme,diverso,SI,SIS,control;
};

#endif // PLOTTERDN_H
