#ifndef ANALISISPERSONALIZADO_H
#define ANALISISPERSONALIZADO_H

#include <QWidget>
#include <QVector>

namespace Ui {
class AnalisisPersonalizado;
}

class AnalisisPersonalizado : public QWidget
{
    Q_OBJECT

public:
    explicit AnalisisPersonalizado(QWidget *parent = 0);
    ~AnalisisPersonalizado();

private slots:
    //RADIOBUTTONS
    void on_IncrementradioButton_clicked();
    void on_StaticradioButton_clicked();
    void on_DStaticradioButton_clicked();
    void on_DIncrementradioButton_clicked();
    void on_BStaticradioButton_clicked();
    void on_BIncrementradioButton_clicked();
    //CHECKBOXES
    void Ucheckboxes_seleccionadas();
    void Dcheckboxes_seleccionadas();
    void Bcheckboxes_seleccionadas();
    //ACCIONES RADIOBUTTONS
    void botones_uniformes();
    void botones_diversos();
    void botones_both();
    //ACCION CHEKBOXES
    void on_UADTM_clicked();
    void on_UCDTM_clicked();
    void on_UAP_clicked();
    void on_UCP_clicked();
    void on_DAP_clicked();
    void on_DCP_clicked();
    void on_DADTM_clicked();
    void on_DCDTM_clicked();
    void on_BAP_clicked();
    void on_BCP_clicked();
    void on_BADTM_clicked();
    void on_BCDTM_clicked();
    void on_OK_clicked();
    //GUARDADORES
    void Info_Uniforme(bool UPT, bool UAP, bool UCP, bool UHCC, bool UCCC, bool UCC,
                       bool UCCA, bool UGCC, bool UADTM, bool UCDTM, bool estatico, bool incremento);
    void Info_Diversa(bool DPT, bool DAP, bool DCP, bool DHCC, bool DCCC, bool DCC,
                      bool DCCA, bool DGCC, bool DADTM, bool DCDTM, bool estatico, bool incremento);
    void Info_Both(bool BPT, bool BAP, bool BCP, bool BHCC, bool BCCC, bool BCC,
                   bool BCCA, bool BGCC, bool BADTM, bool BCDTM, bool estatico, bool incremento);
    //COMBOBOXES
    void on_UFCapcomboBox_currentIndexChanged(int index);
    void on_Ucapital_units_currentIndexChanged(int index);
    void on_UTodcomboBox_currentIndexChanged(int index);
//    void on_DTodcomboBox_currentIndexChanged(int index);
    void on_DFCapcomboBox_currentIndexChanged(int index);
    void on_Dcapital_units_currentIndexChanged(int index);
    void on_BTodcomboBox_currentIndexChanged(int index);
    void on_BFCapcomboBox_currentIndexChanged(int index);
    void on_Bcapital_units_currentIndexChanged(int index);
    //AUXILIARES
    QVector<double> ConvertirOperationCost(QVector<double> OperationCost, bool SI, bool SIS, int A);
    //COMPROBADORES
    bool metodoservicios_uniformes();
    bool metodocapital_uniformes();
    bool metodooperacional_uniformes();
    bool metodoservicios_diverso();
    bool metodocapital_diverso();
    bool metodooperacional_diverso();
    bool metodoservicios_both();
    bool metodocapital_both();
    bool metodooperacional_both();
private:
    Ui::AnalisisPersonalizado *ui;
    double Max,Min,Inc,K;
    bool estatico,incremento,SI,SIS;
    int VENTANAMADRE;
    QStringList titulos;
//    QVector<double> Checkboxes, TS,TE,WCP,H,Enfriamento,Calentamiento,OperationCost;
//    QVector<QVector<double>> CapitalCost;
};

#endif // ANALISISPERSONALIZADO_H
