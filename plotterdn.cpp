#include "plotterdn.h"
#include "ui_plotterdn.h"
#include "qcustomplot.h"
#include "duvaloraction.h"
#include "graficos.h"
#include "valoraction.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>

plotterDN::plotterDN(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotterDN)
{
    ui->setupUi(this);
    //NumUniones = 0;
    contadorSER = 0;
    contadorSERCAL = 0;
    contadorSERFRI = 0;
    espaciadoruniones = 0;
    contadorPlots = 0;
    contadorUniones = 0;
    QFile F(WORKSPACE_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&F);
    in2.setVersion(QDataStream::Qt_5_4);
    QVector<QVector<double>> prueba;
    int filas = 10; // default
    int columnas = 10; //defalt
    prueba.resize(filas);
    for(int i = 0; i < filas; i++)
    {
        prueba[i].resize(columnas);
    }
    Workspace MATRIZ(prueba);
    in2>> MATRIZ;
    QVector<QVector<double>> Matriz = MATRIZ.getMatriz();
    TS.resize(Matriz.size());
    TE.resize(Matriz.size());
    WCP.resize(Matriz.size());
    H.resize(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        WCP[i] = Matriz[i][2];
        H[i] = Matriz[i][3];
    }
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    int UTemp = units.getUTemp();
    int UWcp = units.getUWcp();
    //int Uh = units.getUh();
    SI = units.getSI();
    SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();

    QFile FileBools(BOOL_DESIGNGRID_FILENAME);
    if (!FileBools.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in32(&FileBools);
    in32.setVersion(QDataStream::Qt_5_4);
    bool Uni = false, Div = false;
    VecDesigBool VDB(Uni,Div);
    in32 >> VDB;
    uniforme = VDB.getUniforme();
    diverso = VDB.getDiverso();
    if(uniforme == true){
        QFile FileCostos(UNIFORM_DESIGNGRID_FILENAME);
        if (!FileCostos.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
            return;
        }
        QDataStream in30(&FileCostos);
        in30.setVersion(QDataStream::Qt_5_4);
        bool diverso2 = false, uniforme2 = false;
        QVector<double> Calentamiento2,Enfriamento2,OperationCost2;
        Calentamiento2.resize(10);
        Enfriamento2.resize(10);
        OperationCost2.resize(10);
        QVector<QVector<double>> CapitalCost2;
        CapitalCost2.resize(10);
        for(int i = 0; i < CapitalCost2.size(); i++){
            CapitalCost2[i].resize(10);
        }
        int CTo2 = 0, CCo2 = 0;
        double DTmin2 = 0;
        VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,
                              CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
        in30 >> VCUD;
        uniforme = VCUD.getUniforme();
        diverso = VCUD.getDiverso();
        DTmin = VCUD.getDTmin();
        K = 0;
        AlgoritmoCorrientes(TS,TE,WCP,H,uniforme,diverso,DTmin,K);
        FileCostos.flush();
        FileCostos.close();
    }else if(diverso == true){

    }

    FileUnidades.flush();
    FileUnidades.close();
    FileBools.flush();
    FileBools.close();

}

plotterDN::~plotterDN()
{
    delete ui;
}

void plotterDN::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->qcustomplot->replot();
    }
  }
}

void plotterDN::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->qcustomplot->replot();
    }
  }
}

void plotterDN::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->qcustomplot->replot();
    }
  }
}

void plotterDN::selectionChanged()
{

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->qcustomplot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->qcustomplot->graph(i);
    QCPPlottableLegendItem *item = ui->qcustomplot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void plotterDN::mousePress()
{
  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeDrag(ui->qcustomplot->xAxis->orientation());
  else if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeDrag(ui->qcustomplot->yAxis->orientation());
  else
    ui->qcustomplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void plotterDN::mouseWheel()
{
  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->xAxis->orientation());
  else if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->yAxis->orientation());
  else
    ui->qcustomplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void plotterDN::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->qcustomplot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  }else{
    if (ui->qcustomplot->selectedGraphs().size() > 0)
      menu->addAction("Define the hot current output temperature", this, SLOT(conectStream()));
      menu->addAction("Default connection",this,SLOT(conectdefault()));
      menu->addAction("Add auxiliary service",this,SLOT(AuxiliaryService()));
      menu->addAction("Split stream",this,SLOT(splitStream()));
    if (ui->qcustomplot->selectedItems().size() > 0 ){
        menu->addAction("Hide text",this,SLOT(hideItemText()));
    }
  }

  menu->popup(ui->qcustomplot->mapToGlobal(pos));
}

void plotterDN::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->qcustomplot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->qcustomplot->replot();
    }
  }
}

void plotterDN::AlgoritmoCorrientes(QVector<double> TS, QVector<double> TE, QVector<double> WCP,
                                        QVector<double> H, bool uniforme, bool diverso,double DTmin,double k)
{
    if(uniforme == true){
        Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
        QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
        QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
        QVector<QVector<double>> VectorCalientesMatriz = plot.getVectorCalientesMATRIZ();
        QVector<QVector<double>> VectorFriasMatriz = plot.getVectorFriasMATRIZ();
        QVector<double> VAL2;
        VAL2.resize(VecAdjHeatFlow.size());
        for(int i = 0; i < VecAdjHeatFlow.size(); i++){
            VAL2[i] = VecAdjHeatFlow[i][1];
        }
        double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
        QVector<double> PuntodePliegue;
        PuntodePliegue.resize(2);
        PuntodePliegue[0] = VecCorrientesTotal[min_pos+1][0];
        PuntodePliegue[1] = VecCorrientesTotal[min_pos+1][1];
        AlgoritmoCoorrGrafi(VectorCalientesMatriz,VectorFriasMatriz,PuntodePliegue);
    }else if(diverso == true){
        float punto1 = .5, punto2 = 10;
        Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
        QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
        QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
        QVector<double> VAL2;
        VAL2.resize(VecAdjHeatFlow.size());
        for(int i = 0; i < VecAdjHeatFlow.size(); i++){
            VAL2[i] = VecAdjHeatFlow[i][1];
        }
        double min_pos = std::distance(VAL2.begin(),std::min_element(VAL2.begin(),VAL2.end()));
        QVector<double> PuntodePliegue;
        PuntodePliegue.resize(2);
        PuntodePliegue[0] = VecCorrientesTotal[min_pos+1][0]; //LADO FRIO
        PuntodePliegue[1] = VecCorrientesTotal[min_pos+1][1]; //LADO CALIENTE
    }
}

