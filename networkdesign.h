#ifndef NETWORKDESIGN_H
#define NETWORKDESIGN_H
#include "jscene.h"
#include <QWidget>
#include "qcustomplot.h"
namespace Ui {
class NetworkDesign;
}

class NetworkDesign : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkDesign(QWidget *parent = 0);
    ~NetworkDesign();
private slots:
    //BOTONES
    void on_Start_clicked();
    //INICIO
    void on_Uniform_clicked();
    void on_Diverse_clicked();
    void RADIOBUTTONS();
    void accionguardar();

    //SPINBOXES
    void on_doubleSpinBoxK_valueChanged();
    void on_doubleSpinBoxMin_valueChanged();

    //COMBOBOXES
    void on_TypeDesign_currentIndexChanged(int index);
    void on_comboBoxCapital_currentIndexChanged(int index);
    void on_comboBoxUOP_currentIndexChanged(int index);
    //ACCIONES
    bool confirmartablas();
    QVector<double> ConvertirOperationCost(QVector<double> OperationCost,bool SI,bool SIS,int A);
    //TABLAS
    void on_Services_cellChanged();
    void on_capitalcost_cellChanged();
    void on_operationcost_cellChanged();

    void on_tabWidget_tabCloseRequested(int index);

private:
    Ui::NetworkDesign *ui;
    Jscene *scene;
    QStringList titulos;
//    QVector<double> TS,TE,WCP,H,Enfriamento,Calentamiento;
//    int CTo,CCo;
    int CONTADORTAB;
    double DTmin, K;
    bool uniforme,diverso,SI,SIS;
};

#endif // NETWORKDESIGN_H
