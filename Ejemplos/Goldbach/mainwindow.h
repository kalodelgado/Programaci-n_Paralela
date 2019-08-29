#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

namespace Ui {
class MainWindow;
}

//forward declaration; cuando declaramos punteros
class QProgressBar;
class GoldbachWorker;
class GoldbachModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    bool userStopped = false;
    QProgressBar* progressBar = nullptr;
    GoldbachModel* model = nullptr;

    Ui::MainWindow *ui = nullptr;
    QTime time;

private slots://cuando alguien hace una accion; me avisan
    void on_lineEditNumber_textEdited(const QString &arg1);

    void on_pushButtonStop_clicked();

    void on_pushButtonCalculate_clicked();

    void calculationDone(long long sumCount);

    void UpdateProgressBar(int percent); //Ahora en model
};

#endif // MAINWINDOW_H