void plotterDN::AlgoritmoCoorrGrafi(QVector<QVector<double>> VectorCalientesMATRIZ,QVector<QVector<double>> VectorFriasMATRIZ ,
                                        QVector<double> PuntodePliegue)
{
    //PROBLEMA CALIENTE
    //corrientes calientes;
    QVector<QVector<double>> TCalPINCH_HOT;
    TCalPINCH_HOT.resize(VectorCalientesMATRIZ.size());
    int ncols = 3;
    for(int i = 0; i < TCalPINCH_HOT.size(); i ++){
        TCalPINCH_HOT[i].resize(ncols);
    }
    QVector<QVector<double>> TFriPINCH_HOT;
    TFriPINCH_HOT.resize(VectorCalientesMATRIZ.size());
    ncols = 3;
    for(int i = 0; i < TFriPINCH_HOT.size(); i ++){
        TFriPINCH_HOT[i].resize(ncols);
    }
    for(int i = 0; i < VectorCalientesMATRIZ.size(); i++){
        if(PuntodePliegue[1] <= VectorCalientesMATRIZ[i][0] && PuntodePliegue[1] >= VectorCalientesMATRIZ[i][1]){
            if(comparadortemperaturas(PuntodePliegue[1],VectorCalientesMATRIZ[i][0],VectorCalientesMATRIZ[i][1]) == true){
                if(PuntodePliegue[1] == VectorCalientesMATRIZ[i][0]){
                    TCalPINCH_HOT[i][0] = 0;
                    TCalPINCH_HOT[i][1] = 0;
                    TCalPINCH_HOT[i][2] = 0;
                    TFriPINCH_HOT[i][0] = VectorCalientesMATRIZ[i][0];
                    TFriPINCH_HOT[i][1] = VectorCalientesMATRIZ[i][1];
                    TFriPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];
                }else if(PuntodePliegue[1] == VectorCalientesMATRIZ[i][1]){
                    TCalPINCH_HOT[i][0] = VectorCalientesMATRIZ[i][0];
                    TCalPINCH_HOT[i][1] = VectorCalientesMATRIZ[i][1];
                    TCalPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];
                    TFriPINCH_HOT[i][0] = 0;
                    TFriPINCH_HOT[i][1] = 0;
                    TFriPINCH_HOT[i][2] = 0;
                }
            }else{
                TCalPINCH_HOT[i][0] = VectorCalientesMATRIZ[i][0];
                TCalPINCH_HOT[i][1] = PuntodePliegue[1];
                TCalPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];
                TFriPINCH_HOT[i][0] = PuntodePliegue[1];
                TFriPINCH_HOT[i][1] = VectorCalientesMATRIZ[i][1];
                TFriPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];

            }
        }else if(PuntodePliegue[1] > VectorCalientesMATRIZ[i][0]  &&  PuntodePliegue[1] > VectorCalientesMATRIZ[i][1]){
            TCalPINCH_HOT[i][0] = 0;
            TCalPINCH_HOT[i][1] = 0;
            TCalPINCH_HOT[i][2] = 0;
            TFriPINCH_HOT[i][0] = VectorCalientesMATRIZ[i][0];
            TFriPINCH_HOT[i][1] = VectorCalientesMATRIZ[i][1];
            TFriPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];
        }else if(PuntodePliegue[1] < VectorCalientesMATRIZ[i][0] && PuntodePliegue[1] < VectorCalientesMATRIZ[i][1]){
            TCalPINCH_HOT[i][0] = VectorCalientesMATRIZ[i][0];
            TCalPINCH_HOT[i][1] = VectorCalientesMATRIZ[i][1];
            TCalPINCH_HOT[i][2] = VectorCalientesMATRIZ[i][2];
            TFriPINCH_HOT[i][0] = 0;
            TFriPINCH_HOT[i][1] = 0;
            TFriPINCH_HOT[i][2] = 0;
        }
    }
    QVector<QVector<double>> TCalPINCH_COL;
    TCalPINCH_COL.resize(VectorFriasMATRIZ.size());
    ncols = 3;
    for(int i = 0; i < TCalPINCH_COL.size(); i ++){
        TCalPINCH_COL[i].resize(ncols);
    }
    QVector<QVector<double>> TFriPINCH_COL;
    TFriPINCH_COL.resize(VectorFriasMATRIZ.size());
    ncols = 3;
    for(int i = 0; i < TFriPINCH_COL.size(); i ++){
        TFriPINCH_COL[i].resize(ncols);
    }
    for(int i = 0; i < VectorFriasMATRIZ.size() ; i++){
        if(PuntodePliegue[0] >= VectorFriasMATRIZ[i][0] && PuntodePliegue[0] <= VectorFriasMATRIZ[i][1]){
            if(comparadortemperaturas(PuntodePliegue[0],VectorFriasMATRIZ[i][0],VectorFriasMATRIZ[i][1])==true){
                if(PuntodePliegue[0] == VectorFriasMATRIZ[i][0]){
                    TCalPINCH_COL[i][0] = VectorFriasMATRIZ[i][1];
                    TCalPINCH_COL[i][1] = VectorFriasMATRIZ[i][0];
                    TCalPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
                    TFriPINCH_COL[i][0] = 0;
                    TFriPINCH_COL[i][1] = 0;
                    TFriPINCH_COL[i][2] = 0;
                }else if(PuntodePliegue[0] == VectorFriasMATRIZ[i][1]){
                    TCalPINCH_COL[i][0] = 0;
                    TCalPINCH_COL[i][1] = 0;
                    TCalPINCH_COL[i][2] = 0;
                    TFriPINCH_COL[i][0] = VectorFriasMATRIZ[i][1];
                    TFriPINCH_COL[i][1] = VectorFriasMATRIZ[i][0];
                    TFriPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
                }
            }else{
                TCalPINCH_COL[i][0] = VectorFriasMATRIZ[i][1];
                TCalPINCH_COL[i][1] = PuntodePliegue[0];
                TCalPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
                TFriPINCH_COL[i][0] = PuntodePliegue[0];
                TFriPINCH_COL[i][1] = VectorFriasMATRIZ[i][0];
                TFriPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
            }
        }else if(PuntodePliegue[0] > VectorFriasMATRIZ[i][0] && PuntodePliegue[0] > VectorFriasMATRIZ[i][1]){
            TCalPINCH_COL[i][0] = 0;
            TCalPINCH_COL[i][1] = 0;
            TCalPINCH_COL[i][2] = 0;
            TFriPINCH_COL[i][0] = VectorFriasMATRIZ[i][1];
            TFriPINCH_COL[i][1] = VectorFriasMATRIZ[i][0];
            TFriPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
        }else if(PuntodePliegue[0] < VectorFriasMATRIZ[i][0] && PuntodePliegue[0] < VectorFriasMATRIZ[i][1]){
            TCalPINCH_COL[i][0] = VectorFriasMATRIZ[i][1];
            TCalPINCH_COL[i][1] = VectorFriasMATRIZ[i][0];
            TCalPINCH_COL[i][2] = VectorFriasMATRIZ[i][2];
            TFriPINCH_COL[i][0] = 0;
            TFriPINCH_COL[i][1] = 0;
            TFriPINCH_COL[i][2] = 0;
        }
    }
    double n = VectorCalientesMATRIZ.size() + VectorFriasMATRIZ.size();
    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                     QCP::iSelectLegend | QCP::iSelectItems | QCP::iSelectPlottables | QCP::iMultiSelect );
    ui->qcustomplot->xAxis->setRange(0, 12);
    ui->qcustomplot->yAxis->setRange(0, n*4);
    ui->qcustomplot->axisRect()->setupFullAxesBox();
    ui->qcustomplot->xAxis->setLabel("x Axis");
    ui->qcustomplot->yAxis->setLabel("y Axis");
    ui->qcustomplot->legend->setVisible(false);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->qcustomplot->legend->setFont(legendFont);
    ui->qcustomplot->legend->setSelectedFont(legendFont);
    ui->qcustomplot->legend->setSelectableParts(QCPLegend::spItems);

    //  PROBLEMA CALIENTE GRAFICACION

    QVector<double> x1,y1;
    x1.resize(2);
    y1.resize(2);
    x1[0] = 0;
    y1[0] = n*4;
    x1[1] = 5;
    y1[1] = n*4;
    contadorPlots = 0;
    int matrizPC = 0;
    //LadoCaliente;
    LadoCaliente.resize(n);
    for(int i = 0; i < LadoCaliente.size(); i++){
        LadoCaliente[i].resize(12);
    }
    for(int i = 0; i < TCalPINCH_HOT.size() ; i++){ // primero la parte del problema calientes
        if(TCalPINCH_HOT[i][0] == 0 && TCalPINCH_HOT[i][1] ==0 && TCalPINCH_HOT[i][2] ==0){
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::black));
            LadoCaliente[matrizPC][0] = contadorPlots;
            LadoCaliente[matrizPC][1] = 0; // caliente
            LadoCaliente[matrizPC][2] = 0; // Disponibilidad
            LadoCaliente[matrizPC][3] = 0; // Principal
            LadoCaliente[matrizPC][4] = 0; // Temperatura inicial
            LadoCaliente[matrizPC][5] = 0; // Temperatura final
            LadoCaliente[matrizPC][6] = 0; // Wcp
            LadoCaliente[matrizPC][7] = 0; // Energia
            LadoCaliente[matrizPC][8] = x1[0]; //
            LadoCaliente[matrizPC][9] = y1[0]; //
            LadoCaliente[matrizPC][10] = x1[1]; //
            LadoCaliente[matrizPC][11] = y1[1]; //

            QPointF posMed;
            posMed.setX((x1[0] + x1[1])/ 2);
            posMed.setY(y1[0]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posMed);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }else{
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::red));
            LadoCaliente[matrizPC][0] = contadorPlots;
            LadoCaliente[matrizPC][1] = 0; // caliente
            LadoCaliente[matrizPC][2] = 1; // Disponibilidad
            LadoCaliente[matrizPC][3] = 0; // Principal
            LadoCaliente[matrizPC][4] = TCalPINCH_HOT[i][0]; // Temperatura inicial
            LadoCaliente[matrizPC][5] = TCalPINCH_HOT[i][1]; // Temperatura final
            LadoCaliente[matrizPC][6] = TCalPINCH_HOT[i][2]; // WCP
            LadoCaliente[matrizPC][7] = qFabs(TCalPINCH_HOT[i][0]-TCalPINCH_HOT[i][1])*TCalPINCH_HOT[i][2]; // Disponible
            LadoCaliente[matrizPC][8] = x1[0]; //
            LadoCaliente[matrizPC][9] = y1[0]; //
            LadoCaliente[matrizPC][10] = x1[1]; //
            LadoCaliente[matrizPC][11] = y1[1]; //

            QPointF posIni,posFin;
            posIni.setX(x1[0]-.25);
            posIni.setY(y1[0]+.5);
            posFin.setX(x1[1]+.25);
            posFin.setY(y1[1]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posIni);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_HOT[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel2->position->setCoords(posFin);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_HOT[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }
    }
    for(int i = 0 ; i < TCalPINCH_COL.size() ; i++){
        if(TCalPINCH_COL[i][0] == 0 && TCalPINCH_COL[i][1] == 0 && TCalPINCH_COL[i][2] == 0){
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::black));


            LadoCaliente[matrizPC][0] = contadorPlots;
            LadoCaliente[matrizPC][1] = 1; // fria
            LadoCaliente[matrizPC][2] = 0; // Disponibilidad
            LadoCaliente[matrizPC][3] = 0; // Principal
            LadoCaliente[matrizPC][4] = 0; // Temperatura inicial
            LadoCaliente[matrizPC][5] = 0; // Temperatura final
            LadoCaliente[matrizPC][6] = 0; // Wcp
            LadoCaliente[matrizPC][7] = 0; // Energia
            LadoCaliente[matrizPC][8] = x1[0]; //
            LadoCaliente[matrizPC][9] = y1[0]; //
            LadoCaliente[matrizPC][10] = x1[1]; //
            LadoCaliente[matrizPC][11] = y1[1]; //
            QPointF posMed;
            posMed.setX((x1[0] + x1[1])/ 2);
            posMed.setY(y1[0]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posMed);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }else{
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::blue));
            LadoCaliente[matrizPC][0] = contadorPlots;
            LadoCaliente[matrizPC][1] = 1; // fria
            LadoCaliente[matrizPC][2] = 1; // Disponibilidad
            LadoCaliente[matrizPC][3] = 0; // Principal
            LadoCaliente[matrizPC][4] = TCalPINCH_COL[i][0]; // Temperatura inicial
            LadoCaliente[matrizPC][5] = TCalPINCH_COL[i][1]; // Temperatura final
            LadoCaliente[matrizPC][6] = TCalPINCH_COL[i][2]; // Wcp
            LadoCaliente[matrizPC][7] = qFabs(TCalPINCH_COL[i][0]-TCalPINCH_COL[i][1])*TCalPINCH_COL[i][2]; // Energia
            LadoCaliente[matrizPC][8] = x1[0]; //
            LadoCaliente[matrizPC][9] = y1[0]; //
            LadoCaliente[matrizPC][10] = x1[1]; //
            LadoCaliente[matrizPC][11] = y1[1]; //

            QPointF posIni,posFin;
            posIni.setX(x1[0]-.25);
            posIni.setY(y1[0]+.5);
            posFin.setX(x1[1]+.25);
            posFin.setY(y1[1]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posIni);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_COL[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel2->position->setCoords(posFin);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_COL[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }
    }

    //  PROBLEMA FRIO GRAFICACION

    QVector<double> x2,y2;
    x2.resize(2);
    y2.resize(2);
    x2[0] = 7;
    y2[0] = n*4;
    x2[1] = 12;
    y2[1] = n*4;
    //LadoFrio;
    LadoFrio.resize(n);
    for(int i = 0; i < LadoFrio.size(); i++){
        LadoFrio[i].resize(12);
    }
    matrizPC = 0;
    for(int i = 0; i < TFriPINCH_HOT.size() ; i++){ // primero la parte del problema calientes
        if(TFriPINCH_HOT[i][0] == 0 && TFriPINCH_HOT[i][1] == 0 && TFriPINCH_HOT[i][2] == 0){
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x2,y2);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::black));
            LadoFrio[matrizPC][0] = contadorPlots; //
            LadoFrio[matrizPC][1] = 0; // caliente
            LadoFrio[matrizPC][2] = 0; // disponible
            LadoFrio[matrizPC][3] = 0; // disponible
            LadoFrio[matrizPC][4] = 0; // Temperatura inicial
            LadoFrio[matrizPC][5] = 0; // Temperatura final
            LadoFrio[matrizPC][6] = 0; // Wcp
            LadoFrio[matrizPC][7] = 0; // Energie
            LadoFrio[matrizPC][8] = x2[0]; //
            LadoFrio[matrizPC][9] = y2[0]; //
            LadoFrio[matrizPC][10] = x2[1]; //
            LadoFrio[matrizPC][11] = y2[1]; //

            QPointF posMed;
            posMed.setX((x2[0] + x2[1])/ 2);
            posMed.setY(y2[0]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posMed);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }else{
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x2,y2);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::red));
            LadoFrio[matrizPC][0] = contadorPlots; //
            LadoFrio[matrizPC][1] = 0; // caliente
            LadoFrio[matrizPC][2] = 1; // disponible
            LadoFrio[matrizPC][3] = 0; // disponible
            LadoFrio[matrizPC][4] = TFriPINCH_HOT[i][0]; // Temperatura inicial
            LadoFrio[matrizPC][5] = TFriPINCH_HOT[i][1]; // Temperatura final
            LadoFrio[matrizPC][6] = TFriPINCH_HOT[i][2]; // Wcp
            LadoFrio[matrizPC][7] = qFabs(TFriPINCH_HOT[i][0] -TFriPINCH_HOT[i][1])*TFriPINCH_HOT[i][2]; // Energia
            LadoFrio[matrizPC][8] = x2[0]; //
            LadoFrio[matrizPC][9] = y2[0]; //
            LadoFrio[matrizPC][10] = x2[1]; //
            LadoFrio[matrizPC][11] = y2[1]; //

            QPointF posIni,posFin;
            posIni.setX(x2[0]-.25);
            posIni.setY(y2[0]+.5);
            posFin.setX(x2[1]+.25);
            posFin.setY(y2[1]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posIni);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_HOT[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel2->position->setCoords(posFin);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_HOT[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }
    }
    for(int i = 0 ; i < TFriPINCH_COL.size() ; i++){
        if(TFriPINCH_COL[i][0] == 0 && TFriPINCH_COL[i][1] == 0 && TFriPINCH_COL[i][2] == 0){
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x2,y2);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::black));
            LadoFrio[matrizPC][0] = contadorPlots; //
            LadoFrio[matrizPC][1] = 1; // fria
            LadoFrio[matrizPC][2] = 0; // disponible
            LadoFrio[matrizPC][3] = 0; // disponible
            LadoFrio[matrizPC][4] = 0; // Temperatura inicial
            LadoFrio[matrizPC][5] = 0; // Temperatura final
            LadoFrio[matrizPC][6] = 0; // Wcp
            LadoFrio[matrizPC][7] = 0; // Energia
            LadoFrio[matrizPC][8] = x2[0]; //
            LadoFrio[matrizPC][9] = y2[0]; //
            LadoFrio[matrizPC][10] = x2[1]; //
            LadoFrio[matrizPC][11] = y2[1]; //

            QPointF posMed;
            posMed.setX((x2[0] + x2[1])/ 2);
            posMed.setY(y2[0]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posMed);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }else{
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setData(x2,y2);
            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
            ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::blue));
            LadoFrio[matrizPC][0] = contadorPlots; //
            LadoFrio[matrizPC][1] = 1; // fria
            LadoFrio[matrizPC][2] = 1; // disponible
            LadoFrio[matrizPC][3] = 0; // disponible
            LadoFrio[matrizPC][4] = TFriPINCH_COL[i][0]; // Temperatura inicial
            LadoFrio[matrizPC][5] = TFriPINCH_COL[i][1]; // Temperatura final
            LadoFrio[matrizPC][6] = TFriPINCH_COL[i][2]; // Wcp
            LadoFrio[matrizPC][7] = qFabs(TFriPINCH_COL[i][0] -TFriPINCH_COL[i][1])*TFriPINCH_COL[i][2]; // Energia
            LadoFrio[matrizPC][8] = x2[0]; //
            LadoFrio[matrizPC][9] = y2[0]; //
            LadoFrio[matrizPC][10] = x2[1]; //
            LadoFrio[matrizPC][11] = y2[1]; //

            QPointF posIni,posFin;
            posIni.setX(x2[0]-.25);
            posIni.setY(y2[0]+.5);
            posFin.setX(x2[1]+.25);
            posFin.setY(y2[1]+.5);
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel1->position->setCoords(posIni);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_COL[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
            textLabel2->position->setCoords(posFin);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_COL[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger

            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }
    }
    ui->qcustomplot->replot();
    connect(ui->qcustomplot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(ui->qcustomplot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->qcustomplot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    connect(ui->qcustomplot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->qcustomplot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->qcustomplot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->qcustomplot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->qcustomplot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->qcustomplot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    ui->qcustomplot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->qcustomplot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    qDebug() << LadoCaliente << "LadoCaliente";
    qDebug() << LadoFrio << "Ladofrio";
}

void plotterDN::on_Legend_clicked()
{
    if(ui->Legend->isChecked() == true){
        ui->qcustomplot->legend->setVisible(true);
    }else if(ui->Legend->isChecked() ==  false){
        ui->qcustomplot->legend->setVisible(false);
    }
    ui->qcustomplot->replot();
}

bool plotterDN::comparadortemperaturas(double A, double B, double C)
{
    if(A == B){
        return true;
    }else if(A == C){
        return true;
    }
    return false;
}

bool plotterDN::controlintercambio(double DTmin, double dif1, double dif2, double X1, double X2, double TobjC, double TobjF)
{
//    qDebug() << DTmin << dif1 << dif2 << X1 << X2 <<  TobjC << TobjF ;
    if(dif1 < DTmin){
        QMessageBox::warning(this,tr("Error"),tr("Violation of Delta T min"));
        return true;
    }else if(dif2 < DTmin){
        QMessageBox::warning(this,tr("Error"),tr("Violation of Delta T min"));
        return true;
    }else if(X2 < 0){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return true;
    }else if(X1 < TobjC ){
        QMessageBox::warning(this,tr("Error"),tr("Exceed hot target temperature"));
        return true;
    }else if(X2 > TobjF){
        QMessageBox::warning(this,tr("Error"),tr("Exceed cold target temperature"));
        return true;
    }
    return false;
}

void plotterDN::AuxiliaryService()
{
    QFile FileCostos(UNIFORM_DESIGNGRID_FILENAME);
    if (!FileCostos.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in30(&FileCostos);
    in30.setVersion(QDataStream::Qt_5_4);
    bool diverso2 = false, uniforme2 = false;
    QVector<double> Calentamiento2,Enfriamento2,OperationCost2;
    Calentamiento2.resize(10);
    Enfriamento2.resize(10);
    OperationCost2.resize(10);
    QVector<QVector<double>> CapitalCost2;
    CapitalCost2.resize(10);
    for(int i = 0; i < CapitalCost2.size(); i++){
        CapitalCost2[i].resize(10);
    }
    int CTo2 = 0, CCo2 = 0;
    double DTmin2 = 0;
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,
                          CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
    in30 >> VCUD;
    DTmin = VCUD.getDTmin();
    Calentamiento = VCUD.getCalentamiento();
    Enfriamento = VCUD.getEnfriamento();
    FileCostos.flush();
    FileCostos.close();
    if(ui->qcustomplot->selectedGraphs().size() > 2){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QVector<double> GraphSelect;
    QVector<double> Conexion;
    GraphSelect.resize(2);
    Conexion.resize(2);
    int j = 0;
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){
        QCPGraph * graph = ui->qcustomplot->graph(i);
        if(graph->selected()){
            GraphSelect[j]  = i;
            if(i+1  <= LadoCaliente.size()){
                if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 0){ // es caliente inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = 0 ; // Es caliente
                    Conexion[1] = i ; // Numero de corriente
                    break;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[0] = 1 ; // Es fria
                    Conexion[1] = i ; // Numero de corriente
                    break;
                }
            }else if(i+1 >= LadoCaliente.size() ){
                if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 0){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = 0 ; // Es caliente
                    Conexion[1] = i ; // Numero de corriente
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[0] = 1 ; // Es fria
                    Conexion[1] = i ; // Numero de corriente
                    break;
                }
            }
            j++;
        }
    }
    qDebug() << Conexion;
    if(control == true){
        return;  // es decir que esta mal
    }else if(control == false){
        if(Conexion[0] == 0){ //SE REQUIERE UN SERVICIO DE ENFRIAMENTO PARA CORRIENTE CALIENTE
            // ES CALIENTE
            qDebug() << Conexion[1];
            if(Conexion[1]  < LadoCaliente.size() ){ // LADO IZQUIERO
                qDebug() << "1";
                contadorSER = contadorSER + 1;
                contadorSERCAL = contadorSERCAL + 1;
                double corriente = Conexion[1]; // corriente caliente
                double Q1 = LadoCaliente[corriente][6]*(LadoCaliente[corriente][4]-LadoCaliente[corriente][5]) ;
                if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                    QVector<double> x1,y1;
                    x1.resize(2),y1.resize(2);
                    x1[0] = LadoCaliente[corriente][10]; // x
                    y1[0] = LadoCaliente[corriente][11]; // y
                    x1[1] = LadoCaliente[corriente][10]; // x
                    y1[1] = LadoCaliente[corriente][11]; // y
//                    qDebug() << "1";
                    QPen pen;
                    pen.setStyle(Qt::DashLine);
                    pen.setWidth(2);
                    pen.setColor(Qt::blue);
                    ui->qcustomplot->addGraph();
                    ui->qcustomplot->graph(contadorPlots)->setName(QString("Heating service %1").arg(QString::number(contadorSERCAL)));
                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                    ui->qcustomplot->replot();
                    contadorPlots++;
//                    qDebug() << "1";
                    //ALMACENA EN EL VECTOR SERVICIOES
                    Servicios.resize(contadorSER);
                    for(int i = 0; i < Servicios.size(); i++){
                        Servicios[i].resize(5);
                    }
//                    qDebug() << "1";
                    Servicios[contadorSER-1][0] = corriente;
//                    qDebug() << "1";
                    Servicios[contadorSER-1][1] = LadoCaliente[corriente][4]; // entrada caliente
//                    qDebug() << "1";
                    Servicios[contadorSER-1][2] = LadoCaliente[corriente][5]; // salida caliente
//                    qDebug() << "1";
                    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
//                    qDebug() << "1";
                    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida fria
//                    qDebug() << "1";
                }else if(Q1 <= 0){ // NO SE APLICA
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    return;
                }
            }else if(Conexion[1] >= LadoCaliente.size() ){ // LADO DERECHO
                qDebug() << "2";
                contadorSER = contadorSER + 1;
                contadorSERCAL = contadorSERCAL + 1;
                double corriente = Conexion[1] - LadoCaliente.size(); // corriente caliente
                qDebug() << LadoFrio;
                double Q1 = LadoFrio[corriente][6]*(LadoFrio[corriente][4]-LadoFrio[corriente][5]) ;

                qDebug() << corriente;
                if(Q1 > 0){
                    QVector<double> x1,y1;
                    x1.resize(2),y1.resize(2);
                    x1[0] = LadoFrio[corriente][10]; // x
                    y1[0] = LadoFrio[corriente][11]; // y
                    x1[1] = LadoFrio[corriente][10]; // x
                    y1[1] = LadoFrio[corriente][11]; // y
                    QPen pen;
                    pen.setStyle(Qt::DashLine);
                    pen.setWidth(2);
                    pen.setColor(Qt::blue);
                    ui->qcustomplot->addGraph();
                    ui->qcustomplot->graph(contadorPlots)->setName(QString("Heating service %1").arg(QString::number(contadorSERCAL)));
                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                    ui->qcustomplot->replot();
                    contadorPlots++;
                    //ALMACENA EN EL VECTOR SERVICIOES
                    Servicios.resize(contadorSER);
                    for(int i = 0; i < Servicios.size(); i++){
                        Servicios[i].resize(5);
                    }
                    Servicios[contadorSER-1][0] = corriente;
                    Servicios[contadorSER-1][1] = LadoFrio[corriente][4]; // entrada caliente
                    Servicios[contadorSER-1][2] = LadoFrio[corriente][5]; // salida caliente
                    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
                    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida fria
                }else if(Q1 <= 0){
                    QMessageBox::warning(this,tr("Error"), tr("Error"));
                    return;
                }
            }
        }else if(Conexion[0] == 1){ // SE REQUIERE UN SERVICIO DE CALENTAMIENTO PARA CORRIENTE FRIAS
            if(Conexion[1]  < LadoCaliente.size() ){ // LADO IZQUIERO
                contadorSER = contadorSER + 1;
                contadorSERFRI = contadorSERFRI + 1;
                double corriente = Conexion[1]; // corriente caliente
                double Q1 = LadoCaliente[corriente][6]*(LadoCaliente[corriente][4]-LadoCaliente[corriente][5]) ;
                if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                    QVector<double> x1,y1;
                    x1.resize(2),y1.resize(2);
                    x1[0] = LadoCaliente[corriente][8]; // x
                    y1[0] = LadoCaliente[corriente][9]; // y
                    x1[1] = LadoCaliente[corriente][8]; // x
                    y1[1] = LadoCaliente[corriente][9]; // y
                    QPen pen;
                    pen.setStyle(Qt::DashLine);
                    pen.setWidth(2);
                    pen.setColor(Qt::red);
                    ui->qcustomplot->addGraph();
                    ui->qcustomplot->graph(contadorPlots)->setName(QString("Heating service %1").arg(QString::number(contadorSERFRI)));
                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                    ui->qcustomplot->replot();
                    contadorPlots++;
                    //ALMACENA EN EL VECTOR SERVICIOES
                    Servicios.resize(contadorSER);
                    for(int i = 0; i < Servicios.size(); i++){
                        Servicios[i].resize(5);
                    }
                    Servicios[contadorSER-1][0] = corriente;
                    Servicios[contadorSER-1][1] = LadoCaliente[corriente][4]; // entrada caliente
                    Servicios[contadorSER-1][2] = LadoCaliente[corriente][5]; // salida caliente
                    Servicios[contadorSER-1][3] = Calentamiento[0]; // entrada caliente
                    Servicios[contadorSER-1][4] = Calentamiento[1]; // salida fria
                }else if(Q1 <= 0){ // NO SE APLICA
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    return;
                }
            }else if(Conexion[1] >= LadoCaliente.size() ){ // LADO DERECHO
                contadorSER = contadorSER + 1;
                contadorSERFRI = contadorSERFRI + 1;
                double corriente = Conexion[1] - LadoCaliente.size() ; // corriente caliente
                //qDebug() << corriente << LadoFrio[corriente][1] ;
                double Q1 = LadoFrio[corriente][6]*(LadoFrio[corriente][4]- LadoFrio[corriente][5]) ;
                //qDebug() << Q1;
                if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                    QVector<double> x1,y1;
                    x1.resize(2),y1.resize(2);
                    x1[0] = LadoFrio[corriente][8]; // x
                    y1[0] = LadoFrio[corriente][9]; // y
                    x1[1] = LadoFrio[corriente][8]; // x
                    y1[1] = LadoFrio[corriente][9]; // y
                    QPen pen;
                    pen.setStyle(Qt::DashLine);
                    pen.setWidth(2);
                    pen.setColor(Qt::red);
                    //qDebug() << "grafica";
                    ui->qcustomplot->addGraph();
                    ui->qcustomplot->graph(contadorPlots)->setName(QString("Heating service %1").arg(QString::number(contadorSERFRI)));
                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                    ui->qcustomplot->replot();
                    //qDebug() << "rploteo";
                    contadorPlots++;
                    //ALMACENA EN EL VECTOR SERVICIOES
                    Servicios.resize(contadorSER);
                    for(int i = 0; i < Servicios.size(); i++){
                        Servicios[i].resize(5);
                    }
                    Servicios[contadorSER-1][0] = corriente;
                    Servicios[contadorSER-1][1] = LadoFrio[corriente][4]; // entrada caliente
                    Servicios[contadorSER-1][2] = LadoFrio[corriente][5]; // salida caliente
                    Servicios[contadorSER-1][3] = Calentamiento[0]; // entrada caliente
                    Servicios[contadorSER-1][4] = Calentamiento[1]; // salida fria
                    return;
                }else if(Q1 <= 0){ // NO SE APLICA
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    return;
                }
            }
        }
    }
    qDebug() << Servicios;

}

