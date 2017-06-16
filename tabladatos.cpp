#include "duvaloraction.h"
#include "valoraction.h"
#include "tabladatos.h"
#include "ui_tabladatos.h"
#include <QVector>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QTableWidget>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QDebug>

TablaDatos::TablaDatos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TablaDatos)
{
    ui->setupUi(this);
    //TREEVIEW
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirmodel->setRootPath(QDir::rootPath());
    ui->treeView->setModel(dirmodel);
    //LISTVIEW
    filemodel = new QFileSystemModel(this);
    filemodel->setFilter((QDir::NoDotAndDotDot | QDir::Files));
    filemodel->setRootPath(QDir::rootPath());
    ui->listView->setModel(filemodel);
    QFile F(VALORACTION_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in1(&F);
    in1.setVersion(QDataStream::Qt_5_4);
    Duvaloraction valoraction;
    while(!in1.atEnd()){
        in1 >> valoraction;
        int accion = valoraction.getvalact();
        if(valoraction.getvalact() == 1){  //NEW
            INICIO(accion);
        }
        else if (valoraction.getvalact() == 2){ //OPEN
            //CARGAR ACCIÓN DE ABRIR
            //ABRIR ARCHIVO .CSV
            INICIO(accion);
            QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
            QFile file(file_name);
            file_path = file_name;
            if (file_path.contains(".csv")){
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    int contadorlines = 1; // file line counter
                    QTextStream in(&file);
                    QStringList Almacenador;
                    //ui->Workspace->setRowCount(lineindex);
//                    disconnect(ui->TcomboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
//                    disconnect(ui->WcomboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
//                    disconnect(ui->FcomboBox, SIGNAL(currentIndexChanged(int)), 0, 0);
                    while (!in.atEnd()) {
                        if(contadorlines==1){ // se lee el tipo de sistema de unideade
                            QString fileLine = in.readLine();
                            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                            QString Si = lineToken.at(0); //
                            QString Sis = lineToken.at(1); //
                            QStringList Lista1,Lista2,Lista3;
                            int SI = Si.toInt();
                            int SIS = Sis.toInt();
                            if(SI==1){
                                ui->SIradioButton->setAutoExclusive(false);
                                ui->SIradioButton->setChecked(true);
                                ui->SIradioButton->setAutoExclusive(true);
                                ui->SISradioButton->setAutoExclusive(false);
                                ui->SISradioButton->setChecked(false);
                                ui->SISradioButton->setAutoExclusive(true);
                                Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
                                Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
                                Lista3 << "Select one..." << "Btu/hr °F'" << "Btu/min °F" << "Btu/seg °F" << "Btu/hr °C"
                                       << "Btu/min °C" << "Btu/seg °C" << "Btu/hr °R" << "Btu/min °R" << "Btu/seg °R";
                            }else if(SIS==1){
                                ui->SIradioButton->setAutoExclusive(false);
                                ui->SIradioButton->setChecked(false);
                                ui->SIradioButton->setAutoExclusive(true);
                                ui->SISradioButton->setAutoExclusive(false);
                                ui->SISradioButton->setChecked(true);
                                ui->SISradioButton->setAutoExclusive(true);
                                Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
                                Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
                                Lista3 << "Select one..." << "Joules/hr °K" << "Joules/min °K" << "Joules/seg °K" << "Joules/hr °C"
                                       << "Joules/min °C" << "Joules/seg °C" << "KiloJoules/hr °K" << "KiloJoules/min °K"
                                       << "KiloJoules/seg °K" << "KiloJoules/hr °C" << "KiloJoules/min °C" << "KiloJoules/seg °C"
                                       << "KiloCal/hr °C" << "KiloCal/min °C" << "KiloCal/seg °C";
                            }
                                ui->TcomboBox->setVisible(true);
                                ui->WcomboBox->setVisible(true);
                                ui->FcomboBox->setVisible(true);
                                ui->TcomboBox->clear();
                                ui->FcomboBox->clear();
                                ui->WcomboBox->clear();
                                ui->TcomboBox->addItems(Lista1);
                                ui->FcomboBox->addItems(Lista2);
                                ui->WcomboBox->addItems(Lista3);
                                ui->label_2->setVisible(true);
                                ui->label_3->setVisible(true);
                                ui->label_4->setVisible(true);
                        }
                        else if(contadorlines==2){
                                QString fileLine = in.readLine();
                                QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                                QString Tunidades = lineToken.at(0); //
                                QString Wunidades = lineToken.at(1); //
                                QString Funidades = lineToken.at(2);
                                int TUNIDADES = Tunidades.toInt();
                                int WUNIDADES = Wunidades.toInt();
                                int FUNIDADES = Funidades.toInt();
                                ui->TcomboBox->setCurrentIndex(TUNIDADES);
                                ui->WcomboBox->setCurrentIndex(WUNIDADES);
                                ui->FcomboBox->setCurrentIndex(FUNIDADES);
                        }
                        else if(contadorlines>=3){
                            QString fileLine = in.readLine();
                            QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                            Almacenador.append(lineToken);
                        }
                        contadorlines++;
                    }
                    file.close();
                    //connect(ui->TcomboBox, &QComboBox::currentIndexChanged,this, &TablaDatos::on_TcomboBox_currentIndexChanged);
                    //connect(ui->WcomboBox, &QComboBox::currentIndexChanged,this, &TablaDatos::on_WcomboBox_currentIndexChanged);
                    //connect(ui->FcomboBox, &QComboBox::currentIndexChanged,this, &TablaDatos::on_FcomboBox_currentIndexChanged);
                    disconnect(ui->Workspace, SIGNAL(cellChanged(int,int)), 0, 0);
                    int filas = Almacenador.size()/4;
                    int columnas = 4;
                    ui->Workspace->setRowCount(filas);
                    ui->Workspace->setColumnCount(columnas);
                    int h = 0;
                    for( int r = 0 ; r < ui->Workspace->rowCount(); ++r )
                    {
                        for( int c = 0; c < ui->Workspace->columnCount(); ++c )
                        {
                            QString value = Almacenador[h];
                            h++;
                            ui->Workspace->setItem(r, c, new QTableWidgetItem(value));
                        }
                    }
                    connect(ui->Workspace, &QTableWidget::cellChanged,this, &TablaDatos::on_Workspace_cellChanged);
                    QFile FileWork(WORKSPACE_FILENAME);
                    if (!FileWork.open(QIODevice::WriteOnly)){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                    QVector<QVector<double>> WORKSPACE_matrix;
                    filas = ui->Workspace->rowCount();
                    columnas = ui->Workspace->columnCount();
                    WORKSPACE_matrix.resize(filas);
                    for(int i = 0; i < filas; i++)
                    {
                        WORKSPACE_matrix[i].resize(columnas);
                    }
                    for(int i = 0; i < filas; i++)
                    {
                        for(int j = 0; j < columnas; j++){
                            WORKSPACE_matrix[i][j] = ui->Workspace->item(i,j)->text().toDouble();
                        }
                    }
                    QDataStream out2(&FileWork);
                    out2.setVersion(QDataStream::Qt_5_4);
                    Workspace MATRIZ(WORKSPACE_matrix);
                    out2 << MATRIZ;
                    FileWork.flush();
                    FileWork.close();
                    //
                    QFile FileUnidades(UNIDADES_FILENAME);
                    if (!FileUnidades.open(QIODevice::WriteOnly)){
                        QMessageBox::warning(this,tr("Error"),tr("Error"));
                        return;
                    }
                    QDataStream out3(&FileUnidades);
                    out3.setVersion(QDataStream::Qt_5_4);
                    int ITemp = ui->TcomboBox->currentIndex(), IWcp = ui->WcomboBox->currentIndex();
                    int Ih  = ui->FcomboBox->currentIndex();
                    bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
                    Unidades units(SI,SIS,ITemp,IWcp,Ih);
                    out3 << units;
                    FileUnidades.flush();
                    FileUnidades.close();
                }
            }
        }
        F.flush();
        F.close();
        file_path=""; //CORREGIR;
        BORRARFILA=-1;
    }
}

TablaDatos::~TablaDatos()
{
    delete ui;
}

void TablaDatos::on_Addcorrientes_clicked()
{
    int NumCorri = ui->NumeroCorrientes->value();
    int TempU = ui->TcomboBox->currentIndex();
    int FilmU = ui->FcomboBox->currentIndex();
    int WcpU  = ui->WcomboBox->currentIndex();
    if (TempU==0 || FilmU==0 || WcpU==0 )
    {
        QMessageBox::warning(this,tr("Error"),tr("Missing units."));
        return;
    }
    int data = ui->Workspace->rowCount();
    if(data == 0 ){
        ui->Workspace->setRowCount(NumCorri);
        for( int r = 0; r < ui->Workspace->rowCount(); ++r )
        {
            for( int c = 0; c < ui->Workspace->columnCount(); ++c )
            {
                ui->Workspace->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }else{
        ui->Workspace->setRowCount(NumCorri+data);
        for( int r = data; r < ui->Workspace->rowCount(); ++r )
        {
            for( int c = 0; c < ui->Workspace->columnCount(); ++c )
            {
                ui->Workspace->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void TablaDatos::on_SIradioButton_clicked()
{
    QStringList Lista1,Lista2,Lista3;
    ui->TcomboBox->clear();
    ui->FcomboBox->clear();
    ui->WcomboBox->clear();
    Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
    Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
    Lista3 << "Select one..." << "Btu/hr °F'" << "Btu/min °F" << "Btu/seg °F" << "Btu/hr °C"
           << "Btu/min °C" << "Btu/seg °C" << "Btu/hr °R" << "Btu/min °R" << "Btu/seg °R";
    ui->TcomboBox->addItems(Lista1);
    ui->FcomboBox->addItems(Lista2);
    ui->WcomboBox->addItems(Lista3);
}

void TablaDatos::on_SISradioButton_clicked()
{   //corregir
    QStringList Lista1,Lista2,Lista3;
    ui->TcomboBox->clear();
    ui->FcomboBox->clear();
    ui->WcomboBox->clear();
    Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
    Lista3 << "Select one..." << "Joules/hr °K" << "Joules/min °K" << "Joules/seg °K" << "Joules/hr °C"
           << "Joules/min °C" << "Joules/seg °C" << "KiloJoules/hr °K" << "KiloJoules/min °K"
           << "KiloJoules/seg °K" << "KiloJoules/hr °C" << "KiloJoules/min °C" << "KiloJoules/seg °C"
           << "KiloCal/hr °C" << "KiloCal/min °C" << "KiloCal/seg °C";
    Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
    ui->TcomboBox->addItems(Lista1);
    ui->FcomboBox->addItems(Lista2);
    ui->WcomboBox->addItems(Lista3);
}

void TablaDatos::on_AddnewpushButton_clicked()
{
    //corregir
    int NumCorri = ui->Workspace->rowCount();
    int TempU = ui->TcomboBox->currentIndex();
    int FilmU = ui->FcomboBox->currentIndex();
    int WcpU  = ui->WcomboBox->currentIndex();
        //int wcp = WCP.toInt(), tt = TT.toInt(), to = TO.toInt(), fc=FC.toInt();
    if (TempU==0 || FilmU==0 || WcpU==0 )
    {
        QMessageBox::warning(this,tr("Error"),tr("Missing units"));
        return;
    }
    ui->Workspace->setRowCount(NumCorri+1);
}

void TablaDatos::on_Save_clicked()  //SAVE
{
        //ACCION DE GUARDAR ARCHIVO .CSV
        if (file_path == "")
        { // es decir no se ha guardado entonces se tiene guardar por primera vez
            QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
            if(!file_name.contains(".csv")){
                file_name = file_name + ".csv";
            }
            QFile csvFile(file_name);
            file_path = file_name;
            if (csvFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
            {
                QTextStream data(&csvFile );
                QStringList strList;
                int Tcombo = ui->TcomboBox->currentIndex();
                int Wcombo = ui->WcomboBox->currentIndex();
                int Fcombo = ui->FcomboBox->currentIndex();
                bool SI = ui->SIradioButton->isChecked();
                bool SIS = ui->SISradioButton->isChecked();
                QString TCOMBO = QString::number(Tcombo);
                QString WCOMBO = QString::number(Wcombo);
                QString FCOMBO = QString::number(Fcombo);
                QString MSI = QString::number(SI);
                QString MSIS = QString::number(SIS);
                strList << MSI;
                strList << MSIS;
                data << strList.join(",")+"\n";
                strList.clear();
                strList << TCOMBO;
                strList << WCOMBO;
                strList << FCOMBO;
                data << strList.join(",")+"\n";
                strList.clear();
                for( int r = 0; r < ui->Workspace->rowCount(); ++r )
                {
                    strList.clear();
                    for( int c = 0; c < ui->Workspace->columnCount(); ++c )
                    {   QTableWidgetItem* item = ui->Workspace->item(r,c);        //Load items
                        if (!item || item->text().isEmpty())                        //Test if there is something at item(r,c)
                        {
                            ui->Workspace->setItem(r,c,new QTableWidgetItem("0"));//IF there is nothing write 0
                        }
                        strList << ui->Workspace->item( r, c )->text();
                    }
                    data << strList.join( "," )+"\n";
                }
                csvFile.close();
            }
        }
        else
        {
            QFile csvFile(file_path);
            if (csvFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
            {
                QTextStream data(&csvFile );
                QStringList strList;
                int Tcombo = ui->TcomboBox->currentIndex();
                int Wcombo = ui->WcomboBox->currentIndex();
                int Fcombo = ui->FcomboBox->currentIndex();
                bool SI = ui->SIradioButton->isChecked();
                bool SIS = ui->SISradioButton->isChecked();
                QString TCOMBO = QString::number(Tcombo);
                QString WCOMBO = QString::number(Wcombo);
                QString FCOMBO = QString::number(Fcombo);
                QString MSI = QString::number(SI);
                QString MSIS = QString::number(SIS);
                strList << MSI;
                strList << MSIS;
                data << strList.join(",")+"\n";
                strList.clear();
                strList << TCOMBO;
                strList << WCOMBO;
                strList << FCOMBO;
                data << strList.join(",")+"\n";
                strList.clear();
                for( int r = 0; r < ui->Workspace->rowCount(); ++r )
                {
                    strList.clear();
                    for( int c = 0; c < ui->Workspace->columnCount(); ++c )
                    {   QTableWidgetItem* item = ui->Workspace->item(r,c);        //Load items
                        if (!item || item->text().isEmpty())                        //Test if there is something at item(r,c)
                        {
                            ui->Workspace->setItem(r,c,new QTableWidgetItem("0"));//IF there is nothing write 0
                        }
                        strList << ui->Workspace->item( r, c )->text();
                    }
                    data << strList.join( "," )+"\n";
                }
                csvFile.close();
            }
        }
}

void TablaDatos::on_Saveas_clicked()  //SAVE AS
{
    //ACCION DE GUARDAR ARCHIVO .CSV
    QString file_name = QFileDialog::getSaveFileName(this,"Save the file");
    if(!file_name.contains(".csv")){
        file_name = file_name + ".csv";
    }
    QFile csvFile(file_name);
    file_path = file_name;
    if (csvFile.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        QTextStream data(&csvFile );
        QStringList strList;
        int Tcombo = ui->TcomboBox->currentIndex();
        int Wcombo = ui->WcomboBox->currentIndex();
        int Fcombo = ui->FcomboBox->currentIndex();
        bool SI = ui->SIradioButton->isChecked();
        bool SIS = ui->SISradioButton->isChecked();
        QString TCOMBO = QString::number(Tcombo);
        QString WCOMBO = QString::number(Wcombo);
        QString FCOMBO = QString::number(Fcombo);
        QString MSI = QString::number(SI);
        QString MSIS = QString::number(SIS);
        strList << MSI;
        strList << MSIS;
        data << strList.join(",")+"\n";
        strList.clear();
        strList << TCOMBO;
        strList << WCOMBO;
        strList << FCOMBO;
        data << strList.join(",")+"\n";
        strList.clear();
        for( int r = 0; r < ui->Workspace->rowCount(); ++r )
        {
            strList.clear();
            for( int c = 0; c < ui->Workspace->columnCount(); ++c )
            {   QTableWidgetItem* item = ui->Workspace->item(r,c);        //Load items
                if (!item || item->text().isEmpty())                        //Test if there is something at item(r,c)
                {
                    ui->Workspace->setItem(r,c,new QTableWidgetItem("0"));//IF there is nothing write 0
                }
                strList << ui->Workspace->item( r, c )->text();
            }
            data << strList.join( "," )+"\n";
        }
        csvFile.close();
    }
}

void TablaDatos::on_DeleteSelpushButton_clicked()
{
    if (BORRARFILA==-1)
    {
        //NO HAY ITEM SELECCIONADO
    }
    else
    {
    ui->Workspace->removeRow(BORRARFILA);
    }
}

void TablaDatos::on_DeleteFirstpushButton_clicked() //BORRAR EL PRIMERO
{
    ui->Workspace->removeRow(0);
}

void TablaDatos::on_DeleteLastpushButton_clicked()
{
    ui->Workspace->removeRow(ui->Workspace->rowCount() - 1);
}

void TablaDatos::on_DeleteAllpushButton_clicked()
{
    ui->Workspace->setRowCount(0);
}

void TablaDatos::on_Workspace_itemClicked(QTableWidgetItem *item)
{
    BORRARFILA = item->row(); //SELECCIONAR FILA A ELIMINAR
}

void TablaDatos::INICIO(int accion)
{
    if(accion==1){  //la accion es NEW
        ui->TcomboBox->setVisible(true);
        ui->WcomboBox->setVisible(true);
        ui->FcomboBox->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_4->setVisible(true);
        ui->label_3->setVisible(true);
        ui->Workspace->setColumnCount(4);
        titulos.clear();
        titulos << "Supply Temperature" << "Target Temperature " <<"WCp" << "Film Coefficient" ;
        ui->Workspace->setHorizontalHeaderLabels(titulos);
        ui->TcomboBox->addItem( "Select a system unit...");
        ui->FcomboBox->addItem( "Select a system unit...");
        ui->WcomboBox->addItem( "Select a system unit...");
        ui->SIradioButton->setEnabled(true);
        ui->SISradioButton->setEnabled(true);
    }else if(accion==2){ //la accion es OPEN
        ui->TcomboBox->setVisible(true);
        ui->FcomboBox->setVisible(true);
        ui->WcomboBox->setVisible(true);
        ui->label_2->setVisible(true);
        ui->label_3->setVisible(true);
        ui->label_4->setVisible(true);
        ui->Workspace->setColumnCount(4);
        titulos.clear();
        titulos << "Supply Temperature" << "Target Temperature " <<"WCp" << "Film Coefficient" ;
        ui->Workspace->setHorizontalHeaderLabels(titulos);
        ui->TcomboBox->addItem( "Select a system unit...");
        ui->FcomboBox->addItem( "Select a system unit...");
        ui->WcomboBox->addItem( "Select a system unit...");
        ui->SIradioButton->setEnabled(true);
        ui->SISradioButton->setEnabled(true);
    }
}

void TablaDatos::on_treeView_clicked(const QModelIndex &index)
{
    sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));
}

void TablaDatos::on_listView_doubleClicked(const QModelIndex &index)
{
    QString file_name = index.data(Qt::DisplayRole).toString();
    QFile file(sPath + "/" + file_name);
    file_path = file_name;
    if (file_path.contains(".csv")){
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            int lineindex=0;                     // file line counter
            QTextStream in(&file);
            int contadorlines=1; // file line counter
            ui->Workspace->clear();
            ui->Workspace->setRowCount(0);
            titulos.clear();
            titulos << "Supply Temperature" << "Target Temperature " <<"WCp" << "Film Coefficient" ;
            ui->Workspace->setHorizontalHeaderLabels(titulos);
            disconnect(ui->Workspace, SIGNAL(cellChanged(int,int)), 0, 0);
            while (!in.atEnd()) {
                if(contadorlines==1){ // se lee el tipo de sistema de unideade
                    QString fileLine = in.readLine();
                    QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                    QString Si = lineToken.at(0); //
                    QString Sis = lineToken.at(1); //
                    QStringList Lista1,Lista2,Lista3;
                    int SI = Si.toInt();
                    int SIS = Sis.toInt();
                    if(SI==1){
                        ui->SIradioButton->setAutoExclusive(false);
                        ui->SIradioButton->setChecked(true);
                        ui->SIradioButton->setAutoExclusive(true);
                        ui->SISradioButton->setAutoExclusive(false);
                        ui->SISradioButton->setChecked(false);
                        ui->SISradioButton->setAutoExclusive(true);
                        Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
                        Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
                        Lista3 << "Select one..." << "Btu/hr °F'" << "Btu/min °F" << "Btu/seg °F" << "Btu/hr °C"
                               << "Btu/min °C" << "Btu/seg °C" << "Btu/hr °R" << "Btu/min °R" << "Btu/seg °R";
                    }else if(SIS==1){
                        ui->SIradioButton->setAutoExclusive(false);
                        ui->SIradioButton->setChecked(false);
                        ui->SIradioButton->setAutoExclusive(true);
                        ui->SISradioButton->setAutoExclusive(false);
                        ui->SISradioButton->setChecked(true);
                        ui->SISradioButton->setAutoExclusive(true);
                        Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
                        Lista2 << "Select one..." << "W/m^2 °K" << "W/m^2 °C";
                        Lista3 << "Select one..." << "Joules/hr °K" << "Joules/min °K" << "Joules/seg °K" << "Joules/hr °C"
                               << "Joules/min °C" << "Joules/seg °C" << "KiloJoules/hr °K" << "KiloJoules/min °K"
                               << "KiloJoules/seg °K" << "KiloJoules/hr °C" << "KiloJoules/min °C" << "KiloJoules/seg °C"
                               << "KiloCal/hr °C" << "KiloCal/min °C" << "KiloCal/seg °C";
                    }
                    ui->TcomboBox->setVisible(true);
                    ui->WcomboBox->setVisible(true);
                    ui->FcomboBox->setVisible(true);
                    ui->TcomboBox->clear();
                    ui->FcomboBox->clear();
                    ui->WcomboBox->clear();
                    ui->TcomboBox->addItems(Lista1);
                    ui->FcomboBox->addItems(Lista2);
                    ui->WcomboBox->addItems(Lista3);
                    ui->label_2->setVisible(true);
                    ui->label_3->setVisible(true);
                    ui->label_4->setVisible(true);
                }else if(contadorlines==2){
                    QString fileLine = in.readLine();
                    QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                    QString Tunidades = lineToken.at(0); //
                    QString Wunidades = lineToken.at(1); //
                    QString Funidades = lineToken.at(2);
                    int TUNIDADES = Tunidades.toInt();
                    int WUNIDADES = Wunidades.toInt();
                    int FUNIDADES = Funidades.toInt();
                    ui->TcomboBox->setCurrentIndex(TUNIDADES);
                    ui->WcomboBox->setCurrentIndex(WUNIDADES);
                    ui->FcomboBox->setCurrentIndex(FUNIDADES);
                }
                else if(contadorlines>=3){
                    ui->Workspace->insertRow(ui->Workspace->rowCount());
                    QString fileLine = in.readLine();
                    QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                    ui->Workspace->setColumnCount(lineToken.size());
                    for (int j = 0; j < lineToken.size(); j++) {
                        QString value = lineToken.at(j);
                        ui->Workspace->setItem(lineindex, j, new QTableWidgetItem(value));
                    }
                    lineindex++;
                }
                contadorlines++;
            }
            file.close();
            connect(ui->Workspace, &QTableWidget::cellChanged,this, &TablaDatos::on_Workspace_cellChanged);
            QFile FileWork(WORKSPACE_FILENAME);
            if (!FileWork.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QVector<QVector<double>> WORKSPACE_matrix;
            int filas = ui->Workspace->rowCount();
            int columnas = ui->Workspace->columnCount();
            WORKSPACE_matrix.resize(filas);
            for(int i = 0; i < filas; i++)
            {
                WORKSPACE_matrix[i].resize(columnas);
            }
            for(int i = 0; i < filas; i++)
            {
                for(int j = 0; j < columnas; j++){
                    WORKSPACE_matrix[i][j] = ui->Workspace->item(i,j)->text().toDouble();
                }
            }
            QDataStream out2(&FileWork);
            out2.setVersion(QDataStream::Qt_5_4);
            Workspace MATRIZ(WORKSPACE_matrix);
            out2 << MATRIZ;
            FileWork.flush();
            FileWork.close();
            // GUARDAR UNIDADES Y SISTEMAS
            QFile FileUnidades(UNIDADES_FILENAME);
            if (!FileUnidades.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out3(&FileUnidades);
            out3.setVersion(QDataStream::Qt_5_4);
            int ITemp = ui->TcomboBox->currentIndex(), IWcp = ui->WcomboBox->currentIndex();
            int Ih  = ui->FcomboBox->currentIndex();
            bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
            Unidades units(SI,SIS,ITemp,IWcp,Ih);
            out3 << units;
            FileUnidades.flush();
            FileUnidades.close();
        }
    }
}

void TablaDatos::on_UploadExistinpushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,"Open the file");
    if(!file_name.contains(".csv")){
        return;
    }
    QFile file(file_name);
    file_path = file_name;
    if (file_path.contains(".csv")){
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            int contadorlines=0;
            disconnect(ui->Workspace, SIGNAL(cellChanged(int,int)), 0, 0);
            int lineindex= ui->Workspace->rowCount();
            while (!in.atEnd()) {
                if(contadorlines<2){
                    QString fileLine = in.readLine();
                }
                else if(contadorlines>=3){
                    ui->Workspace->insertRow(ui->Workspace->rowCount());
                    QString fileLine = in.readLine();
                    QStringList lineToken = fileLine.split(",", QString::SkipEmptyParts);
                    int ColumAct = ui->Workspace->columnCount();
                    int ColumDes = lineToken.size();
                    if(ColumAct==3 && ColumDes==3){
                        ui->Workspace->setColumnCount(ColumAct);
                        titulos.clear();
                        titulos << "Supply Temperature" << "Target Temperature " <<"WCp";
                        ui->Workspace->setHorizontalHeaderLabels(titulos);
                    }
                    else if(ColumAct==4 && ColumDes==3){
                        ui->Workspace->setColumnCount(ColumAct);
                        titulos.clear();
                        titulos << "Supply Temperature" << "Target Temperature " <<"WCp";
                        ui->Workspace->setHorizontalHeaderLabels(titulos);
                    }else if(ColumAct==3 && ColumDes==4){
                        ui->Workspace->setColumnCount(ColumDes);
                        titulos.clear();
                        titulos << "Supply Temperature" << "Target Temperature " <<"WCp" << "Film Coefficient" ;
                        ui->Workspace->setHorizontalHeaderLabels(titulos);
                    }else if(ColumAct==4 && ColumDes==4){
                        ui->Workspace->setColumnCount(ColumDes);
                        titulos.clear();
                        titulos << "Supply Temperature" << "Target Temperature " <<"WCp" << "Film Coefficient" ;
                        ui->Workspace->setHorizontalHeaderLabels(titulos);
                    }
                    for (int j = 0; j < lineToken.size(); j++) {
                        QString value = lineToken.at(j);
                        ui->Workspace->setItem(lineindex, j, new QTableWidgetItem(value));
                    }
                    lineindex++;
                }
                contadorlines++;
            }
            file.close();
            connect(ui->Workspace, &QTableWidget::cellChanged,this, &TablaDatos::on_Workspace_cellChanged);
            QFile FileWork(WORKSPACE_FILENAME);
            if (!FileWork.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QVector<QVector<double>> WORKSPACE_matrix;
            int filas = ui->Workspace->rowCount();
            int columnas = ui->Workspace->columnCount();
            WORKSPACE_matrix.resize(filas);
            for(int i = 0; i < filas; i++)
            {
                WORKSPACE_matrix[i].resize(columnas);
            }
            for(int i = 0; i < filas; i++)
            {
                for(int j = 0; j < columnas; j++){
                    WORKSPACE_matrix[i][j] = ui->Workspace->item(i,j)->text().toDouble();
                }
            }
            QDataStream out2(&FileWork);
            out2.setVersion(QDataStream::Qt_5_4);
            Workspace MATRIZ(WORKSPACE_matrix);
            out2 << MATRIZ;
            FileWork.flush();
            FileWork.close();
            // GUARDAR UNIDADES Y SISTEMAS
            QFile FileUnidades(UNIDADES_FILENAME);
            if (!FileUnidades.open(QIODevice::WriteOnly)){
                QMessageBox::warning(this,tr("Error"),tr("Error"));
                return;
            }
            QDataStream out3(&FileUnidades);
            out3.setVersion(QDataStream::Qt_5_4);
            int ITemp = ui->TcomboBox->currentIndex(), IWcp = ui->WcomboBox->currentIndex();
            int Ih  = ui->FcomboBox->currentIndex();
            bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
            Unidades units(SI,SIS,ITemp,IWcp,Ih);
            out3 << units;
            FileUnidades.flush();
            FileUnidades.close();
        }
    }
}

void TablaDatos::on_Workspace_cellChanged(int row, int column)
{
    row = ui->Workspace->rowCount();
    column = ui->Workspace->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->Workspace->item(i,j)){
               return;
            }else if (ui->Workspace->item(i,j)->text() == "Empty"){
               return;
            }else if (ui->Workspace->item(i,j)->text() == "0"){
               return;
            }
        }
    }
    QFile FileWork(WORKSPACE_FILENAME);
    if (!FileWork.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QVector<QVector<double>> WORKSPACE_matrix;
    int filas = ui->Workspace->rowCount();
    int columnas = ui->Workspace->columnCount();
    WORKSPACE_matrix.resize(filas);
    for(int i = 0; i < filas; i++)
    {
        WORKSPACE_matrix[i].resize(columnas);
    }
    for(int i = 0; i < filas; i++)
    {
        for(int j = 0; j < columnas; j++){
            WORKSPACE_matrix[i][j] = ui->Workspace->item(i,j)->text().toDouble();
        }
    }
    QDataStream out2(&FileWork);
    out2.setVersion(QDataStream::Qt_5_4);
    Workspace MATRIZ(WORKSPACE_matrix);
    out2 << MATRIZ;
    FileWork.flush();
    FileWork.close();
    // GUARDAR UNIDADES Y SISTEMAS
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out3(&FileUnidades);
    out3.setVersion(QDataStream::Qt_5_4);
    int ITemp = ui->TcomboBox->currentIndex(), IWcp = ui->WcomboBox->currentIndex();
    int Ih  = ui->FcomboBox->currentIndex();
    bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
    Unidades units(SI,SIS,ITemp,IWcp,Ih);
    out3 << units;
    FileUnidades.flush();
    FileUnidades.close();
}

void TablaDatos::on_TcomboBox_currentIndexChanged(int index)
{
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out3(&FileUnidades);
    out3.setVersion(QDataStream::Qt_5_4);
    int ITemp = index, IWcp = ui->WcomboBox->currentIndex();
    int Ih  = ui->FcomboBox->currentIndex();
    bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
    Unidades units(SI,SIS,ITemp,IWcp,Ih);
    out3 << units;
    FileUnidades.flush();
    FileUnidades.close();
}

void TablaDatos::on_WcomboBox_currentIndexChanged(int index)
{
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out3(&FileUnidades);
    out3.setVersion(QDataStream::Qt_5_4);
    int ITemp = ui->TcomboBox->currentIndex(), IWcp = index;
    int Ih  = ui->FcomboBox->currentIndex();
    bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
    Unidades units(SI,SIS,ITemp,IWcp,Ih);
    out3 << units;
    FileUnidades.flush();
    FileUnidades.close();
}

void TablaDatos::on_FcomboBox_currentIndexChanged(int index)
{
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out3(&FileUnidades);
    out3.setVersion(QDataStream::Qt_5_4);
    int ITemp = ui->TcomboBox->currentIndex(), IWcp = ui->WcomboBox->currentIndex();
    int Ih  = index;
    bool SI = ui->SIradioButton->isChecked(), SIS = ui->SISradioButton->isChecked();
    Unidades units(SI,SIS,ITemp,IWcp,Ih);
    out3 << units;
    FileUnidades.flush();
    FileUnidades.close();
}

void TablaDatos::SaveToogle(bool Checked)
{
    if(Checked == true){
        on_Save_clicked();
    }
}

void TablaDatos::SaveAsToogle(bool Checked)
{
    if(Checked == true){
        on_Saveas_clicked();
    }
}

