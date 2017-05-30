#include "analisispersonalizado.h"
#include "ui_analisispersonalizado.h"
#include <QMessageBox>
#include "duvaloraction.h"
#include "graficos.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
#include "valoraction.h"


AnalisisPersonalizado::AnalisisPersonalizado(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalisisPersonalizado)
{
    ui->setupUi(this);
    //UNIFORME
    ui->Utext9->setVisible(false);
    ui->Utext10->setVisible(false);
    ui->Utext11->setVisible(false);
    ui->Increment->setVisible(false);
    ui->Maximun->setVisible(false);
    ui->Minimun->setVisible(false);
    //DIVERSO
    ui->Dtext4->setVisible(false);
    ui->Dtext5->setVisible(false);
    ui->Dtext6->setVisible(false);
    ui->Dtext7->setVisible(false);
    ui->Dminimun->setVisible(false);
    ui->Dmaximun->setVisible(false);
    ui->Dincrement->setVisible(false);
    ui->Dk->setVisible(false);
    QStringList tituloscapital,titulosoperacion;
    QString titulos;
    tituloscapital << "a" << "b" <<"c" << "i" << "n";
    titulosoperacion << "Heating Service" << "Cooling Service";
    titulos = "$$ US/Btu";
    //CHECKBOXES
    ui->UPT->setVisible(false);
    ui->UAP->setVisible(false);
    ui->UCP->setVisible(false);
    ui->UHCC->setVisible(false);
    ui->UCCC->setVisible(false);
    ui->UCC->setVisible(false);
    ui->UCCA->setVisible(false);
    ui->UGCC->setVisible(false);
    ui->UADTM->setVisible(false);
    ui->UCDTM->setVisible(false);
    ui->DPT->setVisible(false);
    ui->DAP->setVisible(false);
    ui->DCP->setVisible(false);
    ui->DHCC->setVisible(false);
    ui->DCCC->setVisible(false);
    ui->DCC->setVisible(false);
    ui->DCCA->setVisible(false);
    ui->DGCC->setVisible(false);
    ui->DADTM->setVisible(false);
    ui->DCDTM->setVisible(false);
    //UNIFORME
    ui->UtableCapital->setColumnCount(5);
    ui->UtableCapital->setHorizontalHeaderLabels(tituloscapital);
    ui->UtableCapital->setItem(0, 0, new QTableWidgetItem("Empty"));
    ui->UtableCapital->setItem(0, 1, new QTableWidgetItem("Empty"));
    ui->UtableCapital->setItem(0, 2, new QTableWidgetItem("Empty"));
    ui->UtableCapital->setItem(0, 3, new QTableWidgetItem("Empty"));
    ui->UtableCapital->setItem(0, 4, new QTableWidgetItem("Empty"));
    ui->UtableOperation->setColumnCount(1);
    ui->UtableOperation->setHorizontalHeaderItem(0,new QTableWidgetItem(titulos));
    ui->UtableOperation->setVerticalHeaderLabels(titulosoperacion);
    ui->UtableOperation->setItem(0,0,new QTableWidgetItem("Empty"));
    ui->UtableOperation->setItem(1,0,new QTableWidgetItem("Empty"));
    //DIVERSO
    ui->DtableCapital->setColumnCount(5);
    ui->DtableCapital->setHorizontalHeaderLabels(tituloscapital);
    ui->DtableCapital->setItem(0, 0, new QTableWidgetItem("Empty"));
    ui->DtableCapital->setItem(0, 1, new QTableWidgetItem(""));
    ui->DtableCapital->setItem(0, 2, new QTableWidgetItem(""));
    ui->DtableCapital->setItem(0, 3, new QTableWidgetItem(""));
    ui->DtableCapital->setItem(0, 4, new QTableWidgetItem(""));
    ui->DtableOperation->setColumnCount(1);
    ui->DtableOperation->setHorizontalHeaderItem(0,new QTableWidgetItem(titulos));
    ui->DtableOperation->setVerticalHeaderLabels(titulosoperacion);
    ui->DtableOperation->setItem(0,0,new QTableWidgetItem(""));
    ui->DtableOperation->setItem(1,0,new QTableWidgetItem(""));
    //a+b*(A)^c

}

AnalisisPersonalizado::~AnalisisPersonalizado()
{
    delete ui;
}

void AnalisisPersonalizado::on_StaticradioButton_clicked()
{
    botones_uniformes();
}


