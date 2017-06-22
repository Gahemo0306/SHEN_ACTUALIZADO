#ifndef COSTTARGETING_H
#define COSTTARGETING_H

#include <QWidget>

namespace Ui {
class Costtargeting;
}

class Costtargeting : public QWidget
{
    Q_OBJECT

public:
    explicit Costtargeting(QWidget *parent = 0);
    ~Costtargeting();    
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
    void on_OK_clicked();
    void Info_Uniforme();
    void Info_Diversa();

private:
    Ui::Costtargeting *ui;
    double K, Min,Max,Inc;
    QStringList titulos;
    int VENTANAMADRE;
    bool SI,SIS;
};

#endif // COSTTARGETING_H