void plotterDN::conectStream()
{
    QFile FileCostos(UNIFORM_DESIGNGRID_FILENAME);
    if (!FileCostos.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in30(&FileCostos);
    in30.setVersion(QDataStream::Qt_5_4);
    bool diverso2 = false, uniforme2 = false;
    QVector<double> Calentamiento2,Enfriamento2,OperationCost2;
    Calentamiento2.resize(10);
    Enfriamento2.resize(10);
    OperationCost2.resize(10);
    QVector<QVector<double>> CapitalCost2;
    CapitalCost2.resize(10);
    for(int i = 0; i < CapitalCost2.size(); i++){
        CapitalCost2[i].resize(10);
    }
    int CTo2 = 0, CCo2 = 0;
    double DTmin2 = 0;
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,
                          CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
    in30 >> VCUD;
    DTmin = VCUD.getDTmin();
    FileCostos.flush();
    FileCostos.close();
    QVector<double> GraphSelect;
    QVector<double> Conexion;
    GraphSelect.resize(2);
    Conexion.resize(2);
    int j = 0;
    if(ui->qcustomplot->selectedGraphs().size() == 1 || ui->qcustomplot->selectedGraphs().size() > 2){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){
        QCPGraph * graph = ui->qcustomplot->graph(i);
        if(graph->selected()){
            GraphSelect[j]  = i;
            if(i+1  <= LadoCaliente.size()){
                if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 0){ // es caliente inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = i ; // caliente
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[1] = i; //  fria
                }
            }else if(i+1 >= LadoCaliente.size() ){
                if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 0){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = i ; // caliente
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[1] = i; //  fria
                }
            }
            j++;
        }
    }
    if(control == true){
        return;  // es decir que esta mal
    }else if(control == false){
        if(Uniones.size() == 0){ // es decir que no se ha hecho uniones
            if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                double Caliente = Conexion[0]; // corriente caliente
                double Fria = Conexion[1]; // corriente fria
                if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria
                        //calculos
                        double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                        double X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
                        double dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                        double dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoCaliente[Caliente][8] + .4; // x
                            y1[0] = LadoCaliente[Caliente][9]; // y
                            x1[1] = LadoCaliente[Caliente][8] + .4; // x
                            y1[1] = LadoCaliente[Caliente][9] - 1; // y
                            x1[2] = LadoCaliente[Caliente][10] - .4; // x
                            y1[2] = LadoCaliente[Caliente][11] - 1; // y
                            x1[3] = LadoCaliente[Fria][10] - .4; // x
                            y1[3] = LadoCaliente[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoCaliente[Caliente][4]  = X1;
                            LadoCaliente[Fria][5] = X2;
                            for(int i = 0; i < LadoCaliente.size(); i++){
                                LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria

                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }
            }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size()){
                double Caliente = Conexion[0]- LadoCaliente.size();
                double Fria = Conexion[1]- LadoCaliente.size();
                if(LadoFrio[Caliente][3] == 0){ // es principal
                    if(LadoFrio[Fria][3] == 0){ // es principal fria
                        //calculos
                        double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                        double X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
                        double dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                        double dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoFrio[Caliente][8] + .4; // x
                            y1[0] = LadoFrio[Caliente][9]; // y
                            x1[1] = LadoFrio[Caliente][8] + .4; // x
                            y1[1] = LadoFrio[Caliente][9] - 1; // y
                            x1[2] = LadoFrio[Caliente][10] - .4; // x
                            y1[2] = LadoFrio[Caliente][11] - 1; // y
                            x1[3] = LadoFrio[Fria][10] - .4; // x
                            y1[3] = LadoFrio[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoFrio[Caliente][4]  = X1;
                            LadoFrio[Fria][5] = X2;
                            for(int i = 0; i < LadoFrio.size(); i++){
                                LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                LadoFrio[i][9] = LadoFrio[i][9]; // y
                                LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                LadoFrio[i][11]= LadoFrio[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoFrio[Fria][3] == 1){ // es subred fria

                    }
                }else if(LadoFrio[Fria][3] == 1){ // es subred
                    if(LadoFrio[Fria][3] == 0){ // es principal fria

                    }else if(LadoFrio[Fria][3] == 1){ // es subred fria

                    }
                }
            }
        }else if(Uniones.size() > 0){ // HAY MAS UNIONES
            for(int i = 0; i < Uniones.size(); i++){
                if(Conexion[0] == Uniones[i][0]){
                    if(Conexion[0] < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                        double Caliente = Conexion[0]; // corriente caliente
                        double Fria = Conexion[1]; // corriente fria
                        if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                            if(LadoCaliente[Fria][3] == 0){ // es principal fria
                                //calculos
                                double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                                double X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
                                double dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                                double dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                                if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                                    contadorUniones = contadorUniones + 1;
                                    QVector<double> x1,y1;
                                    espaciadoruniones = espaciadoruniones + 1;
                                    //MODIFICAR ESTAS UNIONES
                                    x1.resize(4),y1.resize(4);
                                    x1[0] = LadoCaliente[Caliente][8] + .4; // x
                                    y1[0] = LadoCaliente[Caliente][9]; // y
                                    x1[1] = LadoCaliente[Caliente][8] + .4; // x
                                    y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y // se debe modificar solamnte esta
                                    x1[2] = LadoCaliente[Caliente][10] - .4; // x
                                    y1[2] = LadoCaliente[Caliente][11] - 1 - espaciadoruniones ; // y // se debe modificar solamente esta
                                    x1[3] = LadoCaliente[Fria][10] - .4; // x
                                    y1[3] = LadoCaliente[Fria][11]; // y
                                    QPen pen;
                                    pen.setStyle(Qt::DashLine);
                                    pen.setWidth(2);
                                    pen.setColor(Qt::black);
                                    ui->qcustomplot->addGraph();
                                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                                    QPointF posIni,posFin;
                                    posIni.setX(x1[0] + .2);
                                    posIni.setY(y1[0] + .5);
                                    posFin.setX(x1[3] - .2);
                                    posFin.setY(y1[3] + .5);
                                    QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                                    textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel1->position->setCoords(posIni);
                                    textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                                    textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                                    textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel2->position->setCoords(posFin);
                                    textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                                    textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    ui->qcustomplot->replot();
                                    // ALMECENAJE EN EL VECTOR UNION
                                    Uniones.resize(contadorUniones);
                                    for(int i = 0; i < Uniones.size(); i++){
                                        Uniones[i].resize(8);
                                    }
                                    Uniones[contadorUniones-1][0] = Caliente;
                                    Uniones[contadorUniones-1][1] = Fria;
                                    Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                                    Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                                    Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                                    Uniones[contadorUniones-1][5] = X1; // salida caliente
                                    Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                                    Uniones[contadorUniones-1][7] = X2; // salida fria
                                    //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                                    LadoCaliente[Caliente][4]  = X1;
                                    LadoCaliente[Fria][5] = X2;
                                    for(int i = 0; i < LadoCaliente.size(); i++){
                                        LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                        LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                        LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                        LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                                    }
                                    contadorPlots++;
                                }else{
                                    return;
                                }
                            }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                            }
                        }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                            if(LadoCaliente[Fria][3] == 0){ // es principal fria

                            }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                            }
                        }
                        return;
                    }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size() ){
                        double Caliente = Conexion[0] - LadoCaliente.size();
                        double Fria = Conexion[1] - LadoCaliente.size();
                        if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                            if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                                //calculos
                                double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                                double X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
                                double dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                                double dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                                if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                                    contadorUniones = contadorUniones + 1;
                                    QVector<double> x1,y1;
                                    espaciadoruniones = espaciadoruniones + 1;
                                    //MODIFICAR ESTAS UNIONES
                                    x1.resize(4),y1.resize(4);
                                    x1[0] = LadoFrio[Caliente][8] + .4; // x
                                    y1[0] = LadoFrio[Caliente][9]; // y
                                    x1[1] = LadoFrio[Caliente][8] + .4; // x
                                    y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y // se debe modificar solamnte esta
                                    x1[2] = LadoFrio[Caliente][10] - .4; // x
                                    y1[2] = LadoFrio[Caliente][11] - 1 - espaciadoruniones ; // y // se debe modificar solamente esta
                                    x1[3] = LadoFrio[Fria][10] - .4; // x
                                    y1[3] = LadoFrio[Fria][11]; // y
                                    QPen pen;
                                    pen.setStyle(Qt::DashLine);
                                    pen.setWidth(2);
                                    pen.setColor(Qt::black);
                                    ui->qcustomplot->addGraph();
                                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                                    QPointF posIni,posFin;
                                    posIni.setX(x1[0] + .2);
                                    posIni.setY(y1[0] + .5);
                                    posFin.setX(x1[3] - .2);
                                    posFin.setY(y1[3] + .5);
                                    QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                                    textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel1->position->setCoords(posIni);
                                    textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                                    textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                                    textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel2->position->setCoords(posFin);
                                    textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                                    textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    ui->qcustomplot->replot();
                                    // ALMECENAJE EN EL VECTOR UNION
                                    Uniones.resize(contadorUniones);
                                    for(int i = 0; i < Uniones.size(); i++){
                                        Uniones[i].resize(8);
                                    }
                                    Uniones[contadorUniones-1][0] = Caliente;
                                    Uniones[contadorUniones-1][1] = Fria;
                                    Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                                    Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                                    Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                                    Uniones[contadorUniones-1][5] = X1; // salida caliente
                                    Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                                    Uniones[contadorUniones-1][7] = X2; // salida fria
                                    //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                                    LadoFrio[Caliente][4]  = X1;
                                    LadoFrio[Fria][5] = X2;
                                    for(int i = 0; i < LadoFrio.size(); i++){
                                        LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                        LadoFrio[i][9] = LadoFrio[i][9]; // y
                                        LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                        LadoFrio[i][11]= LadoFrio[i][11]; // y
                                    }
                                    contadorPlots++;
                                }else{
                                    return;
                                }
                            }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                            }
                        }else if(LadoFrio[Fria][3] == 1){ // es subred CALIENTE
                            if(LadoFrio[Fria][3] == 0){ // es principal FRIA

                            }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                            }
                        }
                        return;
                    }
                }
            } // NO HAY UNION REGISTRADA ENTONCES HACE ANALISIS
            espaciadoruniones = 0;
            if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                double Caliente = Conexion[0]; // corriente caliente
                double Fria = Conexion[1]; // corriente fria
                if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria
                        //calculos
                        double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                        double X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
                        double dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                        double dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoCaliente[Caliente][8] + .4; // x
                            y1[0] = LadoCaliente[Caliente][9]; // y
                            x1[1] = LadoCaliente[Caliente][8] + .4; // x
                            y1[1] = LadoCaliente[Caliente][9] - 1; // y
                            x1[2] = LadoCaliente[Caliente][10] - .4; // x
                            y1[2] = LadoCaliente[Caliente][11] - 1; // y
                            x1[3] = LadoCaliente[Fria][10] - .4; // x
                            y1[3] = LadoCaliente[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoCaliente[Caliente][4]  = X1;
                            LadoCaliente[Fria][5] = X2;
                            for(int i = 0; i < LadoCaliente.size(); i++){
                                LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria

                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }
            }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size() ){
                double Caliente = Conexion[0] - LadoCaliente.size();
                double Fria = Conexion[1] - LadoCaliente.size();
                if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                        //calculos
                        double X1 = QInputDialog::getDouble(this, "Specification", "Output temperature for hot stream:");
                        double X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
                        double dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                        double dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoFrio[Caliente][8] + .4; // x
                            y1[0] = LadoFrio[Caliente][9]; // y
                            x1[1] = LadoFrio[Caliente][8] + .4; // x
                            y1[1] = LadoFrio[Caliente][9] - 1; // y
                            x1[2] = LadoFrio[Caliente][10] - .4; // x
                            y1[2] = LadoFrio[Caliente][11] - 1; // y
                            x1[3] = LadoFrio[Fria][10] - .4; // x
                            y1[3] = LadoFrio[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoFrio[Caliente][4]  = X1;
                            LadoFrio[Fria][5] = X2;
                            for(int i = 0; i < LadoFrio.size(); i++){
                                LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                LadoFrio[i][9] = LadoFrio[i][9]; // y
                                LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                LadoFrio[i][11]= LadoFrio[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }else if(LadoFrio[Fria][3] == 1){ // es subred CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA

                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }
                return;
            }
        }

    }
}

