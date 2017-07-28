#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "qcustomplot.h"

namespace Ui {
class Summary;
}

class Summary : public QWidget
{
    Q_OBJECT

public:
    explicit Summary(QWidget *parent = 0);
    ~Summary();
private slots:
    //SEÑAL
    void on_treeWidget_doubleClicked();
    //MENU
    void AddRoots(QString name, QStringList listas);
    void AddChild(QTreeWidgetItem *parent,QString name);
    //MOSTRAR INFO
    void desplegar_info(QString text);
    //OPERACIONES
    void tablaproblema_estatico_uniforme(double DTmin,QString text);
    void tablaproblema_incremento_uniforme(double DTmin,QString text);
    void tablaproblema_estatico_diverso(double DTmin,double k,QString text);
    void tablaproblema_incremento_diverso(double DTmin, double k, QString text, float punto1, float punto2);
    void tablaproblema_estatico_both(double DTmin, double k, QString text, float punto1, float punto2);
    void areas_estatico_uniforme(double DTmin, QString text);
    void areas_incremento_uniforme(double DTmin,QString text);
    void areas_estatico_diverso(double DTmin,double k,QString text,float punto1,float punto2);
    void areas_incremento_diverso(double DTmin,double k,QString text,float punto1,float punto2);
    void areas_estatico_both(double DTmin,double k , QString text,float punto1,float punto2);
    void areas_incremento_both(double DTmin,double k,QString text, float punto1, float punto2);
    void costos_estatico_uniforme(double DTmin, QString text);
    void costos_incremento_uniforme(double DTmin,QString text);
    void costos_estatico_diversa(double DTmin, double k, QString text, float punto1,float punto2);
    void costos_incremento_diverso(double DTmin, double k, QString text, float punto1,float punto2);
    void costos_estatico_both(double DTmin,double k , QString text,float punto1,float punto2);
    void costos_incremento_both(double DTmin,double k , QString text,float punto1,float punto2);
    //OPERACIONES
    void areas_grid_uniforme(double DTmin, QString text);
    void areas_grid_diversa(double DTmin, double k, QString text);
    void costos_grid_uniforme(double DTmin, QString text);
    void costos_grid_diversa(double DTmin, double k, QString text);
    //GRAFICOS
    void grafico_HCC_estatico_uniforme();
    void grafico_CCC_estatico_uniforme();
    void grafico_HCC_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void grafico_CCC_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void grafico_HCC_estatico_both(double DTmin, double k , float punto1,float punto2);
    void grafico_CCC_estatico_both(double DTmin, double k , float punto1,float punto2);
    void grafico_CurvasCompuestas_estatico_uniforme();
    void grafico_CurvasCompuestas_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void grafico_CurvasCompuestas_estatico_both(double DTmin, double k , float punto1,float punto2);
    void grafico_CurvasCompuestasAjustadas_estatico_uniforme(double DTmin);
    void grafico_CurvasCompuestasAjustadas_incremento_uniforme(double MAX, double MIN, double INC);
    void grafico_CurvasCompuestasAjustadas_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void grafico_CurvasCompuestasAjustadas_incremento_diverso(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void grafico_CurvasCompuestasAjustadas_estatico_both(double DTmin, double k , float punto1,float punto2);
    void grafico_CurvasCompuestasAjustadaas_incremento_both(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void grafico_GranCurvaCompuesta_estatico_uniforme(double DTmin);
    void grafico_GranCurvaCompuesta_incremento_uniforme(double MAX, double MIN, double INC);
    void grafico_GranCurvaCompuesta_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void grafico_GranCurvaCompuestas_incremento_diverso(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void grafico_GranCurvaCompuesta_estatico_both(double DTmin, double k , float punto1,float punto2);
    void grafico_GranCurvaCompuesta_incremento_both(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void grafico_Areas_uniforme(double MIN,double MAX,double INC);
    void grafico_Areas_diverso(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void grafico_Areas_both(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void grafico_Costos_uniforme(double MIN,double MAX,double INC);
    void grafico_Costos_diverso(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void grafico_Costos_both(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    //Puntos de graficas
    void datapoints_HCC_estatico_uniforme();
    void datapoints_CCC_estatico_uniforme();
    void datapoints_HCC_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void datapoints_CCC_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void datapoints_HCC_estatico_both(double DTmin, double k , float punto1,float punto2);
    void datapoints_CCC_estatico_both(double DTmin, double k , float punto1,float punto2);
    void datapoints_CurvasCompuestas_estatico_uniforme();
    void datapoints_CurvasCompuestas_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void datapoints_CurvasCompuestas_estatico_both(double DTmin, double k , float punto1,float punto2);
    void datapoints_CurvasCompuestasAjustadas_estatico_uniforme(double DTmin);
    void datapoints_CurvasCompuestasAjustadas_incremento_uniforme(double MAX, double MIN, double INC);
    void datapoints_CurvasCompuestasAjustadas_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void datapoints_CurvasCompuestasAjustadas_incremento_diverso(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void datapoints_CurvasCompuestasAjustadas_estatico_both(double DTmin, double k , float punto1,float punto2);
    void datapoints_CurvasCompuestasAjustadaas_incremento_both(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void datapoints_GranCurvaCompuesta_estatico_uniforme(double DTmin);
    void datapoints_GranCurvaCompuesta_incremento_uniforme(double MAX, double MIN, double INC);
    void datapoints_GranCurvaCompuesta_estatico_diverso(double DTmin, double k , float punto1,float punto2);
    void datapoints_GranCurvaCompuestas_incremento_diverso(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void datapoints_GranCurvaCompuesta_estatico_both(double DTmin, double k , float punto1,float punto2);
    void datapoints_GranCurvaCompuesta_incremento_both(double MIN,double MAX, double INC, double k, float punto1, float punto2);
    void datapoints_Areas_uniforme(double MIN,double MAX,double INC);
    void datapoints_Areas_diverso(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void datapoints_Areas_both(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void datapoints_Costos_uniforme(double MIN,double MAX,double INC);
    void datapoints_Costos_diverso(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void datapoints_Costos_both(double MIN,double MAX,double INC,double k,float punto1,float punto2);
    void DataPoints(QString text);
    //FECHA
    QString currDate();
    //BOTON PDF
    void on_pushButton_clicked();
    QString espaceadorcalculos(QString text, QString row, int j);
    QStringList unidadestablas(QString item_select);
    //QString acompletador(int diferencia, int syzebytes, int tamano);
    bool compararrows(QString row1,QString row2);
    void on_pushButton_4_clicked();
    void titleDoubleClick(QMouseEvent* event);
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
private:
    Ui::Summary *ui;
    QString item_select, item_select_datapoint;
    QCPTextElement *title;
    QTreeWidgetItem *clicked;
    int TabAnali,Ventanamadre;
    double Max,Min,Inc,K1,K;
    bool estatico,incremento,uniforme,diverso,SI,SIS;
    int VENTANAMADRE,CTo,CCo,CONTADORFILAS;
    int CONTADORBOTH;
    QStringList titulos,CONTADORROWS;
    QVector<double> Checkboxes, TS,TE,WCP,H,Enfriamento,Calentamiento,OperationCost;
    QVector<QVector<double>> CapitalCost, Uniones,Servicios;
};

#endif // SUMMARY_H
