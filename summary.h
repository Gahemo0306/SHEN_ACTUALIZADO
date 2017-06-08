#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWidget>
#include <QTreeWidgetItem>

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
//    void costos_incremento_uniforme(double DTmin,QString text);
//    void costos_estatico_diverso(double DTmin,double k,QString text,float punto1,float punto2);
//    void costos_incremento_diverso(double DTmin,double k,QString text,float punto1,float punto2);
    //FECHA
    QString currDate();

    //BOTON PDF
    void on_pushButton_clicked();
    QString espaceadorcalculos(QString text,QString row);
    //QString acompletador(int diferencia, int syzebytes, int tamano);
    bool compararrows(QString row1,QString row2);

private:
    Ui::Summary *ui;
    QString item_select;
    int TabAnali,Ventanamadre;
    double Max,Min,Inc,K1,K;
    bool estatico,incremento,SI,SIS;
    int VENTANAMADRE,CTo,CCo,CONTADORFILAS;
    QStringList titulos,CONTADORROWS;
    QVector<double> Checkboxes, TS,TE,WCP,H,Enfriamento,Calentamiento,OperationCost;
    QVector<QVector<double>> CapitalCost;
};

#endif // SUMMARY_H
