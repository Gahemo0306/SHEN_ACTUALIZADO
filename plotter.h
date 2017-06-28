#ifndef PLOTTER_H
#define PLOTTER_H
#include <QFile>
#include <QDataStream>
#include <QVector>
#include "qcustomplot.h"
#include <QWidget>

namespace Ui {
class plotter;
}

class plotter : public QWidget
{
    Q_OBJECT

public:
    explicit plotter(QWidget *parent = 0);
    ~plotter();
    void plot(int ventanaplot, bool uniforme, bool diverso, bool estatico, bool incremento,
              QVector<double> TS, QVector<double> TE, QVector<double> Wcp, QVector<double> h,
              QVector<double> Calentamiento, QVector<double> Enfriamento, int CTo, int CCo, double Min,
              double MAx, double Inc, double K, QVector<QVector<double> > CapitalCost, QVector<double> OperaetionCost2);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    //INTERACCIONES CON LA TABLA
    void titleDoubleClick(QMouseEvent* event);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    //void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

private:
    Ui::plotter *ui;
    QCPTextElement *title;
};

#endif // PLOTTER_H
