#ifndef AREATARGETING_H
#define AREATARGETING_H

#include <QWidget>

namespace Ui {
class areatargeting;
}

class areatargeting : public QWidget
{
    Q_OBJECT

public:
    explicit areatargeting(QWidget *parent = 0);
    ~areatargeting();

private slots:
    void on_uniform_clicked();
    void on_diverse_clicked();
    void on_TypeDesign_currentIndexChanged(int index);
    void on_Services_cellChanged(int row, int column);
    void accionguardar();
    void RADIOBUTTONS();
    void on_SBk_valueChanged();
    void on_SBmin_valueChanged();
    void on_SBmax_valueChanged();
    void on_SBinc_valueChanged();
    void Info_Uniforme();
    void Info_Diversa();
    void on_OK_clicked();

private:
    Ui::areatargeting *ui;
    QStringList titulos;
    double K , Min  ,Max ,Inc;
    int VENTANAMADRE;
    bool SI,SIS;
};

#endif // AREATARGETING_H