void plotterDN::splitStream()
{
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){
        QCPGraph * graph = ui->qcustomplot->graph(i);
        if(graph->selected()){
            if(i+1 <= LadoCaliente.size()){
                //se reajusta las de lado caliente
                if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 0){ // es caliente inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 1){ // es caliente habilitada
                    control = false;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 1){ // es fria habilitada
                    control = false;
                }
            }else if(i+1 >= LadoCaliente.size()){
                //se reajusta las LadoFrio
                if(LadoFrio[i][1] == 0 && LadoFrio[i][2] == 0){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i][1] == 1 && LadoFrio[i][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i][1] == 0 && LadoFrio[i][2] == 1){ // es caliente habilitada
                    control = false;
                }else if(LadoFrio[i][1] == 1 && LadoFrio[i][2] == 1){ // es fria habilitada
                    control = false;
                }
            }
        }
    }
    if(control == true){
        return;
    }else if(control == false){

    }
}

void plotterDN::hideItemText()
{
    for(int i = 0; i <  ui->qcustomplot->itemCount() ; i++){
        QCPItemText *pItem = qobject_cast<QCPItemText*>(ui->qcustomplot->item(i));
        if(pItem->selected() ){
            pItem->setVisible(false);
        }
    }
    ui->qcustomplot->replot();
}

