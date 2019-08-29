#include "GoldbachWorker.h"
#include <QtMath>

GoldbachWorker::GoldbachWorker(long long number, int workerNumber, int workerCount, QVector<QString>& results, QObject *parent)
    : QThread{parent}
    , number{number}//inicializacion
    , workerId{workerNumber}
    , workerCount{workerCount}
    , results{results}
{

}

void GoldbachWorker::run()
{
    emit calculationDone(this->calculate(this->number));
}

void GoldbachWorker::updateProgress(int newPercent)
{
    if(progressPercent != newPercent){
        this->progressPercent = newPercent;
        emit progressUpdated(this->workerId, newPercent);
    }
}

long long GoldbachWorker::calculateStart(long long number, int workerCount, int workerId)
{
    return workerId * number / workerCount + qMin(static_cast<long long>(workerId), number % workerCount);
}

long long GoldbachWorker::calculateFinish(long long number, int workerCount, int workerId)
{
    return  calculateStart(number, workerCount, workerId +1);
}

long long GoldbachWorker::calculate(long long number)
{
    if ( number < 4 || number == 5 ) return 0;
    this->progressPercent = 0;
    return number % 2 == 0 ? calculateEvenGoldbach(number) : calculateOddGoldbach(number);
}

long long GoldbachWorker::calculateEvenGoldbach(long long number)
{
    long long resultCount = 0;
    long long start = 2 + calculateStart(number / 2, this->workerCount, this->workerId);
    long long finish = 2 + calculateFinish(number / 2, this->workerCount, this->workerId);

    for ( long long a = start; a < finish && this->isInterruptionRequested() == false; ++a )
    {

        updateProgress(static_cast<int>(100LL * (a - start + 1) / (finish - start)));

        if ( ! isPrime(a) ) continue;
        long long b = number - a;
        if ( b >= a && isPrime(b) ){
            this->results.append( tr("%1 + %2").arg(a).arg(b));//Funciona como un pushback
            ++resultCount;
        }
        //QApplication::processEvents();
    }
    return resultCount;
}

long long GoldbachWorker::calculateOddGoldbach(long long number)
{    
    long long resultCount = 0;
    long long start = 2 + calculateStart(number / 3 + 1, this->workerCount, this->workerId);
    long long finish = 2 + calculateFinish(number / 3 + 1, this->workerCount, this->workerId);

    for ( long long a = start; a < finish; ++a )
    {
        //this->progressBar->setValue(static_cast<int>(100LL * a / number));
        updateProgress(static_cast<int>(100LL * (a - start + 1) / (finish - start)));

        if ( ! isPrime(a) ) continue;
        for ( long long b = a; b < number; ++b )
        {
            if(this->isInterruptionRequested())
                return resultCount;

            if ( ! isPrime(b) ) continue;
            long long c = number - a - b;
            if ( c >= b && isPrime(c) ){
                this->results.append( tr("%1 + %2 + %3").arg(a).arg(b).arg(c) );//Funciona como un pushback
                ++resultCount;
            }
        }
    }
    return resultCount;
}

bool GoldbachWorker::isPrime(long long number)
{
    if ( number < 2 ) return false;
    if (number == 2) return false;
    if (number % 2 == 0) return false;

    long long last = static_cast<long long>(qSqrt(number));
    for ( long long i = 3; i <= last; ++i )
        if ( number % i == 0 )
            return false;

    return true;
}
