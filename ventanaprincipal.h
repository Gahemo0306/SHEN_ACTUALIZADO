#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QtCore>
#include <QtGui>

#include "tabladatos.h"
#include "help.h"

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionCustom_Analysis_triggered();
    void on_actionPlots_triggered();
    void on_actionHelp_triggered();
    void loadSubWindow(QWidget *widget,int ValorACTION);
    void loadSubWindowPrincipal(QWidget *widget);
    void on_actionSummary_triggered();
    void on_actionProblem_Table_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();


    void on_actionArea_Targets_triggered();

    void on_actionSuper_Targeting_triggered();

private:
    Ui::VentanaPrincipal *ui;

signals:
    void actionSave(const bool checked);
    void actionSaveas(const bool checked);
};

#endif // VENTANAPRINCIPAL_H
