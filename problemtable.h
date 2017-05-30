#ifndef PROBLEMTABLE_H
#define PROBLEMTABLE_H
#include <QFileSystemModel>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QWidget>
#include "duvaloraction.h"
#include "valoraction.h"

namespace Ui {
class problemtable;
}

class problemtable : public QWidget
{
    Q_OBJECT

public:
    explicit problemtable(QWidget *parent = 0);
    ~problemtable();

private slots:
    void on_uniform_clicked();
    void on_diverse_clicked();
    void on_pushButton_clicked();
    void radiobuttons();
private:
    Ui::problemtable *ui;
};

#endif // PROBLEMTABLE_H
