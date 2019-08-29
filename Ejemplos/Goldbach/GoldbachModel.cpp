#include "GoldbachModel.h"
#include "GoldbachWorker.h"

#include <algorithm>

GoldbachModel::GoldbachModel(QObject *parent) : QAbstractListModel(parent){}

int GoldbachModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->fetchedowCount;
}

QVariant GoldbachModel::data(const QModelIndex &index, int role) const //QVariant una clase que recibe distintos tipos de datos
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->countResults() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
        return tr("%1: %2").arg( index.row() + 1).arg( this->findValue( index.row() ) );

    return QVariant();
}

const QString &GoldbachModel::findValue(int indexCol) const
{
    int rowIndex = 0;

    while (indexCol >= this->results[rowIndex].count())
    {
        indexCol = indexCol - this->results[rowIndex++].count();
    }

    return this->results[rowIndex][indexCol];
}

int GoldbachModel::countResults() const
{
    int resultCount = 0;

    for (auto vector : this->results)
        resultCount += vector.count();

    return resultCount;
}

QVector<QString> GoldbachModel::fetchAllSums() const
{
    QVector<QString> allSums;

    allSums.reserve( this->countResults() );

    for(const QVector<QString>& resultsOfWorker : this->results)
        for(const QString& sum : resultsOfWorker)
            allSums.append( tr( "%1: %2" ).arg( allSums.count() + 1 ).arg(sum) );

    return allSums;
}

void GoldbachModel::calculate(long long number)
{    
    this->beginResetModel();

    for ( GoldbachWorker* worker : this->workers)
        worker->deleteLater();
    this->workers.clear();

    int workerCount = qMin(QThread::idealThreadCount(), static_cast<int>(number - 2));
    workerCount = qMax(workerCount, 1);

    this->progressPercents.clear();
    this->progressPercents.resize(workerCount);

    this->results.clear();
    this->results.resize( workerCount );

    this->finishWorkerCount = 0;

    this->results.clear();
    this->results.resize( workerCount );

    this->workers.reserve(workerCount);

    for (int current = 0; current < workerCount; ++current)
    {
        //this->workers.append( new GoldbachWorker{number,this->results, this} ); //Llena el vector
        GoldbachWorker* worker = new GoldbachWorker{number, current, workerCount, this->results[current], this};
        this->workers.append( worker );

        this->connect(worker, &GoldbachWorker::calculationDone, this, &GoldbachModel::workerDone);//cuando un worker calcule, invoqueme a mi y haga lo del metodo workerDone
        this->connect(worker, &GoldbachWorker::progressUpdated, this, &GoldbachModel::updateProgress);

        worker->start();
    }

    this->endResetModel();
}

void GoldbachModel::stop()
{
    Q_ASSERT(this->workers.count() > 0);

    for ( GoldbachWorker* worker : this->workers)
        worker->requestInterruption();
}

void GoldbachModel::workerDone(long long sumCount)
{
    Q_UNUSED(sumCount);
    Q_ASSERT( this->finishWorkerCount < this->workers.count() );

    if(++this->finishWorkerCount == this->workers.count())
        emit this->calculationDone( this->countResults() );
}

bool GoldbachModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return this->fetchedowCount < this->countResults();
}

void GoldbachModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent);

    int remainder = this->countResults() - this->fetchedowCount;
    int itemsToFetch = qMin(100, remainder);

    if (itemsToFetch <= 0)
        return;

    beginInsertRows(QModelIndex(), this->fetchedowCount, this->fetchedowCount+itemsToFetch-1);
    this->fetchedowCount += itemsToFetch;
    endInsertRows();
}

void GoldbachModel::updateProgress(int workerId, int percent)
{
    Q_ASSERT(workerId >= 0 && workerId < this->progressPercents.count());
    this->progressPercents[workerId] = percent;

    int minProgressPercent = * std::min_element( this->progressPercents.constBegin(), this->progressPercents.constEnd() );

    if(this->overallProgressPercent != minProgressPercent)
    {
        this->overallProgressPercent = minProgressPercent;
        emit this->progressUpdated(percent);
    }

    if( this->fetchedowCount <= 0)
        this->fetchMore(QModelIndex());
}