void plotterDN::conectdefault()
{
    QFile FileCostos(UNIFORM_DESIGNGRID_FILENAME);
    if (!FileCostos.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in30(&FileCostos);
    in30.setVersion(QDataStream::Qt_5_4);
    bool diverso2 = false, uniforme2 = false;
    QVector<double> Calentamiento2,Enfriamento2,OperationCost2;
    Calentamiento2.resize(10);
    Enfriamento2.resize(10);
    OperationCost2.resize(10);
    QVector<QVector<double>> CapitalCost2;
    CapitalCost2.resize(10);
    for(int i = 0; i < CapitalCost2.size(); i++){
        CapitalCost2[i].resize(10);
    }
    int CTo2 = 0, CCo2 = 0;
    double DTmin2 = 0;
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,
                          CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
    in30 >> VCUD;
    DTmin = VCUD.getDTmin();
    FileCostos.flush();
    FileCostos.close();
    QVector<double> GraphSelect;
    QVector<double> Conexion;
    GraphSelect.resize(2);
    Conexion.resize(2);
    int j = 0;
    if(ui->qcustomplot->selectedGraphs().size() == 1 || ui->qcustomplot->selectedGraphs().size() > 2){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){
        QCPGraph * graph = ui->qcustomplot->graph(i);
        if(graph->selected()){
            GraphSelect[j]  = i;
            if(i+1  <= LadoCaliente.size()){
                if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 0){ // es caliente inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoCaliente[i][1] == 0 && LadoCaliente[i][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = i ; // caliente
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[1] = i; //  fria
                }
            }else if(i+1 >= LadoCaliente.size() ){
                if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 0){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es caliente habilitada
                    control = false;
                    Conexion[0] = i ; // caliente
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es fria habilitada
                    control = false;
                    Conexion[1] = i; //  fria
                }
            }
            j++;
        }
    }
    if(control == true){
        return;
    }else if(control == false){
        if(Uniones.size() == 0){ // es decir que no se ha hecho uniones
            if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                double Caliente = Conexion[0]; // corriente caliente
                double Fria = Conexion[1]; // corriente fria
                if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria
                        //CALCULOS
                        double Q1 = LadoCaliente[Caliente][6]*(LadoCaliente[Caliente][4]-LadoCaliente[Caliente][5]) ;
                        double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
                        double X1,X2,dif1,dif2;
                        if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                          X1 = LadoCaliente[Caliente][5];
                          X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                          dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                          dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        }else if(Q2 > Q1){ // se trasmite lo mayor posible
                          X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
                          X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                          dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                          dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        }
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoCaliente[Caliente][8] + .4; // x
                            y1[0] = LadoCaliente[Caliente][9]; // y
                            x1[1] = LadoCaliente[Caliente][8] + .4; // x
                            y1[1] = LadoCaliente[Caliente][9] - 1; // y
                            x1[2] = LadoCaliente[Caliente][10] - .4; // x
                            y1[2] = LadoCaliente[Caliente][11] - 1; // y
                            x1[3] = LadoCaliente[Fria][10] - .4; // x
                            y1[3] = LadoCaliente[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoCaliente[Caliente][4]  = X1;
                            LadoCaliente[Fria][5] = X2;
                            for(int i = 0; i < LadoCaliente.size(); i++){
                                LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria

                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }
            }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size()){ //LADO FRIO
                double Caliente = Conexion[0] - LadoCaliente.size();
                double Fria = Conexion[1] - LadoCaliente.size();
                if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                        double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]- LadoFrio[Caliente][5]) ;
                        double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] -LadoFrio[Fria][5]);
                        double X1,X2,dif1,dif2;
                        if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                          X1 = LadoFrio[Caliente][5];
                          X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                          dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                          dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        }else if(Q2 > Q1){ // se trasmite lo mayor posible
                          X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
                          X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                          dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                          dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        }
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoFrio[Caliente][8] + .4; // x
                            y1[0] = LadoFrio[Caliente][9]; // y
                            x1[1] = LadoFrio[Caliente][8] + .4; // x
                            y1[1] = LadoFrio[Caliente][9] - 1; // y
                            x1[2] = LadoFrio[Caliente][10] - .4; // x
                            y1[2] = LadoFrio[Caliente][11] - 1; // y
                            x1[3] = LadoFrio[Fria][10] - .4; // x
                            y1[3] = LadoFrio[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoFrio[Caliente][4]  = X1;
                            LadoFrio[Fria][5] = X2;
                            for(int i = 0; i < LadoFrio.size(); i++){
                                LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                LadoFrio[i][9] = LadoFrio[i][9]; // y
                                LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                LadoFrio[i][11]= LadoFrio[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }else if(LadoFrio[Fria][3] == 1){ // es subred CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA

                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }
            }
        }else if(Uniones.size() > 0){ // hay mas uniones
            for(int i = 0; i < Uniones.size(); i++){
                if(Conexion[0] == Uniones[i][0]){
                    if(Conexion[0] < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                        double Caliente = Conexion[0]; // corriente caliente
                        double Fria = Conexion[1]; // corriente fria
                        if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                            if(LadoCaliente[Fria][3] == 0){ // es principal fria
                                //CALCULOS
                                double Q1 = LadoCaliente[Caliente][6]*(LadoCaliente[Caliente][4]-LadoCaliente[Caliente][5]) ;
                                double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
                                double X1,X2,dif1,dif2;
                                if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                                    X1 = LadoCaliente[Caliente][5];
                                    X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                                    dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                                    dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                                }else if(Q2 > Q1){ // se trasmite lo mayor posible
                                    X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) + LadoCaliente[Caliente][4] );
                                    X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                                    dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                                    dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                                }
                                if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                                    contadorUniones = contadorUniones + 1;
                                    QVector<double> x1,y1;
                                    espaciadoruniones = espaciadoruniones + 1;
                                    //MODIFICAR ESTAS UNIONES
                                    x1.resize(4),y1.resize(4);
                                    x1[0] = LadoCaliente[Caliente][8] + .4; // x
                                    y1[0] = LadoCaliente[Caliente][9]; // y
                                    x1[1] = LadoCaliente[Caliente][8] + .4; // x
                                    y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y // se debe modificar solamnte esta
                                    x1[2] = LadoCaliente[Caliente][10] - .4; // x
                                    y1[2] = LadoCaliente[Caliente][11] - 1 - espaciadoruniones ; // y // se debe modificar solamente esta
                                    x1[3] = LadoCaliente[Fria][10] - .4; // x
                                    y1[3] = LadoCaliente[Fria][11]; // y
                                    QPen pen;
                                    pen.setStyle(Qt::DashLine);
                                    pen.setWidth(2);
                                    pen.setColor(Qt::black);
                                    ui->qcustomplot->addGraph();
                                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                                    QPointF posIni,posFin;
                                    posIni.setX(x1[0] + .2);
                                    posIni.setY(y1[0] + .5);
                                    posFin.setX(x1[3] - .2);
                                    posFin.setY(y1[3] + .5);
                                    QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                                    textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel1->position->setCoords(posIni);
                                    textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                                    textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                                    textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel2->position->setCoords(posFin);
                                    textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                                    textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    ui->qcustomplot->replot();
                                    // ALMECENAJE EN EL VECTOR UNION
                                    Uniones.resize(contadorUniones);
                                    for(int i = 0; i < Uniones.size(); i++){
                                        Uniones[i].resize(8);
                                    }
                                    Uniones[contadorUniones-1][0] = Caliente;
                                    Uniones[contadorUniones-1][1] = Fria;
                                    Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                                    Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                                    Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                                    Uniones[contadorUniones-1][5] = X1; // salida caliente
                                    Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                                    Uniones[contadorUniones-1][7] = X2; // salida fria
                                    //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                                    LadoCaliente[Caliente][4]  = X1;
                                    LadoCaliente[Fria][5] = X2;
                                    for(int i = 0; i < LadoCaliente.size(); i++){
                                        LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                        LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                        LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                        LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                                    }
                                    contadorPlots++;
                                }else{
                                    return;
                                }
                            }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                            }
                        }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                            if(LadoCaliente[Fria][3] == 0){ // es principal fria

                            }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                            }
                        }
                        return;
                    }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size() ){
                        double Caliente = Conexion[0] - LadoCaliente.size();
                        double Fria = Conexion[1] - LadoCaliente.size();
                        if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                            if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                                double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]- LadoFrio[Caliente][5]) ;
                                double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] -LadoFrio[Fria][5]);
                                double X1,X2,dif1,dif2;
                                if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                                  X1 = LadoFrio[Caliente][5];
                                  X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                                  dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                                  dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                                }else if(Q2 > Q1){ // se trasmite lo mayor posible
                                  X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
                                  X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                                  dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                                  dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                                }
                                if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                                    contadorUniones = contadorUniones + 1;
                                    QVector<double> x1,y1;
                                    espaciadoruniones = espaciadoruniones + 1;
                                    //MODIFICAR ESTAS UNIONES
                                    x1.resize(4),y1.resize(4);
                                    x1[0] = LadoFrio[Caliente][8] + .4; // x
                                    y1[0] = LadoFrio[Caliente][9]; // y
                                    x1[1] = LadoFrio[Caliente][8] + .4; // x
                                    y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y // se debe modificar solamnte esta
                                    x1[2] = LadoFrio[Caliente][10] - .4; // x
                                    y1[2] = LadoFrio[Caliente][11] - 1 - espaciadoruniones ; // y // se debe modificar solamente esta
                                    x1[3] = LadoFrio[Fria][10] - .4; // x
                                    y1[3] = LadoFrio[Fria][11]; // y
                                    QPen pen;
                                    pen.setStyle(Qt::DashLine);
                                    pen.setWidth(2);
                                    pen.setColor(Qt::black);
                                    ui->qcustomplot->addGraph();
                                    ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                                    ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                                    ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                                    QPointF posIni,posFin;
                                    posIni.setX(x1[0] + .2);
                                    posIni.setY(y1[0] + .5);
                                    posFin.setX(x1[3] - .2);
                                    posFin.setY(y1[3] + .5);
                                    QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                                    textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel1->position->setCoords(posIni);
                                    textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                                    textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                                    textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                                    textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                                    textLabel2->position->setCoords(posFin);
                                    textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                                    textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                                    ui->qcustomplot->replot();
                                    // ALMECENAJE EN EL VECTOR UNION
                                    Uniones.resize(contadorUniones);
                                    for(int i = 0; i < Uniones.size(); i++){
                                        Uniones[i].resize(8);
                                    }
                                    Uniones[contadorUniones-1][0] = Caliente;
                                    Uniones[contadorUniones-1][1] = Fria;
                                    Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                                    Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                                    Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                                    Uniones[contadorUniones-1][5] = X1; // salida caliente
                                    Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                                    Uniones[contadorUniones-1][7] = X2; // salida fria
                                    //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                                    LadoFrio[Caliente][4]  = X1;
                                    LadoFrio[Fria][5] = X2;
                                    for(int i = 0; i < LadoFrio.size(); i++){
                                        LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                        LadoFrio[i][9] = LadoFrio[i][9]; // y
                                        LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                        LadoFrio[i][11]= LadoFrio[i][11]; // y
                                    }
                                    contadorPlots++;
                                }else{
                                    return;
                                }
                            }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                            }
                        }else if(LadoFrio[Fria][3] == 1){ // es subred CALIENTE
                            if(LadoFrio[Fria][3] == 0){ // es principal FRIA

                            }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                            }
                        }
                        return;
                    }
                }
            }// NO HAY UNION REGISTRADA ENTONCES HACE ANALISIS
            espaciadoruniones = 0;
            if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
                double Caliente = Conexion[0]; // corriente caliente
                double Fria = Conexion[1]; // corriente fria
                if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria
                        //CALCULOS
                        double Q1 = LadoCaliente[Caliente][6]*(LadoCaliente[Caliente][4]-LadoCaliente[Caliente][5]) ;
                        double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
                        double X1,X2,dif1,dif2;
                        if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                            X1 = LadoCaliente[Caliente][5];
                            X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                            dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                            dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        }else if(Q1 > Q2){ // se trasmite lo mayor posible
                            X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
                            X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
                            dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
                            dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
                        }
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoCaliente[Caliente][8] + .4; // x
                            y1[0] = LadoCaliente[Caliente][9]; // y
                            x1[1] = LadoCaliente[Caliente][8] + .4; // x
                            y1[1] = LadoCaliente[Caliente][9] - 1; // y
                            x1[2] = LadoCaliente[Caliente][10] - .4; // x
                            y1[2] = LadoCaliente[Caliente][11] - 1; // y
                            x1[3] = LadoCaliente[Fria][10] - .4; // x
                            y1[3] = LadoCaliente[Fria][11]; // y
                            x1[0] = LadoCaliente[Caliente][8] + .4; // x
                            y1[0] = LadoCaliente[Caliente][9]; // y
                            x1[1] = LadoCaliente[Caliente][8] + .4; // x
                            y1[1] = LadoCaliente[Caliente][9] - 1; // y
                            x1[2] = LadoCaliente[Caliente][10] - .4; // x
                            y1[2] = LadoCaliente[Caliente][11] - 1; // y
                            x1[3] = LadoCaliente[Fria][10] - .4; // x
                            y1[3] = LadoCaliente[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoCaliente[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoCaliente[Caliente][4]  = X1;
                            LadoCaliente[Fria][5] = X2;
                            for(int i = 0; i < LadoCaliente.size(); i++){
                                LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
                                LadoCaliente[i][9] = LadoCaliente[i][9]; // y
                                LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
                                LadoCaliente[i][11]= LadoCaliente[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }else if(LadoCaliente[Caliente][3] == 1){  // es subred caliente
                    if(LadoCaliente[Fria][3] == 0){ // es principal fria

                    }else if(LadoCaliente[Fria][3] == 1){ // es subred fria

                    }
                }
            }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size() ){
                double Caliente = Conexion[0] - LadoCaliente.size() ; // corriente caliente
                double Fria = Conexion[1] - LadoCaliente.size() ; // corriente fria
                if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                        double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]- LadoFrio[Caliente][5]) ;
                        double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] -LadoFrio[Fria][5]);
                        double X1,X2,dif1,dif2;
                        if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
                          X1 = LadoFrio[Caliente][5];
                          X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                          dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                          dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        }else if(Q2 > Q1){ // se trasmite lo mayor posible
                          X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
                          X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
                          dif1 = qFabs(X1 - LadoFrio[Fria][5] );
                          dif2 = qFabs(LadoFrio[Caliente][4] - X2);
                        }
                        if(controlintercambio(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
                            contadorUniones = contadorUniones + 1;
                            QVector<double> x1,y1;
                            x1.resize(4),y1.resize(4);
                            x1[0] = LadoFrio[Caliente][8] + .4; // x
                            y1[0] = LadoFrio[Caliente][9]; // y
                            x1[1] = LadoFrio[Caliente][8] + .4; // x
                            y1[1] = LadoFrio[Caliente][9] - 1; // y
                            x1[2] = LadoFrio[Caliente][10] - .4; // x
                            y1[2] = LadoFrio[Caliente][11] - 1; // y
                            x1[3] = LadoFrio[Fria][10] - .4; // x
                            y1[3] = LadoFrio[Fria][11]; // y
                            QPen pen;
                            pen.setStyle(Qt::DashLine);
                            pen.setWidth(2);
                            pen.setColor(Qt::black);
                            ui->qcustomplot->addGraph();
                            ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
                            ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
                            ui->qcustomplot->graph(contadorPlots)->setPen(pen);
                            QPointF posIni,posFin;
                            posIni.setX(x1[0] + .2);
                            posIni.setY(y1[0] + .5);
                            posFin.setX(x1[3] - .2);
                            posFin.setY(y1[3] + .5);
                            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
                            textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel1->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel1->position->setCoords(posIni);
                            textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
                            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
                            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
                            textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
                            textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
                            textLabel2->position->setCoords(posFin);
                            textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
                            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
                            ui->qcustomplot->replot();
                            // ALMECENAJE EN EL VECTOR UNION
                            Uniones.resize(contadorUniones);
                            for(int i = 0; i < Uniones.size(); i++){
                                Uniones[i].resize(8);
                            }
                            Uniones[contadorUniones-1][0] = Caliente;
                            Uniones[contadorUniones-1][1] = Fria;
                            Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
                            Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
                            Uniones[contadorUniones-1][4] = LadoFrio[Caliente][4]; // entrada caliente
                            Uniones[contadorUniones-1][5] = X1; // salida caliente
                            Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
                            Uniones[contadorUniones-1][7] = X2; // salida fria
                            //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
                            LadoFrio[Caliente][4]  = X1;
                            LadoFrio[Fria][5] = X2;
                            for(int i = 0; i < LadoFrio.size(); i++){
                                LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
                                LadoFrio[i][9] = LadoFrio[i][9]; // y
                                LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
                                LadoFrio[i][11]= LadoFrio[i][11]; // y
                            }
                            contadorPlots++;
                        }else{
                            return;
                        }
                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }else if(LadoFrio[Fria][3] == 1){ // es subred CALIENTE
                    if(LadoFrio[Fria][3] == 0){ // es principal FRIA

                    }else if(LadoFrio[Fria][3] == 1){ // es subred FRIA

                    }
                }
            }
        }
    }
}


