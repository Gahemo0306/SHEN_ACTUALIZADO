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
    ui->Dhelp->setVisible(false);
    //BOTH
    ui->Btext4->setVisible(false);
    ui->Btext5->setVisible(false);
    ui->Btext6->setVisible(false);
    ui->Btext7->setVisible(false);
    ui->Bminimun->setVisible(false);
    ui->Bmaximun->setVisible(false);
    ui->Bincrement->setVisible(false);
    ui->Bk->setVisible(false);
    ui->Bhelp->setVisible(false);
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
    ui->BPT->setVisible(false);
    ui->BAP->setVisible(false);
    ui->BCP->setVisible(false);
    ui->BHCC->setVisible(false);
    ui->BCCC->setVisible(false);
    ui->BCC->setVisible(false);
    ui->BCCA->setVisible(false);
    ui->BGCC->setVisible(false);
    ui->BADTM->setVisible(false);
    ui->BCDTM->setVisible(false);
    QFile FileUnidades(UNIDADES_FILENAME);
    if (!FileUnidades.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in3(&FileUnidades);
    in3.setVersion(QDataStream::Qt_5_4);
    Unidades units;
    in3 >> units;
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    FileUnidades.flush();
    FileUnidades.close();
    QStringList Lista1,Lista2,Lista3;
    if(SI == true){
        ui->Uunits_temp->clear();
        ui->Uunits_film->clear();
        ui->Ucapital_units->clear();
        ui->Dunits_temp->clear();
        ui->Dunits_film->clear();
        ui->Dcapital_units->clear();
        ui->Bunits_temp->clear();
        ui->Bunits_film->clear();
        ui->Bcapital_units->clear();
        Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
        Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
        Lista3 << "Select one..." << "US $ / Btu" << "US $ / CHU";
        ui->Uunits_temp->addItems(Lista1);
        ui->Uunits_film->addItems(Lista2);
        ui->Ucapital_units->addItems(Lista3);
        ui->Dunits_temp->addItems(Lista1);
        ui->Dunits_film->addItems(Lista2);
        ui->Dcapital_units->addItems(Lista3);
        ui->Bunits_temp->addItems(Lista1);
        ui->Bunits_film->addItems(Lista2);
        ui->Bcapital_units->addItems(Lista3);
    }else if(SIS == true){
        ui->Uunits_temp->clear();
        ui->Uunits_film->clear();
        ui->Ucapital_units->clear();
        ui->Dunits_temp->clear();
        ui->Dunits_film->clear();
        ui->Dcapital_units->clear();
        ui->Bunits_temp->clear();
        ui->Bunits_film->clear();
        ui->Bcapital_units->clear();
        Lista1 << "Select one..." << "Kelvin (°K)" << "Farenheit (°F)" << "Celsius (°C)" << "Ranking (°R)";
        Lista2 << "Select one..." << "Btu/hr ft^2 °F" << "CHU/hr ft^2 °F";
        Lista3 << "Select one..." << "US $ / Btu" << "US $ / CHU";
        ui->Uunits_temp->addItems(Lista1);
        ui->Uunits_film->addItems(Lista2);
        ui->Ucapital_units->addItems(Lista3);
        ui->Dunits_temp->addItems(Lista1);
        ui->Dunits_film->addItems(Lista2);
        ui->Dcapital_units->addItems(Lista3);
        ui->Bunits_temp->addItems(Lista1);
        ui->Bunits_film->addItems(Lista2);
        ui->Bcapital_units->addItems(Lista3);
    }

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

void AnalisisPersonalizado::on_BStaticradioButton_clicked()
{
    botones_both();
}

void AnalisisPersonalizado::on_BIncrementradioButton_clicked()
{
    botones_both();
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
        ui->Dhelp->setVisible(false);
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
        ui->DAP->setVisible(true);
        ui->DCP->setVisible(true);
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
        ui->Dtext7->setText("K:");
        ui->Dhelp->setVisible(true);
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
        ui->Dhelp->setVisible(true);
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

void AnalisisPersonalizado::botones_both()
{
    bool incremento= ui->BIncrementradioButton->isChecked();
    bool estatico = ui->BStaticradioButton->isChecked();
    if (estatico == true){
        // se borra todo
        ui->Btext4->setVisible(false);
        ui->Btext5->setVisible(false);
        ui->Btext6->setVisible(false);
        ui->Btext7->setVisible(false);
        ui->Bminimun->setVisible(false);
        ui->Bmaximun->setVisible(false);
        ui->Bincrement->setVisible(false);
        ui->Bk->setVisible(false);
        ui->Bhelp->setVisible(false);
        // CECBOXES DIVERSOS ESTATICO
        ui->BPT->setVisible(false);
        ui->BAP->setVisible(false);
        ui->BCP->setVisible(false);
        ui->BHCC->setVisible(false);
        ui->BCCC->setVisible(false);
        ui->BCC->setVisible(false);
        ui->BCCA->setVisible(false);
        ui->BGCC->setVisible(false);
        ui->BADTM->setVisible(false);
        ui->BCDTM->setVisible(false);
        // se pone CHEKCBOXES
        ui->BPT->setVisible(true);
        ui->BAP->setVisible(true);
        ui->BCP->setVisible(true);
        ui->BHCC->setVisible(true);
        ui->BCCC->setVisible(true);
        ui->BCC->setVisible(true);
        ui->BCCA->setVisible(true);
        ui->BGCC->setVisible(true);

        ui->Btext4->setVisible(true);
        ui->Btext4->setText("Value:");
        ui->Bminimun->setVisible(true);
        ui->Bk->setVisible(true);
        ui->Btext7->setVisible(true);
        ui->Btext7->setText("K:");
        ui->Bhelp->setVisible(true);
    }
    else if (incremento == true){
        // se borra todo
        ui->Btext4->setVisible(false);
        ui->Btext5->setVisible(false);
        ui->Btext6->setVisible(false);
        ui->Btext7->setVisible(false);
        ui->Bminimun->setVisible(false);
        ui->Bmaximun->setVisible(false);
        ui->Bincrement->setVisible(false);
        ui->Bk->setVisible(false);
        ui->Bhelp->setVisible(true);
        //CHECKBOXES DIVERSO UNIFORME
        ui->BPT->setVisible(false);
        ui->BAP->setVisible(false);
        ui->BCP->setVisible(false);
        ui->BHCC->setVisible(false);
        ui->BCCC->setVisible(false);
        ui->BCC->setVisible(false);
        ui->BCCA->setVisible(false);
        ui->BGCC->setVisible(false);
        ui->BADTM->setVisible(false);
        ui->BCDTM->setVisible(false);
        // se pone CHECKBOXES
        ui->BPT->setVisible(true);
        ui->BAP->setVisible(true);
        ui->BCP->setVisible(true);
        ui->BHCC->setVisible(true);
        ui->BCCC->setVisible(true);
        ui->BCC->setVisible(true);
        ui->BCCA->setVisible(true);
        ui->BGCC->setVisible(true);
        ui->BADTM->setVisible(true);
        ui->BCDTM->setVisible(true);
        // sB pone
        ui->Btext7->setVisible(true);
        ui->Btext7->setText("K");
        ui->Btext4->setVisible(true);
        ui->Btext4->setText("Minimun:");
        ui->Btext5->setVisible(true);
        ui->Btext5->setText("Maximun");
        ui->Btext6->setVisible(true);
        ui->Btext6->setText("Increment");
        ui->Bk->setVisible(true);
        ui->Bminimun->setVisible(true);
        ui->Bmaximun->setVisible(true);
        ui->Bincrement->setVisible(true);
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

void AnalisisPersonalizado::on_DAP_clicked()
{
    Dcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_DCP_clicked()
{
    Dcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_DADTM_clicked()
{
    Dcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_DCDTM_clicked()
{
    Dcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::Ucheckboxes_seleccionadas()
{
    bool estatico = ui->StaticradioButton->isChecked();
    bool incremento = ui->IncrementradioButton->isChecked();
    bool uap = ui->UAP->isChecked();
    bool ucp = ui->UCP->isChecked();
    bool uadtm = ui->UADTM->isChecked();
    bool ucdtm = ui->UCDTM->isChecked();
    if(estatico == true){
        // aqui se tiene solamente 2 opciones
        if(uap==true && ucp==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
        }
        else if(uap==true && ucp==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
        }
        else if(uap==false && ucp==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
        }
        else if(uap==false && ucp==false){
            ui->Utext4->setEnabled(false); //area
            ui->Utext7->setEnabled(false); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(false);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->Uunits_temp->setEnabled(false); // temperaturas
            ui->Uunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
            ui->UText20->setEnabled(false); // label unidades temp
            ui->UText21->setEnabled(false); // label unidades film
        }
    }
    else if (incremento == true){
        if(uap==true && uadtm==true && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
        }
        else if(uap==false && uadtm==true && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==false && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==false && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==true && uadtm==false && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
        }
        else if(uap==true && uadtm==true && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
        }
        else if(uap==true && uadtm==true && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==true && uadtm==false && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==true && uadtm==false && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==true && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==true && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==true && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
        }
        else if(uap==false && uadtm==false && ucp==true && ucdtm==false){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==true && uadtm==true && ucp==false && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==true && uadtm==false && ucp==true && ucdtm==true){
            ui->Utext4->setEnabled(true); //area
            ui->Utext7->setEnabled(true); //Utext7 type of design
            ui->Utext3->setEnabled(true); //Utext3 cost requiremente
            ui->Utext6->setEnabled(true); //Utext6 capital cost
            ui->Utext8->setEnabled(true); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(true);  // type of desing
            ui->UFCapcomboBox->setEnabled(true); // type costo de capital
            ui->UText20->setEnabled(true); // label unidades temp
            ui->UText21->setEnabled(true); // label unidades film
            ui->UAVISO->setEnabled(true); // label aviso
            ui->U_unitsoperation->setEnabled(true); //label unidades costos
            ui->Uunits_temp->setEnabled(true); // temperaturas
            ui->Uunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(true); // capital unidades
        }
        else if(uap==false && uadtm==false && ucp==false && ucdtm==false){
            ui->Utext4->setEnabled(false); //area
            ui->Utext7->setEnabled(false); //Utext7 type of design
            ui->Utext3->setEnabled(false); //Utext3 cost requiremente
            ui->Utext6->setEnabled(false); //Utext6 capital cost
            ui->Utext8->setEnabled(false); //Utext8 operation cost
            ui->UTodcomboBox->setEnabled(false);  // type of desing
            ui->UFCapcomboBox->setEnabled(false); // type costo de capital
            ui->UText20->setEnabled(false); // label unidades temp
            ui->UText21->setEnabled(false); // label unidades film
            ui->UAVISO->setEnabled(false); // label aviso
            ui->U_unitsoperation->setEnabled(false); //label unidades costos
            ui->Uunits_temp->setEnabled(false); // temperaturas
            ui->Uunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Ucapital_units->setEnabled(false); // capital unidades
        }
    }
}

void AnalisisPersonalizado::Dcheckboxes_seleccionadas()
{
    bool Destatico = ui->DStaticradioButton->isChecked();
    bool Dincremento = ui->DIncrementradioButton->isChecked();
    bool Dap = ui->DAP->isChecked();
    bool Dcp = ui->DCP->isChecked();
    bool Dadtm = ui->DADTM->isChecked();
    bool Dcdtm = ui->DCDTM->isChecked();
    if(Destatico == true){
        // aqui se tiene solamente 2 opciones
        if(Dap==true && Dcp==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
        }
        else if(Dap==true && Dcp==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
        }
        else if(Dap==false && Dcp==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
        }
        else if(Dap==false && Dcp==false){
            ui->Dtext9->setEnabled(false); //areas
            ui->Dtext11->setEnabled(false); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->DTodcomboBox->setEnabled(false);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->Dunits_temp->setEnabled(false); // temperaturas
            ui->Dunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
            ui->Dtext20->setEnabled(false); // label unidades temp
            ui->Dtext21->setEnabled(false); // label unidades film
        }
    }
    else if (Dincremento == true){
        if(Dap==true && Dadtm==true && Dcp==true && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
        }
        else if(Dap==false && Dadtm==true && Dcp==true && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==false && Dcp==true  && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==false && Dcp==false && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==true  && Dadtm==false && Dcp==false && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
        }
        else if(Dap==true  && Dadtm==true  && Dcp==false && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
        }
        else if(Dap==true && Dadtm==true && Dcp==true && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==true && Dadtm==false && Dcp==true && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==true && Dadtm==false && Dcp==false && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==true && Dcp==true && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==true && Dcp==false && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==true && Dcp==false && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
        }
        else if(Dap==false && Dadtm==false && Dcp==true && Dcdtm==false){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==true && Dadtm==true && Dcp==false && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==true && Dadtm==false && Dcp==true && Dcdtm==true){
            ui->Dtext9->setEnabled(true); //areas
            ui->Dtext11->setEnabled(true); // tpe design
            ui->Dtext8->setEnabled(true); // costrequirement
            ui->Dtext10->setEnabled(true); // cost capital
            ui->Dtext12->setEnabled(true); // cost opera
            ui->DTodcomboBox->setEnabled(true);  // type of desing
            ui->DFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Dtext20->setEnabled(true); // label unidades temp
            ui->Dtext21->setEnabled(true); // label unidades film
            ui->DAVISO->setEnabled(true); // label aviso
            ui->D_unitsoperation->setEnabled(true); //label unidades costos
            ui->Dunits_temp->setEnabled(true); // temperaturas
            ui->Dunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(true); // capital unidades
        }
        else if(Dap==false && Dadtm==false && Dcp==false && Dcdtm==false){
            ui->Dtext9->setEnabled(false); //areas
            ui->Dtext11->setEnabled(false); // tpe design
            ui->Dtext8->setEnabled(false); // costrequirement
            ui->Dtext10->setEnabled(false); // cost capital
            ui->Dtext12->setEnabled(false); // cost opera
            ui->DTodcomboBox->setEnabled(false);  // type of desing
            ui->DFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Dtext20->setEnabled(false); // label unidades temp
            ui->Dtext21->setEnabled(false); // label unidades film
            ui->DAVISO->setEnabled(false); // label aviso
            ui->D_unitsoperation->setEnabled(false); //label unidades costos
            ui->Dunits_temp->setEnabled(false); // temperaturas
            ui->Dunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Dcapital_units->setEnabled(false); // capital unidades
        }
    }
}

void AnalisisPersonalizado::Bcheckboxes_seleccionadas()
{
    bool Bestatico = ui->BStaticradioButton->isChecked();
    bool Bincremento = ui->BIncrementradioButton->isChecked();
    bool Bap = ui->BAP->isChecked();
    bool Bcp = ui->BCP->isChecked();
    bool Badtm = ui->BADTM->isChecked();
    bool Bcdtm = ui->BCDTM->isChecked();
    if(Bestatico == true){
        // aqui se tiene solamente 2 opciones
        if(Bap==true && Bcp==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
        }
        else if(Bap==true && Bcp==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
        }
        else if(Bap==false && Bcp==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
        }
        else if(Bap==false && Bcp==false){
            ui->Btext9->setEnabled(false); //areas
            ui->Btext11->setEnabled(false); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->BTodcomboBox->setEnabled(false);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->Bunits_temp->setEnabled(false); // temperaturas
            ui->Bunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
            ui->Btext20->setEnabled(false); // label unidades temp
            ui->Btext21->setEnabled(false); // label unidades film
        }
    }
    else if (Bincremento == true){
        if(Bap==true && Badtm==true && Bcp==true && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
        }
        else if(Bap==false && Badtm==true && Bcp==true && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==false && Bcp==true  && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==false && Bcp==false && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==true  && Badtm==false && Bcp==false && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
        }
        else if(Bap==true  && Badtm==true  && Bcp==false && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
        }
        else if(Bap==true &&  Badtm==true && Bcp==true && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==true && Badtm==false && Bcp==true && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==true && Badtm==false && Bcp==false && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==true && Bcp==true && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==true && Bcp==false && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==true && Bcp==false && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
        }
        else if(Bap==false && Badtm==false && Bcp==true && Bcdtm==false){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==true && Badtm==true && Bcp==false && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==true && Badtm==false && Bcp==true && Bcdtm==true){
            ui->Btext9->setEnabled(true); //areas
            ui->Btext11->setEnabled(true); // tpe design
            ui->Btext8->setEnabled(true); // costrequirement
            ui->Btext10->setEnabled(true); // cost capital
            ui->Btext12->setEnabled(true); // cost opera
            ui->BTodcomboBox->setEnabled(true);  // type of desing
            ui->BFCapcomboBox->setEnabled(true); // type costo de capital
            ui->Btext20->setEnabled(true); // label unidades temp
            ui->Btext21->setEnabled(true); // label unidades film
            ui->BAVISO->setEnabled(true); // label aviso
            ui->B_unitsoperation->setEnabled(true); //label unidades costos
            ui->Bunits_temp->setEnabled(true); // temperaturas
            ui->Bunits_film->setEnabled(true); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(true); // capital unidades
        }
        else if(Bap==false && Badtm==false && Bcp==false && Bcdtm==false){
            ui->Btext9->setEnabled(false); //areas
            ui->Btext11->setEnabled(false); // tpe design
            ui->Btext8->setEnabled(false); // costrequirement
            ui->Btext10->setEnabled(false); // cost capital
            ui->Btext12->setEnabled(false); // cost opera
            ui->BTodcomboBox->setEnabled(false);  // type of desing
            ui->BFCapcomboBox->setEnabled(false); // type costo de capital
            ui->Btext20->setEnabled(false); // label unidades temp
            ui->Btext21->setEnabled(false); // label unidades film
            ui->BAVISO->setEnabled(false); // label aviso
            ui->B_unitsoperation->setEnabled(false); //label unidades costos
            ui->Bunits_temp->setEnabled(false); // temperaturas
            ui->Bunits_film->setEnabled(false); // coeficiente de pelicula
            ui->Bcapital_units->setEnabled(false); // capital unidades
        }
    }
}

void AnalisisPersonalizado::on_OK_clicked()
{
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
            bool UADTM = ui->UADTM->isChecked();
            bool UCDTM = ui->UCDTM->isChecked();
            Min = ui->Minimun->value();
            Max = 0;//ui->Maximun->value();
            Inc = 0;//ui->Increment->value();
            Info_Uniforme(UPT,UAP,UCP,UHCC,UCCC,UCC,UCCA,UGCC,UADTM,UCDTM,estatico,incremento);
        }else if(incremento == true){ // se selecciono analisis dinamico
            bool UPT = ui->UPT->isChecked();
            bool UAP = ui->UAP->isChecked();
            bool UCP = ui->UCP->isChecked();
            bool UHCC = ui->UHCC->isChecked();
            bool UCCC = ui->UCCC->isChecked();
            bool UCC = ui->UCC->isChecked();
            bool UCCA = ui->UCCA->isChecked();
            bool UGCC = ui->UGCC->isChecked();
            bool UADTM = ui->UADTM->isChecked();
            bool UCDTM = ui->UCDTM->isChecked();
            Min = ui->Minimun->value();
            Max = ui->Maximun->value();
            Inc = ui->Increment->value();
            Info_Uniforme(UPT,UAP,UCP,UHCC,UCCC,UCC,UCCA,UGCC,UADTM,UCDTM,estatico,incremento);
        }
    }else if(TabAnali == 1){// se selecciono la metodologia diversa
        estatico = ui->DStaticradioButton->isChecked();
        incremento = ui->DIncrementradioButton->isChecked();
        if(estatico == true){ // se selecciono analisis estatico
            bool DPT = ui->DPT->isChecked();
            bool DAP = ui->DAP->isChecked();
            bool DCP = ui->DCP->isChecked();
            bool DHCC = ui->DHCC->isChecked();
            bool DCCC = ui->DCCC->isChecked();
            bool DCC = ui->DCC->isChecked();
            bool DCCA = ui->DCCA->isChecked();
            bool DGCC = ui->DGCC->isChecked();
            bool DADTM = ui->DADTM->isChecked();
            bool DCDTM = ui->DCDTM->isChecked();
            K = ui->Dk->value();
            Min = ui->Dminimun->value();
            Max = 0;//ui->Maximun->value();
            Inc = 0;//ui->Increment->value();
            Info_Diversa(DPT,DAP,DCP,DHCC,DCCC,DCC,DCCA,DGCC,DADTM,DCDTM,estatico,incremento);
        }else if(incremento == true){ // se selecciono analisis dinamico
            bool DPT = ui->DPT->isChecked();
            bool DAP = ui->DAP->isChecked();
            bool DCP = ui->DCP->isChecked();
            bool DHCC = ui->DHCC->isChecked();
            bool DCCC = ui->DCCC->isChecked();
            bool DCC = ui->DCC->isChecked();
            bool DCCA = ui->DCCA->isChecked();
            bool DGCC = ui->DGCC->isChecked();
            bool DADTM = ui->DADTM->isChecked();
            bool DCDTM = ui->DCDTM->isChecked();
            K = ui->Dk->value();
            Min = ui->Dminimun->value();
            Max = ui->Dmaximun->value();
            Inc = ui->Dincrement->value();
            Info_Diversa(DPT,DAP,DCP,DHCC,DCCC,DCC,DCCA,DGCC,DADTM,DCDTM,estatico,incremento);
        }
    }else if(TabAnali == 2){// se seleccionaron ambas metodologias
        estatico = ui->BStaticradioButton->isChecked();
        incremento = ui->BIncrementradioButton->isChecked();
        if(estatico == true){ // se selecciono analisis estatico
            bool BPT = ui->BPT->isChecked();
            bool BAP = ui->BAP->isChecked();
            bool BCP = ui->BCP->isChecked();
            bool BHCC = ui->BHCC->isChecked();
            bool BCCC = ui->BCCC->isChecked();
            bool BCC = ui->BCC->isChecked();
            bool BCCA = ui->BCCA->isChecked();
            bool BGCC = ui->BGCC->isChecked();
            bool BADTM = ui->BADTM->isChecked();
            bool BCDTM = ui->BCDTM->isChecked();
            K = ui->Bk->value();
            Min = ui->Bminimun->value();
            Max = ui->Bmaximun->value();
            Inc = ui->Bincrement->value();
            Info_Both(BPT,BAP,BCP,BHCC,BCCC,BCC,BCCA,BGCC,BADTM,BCDTM,estatico,incremento);
        }else if(incremento == true){ // se selecciono analisis dinamico
            bool BPT = ui->BPT->isChecked();
            bool BAP = ui->BAP->isChecked();
            bool BCP = ui->BCP->isChecked();
            bool BHCC = ui->BHCC->isChecked();
            bool BCCC = ui->BCCC->isChecked();
            bool BCC = ui->BCC->isChecked();
            bool BCCA = ui->BCCA->isChecked();
            bool BGCC = ui->BGCC->isChecked();
            bool BADTM = ui->BADTM->isChecked();
            bool BCDTM = ui->BCDTM->isChecked();
            K = ui->Bk->value();
            Min = ui->Bminimun->value();
            Max = ui->Bmaximun->value();
            Inc = ui->Bincrement->value();
            Info_Both(BPT,BAP,BCP,BHCC,BCCC,BCC,BCCA,BGCC,BADTM,BCDTM,estatico,incremento);
        }
    }
}

void AnalisisPersonalizado::Info_Uniforme(bool UPT, bool UAP, bool UCP,
                                          bool UHCC, bool UCCC, bool UCC,
                                          bool UCCA, bool UGCC,bool UADTM,
                                          bool UCDTM, bool estatico, bool incremento)
{
    QFile F(INFO_A_P_UNIFORME);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out25(&F);
    out25.setVersion(QDataStream::Qt_5_4);
    QVector<double> Checkboxes;
    QFile Fwork(WORKSPACE_FILENAME);
    if (!Fwork.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&Fwork);
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
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> WCP(Matriz.size()),H(Matriz.size());
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
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    Fwork.flush();
    Fwork.close();
    QVector<double> Enfriamento,Calentamiento,Enf,Cal;
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    int CTo,CCo;
    if(estatico == true){
        Min = ui->Minimun->value();
        Max = 0;
        Inc = 0;
        Checkboxes.resize(8);
        if(UPT == true){
            Checkboxes[0] = 1;
        }else if(UPT == false){
            Checkboxes[0] = 0;
        }
        if(UAP == true){
            if(metodoservicios_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->services->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->services->item(0,j)->text().toDouble();
                Enf[j] = ui->services->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Uunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->UTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->UTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(UAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(UCP == true){
            if(metodocapital_uniformes() == false && metodooperacional_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->UtableCapital->columnCount();
            int rcsize = ui->UtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Ucapital_units->currentIndex();
            int opsize = ui->UtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->UtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(UCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
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
        }else if(UCCC == false){
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
    }else if(incremento == true){
        Min = ui->Minimun->value();
        Max = ui->Maximun->value();
        Inc = ui->Increment->value();
        Checkboxes.resize(10);
        if(UPT == true){
            Checkboxes[0] = 1;
        }else if(UPT == false){
            Checkboxes[0] = 0;
        }
        if(UAP == true){
            if(metodoservicios_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->services->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->services->item(0,j)->text().toDouble();
                Enf[j] = ui->services->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Uunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->UTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->UTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(UAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(UCP == true){
            if(metodocapital_uniformes() == false && metodooperacional_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->UtableCapital->columnCount();
            int rcsize = ui->UtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Ucapital_units->currentIndex();
            int opsize = ui->UtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->UtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(UCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
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
        }else if(UCCC == false){
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
        if(UADTM == true){
            if(metodoservicios_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[8] = 1;
            int ncols;
            ncols = ui->services->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->services->item(0,j)->text().toDouble();
                Enf[j] = ui->services->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Uunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->UTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->UTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(UADTM ==  false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            CTo = 0;
            CCo = 0;
            Checkboxes[8] = 0;
        }
        if(UCDTM == true){
            if(metodocapital_uniformes() == false && metodooperacional_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[9] = 1;
            int ccsize = ui->UtableCapital->columnCount();
            int rcsize = ui->UtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Ucapital_units->currentIndex();
            int opsize = ui->UtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->UtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(UCDTM ==  false){
            Checkboxes[9] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
    }
    infoapuniforme info(Checkboxes,estatico,incremento,SI,SIS,Min,Max,Inc,TS,TE,WCP,H,Enfriamento,
                        Calentamiento,CTo,CCo,CapitalCost,OperationCost);
    out25 << info;
    F.flush();
    F.close();
}

void AnalisisPersonalizado::Info_Diversa(bool DPT, bool DAP, bool DCP,
                                         bool DHCC, bool DCCC, bool DCC,
                                         bool DCCA, bool DGCC, bool DADTM,
                                         bool DCDTM, bool estatico, bool incremento)
{
    QFile F(INFO_A_P_DIVERSO);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out27(&F);
    out27.setVersion(QDataStream::Qt_5_4);
    QVector<double> Checkboxes;
    QFile Fwork(WORKSPACE_FILENAME);
    if (!Fwork.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&Fwork);
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
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> WCP(Matriz.size()),H(Matriz.size());
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
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    Fwork.flush();
    Fwork.close();
    QVector<double> Enfriamento,Calentamiento,Enf,Cal;
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    int CTo,CCo;
    if(estatico == true){
        K = ui->Dk->value();
        Min = ui->Dminimun->value();
        Max = 0;
        Inc = 0;
        Checkboxes.resize(8);
        if(DPT == true){
            Checkboxes[0] = 1;
        }else if(DPT == false){
            Checkboxes[0] = 0;
        }
        if(DAP == true){
            if(metodoservicios_diverso() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->Dservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Dservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Dservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Uunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->UTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->UTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(DAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(DCP == true){
            if(metodocapital_uniformes() == false && metodooperacional_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->UtableCapital->columnCount();
            int rcsize = ui->UtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->UtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Ucapital_units->currentIndex();
            int opsize = ui->UtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->UtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(DCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
        if(DHCC == true){
            Checkboxes[3] = 1;
        }else if(DHCC == false){
            Checkboxes[3] = 0;
        }
        if(DCC == true){
            Checkboxes[4] = 1;
        }else if(DCC == false){
            Checkboxes[4] = 0;
        }
        if(DCCC == true){
            Checkboxes[5] = 1;
        }else if(DCCC == false){
            Checkboxes[5] = 0;
        }
        if(DCCA == true){
            Checkboxes[6] = 1;
        }else if(DCCA == false){
            Checkboxes[6] = 0;
        }
        if(DGCC == true){
            Checkboxes[7] = 1;
        }else if(DGCC ==  false){
            Checkboxes[7] = 0;
        }
    }else if(incremento == true){
        K = ui->Dk->value();
        Min = ui->Dminimun->value();
        Max = ui->Dmaximun->value();
        Inc = ui->Dincrement->value();
        Checkboxes.resize(10);
        if(DPT == true){
            Checkboxes[0] = 1;
        }else if(DPT == false){
            Checkboxes[0] = 0;
        }
        if(DAP == true){
            if(metodoservicios_diverso() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->Dservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Dservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Dservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Dunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->DTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->DTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(DAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(DCP == true){
            if(metodocapital_diverso() == false && metodooperacional_diverso() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->DtableCapital->columnCount();
            int rcsize = ui->DtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->DtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->DtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Dcapital_units->currentIndex();
            int opsize = ui->DtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->DtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(DCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
        if(DHCC == true){
            Checkboxes[3] = 1;
        }else if(DHCC == false){
            Checkboxes[3] = 0;
        }
        if(DCC == true){
            Checkboxes[4] = 1;
        }else if(DCC == false){
            Checkboxes[4] = 0;
        }
        if(DCCC == true){
            Checkboxes[5] = 1;
        }else if(DCCC == false){
            Checkboxes[5] = 0;
        }
        if(DCCA == true){
            Checkboxes[6] = 1;
        }else if(DCCA == false){
            Checkboxes[6] = 0;
        }
        if(DGCC == true){
            Checkboxes[7] = 1;
        }else if(DGCC ==  false){
            Checkboxes[7] = 0;
        }
        if(DADTM == true){
            if(metodoservicios_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[8] = 1;
            int ncols;
            ncols = ui->Dservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Dservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Dservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Dunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->DTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->DTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(DADTM ==  false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            CTo = 0;
            CCo = 0;
            Checkboxes[8] = 0;
        }
        if(DCDTM == true){
            if(metodocapital_uniformes() == false && metodooperacional_uniformes() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[9] = 1;
            int ccsize = ui->DtableCapital->columnCount();
            int rcsize = ui->DtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->DtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->DtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Dcapital_units->currentIndex();
            int opsize = ui->DtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->DtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(DCDTM ==  false){
            Checkboxes[9] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
    }
    //CREAR NUEVO CONSTRUCTOR
    infoapdiversa info(Checkboxes,estatico,incremento,SI,SIS,K,Min,Max,Inc,TS,TE,WCP,H,Enfriamento,
                       Calentamiento,CTo,CCo,CapitalCost,OperationCost);
    out27 << info;
    F.flush();
    F.close();
}

void AnalisisPersonalizado::Info_Both(bool BPT, bool BAP, bool BCP,
                                      bool BHCC, bool BCCC, bool BCC,
                                      bool BCCA, bool BGCC, bool BADTM,
                                      bool BCDTM, bool estatico, bool incremento)
{
    QFile F(INFO_A_P_BOTH);
    if (!F.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream out28(&F);
    out28.setVersion(QDataStream::Qt_5_4);
    QVector<double> Checkboxes;
    QFile Fwork(WORKSPACE_FILENAME);
    if (!Fwork.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,tr("Error"),tr("Error"));
        return;
    }
    QDataStream in2(&Fwork);
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
    QVector<double> TS(Matriz.size()),TE(Matriz.size());
    QVector<double> WCP(Matriz.size()),H(Matriz.size());
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
    bool SI = units.getSI();
    bool SIS = units.getSIS();
    units.ConvertirUnidades(TS,TE,WCP,SI,SIS,UTemp,UWcp);
    TS = units.getST();
    TE = units.getTT();
    WCP = units.getCp();
    FileUnidades.flush();
    FileUnidades.close();
    Fwork.flush();
    Fwork.close();
    QVector<double> Enfriamento,Calentamiento,Enf,Cal;
    QVector<QVector<double>> CapitalCost;
    QVector<double> OperationCost;
    int CTo,CCo;
    if(estatico == true){
        K = ui->Bk->value();
        Min = ui->Bminimun->value();
        Max = ui->Bmaximun->value();
        Inc = ui->Bincrement->value();
        Checkboxes.resize(8);
        if(BPT == true){
            Checkboxes[0] = 1;
        }else if(BPT == false){
            Checkboxes[0] = 0;
        }
        if(BAP == true){
            if(metodoservicios_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->Bservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Bservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Bservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Bunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->BTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->BTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(BAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(BCP == true){
            if(metodocapital_both() == false && metodooperacional_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->BtableCapital->columnCount();
            int rcsize = ui->BtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Bcapital_units->currentIndex();
            int opsize = ui->BtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->BtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(BCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
        if(BHCC == true){
            Checkboxes[3] = 1;
        }else if(BHCC == false){
            Checkboxes[3] = 0;
        }
        if(BCC == true){
            Checkboxes[4] = 1;
        }else if(BCC == false){
            Checkboxes[4] = 0;
        }
        if(BCCC == true){
            Checkboxes[5] = 1;
        }else if(BCCC == false){
            Checkboxes[5] = 0;
        }
        if(BCCA == true){
            Checkboxes[6] = 1;
        }else if(BCCA == false){
            Checkboxes[6] = 0;
        }
        if(BGCC == true){
            Checkboxes[7] = 1;
        }else if(BGCC ==  false){
            Checkboxes[7] = 0;
        }
    }else if(incremento == true){
        K = ui->Bk->value();
        Min = ui->Bminimun->value();
        Max = ui->Bmaximun->value();
        Inc = ui->Bincrement->value();
        Checkboxes.resize(10);
        if(BPT == true){
            Checkboxes[0] = 1;
        }else if(BPT == false){
            Checkboxes[0] = 0;
        }
        if(BAP == true){
            if(metodoservicios_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[1] = 1;
            int ncols;
            ncols = ui->Bservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Bservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Bservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Bunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->BTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->BTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(BAP == false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            Checkboxes[1] = 0;
            CTo = 0;
            CCo = 0;
        }
        if(BCP == true){
            if(metodocapital_both() == false && metodooperacional_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[2] = 1;
            int ccsize = ui->BtableCapital->columnCount();
            int rcsize = ui->BtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Bcapital_units->currentIndex();
            int opsize = ui->BtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->BtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(BCP == false){
            Checkboxes[2] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
        if(BHCC == true){
            Checkboxes[3] = 1;
        }else if(BHCC == false){
            Checkboxes[3] = 0;
        }
        if(BCC == true){
            Checkboxes[4] = 1;
        }else if(BCC == false){
            Checkboxes[4] = 0;
        }
        if(BCCC == true){
            Checkboxes[5] = 1;
        }else if(BCCC == false){
            Checkboxes[5] = 0;
        }
        if(BCCA == true){
            Checkboxes[6] = 1;
        }else if(BCCA == false){
            Checkboxes[6] = 0;
        }
        if(BGCC == true){
            Checkboxes[7] = 1;
        }else if(BGCC ==  false){
            Checkboxes[7] = 0;
        }
        if(BADTM == true){
            if(metodoservicios_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[8] = 1;
            int ncols;
            ncols = ui->Bservices->columnCount();
            Enfriamento.resize(ncols);
            Calentamiento.resize(ncols);
            Enf.resize(ncols);
            Cal.resize(ncols);
            for (int j = 0; j < ncols ; j++){
                Cal[j] = ui->Bservices->item(0,j)->text().toDouble();
                Enf[j] = ui->Bservices->item(1,j)->text().toDouble();
            }
            UnidadesSerAux units2(SI,SIS,0);
            int A = ui->Bunits_temp->currentIndex(); //corregido
            units2.ConvertirUnidades(Cal,Enf,SI,SIS,A);
            Enfriamento = units2.getEnfriamento();
            Calentamiento = units2.getCalentamiento();
            if (ui->BTodcomboBox->currentIndex() == 1){
                CTo = 1;
                CCo = 0;
            }else if(ui->BTodcomboBox->currentIndex() == 2){
                CTo = 0;
                CCo = 1;
            }
        }else if(BADTM ==  false){
            Enfriamento.resize(2);
            Calentamiento.resize(2);
            CTo = 0;
            CCo = 0;
            Checkboxes[8] = 0;
        }
        if(BCDTM == true){
            if(metodocapital_both() == false && metodooperacional_both() == false){
                QMessageBox::warning(this,tr("Error"),tr("Missing information"));
                return;
            }
            Checkboxes[9] = 1;
            int ccsize = ui->BtableCapital->columnCount();
            int rcsize = ui->BtableCapital->rowCount();
            CapitalCost.resize(rcsize);
            if(ccsize == 5){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }else if (ccsize == 4){
                for(int i = 0; i < rcsize ;i++){
                    CapitalCost[i].resize(ccsize);
                }
                for(int j = 0; j < rcsize ; j++){
                    for(int i = 0; i < ccsize;i++){
                        CapitalCost[j][i] =  ui->BtableCapital->item(j,i)->text().toDouble();
                    }
                }
            }
            int cbUOP = ui->Bcapital_units->currentIndex();
            int opsize = ui->BtableOperation->rowCount();
            OperationCost.resize(opsize);
            for(int i = 0; i < opsize ; i++){
                OperationCost[i] = ui->BtableOperation->item(i,0)->text().toDouble();
            }
            OperationCost = ConvertirOperationCost(OperationCost,SI,SIS,cbUOP);
        }else if(BCDTM ==  false){
            Checkboxes[9] = 0;
            CapitalCost.resize(1);
            for(int i = 0; i < 1 ;i++){
                CapitalCost[i].resize(2);
            }
            OperationCost.resize(2);
        }
    }
    //CREAR NUEVO CONSTRUCTOR
    infoapboth info(Checkboxes,estatico,incremento,SI,SIS,K,Min,Max,Inc,TS,TE,WCP,H,Enfriamento,
                       Calentamiento,CTo,CCo,CapitalCost,OperationCost);
    out28 << info;
    F.flush();
    F.close();
}

void AnalisisPersonalizado::on_UFCapcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index == 1){ // Ley de intercambiadores
        ui->UtableCapital->setEnabled(true);
        ui->UAVISO->setVisible(true);
        int ColumAct = 5;
        ui->UtableCapital->clear();
        ui->UtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "a" << "b" <<"c"<<"i"<<"n";
        ui->UtableCapital->setHorizontalHeaderLabels(titulos);
        ui->UtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->UtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->UtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->UtableCapital->columnCount(); ++c )
            {
                ui->UtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }else if(index == 2){
        ui->UtableCapital->setEnabled(true);
        ui->UAVISO->setVisible(true);
        int ColumAct = 4;
        ui->UtableCapital->clear();
        ui->UtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "b" <<"c"<<"i"<<"n";
        ui->UtableCapital->setHorizontalHeaderLabels(titulos);
        ui->UtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->UtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->UtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->UtableCapital->columnCount(); ++c )
            {
                ui->UtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_Ucapital_units_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->UtableOperation->setEnabled(true);
        ui->UtableOperation->setColumnCount(1);
        ui->UtableOperation->setRowCount(2);
        titulos.clear();
        titulos << "Value";
        ui->UtableOperation->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->UtableOperation->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->UtableOperation->rowCount(); i++ )
        {
            for( int c = 0; c < ui->UtableOperation->columnCount(); ++c )
            {
                ui->UtableOperation->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

QVector<double> AnalisisPersonalizado::ConvertirOperationCost(QVector<double> OperationCost, bool SI, bool SIS, int A)
{
    // "US $ / KW" << "US $ / W"; "US $ / Btu" << "US $ / CHU";
    if(SI ==true ){
        if(A == 1){ //US $ / Btu
            OperationCost[0]= OperationCost[0];
            OperationCost[1]= OperationCost[1];
        }else if(A == 1){ //US $ / CHU  ---> US $ / BTU
            OperationCost[0]= OperationCost[0]/1.80;
            OperationCost[1]= OperationCost[1]/1.80;
        }
    }else if(SIS == true){
        if(A == 1){ //US $ / KW
            OperationCost[0]= OperationCost[0];
            OperationCost[1]= OperationCost[1];
        }else if(A == 1){ //US $ / W  ---> US $ / KW
            OperationCost[0]= OperationCost[0]*1000;
            OperationCost[1]= OperationCost[1]*1000;
        }
    }
    return OperationCost;
}

void AnalisisPersonalizado::on_UTodcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->services->setEnabled(true);
        ui->services->setColumnCount(3);
        ui->services->setRowCount(2);
        titulos.clear();
        titulos << "Tsupply" << "Ttarget" << "h";
        ui->services->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->services->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->services->rowCount(); i++ )
        {
            for( int c = 0; c < ui->services->columnCount(); ++c )
            {
                ui->services->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

bool AnalisisPersonalizado::metodoservicios_uniformes()
{
    int row = ui->services->rowCount();
    int column = ui->services->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->services->item(i,j)){
               return false;
            }else if (ui->services->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->services->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodocapital_uniformes()
{
    int row = ui->UtableCapital->rowCount();
    int column = ui->UtableCapital->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->UtableCapital->item(i,j)){
               return false;
            }else if (ui->UtableCapital->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->UtableCapital->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodooperacional_uniformes()
{
    int row = ui->UtableOperation->rowCount();
    int column = ui->UtableOperation->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->UtableOperation->item(i,j)){
               return false;
            }else if (ui->UtableOperation->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->UtableOperation->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodoservicios_diverso()
{
    int row = ui->Dservices->rowCount();
    int column = ui->Dservices->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->Dservices->item(i,j)){
               return false;
            }else if (ui->Dservices->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->Dservices->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodocapital_diverso()
{
    int row = ui->DtableCapital->rowCount();
    int column = ui->DtableCapital->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->DtableCapital->item(i,j)){
               return false;
            }else if (ui->DtableCapital->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->DtableCapital->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodooperacional_diverso()
{
    int row = ui->DtableOperation->rowCount();
    int column = ui->DtableOperation->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->DtableOperation->item(i,j)){
               return false;
            }else if (ui->DtableOperation->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->DtableOperation->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodoservicios_both()
{
    int row = ui->Bservices->rowCount();
    int column = ui->Bservices->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->Bservices->item(i,j)){
               return false;
            }else if (ui->Bservices->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->Bservices->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodocapital_both()
{
    int row = ui->BtableCapital->rowCount();
    int column = ui->BtableCapital->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->BtableCapital->item(i,j)){
               return false;
            }else if (ui->BtableCapital->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->BtableCapital->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

bool AnalisisPersonalizado::metodooperacional_both()
{
    int row = ui->BtableOperation->rowCount();
    int column = ui->BtableOperation->columnCount();
    for(int i = 0; i < row ; i++){
        for(int j = 0; j < column ; j++){
            if (!ui->BtableOperation->item(i,j)){
               return false;
            }else if (ui->BtableOperation->item(i,j)->text() == "Empty"){
               return false;
            }else if (ui->BtableOperation->item(i,j)->text() == "0"){
               return false;
            }
        }
    }
    return true;
}

void AnalisisPersonalizado::on_DTodcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->Dservices->setEnabled(true);
        ui->Dservices->setColumnCount(3);
        ui->Dservices->setRowCount(2);
        titulos.clear();
        titulos << "Tsupply" << "Ttarget" << "h";
        ui->Dservices->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->Dservices->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->Dservices->rowCount(); i++ )
        {
            for( int c = 0; c < ui->Dservices->columnCount(); ++c )
            {
                ui->Dservices->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_DFCapcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index == 1){ // Ley de intercambiadores
        ui->DtableCapital->setEnabled(true);
        ui->DAVISO->setVisible(true);
        int ColumAct = 5;
        ui->DtableCapital->clear();
        ui->DtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "a" << "b" <<"c"<<"i"<<"n";
        ui->DtableCapital->setHorizontalHeaderLabels(titulos);
        ui->DtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->DtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->DtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->DtableCapital->columnCount(); ++c )
            {
                ui->DtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }else if(index == 2){
        ui->DtableCapital->setEnabled(true);
        ui->DAVISO->setVisible(true);
        int ColumAct = 4;
        ui->DtableCapital->clear();
        ui->DtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "b" <<"c"<<"i"<<"n";
        ui->DtableCapital->setHorizontalHeaderLabels(titulos);
        ui->DtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->DtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->DtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->DtableCapital->columnCount(); ++c )
            {
                ui->DtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_Dcapital_units_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->DtableOperation->setEnabled(true);
        ui->DtableOperation->setColumnCount(1);
        ui->DtableOperation->setRowCount(2);
        titulos.clear();
        titulos << "Value";
        ui->DtableOperation->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->DtableOperation->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->DtableOperation->rowCount(); i++ )
        {
            for( int c = 0; c < ui->DtableOperation->columnCount(); ++c )
            {
                ui->DtableOperation->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_BTodcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->Bservices->setEnabled(true);
        ui->Bservices->setColumnCount(3);
        ui->Bservices->setRowCount(2);
        titulos.clear();
        titulos << "Tsupply" << "Ttarget" << "h";
        ui->Bservices->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->Bservices->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->Bservices->rowCount(); i++ )
        {
            for( int c = 0; c < ui->Bservices->columnCount(); ++c )
            {
                ui->Bservices->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_BFCapcomboBox_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index == 1){ // Ley de intercambiadores
        ui->BtableCapital->setEnabled(true);
        ui->BAVISO->setVisible(true);
        int ColumAct = 5;
        ui->BtableCapital->clear();
        ui->BtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "a" << "b" <<"c"<<"i"<<"n";
        ui->BtableCapital->setHorizontalHeaderLabels(titulos);
        ui->BtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->BtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->BtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->BtableCapital->columnCount(); ++c )
            {
                ui->BtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }else if(index == 2){
        ui->BtableCapital->setEnabled(true);
        ui->BAVISO->setVisible(true);
        int ColumAct = 4;
        ui->BtableCapital->clear();
        ui->BtableCapital->setColumnCount(ColumAct);
        titulos.clear();
        titulos << "b" <<"c"<<"i"<<"n";
        ui->BtableCapital->setHorizontalHeaderLabels(titulos);
        ui->BtableCapital->setRowCount(3);
        titulos.clear();
        titulos << "Cooling Equipment" <<  "Recovery Equipment" << "Heating Equipment" ;
        ui->BtableCapital->setVerticalHeaderLabels(titulos);
        for( int r = 0 ; r < ui->BtableCapital->rowCount(); ++r )
        {
            for( int c = 0; c < ui->BtableCapital->columnCount(); ++c )
            {
                ui->BtableCapital->setItem(r, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_Bcapital_units_currentIndexChanged(int index)
{
    if(index == 0){
        return;
    }else if(index >= 1){
        ui->BtableOperation->setEnabled(true);
        ui->BtableOperation->setColumnCount(1);
        ui->BtableOperation->setRowCount(2);
        titulos.clear();
        titulos << "Value";
        ui->BtableOperation->setHorizontalHeaderLabels(titulos);
        titulos.clear();
        titulos << "Heating service" << "Cooling service";
        ui->BtableOperation->setVerticalHeaderLabels(titulos);
        for(int i = 0 ; i < ui->BtableOperation->rowCount(); i++ )
        {
            for( int c = 0; c < ui->BtableOperation->columnCount(); ++c )
            {
                ui->BtableOperation->setItem(i, c, new QTableWidgetItem("Empty"));
            }
        }
    }
}

void AnalisisPersonalizado::on_BAP_clicked()
{
    Bcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_BCP_clicked()
{
    Bcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_BADTM_clicked()
{
    Bcheckboxes_seleccionadas();
}

void AnalisisPersonalizado::on_BCDTM_clicked()
{
    Bcheckboxes_seleccionadas();
}
