

#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

//Dialog::setMainWindow(MainWindow m)
//{
//    myMainWindow = m;
//}

void Dialog::updateGraph()
{
    //MainWindow* w = qobject_cast<MainWindow*>(parent);

    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->graphPlot->addGraph();
    ui->graphPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->graphPlot->xAxis->setLabel("x");
    ui->graphPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->graphPlot->xAxis->setRange(-1, 1);
    ui->graphPlot->yAxis->setRange(0, 1);
}