void AnalisisPersonalizado::on_IncrementradioButton_clicked()
{
    botones_uniformes();
}


void AnalisisPersonalizado::on_DStaticradioButton_clicked()
{
    botones_diversos();
}


void AnalisisPersonalizado::on_DIncrementradioButton_clicked()
{
    botones_diversos();
}



void AnalisisPersonalizado::botones_uniformes()
{
    bool incremento= ui->IncrementradioButton->isChecked();
    bool estatico = ui->StaticradioButton->isChecked();
    if (estatico == true){
        // se borra todo
        ui->Utext9->setVisible(false);
        ui->Utext10->setVisible(false);
        ui->Utext11->setVisible(false);
        ui->Increment->setVisible(false);
        ui->Maximun->setVisible(false);
        ui->Minimun->setVisible(false);
        // CHECBOXES UNIFORME ESTATICO
        ui->UPT->setVisible(false);
        ui->UAP->setVisible(false);
        ui->UCP->setVisible(false);
        ui->UHCC->setVisible(false);
        ui->UCCC->setVisible(false);
        ui->UCC->setVisible(false);
        ui->UCCA->setVisible(false);
        ui->UGCC->setVisible(false);
        ui->UADTM->setVisible(false);
        ui->UCDTM->setVisible(false);
        // se pone CHECBOXES
        ui->UPT->setVisible(true);
        ui->UAP->setVisible(true);
        ui->UCP->setVisible(true);
        ui->UHCC->setVisible(true);
        ui->UCCC->setVisible(true);
        ui->UCC->setVisible(true);
        ui->UCCA->setVisible(true);
        ui->UGCC->setVisible(true);
        //
        ui->Utext9->setVisible(true);
        ui->Utext9->setText("Value:");
        ui->Utext10->setVisible(false);
        ui->Utext11->setVisible(false);
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(false);
        ui->Increment->setVisible(false);

    }
    else if (incremento == true){
        // se borra todo
        ui->Utext9->setVisible(false);
        ui->Utext10->setVisible(false);
        ui->Utext11->setVisible(false);
        ui->Increment->setVisible(false);
        ui->Maximun->setVisible(false);
        ui->Minimun->setVisible(false);
        // CHECBOXES UNIFORME INCREMENTO
        ui->UPT->setVisible(false);
        ui->UAP->setVisible(false);
        ui->UCP->setVisible(false);
        ui->UHCC->setVisible(false);
        ui->UCCC->setVisible(false);
        ui->UCC->setVisible(false);
        ui->UCCA->setVisible(false);
        ui->UGCC->setVisible(false);
        ui->UADTM->setVisible(false);
        ui->UCDTM->setVisible(false);
        // se pone CHECKOBEX
        ui->UPT->setVisible(true);
        ui->UAP->setVisible(true);
        ui->UCP->setVisible(true);
        ui->UHCC->setVisible(true);
        ui->UCCC->setVisible(true);
        ui->UCC->setVisible(true);
        ui->UCCA->setVisible(true);
        ui->UGCC->setVisible(true);
        ui->UADTM->setVisible(true);
        ui->UCDTM->setVisible(true);
        //
        ui->Utext9->setVisible(true);
        ui->Utext9->setText("Minimun:");
        ui->Utext10->setVisible(true);
        ui->Utext10->setText("Maximun");
        ui->Utext11->setVisible(true);
        ui->Utext11->setText("Increment");
        ui->Minimun->setVisible(true);
        ui->Maximun->setVisible(true);
        ui->Increment->setVisible(true);
    }
}

