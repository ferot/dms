#ifndef THREADER_H
#define THREADER_H
#include <QObject>
#include <QThread>
#include <QVector>


class Threader : public QObject
{
    Q_OBJECT
private:
    QVector<QThread*> threads;

public:
    Threader();
    ~Threader();
signals:
    void quitting();
public slots:
    void runInThread(QObject* object);
    void start();
    void quit();
    void wait();
};

#endif // THREADER_H
