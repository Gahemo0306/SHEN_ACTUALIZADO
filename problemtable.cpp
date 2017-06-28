#include "problemtable.h"
#include "ui_problemtable.h"
#include "duvaloraction.h"
#include "duvaloraction.h"
#include "graficos.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>

problemtable::problemtable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::problemtable)
{
    ui->setupUi(this);
    ui->groupBox_3->setVisible(false);
    ui->valork->setVisible(false);
    ui->Minimo->setVisible(false);
    ui->Maximo->setVisible(false);
    ui->Incremento->setVisible(false);
    ui->K->setVisible(false);
    ui->Min->setVisible(false);
    ui->Max->setVisible(false);
    ui->Inc->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->Export->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->help_tableproblem->setVisible(false);
    ui->Tabla->setVisible(false);
}

problemtable::~problemtable()
{
    delete ui;
}

void problemtable::on_uniform_clicked()
{
    radiobuttons();
}

void problemtable::on_diverse_clicked()
{
    radiobuttons();
}

void problemtable::on_estatico_clicked()
{
    radiobuttons();
}

void problemtable::on_incremento_clicked()
{
    radiobuttons();
}

void problemtable::radiobuttons()
{
    ui->groupBox_3->setVisible(false);
    ui->valork->setVisible(false);
    ui->Minimo->setVisible(false);
    ui->Maximo->setVisible(false);
    ui->Incremento->setVisible(false);
    ui->K->setVisible(false);
    ui->Min->setVisible(false);
    ui->Max->setVisible(false);
    ui->Inc->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->Export->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->help_tableproblem->setVisible(false);
    ui->Tabla->setVisible(false);
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
    bool estatico = ui->estatico->isChecked();
    bool incremento = ui->incremento->isChecked();
    if(uniforme == true && estatico == true){
        ui->groupBox_3->setVisible(true);
        ui->Minimo->setVisible(true);
        ui->Minimo->setText("Value:");
        ui->Min->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->Export->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->help_tableproblem->setVisible(false);
        ui->Tabla->setVisible(true);
    }else if(uniforme == true && incremento == true){
        ui->groupBox_3->setVisible(true);
        ui->Minimo->setText("Minimun:");
        ui->Minimo->setVisible(true);
        ui->Maximo->setVisible(true);
        ui->Incremento->setVisible(true);
        ui->Min->setVisible(true);
        ui->Max->setVisible(true);
        ui->Inc->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->Export->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->help_tableproblem->setVisible(false);
        ui->Tabla->setVisible(true);
    }else if(diverso == true && estatico == true){
        ui->groupBox_3->setVisible(true);
        ui->Minimo->setVisible(true);
        ui->Minimo->setText("Value:");
        ui->Min->setVisible(true);
        ui->valork->setVisible(true);
        ui->K->setVisible(true);
        ui->Min->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->Export->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->help_tableproblem->setVisible(true);
        ui->Tabla->setVisible(true);
    }else if(diverso == true &&  incremento == true){
        ui->groupBox_3->setVisible(true);
        ui->Minimo->setText("Minimun:");
        ui->Minimo->setVisible(true);
        ui->Maximo->setVisible(true);
        ui->Incremento->setVisible(true);
        ui->Min->setVisible(true);
        ui->Max->setVisible(true);
        ui->Inc->setVisible(true);
        ui->valork->setVisible(true);
        ui->K->setVisible(true);
        ui->pushButton->setVisible(true);
        ui->Export->setVisible(true);
        ui->pushButton_3->setVisible(true);
        ui->help_tableproblem->setVisible(true);
        ui->Tabla->setVisible(true);
    }
}

