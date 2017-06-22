#ifndef CONTENIDO_PLOTS_COSTOS_H
#define CONTENIDO_PLOTS_COSTOS_H

#include <QWidget>

namespace Ui {
class contenido_plots_costos;
}

class contenido_plots_costos : public QWidget
{
    Q_OBJECT

public:
    explicit contenido_plots_costos(QWidget *parent = 0);
    ~contenido_plots_costos();

private slots:
    void on_uniform_clicked();
    void on_diverse_clicked();
    void on_doubleSpinBox_valueChanged();
    void on_doubleSpinBox_2_valueChanged();
    void on_doubleSpinBox_3_valueChanged();
    void on_doubleSpinBox_4_valueChanged();
    void RADIOBUTTONS();
    void accionguardar();
    void on_comboBoxCapital_currentIndexChanged(int index);
    void on_Services_cellChanged();
    void on_capitalcost_cellChanged();
    void on_operationcost_cellChanged();
    bool confirmartablas();
    void on_comboBoxUOP_currentIndexChanged(int index);
    QVector<double> ConvertirOperationCost(QVector<double> OperationCost,bool SI,bool SIS,int A);
    void on_TypeDesign_currentIndexChanged(int index);

private:
    Ui::contenido_plots_costos *ui;
    double K, Min,Max,Inc;
    QStringList titulos;
};

#endif // CONTENIDO_PLOTS_COSTOS_H
