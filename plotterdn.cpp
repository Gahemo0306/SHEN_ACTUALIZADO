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
    espaciadorunionesSUB = 0;
    contadorDivisionesCalientes = 0;
    contadorDivisionesFrias = 0;
    contadorDivisionesTotal = 0;
    contadorTOTALIZQYDER = 0;
    contadorLADOIZQUIERDO = 0;
    contadorLADODERECHO = 0;
    contadorSEPARACIONES = 0;
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
  //double plotable = ui->qcustomplot->plottableCount() - ui->qcustomplot->graphCount();
  for( int i = 0; i < ui->qcustomplot->plottableCount(); i++){
      QCPAbstractPlottable * curva = ui->qcustomplot->plottable(i);
      QCPPlottableLegendItem *item = ui->qcustomplot->legend->itemWithPlottable(curva);
      if (item->selected() || curva->selected())
      {
        item->setSelected(true);
        //curva->setSelection(QCPDataSelection(curva->->data()->dataRange()));
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

void plotterDN::on_Legend_clicked()
{
    if(ui->Legend->isChecked() == true){
        ui->qcustomplot->legend->setVisible(true);
    }else if(ui->Legend->isChecked() ==  false){
        ui->qcustomplot->legend->setVisible(false);
    }
    ui->qcustomplot->replot();
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
    ui->qcustomplot->setInteractions(QCP::iSelectLegend | QCP::iSelectPlottables | QCP::iMultiSelect | QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes); //| QCP::iSelectItems
    ui->qcustomplot->xAxis->setRange(-1, 13);
    ui->qcustomplot->yAxis->setRange(0, n*6);
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
    int CONTADORCORRIENTES = 0;
    LadoCaliente.resize(n);
    CoorSerIZQ.resize(n);
    for(int i = 0; i < CoorSerIZQ.size(); i++){
        CoorSerIZQ[i].resize(2);
    }
    for(int i = 0; i < LadoCaliente.size(); i++){
        LadoCaliente[i].resize(13);
    }
    for(int i = 0; i < TCalPINCH_HOT.size() ; i++){ // primero la parte del problema calientes
        if(TCalPINCH_HOT[i][0] == 0 && TCalPINCH_HOT[i][1] ==0 && TCalPINCH_HOT[i][2] ==0){
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1;
            CoorSerIZQ[matrizPC][0] = x1[1];
            CoorSerIZQ[matrizPC][1] = y1[1];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName("Not available");
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
            LadoCaliente[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords(x1[0] + x1[1]/2, y1[0]+.5);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }else{
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1 ;
            CoorSerIZQ[matrizPC][0] = x1[1];
            CoorSerIZQ[matrizPC][1] = y1[1];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName(QString("Hot stream %1").arg(QString::number(CONTADORCORRIENTES)));
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
            LadoCaliente[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords(x1[0]-.25,y1[0]+.5);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_HOT[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setClipToAxisRect(true);
            textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel2->position->setCoords(x1[1]+.25,y1[1]+.5);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_HOT[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel2->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }
    }
    for(int i = 0 ; i < TCalPINCH_COL.size() ; i++){
        if(TCalPINCH_COL[i][0] == 0 && TCalPINCH_COL[i][1] == 0 && TCalPINCH_COL[i][2] == 0){
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1;
            CoorSerIZQ[matrizPC][0] = x1[0];
            CoorSerIZQ[matrizPC][1] = y1[0];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName("Not available");
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
            LadoCaliente[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords((x1[0] + x1[1])/ 2,y1[0]+.5);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }else{
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1;
            CoorSerIZQ[matrizPC][0] = x1[0];
            CoorSerIZQ[matrizPC][1] = y1[0];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName(QString("Cold stream %1").arg(QString::number(CONTADORCORRIENTES)));
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
            LadoCaliente[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords(x1[0]-.25,y1[0]+.5);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_COL[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setClipToAxisRect(true);
            textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());//currentLayer());
            textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel2->position->setCoords(x1[1]+.25,y1[1]+.5);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TCalPINCH_COL[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel2->setPadding(QMargins(8, 0, 0, 0));

            matrizPC++;
            contadorPlots++;
            y1[0] = y1[0] - 3;
            y1[1] = y1[1] - 3;
        }
    }
    contadorLADOIZQUIERDO = TCalPINCH_HOT.size() + TCalPINCH_COL.size();
    //  PROBLEMA FRIO GRAFICACION
    CONTADORCORRIENTES = 0;
    QVector<double> x2,y2;
    x2.resize(2);
    y2.resize(2);
    x2[0] = 7;
    y2[0] = n*4;
    x2[1] = 12;
    y2[1] = n*4;
    //LadoFrio;
    LadoFrio.resize(n);
    CoorSerDER.resize(n);
    for(int i = 0; i < CoorSerDER.size(); i++){
        CoorSerDER[i].resize(2);
    }
    for(int i = 0; i < LadoFrio.size(); i++){
        LadoFrio[i].resize(13);
    }
    matrizPC = 0;
    for(int i = 0; i < TFriPINCH_HOT.size() ; i++){ // primero la parte del problema calientes
        if(TFriPINCH_HOT[i][0] == 0 && TFriPINCH_HOT[i][1] == 0 && TFriPINCH_HOT[i][2] == 0){
            CONTADORCORRIENTES = CONTADORCORRIENTES +1 ;
            CoorSerDER[matrizPC][0] = x2[1];
            CoorSerDER[matrizPC][1] = y2[1];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName("Not available");
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
            LadoFrio[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords((x2[0] + x2[1])/ 2,y2[0]+.5);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }else{
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1;
            CoorSerDER[matrizPC][0] = x2[1];
            CoorSerDER[matrizPC][1] = y2[1];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName(QString("Hot stream %1").arg(QString::number(CONTADORCORRIENTES)));
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
            LadoFrio[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords(x2[0]-.25,y2[0]+.5);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_HOT[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            textLabel2->setClipToAxisRect(true);
            textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());//currentLayer());
            textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel2->position->setCoords(x2[1]+.25,y2[1]+.5);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_HOT[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel2->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }
    }
    for(int i = 0 ; i < TFriPINCH_COL.size() ; i++){
        if(TFriPINCH_COL[i][0] == 0 && TFriPINCH_COL[i][1] == 0 && TFriPINCH_COL[i][2] == 0){
            CONTADORCORRIENTES = CONTADORCORRIENTES +1;
            CoorSerDER[matrizPC][0] = x2[0];
            CoorSerDER[matrizPC][1] = y2[0];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName("Not available");
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
            LadoFrio[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords((x2[0] + x2[1])/ 2,y2[0]+.5);
            textLabel1->setText("Not available");
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }else{
            CONTADORCORRIENTES = CONTADORCORRIENTES + 1 ;
            CoorSerDER[matrizPC][0] = x2[0];
            CoorSerDER[matrizPC][1] = y2[0];
            ui->qcustomplot->addGraph();
            ui->qcustomplot->graph(contadorPlots)->setName(QString("Cold stream %1").arg(QString::number(CONTADORCORRIENTES)));
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
            LadoFrio[matrizPC][12] = 0 ; // Si es que aun no se une despues de separarse
            QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
            //textLabel1->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel1->setClipToAxisRect(true);
            textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
            textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel1->position->setCoords(x2[0]-.25,y2[0]+.5);
            textLabel1->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_COL[i][0])));
            textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel1->setPadding(QMargins(8, 0, 0, 0));
            QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
            //textLabel2->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
            textLabel2->setClipToAxisRect(true);
            textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
            textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());//currentLayer());
            textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
            textLabel2->position->setCoords(x2[1]+.25,y2[1]+.5);
            textLabel2->setText(QString("%1 ªF").arg(QString::number(TFriPINCH_COL[i][1])));
            textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
            textLabel2->setPadding(QMargins(8, 0, 0, 0));
            matrizPC++;
            contadorPlots++;
            y2[0] = y2[0] - 3;
            y2[1] = y2[1] - 3;
        }
    }
    contadorLADODERECHO = TFriPINCH_HOT.size() + TFriPINCH_COL.size();
    contadorTOTALIZQYDER = contadorLADODERECHO + contadorLADOIZQUIERDO;
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

bool plotterDN::controlintercambioIZQ(double DTmin, double dif1, double dif2, double X1, double X2, double TobjC, double TobjF)
{
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

bool plotterDN::controlintercambioDER(double DTmin, double dif1, double dif2, double X1, double X2, double TobjC, double TobjF)
{
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

bool plotterDN::ifpasomitadIZQ(double posicionCal, double posicionFri)
{
    if(posicionCal >= posicionFri){
        return true;
    }
    return false;
}

bool plotterDN::ifpasomitadDER(double posicionCal, double posicionFri)
{
    if(posicionCal >= posicionFri){
        return true;
    }
    return false;
}

bool plotterDN::ifisinUnion(double Conexion)
{
    for(int i  = 0 ;  i < Uniones.size(); i++){
        if(Conexion == Uniones[i][0]){
            return true;
        }
    }
    return false;
}

bool plotterDN::ifissplittedCAL(double Corriente)
{
    //COMENZAR BUSQUEDA EN SEPARACIONES CALIENTES
    if(SeparacionesCalientes.size() > 0 ){
        for(int i = 0 ; i < SeparacionesCalientes.size() ; i++){
            if(SeparacionesCalientes[i][1] == Corriente){
                return true;
            }
        }
    }
    return false;
}

bool plotterDN::ifissplittedFRI(double Corriente)
{
    // COMENZAR BUSQUEDA EN SEPARACIONES FRIAS
    if(SeparacionesFrias.size() > 0){
        for( int  i = 0; i < SeparacionesFrias.size(); i ++){
            if(SeparacionesFrias[i][1] == Corriente){
                return true;
            }
        }
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
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
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
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){ //HAY QUE CAMBAIR ESTO
        QCPGraph * graph = ui->qcustomplot->graph(i); //MODIFICADO LOS CICLOS IF EN i+1 <=
        if(graph->selected()){
            GraphSelect[j]  = i;
            if(i  < contadorLADOIZQUIERDO){
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
            }else if(i >= contadorLADOIZQUIERDO && i < contadorTOTALIZQYDER){
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
            }else if(i > contadorTOTALIZQYDER){
                return;
            }
            j++;
        }
    }
    if(control == true){
        return;  // es decir que esta mal
    }else if(control == false){
        if(Conexion[0] == 0){ //SE REQUIERE UN SERVICIO DE ENFRIAMENTO PARA CORRIENTE CALIENTE
            // ES CALIENTE
            if(Conexion[1]  < contadorLADOIZQUIERDO ){ // LADO IZQUIERO
                double corriente = Conexion[1]; // corriente caliente
                if(ifissplittedCAL(corriente) == true){
                    double CalTemProm = 0, CalCoin = 0;
                    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
                        if(SeparacionesCalientes[i][1] == corriente){
                            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
                            CalCoin = CalCoin + 1;
                        }
                    }
                    double TemCal = CalTemProm/CalCoin;
                    double Q1 = LadoCaliente[corriente][6]*qFabs(TemCal - LadoCaliente[corriente][5]) ;
                    if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                        IZQ_cooling_split_Si(corriente,TemCal,Q1);
                    }else if(Q1 <= 0){ // NO SE APLICA
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }else{
                    double Q1 = LadoCaliente[corriente][6]*qFabs(LadoCaliente[corriente][4]-LadoCaliente[corriente][5]) ;
                    if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                        IZQ_cooling_split_No(corriente,Q1);
                    }else if(Q1 <= 0){ // NO SE APLICA
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }
            }else if(Conexion[1] >= contadorLADOIZQUIERDO   && Conexion[1] < contadorTOTALIZQYDER){ // LADO DERECHO
                double corriente = Conexion[1] - LadoCaliente.size(); // corriente caliente
                if(ifissplittedCAL(corriente) == true){
                    double CalTemProm = 0, CalCoin = 0;
                    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
                        if(SeparacionesCalientes[i][1] == corriente){
                            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
                            CalCoin = CalCoin + 1;
                        }
                    }
                    double TemCal = CalTemProm/CalCoin;
                    double Q1 = LadoFrio[corriente][6]*(TemCal -LadoFrio[corriente][5]) ;
                    if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                        DER_cooling_split_Si(corriente,TemCal,Q1);
                    }else if(Q1 <= 0){ // NO SE APLICA
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }else{
                    double Q1 = LadoFrio[corriente][6]*(LadoFrio[corriente][4] -LadoFrio[corriente][5]) ;
                    if(Q1 > 0){ // SE APLICA EL SERVICIO DE CALENTAMIENTO
                        DER_cooling_split_No(corriente,Q1);
                    }else if(Q1 <= 0){ // NO SE APLICA
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }
            }
        }else if(Conexion[0] == 1){ // SE REQUIERE UN SERVICIO DE CALENTAMIENTO PARA CORRIENTE FRIAS
            if(Conexion[1]  < LadoCaliente.size() ){ // LADO IZQUIERO
                double corriente = Conexion[1]; // corriente caliente
                if(ifissplittedFRI(corriente) == true){
                    double FriTemProm = 0, FriCoin = 0;
                    for(int i = 0; i < SeparacionesFrias.size() ; i++){
                        if(SeparacionesFrias[i][1] == corriente){
                            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
                            FriCoin = FriCoin + 1;
                        }
                    }
                    double TemFri = FriTemProm/FriCoin;
                    double Q1 = LadoCaliente[corriente][6]*qFabs(TemFri - LadoCaliente[corriente][5]);
                    if(Q1 > 0){
                        IZQ_heating_split_Si(corriente,TemFri,Q1);
                    }else if(Q1 <= 0 ){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }

                }else{
                    double Q1 = LadoCaliente[corriente][6]*qFabs(LadoCaliente[corriente][4]-LadoCaliente[corriente][5]) ;
                    if(Q1 > 0){
                        IZQ_heating_split_No(corriente,Q1);
                    }else if(Q1 <= 0 ){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }

                }
            }else if(Conexion[1] >= LadoCaliente.size() ){ // LADO DERECHO
                double corriente = Conexion[1]  - LadoCaliente.size() ; // corriente caliente
                if(ifissplittedFRI(corriente) == true){
                    double FriTemProm = 0, FriCoin = 0;
                    for(int i = 0; i < SeparacionesFrias.size() ; i++){
                        if(SeparacionesFrias[i][1] == corriente){
                            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
                            FriCoin = FriCoin + 1;
                        }
                    }
                    double TemFri = FriTemProm/FriCoin;
                    double Q1 = LadoFrio[corriente][6]*qFabs(TemFri - LadoFrio[corriente][5]);
                    if(Q1 > 0){
                        DER_heating_split_Si(corriente,TemFri,Q1);
                    }else if(Q1 <= 0 ){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }else{
                    double Q1 = LadoFrio[corriente][6]*qFabs(LadoFrio[corriente][4]-LadoFrio[corriente][5]) ;
                    if(Q1 > 0){
                        DER_heating_split_No(corriente,Q1);
                    }else if(Q1 <= 0 ){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                }
            }
        }
    }
}

void plotterDN::splitStream()
{
    QVector<double>  corriente;
    corriente.resize(2);
    for(int i = 0; i < ui->qcustomplot->graphCount(); i++){
        QCPGraph * graph = ui->qcustomplot->graph(i);
        if(graph->selected()){
            if(i < LadoCaliente.size()){
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
                    corriente[0] = 0 ; // es caliente
                    corriente[1] = i ; // numero de corriente
                    control = false;
                }else if(LadoCaliente[i][1] == 1 && LadoCaliente[i][2] == 1){ // es fria habilitada
                    corriente[0] = 1 ; // es caliente
                    corriente[1] = i ; // numero de corriente
                    control = false;
                }
            }else if(i >= LadoCaliente.size()){
                //se reajusta las LadoFrio
                if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 0){
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 0){ // es fria inhabilitada
                    QMessageBox::warning(this,tr("Error"),tr("Error"));
                    control = true;
                    break;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 0 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es caliente habilitada
                    corriente[0] = 0 ; // es caliente
                    corriente[1] = i ; // numero de corriente
                    control = false;
                }else if(LadoFrio[i-LadoCaliente.size()][1] == 1 && LadoFrio[i-LadoCaliente.size()][2] == 1){ // es fria habilitada
                    corriente[0] = 1 ; // es fria
                    corriente[1] = i ; // numero de corriente
                    control = false;
                }
            }
        }
    }
    if(control == true){
        return;
    }else if(control == false){
        if(corriente[1] < contadorLADOIZQUIERDO  ){ //ESTA DE LADO IZQUIERDO
            bool ok;
            double Divisiones = QInputDialog::getDouble(this,"Specification","Number of divisions for stream:",0,0,9999999,4,&ok);
            if(ok){
                if(corriente[0] == 0){ // es caliente
                    double Corriente = corriente[1];
                    SeparacionesCalIZQ(Corriente,Divisiones);
                    //TENGO QUE ACTUALIZAR LOS PUNTOS DE GRAFICACION PARA LAS CORRIENTES CALIENTES
                }else if(corriente[0] == 1){ // es fria
                    double Corriente = corriente[1];
                    SeparacionesFriIZQ(Corriente,Divisiones);
                }
            }else{
                return;
            }
        }else if(corriente[1] >= contadorLADOIZQUIERDO && corriente[1] < contadorTOTALIZQYDER){ //ESTA DE LADO DERECHO
            bool ok;
            double Divisiones = QInputDialog::getDouble(this,"Specification","Number of divisions for stream:",0,0,9999999,4,&ok);
            if(ok){
                if(corriente[0] == 0){ // es caliente
                    double stream = corriente[1] - contadorLADODERECHO;
                    SeparacionesCalDER(stream,Divisiones);
                }else if(corriente[0] == 1){ // es fria
                    double stream = corriente[1] - contadorLADOIZQUIERDO;
                    SeparacionesFriDER(stream,Divisiones);
                }
            }else{
                return;
            }
        }else if(corriente[1] >= contadorTOTALIZQYDER){//TAMBIEN SE DEBE DE HACER POR SI UNA UNION SE QUIERE DIVIDIR AUN AUN MAS

        }
    }
}

void plotterDN::SeparacionesCalIZQ(double Corriente, double Divisiones)
{
    contadorDivisionesCalientes = contadorDivisionesCalientes + Divisiones;
    SeparacionesCalientes.resize(contadorDivisionesCalientes);
    for(int i = 0; i < SeparacionesCalientes.size(); i++){
        SeparacionesCalientes[i].resize(10);
    }
    double contadorY = 0;
    double distancia = (LadoCaliente[Corriente][10] - LadoCaliente[Corriente][8])/ Divisiones ;
    QVector<double> x1,y1;
    x1.resize(4),y1.resize(4); //CORREGIR LA DISTANCIA DE LAS DIVISIONES
    double contadorWCP = LadoCaliente[Corriente][6];
    for(int i = contadorDivisionesCalientes - Divisiones; i < contadorDivisionesCalientes; i++){
        contadorSEPARACIONES = contadorSEPARACIONES + 1 ;
        contadorY = contadorY - .65;
        x1[0] = LadoCaliente[Corriente][8] + .1;// x
        y1[0] = LadoCaliente[Corriente][9]; // y
        x1[1] = LadoCaliente[Corriente][8] + .5; // x
        y1[1] = LadoCaliente[Corriente][9] + contadorY; // y
        x1[2] = LadoCaliente[Corriente][8] + distancia; // x
        y1[2] = LadoCaliente[Corriente][9] + contadorY; // y
        x1[3] = LadoCaliente[Corriente][8] + distancia + .5 ; //- .1; // x
        y1[3] = LadoCaliente[Corriente][9]; // y
        ui->qcustomplot->addGraph();
        ui->qcustomplot->graph(contadorPlots)->setName(QString("Sub stream %1").arg(QString::number(contadorSEPARACIONES)));
        ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
        ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::red));
        bool ok;
        double Wcp = QInputDialog::getDouble(this,"Specification",QString("Current value of Wcp: %1, select a value for sub stream:").
                                             arg(QString::number(contadorWCP)),0,0,9999999999,4,&ok);
        if(ok){
            contadorWCP = contadorWCP - Wcp;
        }else{ // se divide sobre el numero de divisiones
            Wcp =contadorWCP/Divisiones;
            contadorWCP = contadorWCP - Wcp;
        }
        SeparacionesCalientes[i][0] = contadorPlots; //NUMERO DE CORRIENTE
        SeparacionesCalientes[i][1] = Corriente; //CORRIENTE MADRE
        SeparacionesCalientes[i][2] = 0; //CALIENTE
        SeparacionesCalientes[i][3] = LadoCaliente[Corriente][4]; //Temperatura entrada
        SeparacionesCalientes[i][4] = LadoCaliente[Corriente][5]; //
        SeparacionesCalientes[i][5] = Wcp; //Wcp LadoCaliente[Caliente][6]
        SeparacionesCalientes[i][6] = x1[1]; //x
        SeparacionesCalientes[i][7] = y1[1]; //y
        SeparacionesCalientes[i][8] = x1[2]; //x
        SeparacionesCalientes[i][9] = y1[2]; //y
        contadorPlots++;
    }
    // ACTUALIZACION DE LAS CORRIENTES CALIENTES
    LadoCaliente[Corriente][8] = x1[3];
    LadoCaliente[Corriente][9] = y1[3];
    ui->qcustomplot->replot();
}

void plotterDN::SeparacionesFriIZQ(double Corriente, double Divisiones)
{
    contadorDivisionesFrias = contadorDivisionesFrias + Divisiones;
    SeparacionesFrias.resize(contadorDivisionesFrias);
    for(int i = 0; i < SeparacionesFrias.size(); i++){
        SeparacionesFrias[i].resize(10);
    }
    double contadorY = 0;
    double distancia = (LadoCaliente[Corriente][10] - LadoCaliente[Corriente][8])/ Divisiones;
    QVector<double> x1,y1;
    x1.resize(4),y1.resize(4);
    double contadorWCP = LadoCaliente[Corriente][6];
    for(int i = contadorDivisionesFrias - Divisiones; i < contadorDivisionesFrias; i++){
        contadorSEPARACIONES = contadorSEPARACIONES + 1 ;
        contadorY = contadorY - .65;
        x1[0] = LadoCaliente[Corriente][10] - .1;// x
        y1[0] = LadoCaliente[Corriente][11]; // y
        x1[1] = LadoCaliente[Corriente][10] - .5; // x
        y1[1] = LadoCaliente[Corriente][11] + contadorY; // y
        x1[2] = LadoCaliente[Corriente][10] -  distancia;// LadoCaliente[Corriente][10] - .5; // x
        y1[2] = LadoCaliente[Corriente][11] + contadorY; // y
        x1[3] = LadoCaliente[Corriente][10] - distancia - .5;  //- .1; // x
        y1[3] = LadoCaliente[Corriente][11]; // y
        ui->qcustomplot->addGraph();
        ui->qcustomplot->graph(contadorPlots)->setName(QString("Sub stream %1").arg(QString::number(contadorSEPARACIONES)));
        ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
        ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::blue));
        bool ok;
        double Wcp = QInputDialog::getDouble(this,"Specification",QString("Current value of Wcp: %1, select a value for sub stream:").
                                             arg(QString::number(contadorWCP)),0,0,9999999999,4,&ok);
        if(ok){
            contadorWCP = contadorWCP - Wcp;
        }else{ // se divide sobre el numero de divisiones
            Wcp = contadorWCP/Divisiones;
            contadorWCP = contadorWCP - Wcp;
        }
        SeparacionesFrias[i][0] = contadorPlots; //NUMERO DE CORRIENTE
        SeparacionesFrias[i][1] = Corriente; //CORRIENTE MADRE
        SeparacionesFrias[i][2] = 0; //CALIENTE
        SeparacionesFrias[i][3] = LadoCaliente[Corriente][4]; //Temperatura salida
        SeparacionesFrias[i][4] = LadoCaliente[Corriente][5]; //Temperatura entrada
        SeparacionesFrias[i][5] = Wcp; //Wcp LadoCaliente[Caliente][6]
        SeparacionesFrias[i][6] = x1[1]; //x
        SeparacionesFrias[i][7] = y1[1]; //y
        SeparacionesFrias[i][8] = x1[2]; //x
        SeparacionesFrias[i][9] = y1[2]; //y
        contadorPlots++;
    }
    // ACTUALIZACION DE LAS CORRIENTES CALIENTES
    LadoCaliente[Corriente][10] = x1[3];
    LadoCaliente[Corriente][11] = y1[3];
    ui->qcustomplot->replot();
}

void plotterDN::SeparacionesCalDER(double Corriente, double Divisiones)
{
    double stream = Corriente;
    contadorDivisionesCalientes = contadorDivisionesCalientes + Divisiones;
    SeparacionesCalientes.resize(contadorDivisionesCalientes);
    for(int i = 0; i < SeparacionesCalientes.size(); i++){
        SeparacionesCalientes[i].resize(10);
    }
    double contadorY = 0;
    double distancia = (LadoFrio[stream][10] - LadoFrio[stream][8])/ Divisiones ;
    QVector<double> x1,y1;
    x1.resize(4),y1.resize(4);
    double contadorWCP = LadoFrio[stream][6];
    for(int i = contadorDivisionesCalientes - Divisiones; i < contadorDivisionesCalientes; i++){
        contadorSEPARACIONES = contadorSEPARACIONES + 1 ;
        contadorY = contadorY - .65;
        x1[0] = LadoFrio[stream][8] + .1;//LadoFrio[stream][8] + .1;// x
        y1[0] = LadoFrio[stream][9];//LadoFrio[stream][9]; // y
        x1[1] = LadoFrio[stream][8] + .5;//LadoFrio[stream][8] + .5; // x
        y1[1] = LadoFrio[stream][9] + contadorY;//LadoFrio[stream][9] + contadorY; // y
        x1[2] = LadoFrio[stream][8] + distancia;//LadoFrio[stream][10] - .5; // x
        y1[2] = LadoFrio[stream][9] + contadorY;//LadoFrio[stream][11] + contadorY; // y
        x1[3] = LadoFrio[stream][8] + distancia + .5;//LadoFrio[stream][10] - .1; // x
        y1[3] = LadoFrio[stream][9];//LadoFrio[stream][11]; // y
        ui->qcustomplot->addGraph();
        ui->qcustomplot->graph(contadorPlots)->setName(QString("Sub stream %1").arg(QString::number(contadorSEPARACIONES)));
        ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
        ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::red));
        bool ok;
        double Wcp = QInputDialog::getDouble(this,"Specification",QString("Current value of Wcp: %1, select a value for sub stream:").
                                             arg(QString::number(contadorWCP )),0,0,9999999999,4,&ok);
        if(ok){
            contadorWCP = contadorWCP - Wcp;
        }else{ // se divide sobre el numero de divisiones
            Wcp = contadorWCP/Divisiones;
            contadorWCP = contadorWCP - Wcp;
        }
        SeparacionesCalientes[i][0] = contadorPlots; //NUMERO DE CORRIENTE PRINCIPAL
        SeparacionesCalientes[i][1] = stream; //CORRIENTE MADRE
        SeparacionesCalientes[i][2] = 0; //CALIENTE
        SeparacionesCalientes[i][3] = LadoFrio[stream][4]; //Temperatura entrada
        SeparacionesCalientes[i][4] = LadoFrio[stream][5]; //Temperatura entrada
        SeparacionesCalientes[i][5] = Wcp; //Wcp LadoCaliente[Caliente][6]
        SeparacionesCalientes[i][6] = x1[1]; //x
        SeparacionesCalientes[i][7] = y1[1]; //y
        SeparacionesCalientes[i][8] = x1[2]; //x
        SeparacionesCalientes[i][9] = y1[2]; //y
        contadorPlots++;
    }
    LadoFrio[stream][8] = x1[3];
    LadoFrio[stream][9] = y1[3];
    ui->qcustomplot->replot();
}

void plotterDN::SeparacionesFriDER(double Corriente, double Divisiones)
{
    double stream = Corriente;
    contadorDivisionesFrias = contadorDivisionesFrias + Divisiones;
    SeparacionesFrias.resize(contadorDivisionesFrias);
    for(int i = 0; i < SeparacionesFrias.size(); i++){
        SeparacionesFrias[i].resize(9);
    }
    double contadorY = 0;
    double distancia = LadoFrio[stream][10] - LadoFrio[stream][8]/ Divisiones ;
    QVector<double> x1,y1;
    x1.resize(4),y1.resize(4);
    double contadorWCP = LadoFrio[stream][6];
    for(int i = contadorDivisionesFrias - Divisiones; i < contadorDivisionesFrias; i++){
        contadorSEPARACIONES = contadorSEPARACIONES + 1 ;
        contadorY = contadorY - .65;
        x1[0] = LadoFrio[stream][10] - .1;//LadoFrio[stream][8] + .1;// x
        y1[0] = LadoFrio[stream][11];//LadoFrio[stream][9]; // y
        x1[1] = LadoFrio[stream][10] - .5;//LadoFrio[stream][8] + .5; // x
        y1[1] = LadoFrio[stream][11] + contadorY;//LadoFrio[stream][9] + contadorY; // y
        x1[2] = LadoFrio[stream][10] - distancia;//LadoFrio[stream][10] - .5; // x
        y1[2] = LadoFrio[stream][11] + contadorY;//LadoFrio[stream][11] + contadorY; // y
        x1[3] = LadoFrio[stream][10] - distancia - .5;//LadoFrio[stream][10] - .1; // x
        y1[3] = LadoFrio[stream][11];//LadoFrio[stream][11]; // y
        ui->qcustomplot->addGraph();
        ui->qcustomplot->graph(contadorPlots)->setName(QString("Sub stream %1").arg(QString::number(contadorSEPARACIONES)));
        ui->qcustomplot->graph(contadorPlots)->setData(x1,y1);
        ui->qcustomplot->graph(contadorPlots)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        ui->qcustomplot->graph(contadorPlots)->setPen(QPen(Qt::blue));

        bool ok;
        double Wcp = QInputDialog::getDouble(this,"Specification",QString("Current value of Wcp: %1, select a value for sub stream:").
                                             arg(QString::number( contadorWCP)),0,0,9999999999,4,&ok);
        if(ok){
            contadorWCP = contadorWCP - Wcp;
        }else{ // se divide sobre el numero de divisiones
            Wcp =  contadorWCP/Divisiones;
             contadorWCP = contadorWCP - Wcp;
        }
        SeparacionesFrias[i][0] = contadorPlots; //NUMERO DE CORRIENTE PRINCIPAL
        SeparacionesFrias[i][1] = stream; //CORRIENTE MADRE
        SeparacionesFrias[i][2] = 0; //CALIENTE
        SeparacionesFrias[i][3] = LadoFrio[stream][4]; //Temperatura entrada
        SeparacionesFrias[i][4] = LadoFrio[stream][5]; //Temperatura entrada
        SeparacionesFrias[i][5] = Wcp; //Wcp LadoCaliente[Caliente][6]
        SeparacionesFrias[i][6] = x1[1]; //x
        SeparacionesFrias[i][7] = y1[1]; //y
        SeparacionesFrias[i][8] = x1[2]; //x
        SeparacionesFrias[i][9] = y1[2]; //y
        contadorPlots++;
    }
    LadoFrio[stream][10] = x1[3];
    LadoFrio[stream][11] = y1[3];
    ui->qcustomplot->replot();
}

void plotterDN::IZQ_cooling_split_Si(double Corriente, double Temp,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERCAL = contadorSERCAL + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Cooling service %1").arg(QString::number(contadorSERCAL)));
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Temp; // entrada caliente
    Servicios[contadorSER-1][2] = LadoCaliente[Corriente][5]; // salida fria
    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida caliente
}

void plotterDN::DER_cooling_split_Si(double Corriente, double Temp,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERCAL = contadorSERCAL + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Cooling service %1").arg(QString::number(contadorSERCAL)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Temp; // entrada caliente
    Servicios[contadorSER-1][2] = LadoFrio[Corriente][5]; // salida fria
    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida caliente
}

void plotterDN::IZQ_heating_split_Si(double Corriente, double Temp,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERFRI = contadorSERFRI + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Heating service %1").arg(QString::number( contadorSERFRI)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Calentamiento[0]; // entrada caliente
    Servicios[contadorSER-1][2] = Calentamiento[1]; // salida caliente
    Servicios[contadorSER-1][3] = Temp; // entrada caliente
    Servicios[contadorSER-1][4] = LadoCaliente[Corriente][4]; // salida fria
}

void plotterDN::DER_heating_split_Si(double Corriente, double Temp,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERFRI = contadorSERFRI + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Heating service %1").arg(QString::number( contadorSERFRI)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Calentamiento[0]; // entrada caliente
    Servicios[contadorSER-1][2] = Calentamiento[1]; // salida caliente
    Servicios[contadorSER-1][3] = Temp; // entrada caliente
    Servicios[contadorSER-1][4] = LadoFrio[Corriente][4]; // salida fria
}

void plotterDN::IZQ_cooling_split_No(double Corriente,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERCAL = contadorSERCAL + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Cooling service %1").arg(QString::number(contadorSERCAL)));
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = LadoCaliente[Corriente][4]; // entrada caliente
    Servicios[contadorSER-1][2] = LadoCaliente[Corriente][5]; // salida fria
    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida caliente
}

void plotterDN::DER_cooling_split_No(double Corriente,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERCAL = contadorSERCAL + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::blue);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Cooling service %1").arg(QString::number(contadorSERCAL)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = LadoFrio[Corriente][4]; // entrada caliente
    Servicios[contadorSER-1][2] = LadoFrio[Corriente][5]; // salida fria
    Servicios[contadorSER-1][3] = Enfriamento[0]; // entrada caliente
    Servicios[contadorSER-1][4] = Enfriamento[1]; // salida caliente
}

void plotterDN::IZQ_heating_split_No(double Corriente,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERFRI = contadorSERFRI + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerIZQ[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerIZQ[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Heating service %1").arg(QString::number( contadorSERFRI)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Calentamiento[0]; // entrada caliente
    Servicios[contadorSER-1][2] = Calentamiento[1]; // salida caliente
    Servicios[contadorSER-1][3] = LadoCaliente[Corriente][5]; // entrada caliente
    Servicios[contadorSER-1][4] = LadoCaliente[Corriente][4]; // salida fria
}

void plotterDN::DER_heating_split_No(double Corriente,double Q)
{
    contadorSER = contadorSER + 1;
    contadorSERFRI = contadorSERFRI + 1;
    QVector<double> x1,y1;
    x1.resize(2),y1.resize(2);
    x1[0] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[0] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    x1[1] = CoorSerDER[Corriente][0];//LadoCaliente[corriente][10]; // x
    y1[1] = CoorSerDER[Corriente][1];//LadoCaliente[corriente][11]; // y
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
    union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
    union1->setName(QString("Heating service %1").arg(QString::number( contadorSERFRI)) );
    union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
    union1->addData(x1,y1);
    union1->setPen(pen);
    ui->qcustomplot->replot();
    //ALMACENA EN EL VECTOR SERVICIOES
    Servicios.resize(contadorSER);
    ENERGIA_SERVICIOS.resize(contadorSER);
    ENERGIA_SERVICIOS[contadorSER-1] = Q;
    for(int i = 0; i < Servicios.size(); i++){
        Servicios[i].resize(5);
    }
    Servicios[contadorSER-1][0] = Corriente;
    Servicios[contadorSER-1][1] = Calentamiento[0]; // entrada caliente
    Servicios[contadorSER-1][2] = Calentamiento[1]; // salida caliente
    Servicios[contadorSER-1][3] = LadoFrio[Corriente][5]; // entrada caliente
    Servicios[contadorSER-1][4] = LadoFrio[Corriente][4]; // salida fria
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
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
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
            if(i  < contadorLADOIZQUIERDO){
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
            }else if(i >= contadorLADOIZQUIERDO && i < contadorTOTALIZQYDER ){
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
            }else if(i >= contadorTOTALIZQYDER){ //busca en el vector de separaciones
                qDebug() << "Entro";
                QVector<double> CALIENTE, FRIA;
                CALIENTE.resize(4);
                FRIA.resize(4);
                //PRIMERO BUSCAMOS EN EL VECTOR PRINCIPAL CALIENTE
                //BUSCAMOS EN EL VECTOR DE SEPARACIONES CALIENTES
                //BUSCAMOS EN EL VECTOR DE PRINCIPALES FRIAS
                //BUSCAMOS EN EL VECTOR DE SEPARACIONES FRIAS
                for(int k = 0 ; k < ui->qcustomplot->graphCount(); k++ ){
                    QCPGraph * graph2 = ui->qcustomplot->graph(k);
                    if(graph2->selected()){
                        if( k < contadorLADOIZQUIERDO ){
                            for(int j = 0; j < LadoCaliente.size(); j++){
                                if(LadoCaliente[j][0] == k){
                                    CALIENTE[0] = 0;//ES PRINCIPAL        LadoCaliente[j][0]; // NUMERO DE CORRIENTE
                                    CALIENTE[1] = LadoCaliente[j][0]; // Corriente
                                    CALIENTE[2] = 0; //CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < LadoCaliente.size(); j++){
                                if(LadoCaliente[j][0] == k){
                                    FRIA[0] = 0;
                                    FRIA[1] = LadoCaliente[j][0] ; //ES PRINCIPAL
                                    FRIA[2] = 0 ; //CORRIENTE MADRE
                                    FRIA[3] = k ;
                                }
                            }
                        }else if(k >= contadorLADOIZQUIERDO && k < contadorTOTALIZQYDER){
                            for(int j = 0; j < LadoFrio.size(); j++){
                                if(LadoFrio[j][0] == k){
                                    CALIENTE[0] = 0;//ES PRINCIPAL        LadoCaliente[j][0]; // NUMERO DE CORRIENTE
                                    CALIENTE[1] = LadoFrio[j][0]; // Corriente
                                    CALIENTE[2] = 0; //CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < LadoFrio.size(); j++){
                                if(LadoFrio[j][0] == k){
                                    FRIA[0] = 0;
                                    FRIA[1] = LadoFrio[j][0] ; //ES PRINCIPAL
                                    FRIA[2] = 0 ; //CORRIENTE MADRE
                                    FRIA[3] = k ;
                                }
                            }
                        }else if(k >= contadorTOTALIZQYDER){
                            for(int j = 0; j < SeparacionesCalientes.size(); j++){
                                if(SeparacionesCalientes[j][0] == k){
                                    CALIENTE[0] = 1;  // subred de la madre caliente Numero de corriente
                                    CALIENTE[1] = SeparacionesCalientes[j][1]; // ES SUBRED
                                    CALIENTE[1] = 1; // CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < SeparacionesFrias.size(); j++){
                                if(SeparacionesFrias[j][0] == k){
                                    FRIA[0] = 1; //subred
                                    FRIA[1] = SeparacionesFrias[j][1] ; //ES PRINCIPAL
                                    FRIA[2] = 1;
                                    FRIA[3] = k ;
                                }
                            }
                        }
                    }
                }
                qDebug() << CALIENTE << FRIA;
                PrimeraOPC_plotsubstream(CALIENTE,FRIA);
                //LLAMAMOS RUTINA PARA GRAFICA DE LADO IZQUIERDO O DERECHO
                return; //SALIMOS TOTALMENTE DEL CICLO
            }
            j++;
        }
    }
    if(control == true){
        return;  // es decir que esta mal
    }else if(control == false){ // TODAS SON PRINCIPALES
        if(ifisinUnion(Conexion[0]) == true){
            espaciadoruniones = espaciadoruniones + .65;
        }
        if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
            double Caliente = Conexion[0]; // corriente caliente
            double Fria = Conexion[1]; // corriente fria
            if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                if(LadoCaliente[Fria][3] == 0){ // es principal fria
                    //calculos
                    if(ifissplittedCAL(Caliente) == true){ // ES QUE ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){ //ES QUE ESTA SEPARADA
                            if(LadoCaliente[Caliente][12] == 0){
                                //SE SACA EL PROMEDIO DE LAS SUBCORRIENTES Y SE HACE LA PRIMERA UNION
                                //SE ACTUALIZA DE 0 A 1 LadoCaliente[Caliente][12]
                                if(LadoCaliente[Fria][12] == 0){
                                    IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoCaliente[Fria][12] == 1){
                                    IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }else if(LadoCaliente[Caliente][12] == 1){
                                if(LadoCaliente[Fria][12] == 0){
                                    IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoCaliente[Fria][12] == 1){
                                    IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }
                        }else{ // NO ESTA SEPARADA PERO CALIENTE SI
                            // COMO NO ESTA SEPARADA LA FRIA POR LO TANTO ES 1
                            if(LadoCaliente[Caliente][12] == 0){
                                IZQ_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoCaliente[Caliente][12] == 1){
                                IZQ_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }
                    }else{ // NO ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){
                            //SI ESTA SEPARADA LA FRIA
                            if(LadoCaliente[Fria][12] == 0){
                                IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoCaliente[Fria][12] == 1){
                                IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }

                        }else{ // NINGUNA ESTA SEPARADA
                            IZQ_PrimeraOPC_sinSeparacionPrincipales(DTmin,Caliente,Fria,espaciadoruniones);
                            return;
                        }
                    }
                }
            }
        }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size()){
            double Caliente = Conexion[0]- LadoCaliente.size();
            double Fria = Conexion[1]- LadoCaliente.size();
            if(LadoFrio[Caliente][3] == 0){ // es principal
                if(LadoFrio[Fria][3] == 0){ // es principal fria
                    if(ifissplittedCAL(Caliente) == true){ // ES QUE ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){ //ES QUE ESTA SEPARADA
                            if(LadoFrio[Caliente][12] == 0){
                                //SE SACA EL PROMEDIO DE LAS SUBCORRIENTES Y SE HACE LA PRIMERA UNION
                                //SE ACTUALIZA DE 0 A 1 LadoCaliente[Caliente][12]
                                if(LadoFrio[Fria][12] == 0){
                                    DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoFrio[Fria][12] == 1){
                                    DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }else if(LadoFrio[Caliente][12] == 1){
                                if(LadoFrio[Fria][12] == 0){
                                    DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoFrio[Fria][12] == 1){
                                    DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }
                        }else{ // NO ESTA SEPARADA PERO CALIENTE SI
                            // COMO NO ESTA SEPARADA LA FRIA POR LO TANTO ES 1
                            if(LadoFrio[Caliente][12] == 0){
                                DER_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoFrio[Caliente][12] == 1){
                                DER_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }
                    }else{ // NO ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){
                            //SI ESTA SEPARADA LA FRIA
                            if(LadoFrio[Fria][12] == 0){
                                DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoFrio[Fria][12] == 1){
                                DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }else{ // NINGUNA ESTA SEPARADA
                            DER_PrimeraOPC_sinSeparacionPrincipales(DTmin,Caliente,Fria,espaciadoruniones);
                            return;
                        }
                    }
                }
            }
        }
    }
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
    VecCostUniDesGri VCUD(uniforme2,diverso2,Calentamiento2,Enfriamento2,CapitalCost2,OperationCost2,DTmin2,CTo2,CCo2);
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
            if(i  < contadorLADOIZQUIERDO ){
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
            }else if(i >= contadorLADOIZQUIERDO && i < contadorTOTALIZQYDER ){
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
            }else if(i >= contadorTOTALIZQYDER ){
                qDebug() << "Entro";
                QVector<double> CALIENTE, FRIA;
                CALIENTE.resize(4);
                FRIA.resize(4);
                //PRIMERO BUSCAMOS EN EL VECTOR PRINCIPAL CALIENTE
                //BUSCAMOS EN EL VECTOR DE SEPARACIONES CALIENTES
                //BUSCAMOS EN EL VECTOR DE PRINCIPALES FRIAS
                //BUSCAMOS EN EL VECTOR DE SEPARACIONES FRIAS
                for(int k = 0 ; k < ui->qcustomplot->graphCount(); k++ ){
                    QCPGraph * graph2 = ui->qcustomplot->graph(k);
                    qDebug() << k;
                    if(graph2->selected()){
                        if( k < contadorLADOIZQUIERDO ){
                            for(int j = 0; j < LadoCaliente.size(); j++){
                                if(LadoCaliente[j][0] == k){
                                    CALIENTE[0] = 0;//ES PRINCIPAL        LadoCaliente[j][0]; // NUMERO DE CORRIENTE
                                    CALIENTE[1] = LadoCaliente[j][0]; // Corriente
                                    CALIENTE[2] = 0; //CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < LadoCaliente.size(); j++){
                                if(LadoCaliente[j][0] == k){
                                    FRIA[0] = 0;
                                    FRIA[1] = LadoCaliente[j][0] ; //ES PRINCIPAL
                                    FRIA[2] = 0 ; //CORRIENTE MADRE
                                    FRIA[3] = k ;
                                }
                            }
                        }else if(k >= contadorLADOIZQUIERDO && k < contadorTOTALIZQYDER){
                            for(int j = 0; j < LadoFrio.size(); j++){
                                if(LadoFrio[j][0] == k){
                                    CALIENTE[0] = 0;//ES PRINCIPAL        LadoCaliente[j][0]; // NUMERO DE CORRIENTE
                                    CALIENTE[1] = LadoFrio[j][0]; // Corriente
                                    CALIENTE[2] = 0; //CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < LadoFrio.size(); j++){
                                if(LadoFrio[j][0] == k){
                                    FRIA[0] = 0;
                                    FRIA[1] = LadoFrio[j][0] ; //ES PRINCIPAL
                                    FRIA[2] = 0 ; //CORRIENTE MADRE
                                    FRIA[3] = k ;
                                }
                            }
                        }else if(k >= contadorTOTALIZQYDER){
                            for(int j = 0; j < SeparacionesCalientes.size(); j++){
                                if(SeparacionesCalientes[j][0] == k){
                                    CALIENTE[0] = 1;  // subred de la madre caliente Numero de corriente
                                    CALIENTE[1] = SeparacionesCalientes[j][1]; // ES SUBRED
                                    CALIENTE[1] = 1; // CORRIENTE MADRE
                                    CALIENTE[3] = k;
                                }
                            }
                            for(int j = 0; j < SeparacionesFrias.size(); j++){
                                if(SeparacionesFrias[j][0] == k){
                                    FRIA[0] = 1; //subred
                                    FRIA[1] = SeparacionesFrias[j][1] ; //ES PRINCIPAL
                                    FRIA[2] = 1;
                                    FRIA[3] = k ;
                                }
                            }
                        }
                    }
                }
                qDebug() << CALIENTE << FRIA;
                SegundaOPC_plotsubstream(CALIENTE,FRIA);
                //LLAMAMOS RUTINA PARA GRAFICA DE LADO IZQUIERDO O DERECHO
                return; //SALIMOS TOTALMENTE DEL CICLO
            }
            j++;
        }
    }
    if(control == true){
        return;
    }else if(control == false){
        if(ifisinUnion(Conexion[0]) == true){
            espaciadoruniones = espaciadoruniones + .65;
        }
        if(Conexion[0]  < LadoCaliente.size() && Conexion[1] < LadoCaliente.size() ){
            double Caliente = Conexion[0]; // corriente caliente
            double Fria = Conexion[1]; // corriente fria
            if(LadoCaliente[Caliente][3] == 0 ){ // es principal caliente
                if(LadoCaliente[Fria][3] == 0){ // es principal fria
                    if(ifissplittedCAL(Caliente) == true){ // ES QUE ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){ //ES QUE ESTA SEPARADA
                            if(LadoCaliente[Caliente][12] == 0){
                                //SE SACA EL PROMEDIO DE LAS SUBCORRIENTES Y SE HACE LA PRIMERA UNION
                                //SE ACTUALIZA DE 0 A 1 LadoCaliente[Caliente][12]
                                if(LadoCaliente[Fria][12] == 0){
                                    IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoCaliente[Fria][12] == 1){
                                    IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }else if(LadoCaliente[Caliente][12] == 1){
                                if(LadoCaliente[Fria][12] == 0){
                                    IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoCaliente[Fria][12] == 1){
                                    IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }
                        }else{ // NO ESTA SEPARADA PERO CALIENTE SI
                            // COMO NO ESTA SEPARADA LA FRIA POR LO TANTO ES 1
                            if(LadoCaliente[Caliente][12] == 0){
                                IZQ_SegundaOPC_Caliente_SI_0_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoCaliente[Caliente][12] == 1){
                                IZQ_SegundaOPC_Caliente_SI_1_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }
                    }else{ // NO ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){
                            //SI ESTA SEPARADA LA FRIA
                            if(LadoCaliente[Fria][12] == 0){
                                IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoCaliente[Fria][12] == 1){
                                IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }else{ // NINGUNA ESTA SEPARADA
                            IZQ_SegundaOPC_sinSeparacionPrincipales(DTmin,Caliente,Fria,espaciadoruniones);
                            return;
                        }
                    }
                }
            }
        }else if(Conexion[0] >= LadoCaliente.size()  && Conexion[1] >= LadoCaliente.size()){ //LADO FRIO
            double Caliente = Conexion[0] - LadoCaliente.size();
            double Fria = Conexion[1] - LadoCaliente.size();
            if(LadoFrio[Caliente][3] == 0){ // es principal CALIENTE
                if(LadoFrio[Fria][3] == 0){ // es principal FRIA
                    if(ifissplittedCAL(Caliente) == true){ // ES QUE ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){ //ES QUE ESTA SEPARADA
                            if(LadoFrio[Caliente][12] == 0){
                                //SE SACA EL PROMEDIO DE LAS SUBCORRIENTES Y SE HACE LA PRIMERA UNION
                                //SE ACTUALIZA DE 0 A 1 LadoCaliente[Caliente][12]
                                if(LadoFrio[Fria][12] == 0){
                                    DER_SegundaOPC_Caliente_SI_0_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoFrio[Fria][12] == 1){
                                    DER_SegundaOPC_Caliente_SI_0_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }else if(LadoFrio[Caliente][12] == 1){
                                if(LadoFrio[Fria][12] == 0){
                                    DER_SegundaOPC_Caliente_SI_1_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }else if(LadoFrio[Fria][12] == 1){
                                    DER_SegundaOPC_Caliente_SI_1_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                    return;
                                }
                            }
                        }else{ // NO ESTA SEPARADA PERO CALIENTE SI
                            // COMO NO ESTA SEPARADA LA FRIA POR LO TANTO ES 1
                            if(LadoFrio[Caliente][12] == 0){
                                DER_SegundaOPC_Caliente_SI_0_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoFrio[Caliente][12] == 1){
                                DER_SegundaOPC_Caliente_SI_1_FRIA_NO_X(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }
                    }else{ // NO ESTA SEPARADA
                        if(ifissplittedFRI(Fria) == true){
                            //SI ESTA SEPARADA LA FRIA
                            if(LadoFrio[Fria][12] == 0){
                                DER_SegundaOPC_Caliente_NO_X_FRIA_SI_0(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }else if(LadoFrio[Fria][12] == 1){
                                DER_SegundaOPC_Caliente_NO_X_FRIA_SI_1(DTmin,Caliente,Fria,espaciadoruniones);
                                return;
                            }
                        }else{ // NINGUNA ESTA SEPARADA
                            DER_SegundaOPC_sinSeparacionPrincipales(DTmin,Caliente,Fria,espaciadoruniones);
                            return;
                        }
                    }
                }
            }
        }
    }
}

//FALTA ACOMPLETAR ESTE CODIGO
void plotterDN::PrimeraOPC_plotsubstream(QVector<double> Caliente, QVector<double> Fria)
{
    if(Caliente[3] < contadorLADOIZQUIERDO){ // ES PRINCIPAL LADO IZQUIERDO
        //POR DEFAULTR LA CALIENTE ES PRINCIPAL
        //LA CORRIENTE FRIA PUEDE SER PRINCIPAL O SUBRED
        double subfria;
        for(int i = 0; i < SeparacionesFrias.size() ; i++){
            if(SeparacionesFrias[i][0] == Fria[3]){
                subfria = i;
                break;
            }
        }
        double CorrienteCaliente = Caliente[3];
        //SE CONECTA DE PRINCIPAL A SUBRED PERO SABER SI ESTA SEPARADA O NO
        if(ifissplittedCAL(CorrienteCaliente) == true){
            IZQ_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }else{
            IZQ_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }
    }else if(Caliente[3] >= contadorLADOIZQUIERDO && Caliente[3] < contadorTOTALIZQYDER){ // ES PRINCIPAL LADO DERECHO
        double subfria;
        for(int i = 0; i < SeparacionesFrias.size() ; i++){
            if(SeparacionesFrias[i][0] == Fria[3]){
                subfria = i;
            }
        }
        double CorrienteCaliente = Caliente[3];
        if(ifissplittedCAL(CorrienteCaliente) == true){
            DER_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }else{
            DER_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }
    }else if(Caliente[3] >= contadorTOTALIZQYDER){ // PROVIENE DE UNA SUB CORRIENTE LA UNION AQUI DEBO DE PROGRAMAR
        double subcaliente;
        for(int i = 0; i < SeparacionesCalientes.size() ; i++){
            if(SeparacionesCalientes[i][0] == Caliente[3] ){
                subcaliente = i;
                break;
            }
        }   
        if(Fria[0] == 0){// es principal
            double CorrienteFria = Fria[3];
            if(Fria[3] < contadorLADOIZQUIERDO){ //DE LADO IZQUIERDO
                if(ifissplittedFRI(Fria[3]) == true){ //DE SUBRED CALIENTE A PRINCIPAL FRIA SEPARADA
                    if(LadoCaliente[CorrienteFria][12] == 0){ // conexion por primera ves - promedio
                        IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }else if(LadoCaliente[CorrienteFria][12] == 1){ // conexion por segunda ves
                        IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }
                }else{      //conexion normal
                    IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                }
            }else if(Fria[3] >= contadorLADOIZQUIERDO && Fria[3] < contadorTOTALIZQYDER){// DE LADO DERECHO
                if(ifissplittedFRI(Fria[3]) == true){ //DE SUBRED CALIENTE A PRINCIPAL FRIA SEPARADA
                    if(LadoCaliente[CorrienteFria][12] == 0){ // conexion por primera ves - promedio
                        DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }else if(LadoCaliente[CorrienteFria][12] == 1){ // conexion por segunda ves
                        DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }
                }else{
                    DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                }
            }
        }else if(Fria[0] == 1){ // ES SUBRED FRIA
            double subfria;
            for(int i = 0; i < SeparacionesFrias.size() ; i++){
                if(SeparacionesFrias[i][0] == Fria[3]){
                    subfria = i;
                }
            }
            PrimeraOPC_Caliente_Subs_X_X_Fria_Subs_X_X(DTmin,subcaliente,subfria,espaciadorunionesSUB);
        }
    }
}

void plotterDN::SegundaOPC_plotsubstream(QVector<double> Caliente, QVector<double> Fria)
{
    if(Caliente[3] < contadorLADOIZQUIERDO){ // ES PRINCIPAL LADO IZQUIERDO
        //POR DEFAULTR LA CALIENTE ES PRINCIPAL
        //LA CORRIENTE FRIA PUEDE SER PRINCIPAL O SUBRED
        double subfria;
        for(int i = 0; i < SeparacionesFrias.size() ; i++){
            if(SeparacionesFrias[i][0] == Fria[3]){
                subfria = i;
                break;
            }
        }
        double CorrienteCaliente = Caliente[3];
        //SE CONECTA DE PRINCIPAL A SUBRED PERO SABER SI ESTA SEPARADA O NO
        if(ifissplittedCAL(CorrienteCaliente) == true){
            IZQ_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }else{
            IZQ_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }
    }else if(Caliente[3] >= contadorLADOIZQUIERDO && Caliente[3] < contadorTOTALIZQYDER){ // ES PRINCIPAL LADO DERECHO
        double subfria;
        for(int i = 0; i < SeparacionesFrias.size() ; i++){
            if(SeparacionesFrias[i][0] == Fria[3]){
                subfria = i;
            }
        }
        double CorrienteCaliente = Caliente[3];
        if(ifissplittedCAL(CorrienteCaliente) == true){
            DER_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }else{
            DER_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(DTmin,CorrienteCaliente,subfria,espaciadorunionesSUB);
        }
    }else if(Caliente[3] >= contadorTOTALIZQYDER){ // PROVIENE DE UNA SUB CORRIENTE LA UNION AQUI DEBO DE PROGRAMAR
        double subcaliente;
        for(int i = 0; i < SeparacionesCalientes.size() ; i++){
            if(SeparacionesCalientes[i][0] == Caliente[3] ){
                subcaliente = i;
                break;
            }
        }
        if(Fria[0] == 0){// es principal
            double CorrienteFria = Fria[3];
            if(Fria[3] < contadorLADOIZQUIERDO){ //DE LADO IZQUIERDO
                if(ifissplittedFRI(Fria[3]) == true){ //DE SUBRED CALIENTE A PRINCIPAL FRIA SEPARADA
                    if(LadoCaliente[CorrienteFria][12] == 0){ // conexion por primera ves - promedio
                        IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }else if(LadoCaliente[CorrienteFria][12] == 1){ // conexion por segunda ves
                        IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }
                }else{
                    IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                }
            }else if(Fria[3] >= contadorLADOIZQUIERDO && Fria[3] < contadorTOTALIZQYDER){// DE LADO DERECHO
                if(ifissplittedFRI(Fria[3]) == true){ //DE SUBRED CALIENTE A PRINCIPAL FRIA SEPARADA
                    if(LadoCaliente[CorrienteFria][12] == 0){ // conexion por primera ves - promedio
                        DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }else if(LadoCaliente[CorrienteFria][12] == 1){ // conexion por segunda ves
                        DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                    }
                }else{
                    DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(DTmin,subcaliente,CorrienteFria,espaciadorunionesSUB);
                }
            }
        }else if(Fria[0] == 1){ // ES SUBRED FRIA
            double subfria;
            for(int i = 0; i < SeparacionesFrias.size() ; i++){
                if(SeparacionesFrias[i][0] == Fria[3]){
                    subfria = i;
                }
            }
            SegundaOPC_Caliente_Subs_X_X_Fria_Subs_X_X(DTmin,subcaliente,subfria,espaciadorunionesSUB);
        }
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){
        X2 = -((LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - TemCal) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            //LA APERTURA VA PARA ATRAS
            x1[2] = LadoCaliente[Fria][10] - .4;
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones;
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            //LA APERTURA VA PARA ADELANTE
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoCaliente[Caliente][4]; // entrada caliente
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
        LadoCaliente[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones) //AQUI ME QUEDE
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones ; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri; //LadoCaliente[Fria][5]; // entrada fria
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
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1; // y
        posicionFri  = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }

}

void plotterDN::IZQ_PrimeraOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{   // SOLO SE DEBEN DE MODIFICAR LAS COORDENADAS Y HACER SUMATORIA DE LAS DIVISIONES
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -((LadoCaliente[Caliente][6]/LadoCaliente[Fria][6] )* (X1 - TemCal) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoCaliente[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri; //LadoCaliente[Fria][5]; // entrada fria
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
        //ACTUALIZACION DE QUE YA SE GRAFICO DESPUES DE LA UNION Y PASA A UNO
        LadoCaliente[Caliente][12] = 1;
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{ // YA NO SE SACA EL PROMEDIO DE LAS SEPARACIONES PERO DE CALIENTE SI
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){
        X2 = -((LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - TemCal) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoCaliente[Caliente][4]; // entrada caliente
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
        LadoCaliente[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{ // SOLO SE SACA EL PROMEDIO DE LAS FRIAS
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri;// LadoCaliente[Fria][5]; // entrada fria
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
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoCaliente[Caliente][6]/LadoCaliente[Fria][6])* (X1 - LadoCaliente[Caliente][4]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -((LadoFrio[Caliente][6]/LadoFrio[Fria][6] )* (X1 - TemCal) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
        textLabel2->setText(QString("%1 ªF").arg(QString::number(X2)));
        textLabel2->setFont(QFont(font().family(), 8)); // make font a bit larger
        ui->qcustomplot->replot();
        Uniones.resize(contadorUniones);
        for(int i = 0; i < Uniones.size(); i++){
            Uniones[i].resize(8);
        }
        Uniones[contadorUniones-1][0] = Caliente;
        Uniones[contadorUniones-1][1] = Fria;
        Uniones[contadorUniones-1][2] = ui->qcustomplot->itemCount() - 1;
        Uniones[contadorUniones-1][3] = ui->qcustomplot->itemCount();
        Uniones[contadorUniones-1][4] = TemCal; //LadoFrio[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri; //LadoFrio[Fria][5]; // entrada fria
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
        LadoFrio[Caliente][12] = 1;
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999,4,&ok);
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){
        X2 = -((LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - TemCal) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoFrio[Caliente][4]; // entrada caliente
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
        LadoFrio[Caliente][12] = 1;
    }else{
        return;
    }

}

void plotterDN::DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        ui->qcustomplot->addGraph();
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri;//LadoFrio[Fria][5]; // entrada fria
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
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }

}

void plotterDN::DER_PrimeraOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }

}

void plotterDN::DER_PrimeraOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){
        X2 = -((LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - TemCal) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoFrio[Caliente][4]; // entrada caliente
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
        LadoCaliente[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri;//LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoFrio[Caliente][4]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }

}

void plotterDN::DER_PrimeraOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (LadoFrio[Caliente][6]/LadoFrio[Fria][6])* (X1 - LadoFrio[Caliente][4]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioDER(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }

}

void plotterDN::IZQ_SegundaOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoCaliente[Caliente][6]*(qFabs(TemCal -LadoCaliente[Caliente][5])) ;
    double Q2 = LadoCaliente[Fria][6]*(qFabs(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]));
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            //LA APERTURA VA PARA ATRAS
            x1[2] = LadoCaliente[Fria][10] - .4;
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones;
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            //LA APERTURA VA PARA ADELANTE
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoCaliente[Caliente][4]; // entrada caliente
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
        LadoCaliente[Caliente][12] = 1;
    }else{
        return;
    }
}

// MODIFICAR TEMCAL TEMFRI
void plotterDN::IZQ_SegundaOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones) //AQUI ME QUEDE
{
    double Q1 = LadoCaliente[Caliente][6]*(qFabs(LadoCaliente[Caliente][4] -LadoCaliente[Caliente][5])) ;
    double Q2 = LadoCaliente[Fria][6]*(qFabs(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]));
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
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones ; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoCaliente[Caliente][6]*(qFabs(LadoCaliente[Caliente][4] -LadoCaliente[Caliente][5])) ;
    double Q2 = LadoCaliente[Fria][6]*(qFabs(LadoCaliente[Fria][4] - TemFri));
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri; //LadoCaliente[Fria][5]; // entrada fria
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
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
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
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1; // y
        posicionFri  = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }

}

void plotterDN::IZQ_SegundaOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{

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
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{   // SOLO SE DEBEN DE MODIFICAR LAS COORDENADAS Y HACER SUMATORIA DE LAS DIVISIONES
    double CalTemProm = 0, CalCoin = 0;
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoCaliente[Caliente][6]*( TemCal- LadoCaliente[Caliente][5]) ;
    double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - TemCal );
        X2 = (Q2/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoCaliente[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri;//LadoCaliente[Fria][5]; // entrada fria
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
        //ACTUALIZACION DE QUE YA SE GRAFICO DESPUES DE LA UNION Y PASA A UNO
        LadoCaliente[Caliente][12] = 1;
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoCaliente[Caliente][6]*(TemCal - LadoCaliente[Caliente][5]) ;
    double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - TemCal );
        X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoCaliente[Caliente][4]; // entrada caliente
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
        LadoCaliente[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoCaliente[Caliente][6]*(LadoCaliente[Caliente][4]-LadoCaliente[Caliente][5]) ;
    double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri;//LadoCaliente[Fria][5]; // entrada fria
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
        LadoCaliente[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
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
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_0_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][4];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoFrio[Caliente][6]*(qFabs(TemCal -LadoFrio[Caliente][5])) ;
    double Q2 = LadoFrio[Fria][6]*(qFabs(LadoFrio[Fria][4] - LadoFrio[Fria][5]));
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
        X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            //LA APERTURA VA PARA ATRAS
            x1[2] = LadoFrio[Fria][10] - .4;
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones;
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            //LA APERTURA VA PARA ADELANTE
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoFrio[Caliente][4]; // entrada caliente
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
        LadoFrio[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_1_FRIA_NO_X(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    double Q1 = LadoFrio[Caliente][6]*(qFabs(LadoFrio[Caliente][4] -LadoFrio[Caliente][5])) ;
    double Q2 = LadoFrio[Fria][6]*(qFabs(LadoFrio[Fria][4] - LadoFrio[Fria][5]));
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
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones ; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_NO_X_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoFrio[Caliente][6]*(qFabs(LadoFrio[Caliente][4] -LadoFrio[Caliente][5])) ;
    double Q2 = LadoFrio[Fria][6]*(qFabs(LadoFrio[Fria][4] - TemFri));
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
        X2 = (Q2/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri;//LadoFrio[Fria][5]; // entrada fria
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
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_NO_X_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]-LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
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
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1; // y
        posicionFri  = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }

}

void plotterDN::DER_SegundaOPC_sinSeparacionPrincipales(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{

    double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]-LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
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
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_0_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{   // SOLO SE DEBEN DE MODIFICAR LAS COORDENADAS Y HACER SUMATORIA DE LAS DIVISIONES
    double CalTemProm = 0, CalCoin = 0;
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoFrio[Caliente][6]*( TemCal- LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - TemCal );
        X2 = (Q2/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoFrio[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri;//LadoFrio[Fria][5]; // entrada fria
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
        //ACTUALIZACION DE QUE YA SE GRAFICO DESPUES DE LA UNION Y PASA A UNO
        LadoFrio[Caliente][12] = 1;
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_0_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoFrio[Caliente][6]*(TemCal - LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - TemCal );
        X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoFrio[Caliente][4]; // entrada caliente
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
        LadoFrio[Caliente][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_1_FRIA_SI_0(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]-LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
        X2 = (Q2/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = TemFri; //LadoFrio[Fria][5]; // entrada fria
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
        LadoFrio[Fria][12] = 1;
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_SI_1_FRIA_SI_1(double DTmin, double Caliente, double Fria, double espaciadoruniones)
{
    double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]-LadoFrio[Caliente][5]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
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
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadoruniones; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
    }else{
        return;
    }
}

//MODIFICAR

void plotterDN::IZQ_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){ // SeparacionesFrias
        X2 = -((LadoCaliente[Caliente][6]/SeparacionesFrias[Fria][5])* (X1 - TemCal) - SeparacionesFrias[Fria][4]);
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5], SeparacionesFrias[Fria][3]) == true){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoCaliente[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoCaliente[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    if(ok){ // SeparacionesFrias
        X2 = -((LadoCaliente[Caliente][6]/SeparacionesFrias[Fria][5])* (X1 - LadoCaliente[Caliente][4]) - SeparacionesFrias[Fria][4]);
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4] ; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoCaliente[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin, double Caliente,double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    if(ok){ // SeparacionesFrias
        X2 = -((LadoFrio[Caliente][6]/SeparacionesFrias[Fria][5])* (X1 - TemCal) - SeparacionesFrias[Fria][4]);
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5], SeparacionesFrias[Fria][3]) == true){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoFrio[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoFrio[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    if(ok){ // SeparacionesFrias
        X2 = -((LadoFrio[Caliente][6]/SeparacionesFrias[Fria][5])* (X1 - LadoFrio[Caliente][4]) - SeparacionesFrias[Fria][4]);
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri =  SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoFrio[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -((SeparacionesCalientes[Caliente][5]/LadoCaliente[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri;//LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        LadoCaliente[Fria][12] = 1;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    if(ok){ // SeparacionesFrias
        X2 = -((SeparacionesCalientes[Caliente][5]/LadoCaliente[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{ // YA NO SE SACA PROMEDIO PARA LA RED PRINCIPAL
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (SeparacionesCalientes[Caliente][5]/LadoCaliente[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - LadoCaliente[Fria][5]);
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    if(ok){
        X2 = -((SeparacionesCalientes[Caliente][5]/LadoFrio[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - TemFri);
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri;//LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        LadoFrio[Fria][12] = 1;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }

}

void plotterDN::DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot sub stream:",0,0,9999999999,4,&ok);
    if(ok){ // SeparacionesFrias
        X2 = -((SeparacionesCalientes[Caliente][5]/LadoFrio[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::DER_PrimeraOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (SeparacionesCalientes[Caliente][5]/LadoCaliente[Fria][6])* (X1 - SeparacionesCalientes[Caliente][3]) - LadoFrio[Fria][5]);
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::PrimeraOPC_Caliente_Subs_X_X_Fria_Subs_X_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    bool ok;
    double X2,dif1,dif2;
    double X1 = QInputDialog::getDouble(this,"Specification","Output temperature for hot stream:",0,0,9999999999,4,&ok);
    if(ok){
        X2 = -( (SeparacionesCalientes[Caliente][5]/SeparacionesFrias[Fria][5])* (X1 - SeparacionesCalientes[Caliente][3]) - SeparacionesFrias[Fria][4]);
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

//MODIFICAR
void plotterDN::IZQ_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    //SUMAMOS SI COINCIDEN LAS TEMPERATURA Y SACAMOS MEDIA
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoCaliente[Caliente][6]*qFabs(TemCal - LadoCaliente[Caliente][5]) ;
    double Q2 = SeparacionesFrias[Fria][5]*qFabs(SeparacionesFrias[Fria][3] - SeparacionesFrias[Fria][4]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - TemCal );
        X2 = (Q2/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5], SeparacionesFrias[Fria][3]) == true){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal;//LadoCaliente[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoCaliente[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = LadoCaliente[Caliente][6]*(LadoCaliente[Caliente][4]-LadoCaliente[Caliente][5]) ;
    double Q2 = SeparacionesFrias[Fria][5]*(SeparacionesFrias[Fria][3] - SeparacionesFrias[Fria][4]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoCaliente[Caliente][5];
        X2 = (Q1/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
        X2 = (Q2/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoCaliente[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoCaliente[Caliente][5],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoCaliente[Caliente][8] + .4; // x
        y1[0] = LadoCaliente[Caliente][9]; // y
        x1[1] = LadoCaliente[Caliente][8] + .4; // x
        y1[1] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoCaliente[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoCaliente[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_Prin_Div_SI_Fria_Sub(double DTmin, double Caliente,double Fria, double espaciadorunionesSUB)
{
    double CalTemProm = 0, CalCoin = 0;
    for(int i = 0; i < SeparacionesCalientes.size() ; i++){
        if(SeparacionesCalientes[i][1] == Caliente){
            CalTemProm = CalTemProm + SeparacionesCalientes[i][3];
            CalCoin = CalCoin + 1;
        }
    }
    double TemCal = CalTemProm/CalCoin;
    double Q1 = LadoFrio[Caliente][6]*(TemCal - LadoFrio[Caliente][5]) ;
    double Q2 = SeparacionesFrias[Fria][5]*(SeparacionesFrias[Fria][3] - SeparacionesFrias[Fria][4]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - TemCal );
        X2 = (Q2/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(TemCal - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5], SeparacionesFrias[Fria][3]) == true){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = TemCal; //LadoFrio[Caliente][4]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoFrio[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_Prin_Div_NO_Fria_Sub(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = LadoFrio[Caliente][6]*(LadoFrio[Caliente][4]-LadoFrio[Caliente][5]) ;
    double Q2 = SeparacionesFrias[Fria][5]*(SeparacionesFrias[Fria][3] - SeparacionesFrias[Fria][4]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = LadoFrio[Caliente][5];
        X2 = (Q1/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
        X2 = (Q2/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else{
        return;
    }
    double posicionCal = LadoFrio[Caliente][8] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,LadoFrio[Caliente][5],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = LadoFrio[Caliente][8] + .4; // x
        y1[0] = LadoFrio[Caliente][9]; // y
        x1[1] = LadoFrio[Caliente][8] + .4; // x
        y1[1] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
        posicionFri =  SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = LadoFrio[Caliente][9] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        LadoFrio[Caliente][4]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]- SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoCaliente[Fria][6]*qFabs(LadoCaliente[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoCaliente[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoCaliente[Caliente][6] ) - LadoCaliente[Caliente][4] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri; //LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        LadoCaliente[Fria][12] = 1;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }

}

void plotterDN::IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]-SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoCaliente[Fria][6]*qFabs(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / SeparacionesCalientes[Caliente][5] ) - SeparacionesCalientes[Caliente][3] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::IZQ_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{ // YA NO SE SACA PROMEDIO PARA LA RED PRINCIPAL
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]- SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoCaliente[Fria][6]*(LadoCaliente[Fria][4] - LadoCaliente[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 /SeparacionesCalientes[Caliente][5] ) - SeparacionesCalientes[Caliente][3] );
        X2 = (Q2/ LadoCaliente[Fria][6]) + LadoCaliente[Fria][5];
        dif1 = qFabs(X1 - LadoCaliente[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoCaliente[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }else{
            x1[2] = LadoCaliente[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoCaliente[Fria][10] - .4; // x
        }
        y1[3] = LadoCaliente[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoCaliente[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoCaliente[Fria][5] = X2;
        for(int i = 0; i < LadoCaliente.size(); i++){
            LadoCaliente[i][8] = LadoCaliente[i][8] + .4; // x
            LadoCaliente[i][9] = LadoCaliente[i][9]; // y
            LadoCaliente[i][10]= LadoCaliente[i][10] - .4; // x
            LadoCaliente[i][11]= LadoCaliente[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_0(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double FriTemProm = 0, FriCoin = 0;
    for(int i = 0; i < SeparacionesFrias.size() ; i++){
        if(SeparacionesFrias[i][1] == Fria){
            FriTemProm = FriTemProm + SeparacionesFrias[i][4];
            FriCoin = FriCoin + 1;
        }
    }
    double TemFri = FriTemProm/FriCoin;
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]- SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoFrio[Fria][6]*qFabs(LadoFrio[Fria][4] - TemFri);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(LadoFrio[Caliente][4] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / LadoFrio[Caliente][6] ) - LadoFrio[Caliente][4] );
        X2 = (Q2/ LadoFrio[Fria][6]) + TemFri;
        dif1 = qFabs(X1 - TemFri );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4])== false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri) == true ){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = TemFri ; //LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        LadoFrio[Fria][12] = 1;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }

}

void plotterDN::DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_1(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]-SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoFrio[Fria][6]*qFabs(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / SeparacionesCalientes[Caliente][5] ) - SeparacionesCalientes[Caliente][3] );
        X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoFrio[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::DER_SegundaOPC_Caliente_Subs_X_X_Fria_Principal_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]- SeparacionesCalientes[Caliente][4]) ;
    double Q2 = LadoFrio[Fria][6]*(LadoFrio[Fria][4] - LadoFrio[Fria][5]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 /SeparacionesCalientes[Caliente][5] ) - SeparacionesCalientes[Caliente][3] );
        X2 = (Q2/ LadoFrio[Fria][6]) + LadoFrio[Fria][5];
        dif1 = qFabs(X1 - LadoFrio[Fria][5] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],LadoFrio[Fria][4]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = LadoCaliente[Fria][10];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }else{
            x1[2] = LadoFrio[Fria][10] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = LadoFrio[Fria][10] - .4; // x
        }
        y1[3] = LadoFrio[Fria][11]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = LadoFrio[Fria][5]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        LadoFrio[Fria][5] = X2;
        for(int i = 0; i < LadoFrio.size(); i++){
            LadoFrio[i][8] = LadoFrio[i][8] + .4; // x
            LadoFrio[i][9] = LadoFrio[i][9]; // y
            LadoFrio[i][10]= LadoFrio[i][10] - .4; // x
            LadoFrio[i][11]= LadoFrio[i][11]; // y
        }
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
    }else{
        return;
    }
}

void plotterDN::SegundaOPC_Caliente_Subs_X_X_Fria_Subs_X_X(double DTmin, double Caliente, double Fria, double espaciadorunionesSUB)
{
    double Q1 = SeparacionesCalientes[Caliente][5]*qFabs(SeparacionesCalientes[Caliente][3]- SeparacionesCalientes[Caliente][4]) ;
    double Q2 = SeparacionesFrias[Fria][5]*qFabs(SeparacionesFrias[Fria][3] - SeparacionesFrias[Fria][4]);
    double X1,X2,dif1,dif2;
    if(Q1 < Q2){ // Prueba si el calor Q1 es menor que Q2 se trasmite lo mayor posible
        X1 = SeparacionesCalientes[Caliente][4];
        X2 = (Q1/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else if(Q2 > Q1){ // se trasmite lo mayor posible
        X1 = -( ( Q2 / SeparacionesCalientes[Caliente][5] ) - SeparacionesCalientes[Caliente][3] );
        X2 = (Q2/ SeparacionesFrias[Fria][5]) + SeparacionesFrias[Fria][4];
        dif1 = qFabs(X1 - SeparacionesFrias[Fria][4] );
        dif2 = qFabs(SeparacionesCalientes[Caliente][3] - X2);
    }else{
        return;
    }
    double posicionCal = SeparacionesCalientes[Caliente][6] + .4 ;
    double posicionFri;
    if(controlintercambioIZQ(DTmin,dif1,dif2,X1,X2,SeparacionesCalientes[Caliente][4],SeparacionesFrias[Fria][3]) == false){
        contadorUniones = contadorUniones + 1;
        QVector<double> x1,y1;
        x1.resize(4),y1.resize(4);
        x1[0] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[0] = SeparacionesCalientes[Caliente][7]; // y
        x1[1] = SeparacionesCalientes[Caliente][6] + .4; // x
        y1[1] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
        posicionFri = SeparacionesFrias[Fria][6];
        if(ifpasomitadIZQ(posicionCal,posicionFri)== true){
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }else{
            x1[2] = SeparacionesFrias[Fria][6] - .4; // x
            y1[2] = SeparacionesCalientes[Caliente][7] - 1 - espaciadorunionesSUB; // y
            x1[3] = SeparacionesFrias[Fria][6] - .4; // x
        }
        y1[3] = SeparacionesFrias[Fria][7]; // y
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setWidth(2);
        pen.setColor(Qt::black);
        QCPCurve *union1 = new QCPCurve(ui->qcustomplot->xAxis,ui->qcustomplot->yAxis);
        union1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        union1->setName(QString("Exchager %1").arg(contadorUniones) );
        union1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle));
        union1->addData(x1,y1);
        union1->setPen(pen);
        QCPItemText *textLabel1 = new QCPItemText(ui->qcustomplot);
        textLabel1->setClipToAxisRect(true);
        textLabel1->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel1->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel1->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel1->position->setCoords(x1[0] + .2,y1[0] + .5);
        textLabel1->setText(QString("%1 ªF").arg(QString::number(X1)));
        textLabel1->setFont(QFont(font().family(), 8)); // make font a bit larger
        QCPItemText *textLabel2 = new QCPItemText(ui->qcustomplot);
        textLabel2->setClipToAxisRect(true);
        textLabel2->setClipAxisRect(ui->qcustomplot->axisRect());
        textLabel2->setLayer(ui->qcustomplot->axisRect()->layer()->name());
        textLabel2->position->setAxisRect(ui->qcustomplot->axisRect());
        textLabel2->position->setCoords(x1[3] - .2,y1[3] + .5);
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
        Uniones[contadorUniones-1][4] = SeparacionesCalientes[Caliente][3]; // entrada caliente
        Uniones[contadorUniones-1][5] = X1; // salida caliente
        Uniones[contadorUniones-1][6] = SeparacionesFrias[Fria][4]; // entrada fria
        Uniones[contadorUniones-1][7] = X2; // salida fria
        //ACTUALIZACION DE LA UNION DE LA ESA CORRIENTES
        SeparacionesCalientes[Caliente][3]  = X1;
        SeparacionesFrias[Fria][4] = X2;
        for(int j = 0; j < SeparacionesCalientes.size(); j++){
            SeparacionesCalientes[j][6] = x1[0];
        }
        for(int j = 0; j < SeparacionesFrias.size(); j++){
            SeparacionesFrias[j][6] = x1[3];
        }
    }else{
        return;
    }
}



void plotterDN::on_pushButton_clicked()
{
    qDebug() << LadoCaliente << "LadoCaliente";
//    qDebug() << LadoFrio << "LadoFrio";
    qDebug() << Uniones << "Uniones";
//    qDebug() << Servicios << "Servicios";
    qDebug() << SeparacionesCalientes << "Separaciones calientes";
    qDebug() << SeparacionesFrias << "Separaciones Frias";
    qDebug() << espaciadoruniones;
//    qDebug() << ui->qcustomplot->graphCount() << "Graficas";
//    qDebug() << ui->qcustomplot->itemCount() << "items";
//    qDebug() << ui->qcustomplot->plottableCount();
}

void plotterDN::on_PDF_clicked()
{
    QString URL = QFileDialog::getSaveFileName(this,"Save the file");
    if(!URL.contains(".pdf") == true){
        URL = URL + ".pdf";
    }
    QFile file(URL);
    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
         {
             QMessageBox::warning(0,"Could not create Project File",QObject::tr( "\n Could not create Project File on disk"));
         }
    ui->qcustomplot->savePdf(URL,0,0,QCP::ExportPen(),"","");
}

void plotterDN::on_IMAGE_clicked()
{

}

void plotterDN::on_SAVE_clicked()
{
    int VENTANAMADRE = 4;
    int TabAnali = 0;
    QFile Filetab(TAB_A_P);
        if (!Filetab.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out26(&Filetab);
    out26.setVersion(QDataStream::Qt_5_4);
    TabAnalisis TA(TabAnali,VENTANAMADRE);
    out26 << TA;
    Filetab.flush();
    Filetab.close();
    QFile FileGrid(UNIDADES_FILENAME);
    if (!FileGrid.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out33(&FileGrid);
    out33.setVersion(QDataStream::Qt_5_4);
    Grid vecgrid(Uniones,Servicios);
    out33 << vecgrid;
    FileGrid.flush();
    FileGrid.close();
}