void problemtable::on_pushButton_clicked()
{
    bool uniforme = ui->uniform->isChecked();
    bool diverso = ui->diverse->isChecked();
    bool estatico = ui->estatico->isChecked();
    bool incremento = ui->incremento->isChecked();
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
    in2 >> MATRIZ;
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
    F.flush();
    F.close();
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
    int UWcp = units.getUWcp();;
    SI = units.getSI();
    SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    ui->Tabla->setEnabled(true);
    if(uniforme==true && estatico == true){
        Min = ui->Min->value();
        ui->Tabla->clear();
        ui->Tabla->setVisible(true);
        QStringList Headers;
        Headers << "Temperature" << "Temperature" << "Deficit" << "Heat_flow_in"
                << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
        ui->Tabla->setColumnCount(7);
        ui->Tabla->setHorizontalHeaderLabels(Headers);
        double DTmin = Min;
        tablaproblema_estatico_uniforme(DTmin);
    }else if(uniforme==true && incremento == true){
        Min = ui->Min->value();
        Max = ui->Max->value();
        Inc = ui->Inc->value();
        ui->Tabla->clear();
        ui->Tabla->setVisible(true);
        QStringList Headers;
        Headers << "Temperature" << "Temperature" << "Deficit" << "Heat_flow_in"
                << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
        ui->Tabla->setColumnCount(7);
        ui->Tabla->setHorizontalHeaderLabels(Headers);
        double Minimo = Min, Maximo = Max, Incremento = Inc;
        double Iteraciones =(Maximo-Minimo)/Incremento;
        double DTmin = Minimo;
        CONTADORFILAS = 0;
        for(int i = 0; i < (Iteraciones+1) ; i++){
            tablaproblema_incremento_uniforme(DTmin);
            DTmin = DTmin + Incremento;
        }
        CONTADORROWS.clear();
    }else if(diverso==true && estatico== true){
        K = ui->K->value();
        ui->Tabla->clear();
        ui->Tabla->setVisible(true);
        QStringList Headers;
        Headers << "Temperature" << "Temperature" << "Deficit" << "Heat_flow_in"
                << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
        ui->Tabla->setColumnCount(7);
        ui->Tabla->setHorizontalHeaderLabels(Headers);
        double DTmin = Min;
        tablaproblema_estatico_diverso(DTmin,K);
    }else if(diverso == true && incremento == true){
        K = ui->K->value();
        Min = ui->Min->value();
        Max = ui->Max->value();
        Inc = ui->Inc->value();
        ui->Tabla->clear();
        ui->Tabla->setVisible(true);
        QStringList Headers;
        Headers << "Temperature" << "Temperature" << "Deficit" << "Heat_flow_in"
                << "Heat_flow_out" << "Heat_flow_in_adjusted" << "Heat_flow_out_adjusted";
        ui->Tabla->setColumnCount(7);
        ui->Tabla->setHorizontalHeaderLabels(Headers);
        double Minimo = Min, Maximo = Max, Incremento = Inc, k = K;
        double Iteraciones =(Maximo-Minimo)/Incremento;
        double DTmin = Minimo;
        float punto1 = .5, punto2 = 10;
        CONTADORFILAS = 0;
        for(int i = 0; i < (Iteraciones+1) ; i++){
            tablaproblema_incremento_diverso(DTmin, k,punto1,punto2);
            punto1 = K1;
            punto2 = 10;
            DTmin = DTmin + Incremento;
        }
        CONTADORROWS.clear();
    }
}

void problemtable::tablaproblema_estatico_uniforme(double DTmin)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    ui->Tabla->setRowCount(VecCorrientesTotal.size()-1);
    int row = ui->Tabla->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin  " + QString::number(DTmin);
    }
    ui->Tabla->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7;
    int k=1,j=0;
    for(int i =0; i < row ; i++){
        val1 = VecCorrientesTotal[k][0];
        val2 = VecCorrientesTotal[k][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        k=k+1;
        j=j+1;
    }
}

void problemtable::tablaproblema_incremento_uniforme(double DTmin)
{
    Plot_curvascompuestasajustadas plot(TS,TE,WCP,DTmin);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->Tabla->setRowCount(CONTADORFILAS);
    int row = ui->Tabla->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin  " + QString::number(DTmin);
    }
    ui->Tabla->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7;
    int k=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[k][0];
        val2 = VecCorrientesTotal[k][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        k=k+1;
        j=j+1;
    }
}