void AnalisisPersonalizado::botones_diversos()
{
    bool incremento= ui->DIncrementradioButton->isChecked();
    bool estatico = ui->DStaticradioButton->isChecked();
    if (estatico == true){
        // se borra todo
        ui->Dtext4->setVisible(false);
        ui->Dtext5->setVisible(false);
        ui->Dtext6->setVisible(false);
        ui->Dtext7->setVisible(false);
        ui->Dminimun->setVisible(false);
        ui->Dmaximun->setVisible(false);
        ui->Dincrement->setVisible(false);
        ui->Dk->setVisible(false);
        // CHECBOXES DIVERSOS ESTATICO
        ui->DPT->setVisible(false);
        ui->DAP->setVisible(false);
        ui->DCP->setVisible(false);
        ui->DHCC->setVisible(false);
        ui->DCCC->setVisible(false);
        ui->DCC->setVisible(false);
        ui->DCCA->setVisible(false);
        ui->DGCC->setVisible(false);
        ui->DADTM->setVisible(false);
        ui->DCDTM->setVisible(false);
        // se pone CHEKCBOXES
        ui->DPT->setVisible(true);
        ui->DHCC->setVisible(true);
        ui->DCCC->setVisible(true);
        ui->DCC->setVisible(true);
        ui->DCCA->setVisible(true);
        ui->DGCC->setVisible(true);

        ui->Dtext4->setVisible(true);
        ui->Dtext4->setText("Value:");
        ui->Dminimun->setVisible(true);
        ui->Dk->setVisible(true);
        ui->Dtext7->setVisible(true);
        ui->Dtext7->setText("K");
    }
    else if (incremento == true){
        // se borra todo
        ui->Dtext4->setVisible(false);
        ui->Dtext5->setVisible(false);
        ui->Dtext6->setVisible(false);
        ui->Dtext7->setVisible(false);
        ui->Dminimun->setVisible(false);
        ui->Dmaximun->setVisible(false);
        ui->Dincrement->setVisible(false);
        ui->Dk->setVisible(false);
        //CHECKBOXES DIVERSO UNIFORME
        ui->DPT->setVisible(false);
        ui->DAP->setVisible(false);
        ui->DCP->setVisible(false);
        ui->DHCC->setVisible(false);
        ui->DCCC->setVisible(false);
        ui->DCC->setVisible(false);
        ui->DCCA->setVisible(false);
        ui->DGCC->setVisible(false);
        ui->DADTM->setVisible(false);
        ui->DCDTM->setVisible(false);
        // se pone CHECKBOXES
        ui->DPT->setVisible(true);
        ui->DAP->setVisible(true);
        ui->DCP->setVisible(true);
        ui->DHCC->setVisible(true);
        ui->DCCC->setVisible(true);
        ui->DCC->setVisible(true);
        ui->DCCA->setVisible(true);
        ui->DGCC->setVisible(true);
        ui->DADTM->setVisible(true);
        ui->DCDTM->setVisible(true);
        // se pone
        ui->Dtext7->setVisible(true);
        ui->Dtext7->setText("K");
        ui->Dtext4->setVisible(true);
        ui->Dtext4->setText("Minimun:");
        ui->Dtext5->setVisible(true);
        ui->Dtext5->setText("Maximun");
        ui->Dtext6->setVisible(true);
        ui->Dtext6->setText("Increment");
        ui->Dk->setVisible(true);
        ui->Dminimun->setVisible(true);
        ui->Dmaximun->setVisible(true);
        ui->Dincrement->setVisible(true);
    }
}

