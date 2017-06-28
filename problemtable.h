#ifndef PROBLEMTABLE_H
#define PROBLEMTABLE_H
#include <QFileSystemModel>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QWidget>
#include "duvaloraction.h"
#include "valoraction.h"
#include "qcustomplot.h"

namespace Ui {
class problemtable;
}

class problemtable : public QWidget
{
    Q_OBJECT

public:
    explicit problemtable(QWidget *parent = 0);
    ~problemtable();
    void helprecibidor(bool checked);
private slots:
    void on_uniform_clicked();
    void on_diverse_clicked();
    void on_pushButton_clicked();
    void radiobuttons();
    void on_estatico_clicked();
    void on_incremento_clicked();
    void tablaproblema_estatico_uniforme(double DTmin);
    void tablaproblema_incremento_uniforme(double DTmin);
    void tablaproblema_estatico_diverso(double DTmin,double k);
    void tablaproblema_incremento_diverso(double DTmin, double k, float punto1, float punto2);
    void on_Export_clicked();
    QString espaceadorcalculos(QString row);
    QStringList unidadestablas();
    bool compararrows(QString row1, QString row2);
    QString currDate();
    void on_help_tableproblem_clicked();
signals:
    void helpsender(bool checked);
private:
    Ui::problemtable *ui;
    QStringList CONTADORROWS;
    int CONTADORFILAS;
    bool SI,SIS;
    double Max,Min,Inc,K1,K;
    QVector<double> TS,TE,WCP,H;
};

#endif // PROBLEMTABLE_H