void problemtable::tablaproblema_estatico_diverso(double DTmin, double k)
{
    float punto1 = .5, punto2 = 10;
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    double valuek = plot.getK();
    ui->Tabla->setRowCount(VecCorrientesTotal.size()-1);
    int row = ui->Tabla->rowCount();
    QStringList Rows;
    for(int i = 0; i < row; i++){
        Rows << "DTmin:" + QString::number(DTmin) + "& K:" + QString::number(valuek);
    }
    ui->Tabla->setVerticalHeaderLabels(Rows);
    double val1, val2, val3,val4,val5,val6,val7;
    int kcontador=1,j=0;
    QVector<double> VAL2;
    VAL2.resize(VecAdjHeatFlow.size());
    for(int i = 0; i < VecAdjHeatFlow.size(); i++){
        VAL2[i] = VecAdjHeatFlow[i][1];
    }
    for(int i =0; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
}

void problemtable::tablaproblema_incremento_diverso(double DTmin, double k,float punto1, float punto2)
{
    Plot_CCAJUSTADA_DIVERSA plot(TS,TE,WCP,H,DTmin,k,punto1,punto2);
    QVector<QVector<double>> VecCorrientesTotal = plot.getVectorCorrientesTotal();
    QVector<QVector<double>> VecHeatFlow = plot.getVecHeatFlow();
    QVector<QVector<double>> VecAdjHeatFlow = plot.getVecAdjHeatFlow();
    QVector<double> Deficit = plot.getDeficit();
    K1 = plot.getK();
    CONTADORFILAS = CONTADORFILAS + VecCorrientesTotal.size()-1;
    ui->Tabla->setRowCount(CONTADORFILAS);
    int row = ui->Tabla->rowCount();
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < CONTADORFILAS; i++){
        CONTADORROWS << "DTmin:  " + QString::number(DTmin) + "& K:" + QString::number(K1);;
    }
    ui->Tabla->setVerticalHeaderLabels(CONTADORROWS);
    double val1, val2, val3,val4,val5,val6,val7;
    int kcontador=1,j=0;
    for(int i = CONTADORFILAS - VecCorrientesTotal.size() + 1; i < row ; i++){
        val1 = VecCorrientesTotal[kcontador][0];
        val2 = VecCorrientesTotal[kcontador][1];
        val3 = Deficit[j];
        val4 = VecHeatFlow[j][0];
        val5 = VecHeatFlow[j][1];
        val6 = VecAdjHeatFlow[j][0];
        val7 = VecAdjHeatFlow[j][1];
        ui->Tabla->setItem(i,0, new QTableWidgetItem(QString::number(val1)));
        ui->Tabla->setItem(i,1, new QTableWidgetItem(QString::number(val2)));
        ui->Tabla->setItem(i,2, new QTableWidgetItem(QString::number(val3)));
        ui->Tabla->setItem(i,3, new QTableWidgetItem(QString::number(val4)));
        ui->Tabla->setItem(i,4, new QTableWidgetItem(QString::number(val5)));
        ui->Tabla->setItem(i,5, new QTableWidgetItem(QString::number(val6)));
        ui->Tabla->setItem(i,6, new QTableWidgetItem(QString::number(val7)));
        kcontador=kcontador+1;
        j=j+1;
    }
}

