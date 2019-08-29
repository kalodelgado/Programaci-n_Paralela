#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "GoldbachModel.h"
#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->progressBar = new QProgressBar();
    this->ui->statusBar->addPermanentWidget(progressBar);
    this->ui->statusBar->showMessage("Ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include <iostream>
void MainWindow::on_lineEditNumber_textEdited(const QString &arg1)
{
    //std::cout << qPrintable(arg1) << std::endl;
    bool enable = arg1.trimmed().length() > 0;
    this->ui->pushButtonCalculate->setEnabled(enable);
}

void MainWindow::on_pushButtonCalculate_clicked()
{
    bool isValid = true;
    const QString& text = this->ui->lineEditNumber->text();
    long long int number = text.toLongLong(&isValid);

    if (number < 4 || number == 5)
        isValid = false;

    if (isValid){
        //fixed the memory leak
        if(this->model)
            this->model->deleteLater();//Cuando se calcula el mainwindow no se desruye; si existen modelos anteriores los elimina
        this->model = new GoldbachModel(this);//Para destruir del objeto creado cuando el mainwindow se destruya auomaticamente

        this->ui->listViewResults->setModel(model);
        this->progressBar->reset();

        this->ui->pushButtonCalculate->setEnabled(false);
        this->ui->pushButtonStop->setEnabled(true);
        this->userStopped = false;

        ui->statusBar->showMessage(QString(tr("Calculating...")));
        
        this->time.start();

        this->connect(this->model,&GoldbachModel::calculationDone, this, &MainWindow::calculationDone);
        this->connect(this->model,&GoldbachModel::progressUpdated, this, &MainWindow::UpdateProgressBar);

        //this->worker->start(); ahora en GModel
        this->model->calculate(number);

        //long long sumsCount = calculate(number); ahora en GWorker        
        //ui->statusBar->showMessage(QString(tr("%1 sums found in %2 seconds.").arg(sumsCount).arg(seconds)));
    }
    else {
        ui->statusBar->showMessage(QString(tr("Invalid number: %1")).arg(text));
    }
}

void MainWindow::calculationDone(long long result)
{
    double seconds = this->time.elapsed() / 1000.0;
    ui->statusBar->showMessage(QString(tr("%1 sums found in %2 seconds.").arg(result).arg(seconds)));


    this->ui->pushButtonCalculate->setEnabled(true);
    this->ui->pushButtonStop->setEnabled(false);
}

void MainWindow::UpdateProgressBar(int num)
{
    this->progressBar->setValue(num);
}

void MainWindow::on_pushButtonStop_clicked()
{
    Q_ASSERT(this->model); //Checkeo en tiempo de ejecucion
    this->model->stop();
}
