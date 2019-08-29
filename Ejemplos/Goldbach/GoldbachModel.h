#ifndef GOLDBACHMODEL_H
#define GOLDBACHMODEL_H

#include <QAbstractListModel>
#include <QVector>

class GoldbachWorker;

class GoldbachModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(GoldbachModel)

public:
    explicit GoldbachModel(QObject *parent = nullptr);
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    const QString& findValue(int index) const;
    int countResults() const;
    QVector<QString> fetchAllSums() const;

    void calculate(long long number);
    void stop();

protected:
    QVector<GoldbachWorker*> workers;
    QVector<int> progressPercents;
    //GoldbachWorker* worker = nullptr; //Creados en el ccp con cada iteracion del ciclo
    int overallProgressPercent = 0;
    int finishWorkerCount = 0;
    QVector< QVector<QString> > results;
    int fetchedowCount = 0;

signals:
    void calculationDone(long long sumCount);
    void progressUpdated(int percent) const;

private slots:
    void workerDone(long long sumCount);
    void updateProgress(int workerId, int percent);

protected:
      bool canFetchMore(const QModelIndex &parent) const override;
      void fetchMore(const QModelIndex &parent) override;
};

#endif // GOLDBACHMODEL_H
