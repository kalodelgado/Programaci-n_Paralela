#ifndef GOLDBACHWORKER_H
#define GOLDBACHWORKER_H

#include <QThread>
#include <QVector>

class GoldbachWorker : public QThread
{
    Q_OBJECT
    //GoldbachWorker(const GoldbachWorker& other) = delete; deshabiita el constructor de copia; esto xq no es necesario
    Q_DISABLE_COPY(GoldbachWorker)

private:
    long long number = 0;
    int workerId = -1;
    int workerCount = 0;
    QVector<QString>& results;
    int progressPercent;

signals:
    void sumFound(const QString& sum) const;
    void calculationDone(long long sumCount) const;
    void progressUpdated(int workerId, int percent) const;

public:
    explicit GoldbachWorker(long long number, int workerNumber, int workerCount, QVector<QString>& results, QObject* parent = nullptr); //Constructor
    void run() override;//Sobreescribe el metodo run de la clase base; El que este en cursiva significaque es virtual

private:
    void updateProgress(int percent);

    /**
     * @brief Calcula el valor donde iniciará un worker
     * @param number El numero dado por el usuario
     * @return Valor de inicio
     */
    long long calculateStart(long long number, int workerCount, int workerId);

    /**
     * @brief Calcula el valor final hasta el que calculará un worker
     * @param number El numero dado por el usuario
     * @return Valor donde finalizará
     */
    long long calculateFinish(long long number, int workerCount, int workerId);

private:
    /**
     * @brief Calcula las sumas de Goldbach para el numero dado y las agrega a una pizarra
     * @param number El numero dado por el usuario
     * @return La cantidad de sumas encontradas
     */
    long long calculate(long long number);
    /**
     * Calcula todas las sumas de dos primos que equivalen al numero dado, y las presenta
     * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
     * de la conjetura fuerte de Goldbach, por ejemplo:
     *
     *   4 == 2 + 2
     *   6 == 3 + 3
     *  10 == 3 + 7 == 5 + 5
     *
     * @param numero Un numero entero par mayor o igual a 4
     * @return la cantidad de sumas de dos primos encontradas
     */
    long long calculateEvenGoldbach(long long number);
    /**
     * Calcula todas las sumas de tres primos que equivalen al numero dado, y las presenta
     * en el area resultado. Incrementa la barra de progreso mientras realiza el calculo
     * de la conjetura debil de Goldbach, por ejemplo:
     *
     *   7 == 2 + 2 + 3
     *   9 == 2 + 2 + 5 == 3 + 3 + 3
     *
     * @param numero Un numero entero impar mayor o igual a 7
     * @return la cantidad de sumas de tres primos encontradas
     */
    long long calculateOddGoldbach(long long number);
    /**
     * Retorna true si numero es primo, false si numero no es primo o menor que 2
     * Por definicion 1 no es primo ni compuesto, este metodo retorna false
     */
    static bool isPrime(long long numero);
};

#endif // GOLDBACHWORKER_H