void AnalisisPersonalizado::on_UAP_clicked()
{
    Ucheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_UCP_clicked()
{
    Ucheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_UADTM_clicked()
{
    Ucheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_UCDTM_clicked()
{
    Ucheckboxes_seleccionadas();
}

//void AnalisisPersonalizado::on_DAP_clicked()
//{
//    Ucheckboxes_seleccionadas();
//}

//void AnalisisPersonalizado::on_DCP_clicked()
//{
//    Ucheckboxes_seleccionadas();
//}

//void AnalisisPersonalizado::on_DADTM_clicked()
//{
//    Ucheckboxes_seleccionadas();
//}

//void AnalisisPersonalizado::on_DCDTM_clicked()
//{
//    Ucheckboxes_seleccionadas();
//}

void AnalisisPersonalizado::Ucheckboxes_seleccionadas()
{
    bool SI = ui->StaticradioButton->isChecked();
    bool SIS = ui->IncrementradioButton->isChecked();
    bool uap = ui->UAP->isChecked();
    bool ucp = ui->UCP->isChecked();
    bool uadtm = ui->UADTM->isChecked();
    bool ucdtm = ui->UCDTM->isChecked();
    if(SI==true){
        // aqui se tiene solamente 2 opciones
        if(uap==true && ucp==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && ucp==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
        else if(uap==false && ucp==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && ucp==false){
            ui->Utext4->setEnabled(false); //area
            ui->Utext7->setEnabled(false); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(false);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
    }
    else if (SIS==true){
        if(uap==true && uadtm==true && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==true && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==false && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==false && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && uadtm==false && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
        else if(uap==true && uadtm==true && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
        else if(uap==true && uadtm==true && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && uadtm==false && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && uadtm==false && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==true && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==true && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==true && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
        else if(uap==false && uadtm==false && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && uadtm==true && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==true && uadtm==false && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UtableCapital->setEnabled(true); // tabla de capital
            ui->UtableOperation->setEnabled(true); // tabla de operation
        }
        else if(uap==false && uadtm==false && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(false); //area
            ui->Utext7->setEnabled(false); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(false);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UtableCapital->setEnabled(false); // tabla de capital
            ui->UtableOperation->setEnabled(false); // tabla de operation
        }
    }
}

void AnalisisPersonalizado::on_OK_clicked()
{
    QFile F(WORKSPACE_FILENAME);
    if (!F.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
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
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> Wcp(Matriz.size()),h(Matriz.size());
    for(int i = 0; i < Matriz.size();i++){
        TS[i] = Matriz[i][0];
        TE[i] = Matriz[i][1];
        Wcp[i] = Matriz[i][2];
        h[i] = Matriz[i][3];
    }
    F.flush();
    F.close();
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Nada no pasa nada"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    int UTemp = units.getUTemp();
    int UWcp = units.getUWcp();;
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,Wcp,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    Wcp = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    int TabAnali = ui->tabWidget->currentIndex();
    VENTANAMADRE = 1; // analisis personalizado
    QFile F(TAB_A_P);
        if (!F.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out26(&F);
    out26.setVersion(QDataStream::Qt_5_4);
    TabAnalisis TA(TabAnali,VENTANAMADRE);
    out26 << TA;
    F.flush();
    F.close();
    if(TabAnali == 0){// se selecciono la metodologia uniforme
        estatico = ui->StaticradioButton->isChecked();
        incremento = ui->IncrementradioButton->isChecked();
        if(estatico == true){ // se selecciono analisis estatico
            bool UPT = ui->UPT->isChecked();
            bool UAP = ui->UAP->isChecked();
            bool UCP = ui->UCP->isChecked();
            bool UHCC = ui->UHCC->isChecked();
            bool UCCC = ui->UCCC->isChecked();
            bool UCC = ui->UCC->isChecked();
            bool UCCA = ui->UCCA->isChecked();
            bool UGCC = ui->UGCC->isChecked();
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Info_Uniforme_Estatico(UPT,UAP,UCP,UHCC,UCCC,UCC,UCCA,UGCC,Min,Max,Inc,estatico,incremento);
        }else if(incremento == true){ // se selecciono analisis dinamico

        }
    }else if(TabAnali == 1){// se selecciono la metodologia diversa

    }else if(TabAnali == 2){// se seleccionaron ambas metodologias

    }
}

void AnalisisPersonalizado::Info_Uniforme_Estatico(bool UPT, bool UAP, bool UCP,
                                                   bool UHCC, bool UCCC, bool UCC,
                                                   bool UCCA, bool UGCC, double Min,
                                                   double Max, double Inc, bool estatico,
                                                   bool incremento)
{
    QFile F(INFO_A_P_UNIFORME);
        if (!F.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out25(&F);
    out25.setVersion(QDataStream::Qt_5_4);
    infoapuniforme info(Checkboxes,Min,Max,Inc,estatico,incremento);
    out25 << info;
    F.flush();
    F.close();
    if(UPT == true){
        Checkboxes[0] = 1;
    }else if(UPT == false){
        Checkboxes[0] = 0;
    }
    if(UAP == true){
        Checkboxes[1] = 1;
    }else if(UAP == false){
        Checkboxes[1] = 0;
    }
    if(UCP == true){
        Checkboxes[2] = 1;
    }else if(UCP == false){
        Checkboxes[2] = 0;
    }
    if(UHCC == true){
        Checkboxes[3] = 1;
    }else if(UHCC == false){
        Checkboxes[3] = 0;
    }
    if(UCC == true){
        Checkboxes[4] = 1;
    }else if(UCC == false){
        Checkboxes[4] = 0;
    }
    if(UCCC == true){
        Checkboxes[5] = 1;
    }else if(UCCC = false){
        Checkboxes[5] = 0;
    }
    if(UCCA == true){
        Checkboxes[6] = 1;
    }else if(UCCA == false){
        Checkboxes[6] = 0;
    }
    if(UGCC == true){
        Checkboxes[7] = 1;
    }else if(UGCC ==  false){
        Checkboxes[7] = 0;
    }
    infoapuniforme info(Checkboxes,Min,Max,Inc,estatico,incremento);
    out25 << info;
    F.flush();
    F.close();
}