void problemtable::on_Export_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save the file");
    if(!filename.contains(".pdf")){
        filename = filename + ".pdf";
    }
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(filename);
    QTextDocument doc;
    QString citydate = "Report, ";
    citydate += currDate();
    QString style("<style>");
    style.append("body { color:black; } ");
    style.append("table { border-collapse: collapse;font-size:10px; }");
    style.append("table, th, td { border: 1px solid black; text-align: center; }");
    style.append("</style>");
    QString text("<html><body>");
    text.append("<h>").append("     " + citydate + "     ").append("</h>");
    text.append("<p>").append("     Program: SHEN (SOFTWARE HEAT EXCHANGERS NETWORKS)     ").append("</p>");
    text.append("<p>").append("     Intellectual Property: Technological Institute of Celaya     ").append("</p>");
    text.append("<p>").append("     Project directed by: Dr. Arturo Jimenez Gutierrez     ").append("</p>");
    text.append("<p>").append("     Contributors: Ing. Gabriel Hernàndez Morales     ").append("</p>");
    text.append("<p>").append("                                                      ").append("</p>");
    bool estatico = ui->estatico->isChecked();
    bool incremento = ui->incremento->isChecked();
    if(estatico == true){
        QString  row    = ui->Tabla->verticalHeaderItem(0)->text();
        QString  titulo = espaceadorcalculos(row);
        text.append("<p>").append( titulo ).append("</p>");
        text.append("<p>").append(" ").append("</p>");
        text.append("<table><tbody>");
        text.append("<tr>");
        for (int i = 0; i < ui->Tabla->columnCount(); i++) {
            text.append("<td>").append("        " + ui->Tabla->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
        }
        text.append("</tr>");
        for (int i = 0; i < ui->Tabla->rowCount(); i++) {
            text.append("<tr>");
            for (int j = 0; j < ui->Tabla->columnCount(); j++) {
                QTableWidgetItem *item = ui->Tabla->item(i, j);
                if (!item || item->text().isEmpty()) {
                    ui->Tabla->setItem(i, j, new QTableWidgetItem("0"));
                }
                text.append("<td>").append("        "+ui->Tabla->item(i, j)->text()+"        ").append("</td>");
            }
            text.append("</tr>");
        }
        QStringList Unidades;
        Unidades = unidadestablas();
        text.append("<tr>");
        for( int i = 0; i < ui->Tabla->columnCount(); i++){
            text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
        }
        text.append("</tr>");
        text.append("</tbody></table>");
        text.append("</body></html>");
    }else if(incremento == true){
        int Iteraciones = (Max-Min)/Inc;
        CONTADORFILAS = 0;
        QString row1,row2,row,titulo;
        for(int j = 0 ; j <  Iteraciones+1 ;j++){
            row    = ui->Tabla->verticalHeaderItem(CONTADORFILAS)->text();
            titulo = espaceadorcalculos(row);
            text.append("<p>").append(titulo).append("</p>");
            text.append("<p>").append(" ").append("</p>");
            text.append("<table><tbody>");
            text.append("<tr>"); //abre
            for (int i = 0; i < ui->Tabla->columnCount(); i++) {
                text.append("<td>").append("        " + ui->Tabla->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString() + "        ").append("</td>");
            }
            text.append("</tr>"); // ciertta
            for (int i = CONTADORFILAS; i < ui->Tabla->rowCount(); i++) {
                text.append("<tr>");
                CONTADORFILAS = CONTADORFILAS + 1;
                for (int j = 0; j < ui->Tabla->columnCount(); j++) {
                    QTableWidgetItem *item = ui->Tabla->item(i, j);
                    if (!item || item->text().isEmpty()) {
                        ui->Tabla->setItem(i, j, new QTableWidgetItem("0"));
                    }
                    text.append("<td>").append("        "+ui->Tabla->item(i, j)->text()+"        ").append("</td>");
                }
                text.append("</tr>");
                if(i < ui->Tabla->rowCount() - 2){
                    row1 = ui->Tabla->verticalHeaderItem(i)->text();
                    row2 = ui->Tabla->verticalHeaderItem(i+1)->text();
                    if(compararrows(row1,row2) == true){ // son diferentes
                        break;
                    }
                }
            }
            QStringList Unidades;
            Unidades = unidadestablas();
            text.append("<tr>");
            for( int i = 0; i < ui->Tabla->columnCount(); i++){
                text.append("<td>").append("        "+ Unidades[i]  +"        ").append("</td>");
            }
            text.append("</tr>");
            text.append("</tbody></table>");
        }
        text.append("</body></html>");
    }
    doc.setDefaultStyleSheet(style);
    doc.setHtml(text);
    doc.setPageSize(printer.pageRect().size());
    doc.print(&printer);
}

QString problemtable::espaceadorcalculos(QString row)
{
    QString espace;
    QString inicio = "Table problem with ";
    QString final;
    if(SI == true ){
        final = " ªF";
        espace = inicio+ row + final;
    }else if(SIS == true){
        final = " ªC";
        espace = inicio+ row + final;
    }
    return espace;
}

QStringList problemtable::unidadestablas()
{
    QStringList lista;
    if(SI == true ){
        lista << "ªF" << "ªF" << "BTU/HR" << "BTU/HR" << "BTU/HR" << "BTU/HR" << "BTU/HR";
    }else if(SIS == true){
        lista << "ªC" << "ªC" << "Watts" << "Watts" << "Watts" << "Watts" << "Watts";
    }
    return lista;
}

bool problemtable::compararrows(QString row1, QString row2)
{
    if(row1 == row2){ // false soy iguales
        return false;
    }else{ // true son diferentes
        return true;
    }
}

QString problemtable::currDate()
{
    QDate date = QDate::currentDate();
    return date.toString("dd.MM.yyyy");
}

void problemtable::on_help_tableproblem_clicked()
{
    int Help = 1; //AYUDA SOBRE PARAMETRO K
    QFile F(AYUDA_FILENAME);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out29(&F);
    out29.setVersion(QDataStream::Qt_5_4);
    savehelp help(Help);
    out29 << help;
    F.flush();
    F.close();
    emit helpsender(true);
}

void problemtable::helprecibidor(bool checked)
{
    if(checked == true){
        on_help_tableproblem_clicked();
    }else{
        return;
    }
}
