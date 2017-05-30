#ifndef CONTENIDO_PLOTS_AREA_H
#define CONTENIDO_PLOTS_AREA_H

#include <QWidget>

namespace Ui {
class contenido_plots_area;
}

class contenido_plots_area : public QWidget
{
    Q_OBJECT

public:
    explicit contenido_plots_area(QWidget *parent = 0);
    ~contenido_plots_area();

private slots:
    void on_Uniform_clicked();
    void on_Diverse_clicked();
    void RADIOBUTTONS();
    void accionguardar();
    void on_SBk_valueChanged();

    void on_SBmin_valueChanged();

    void on_SBmax_valueChanged();

    void on_SBinc_valueChanged();

    void on_Services_cellChanged();

private:
    Ui::contenido_plots_area *ui;
    double K, Min,Max,Inc;
};

#endif // CONTENIDO_PLOTS_AREA_H
