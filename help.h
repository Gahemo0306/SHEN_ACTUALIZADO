#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>


namespace Ui {
class Help;
}

class Help : public QWidget
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();
    void HyperText(const QModelIndex &index);
    void AddRoots(QString name, QStringList listas);
    void AddChild(QTreeWidgetItem *parent,QString name);
private slots:
//    void on_treeWidget_doubleClicked(const QModelIndex &index);

//    void on_treeWidget_clicked(const QModelIndex &index);

private:
    Ui::Help *ui;
};


#endif // HELP_H
