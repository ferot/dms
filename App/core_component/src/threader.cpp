#include "threader.h"

Threader::Threader()
{

}
/**
 * @brief Threader::~Threader
 * Threader destructor. Deletes thread objects from vector.
 */
Threader::~Threader()
{
    quit();
    wait();

    for (auto thread : threads){
        delete thread;
    }
}

/**
 * @brief Threader::runInThread
 * Spawns new thread, adds to thread collection and assigns quit signals.
 *
 * @param object - object to be run as thread
 */
void Threader::runInThread(QObject * object)
{
    QThread* thread = new QThread();

    QObject::connect(this, &Threader::quitting, thread, &QThread::quit);
    threads.append(thread);

    object->moveToThread(thread);
    QObject::connect(thread, &QThread::finished, object, &QObject::deleteLater);
}

/**
 * @brief Threader::start
 * Starts each thread from collection.
 */
void Threader::start()
{
    for (QThread* thread : threads){
        thread->start();
    }
}

/**
 * @brief Threader::quit
 * Sends signal to threads. In fact each of them calls Qthread::quit.
 * Called on threader instance destruction.
 */
void Threader::quit()
{
    emit quitting();
}

/**
 * @brief Threader::wait
 * Waits for each thread to be joined.
 */
void Threader::wait()
{
    for (QThread* thread : threads)
        thread->wait();
}
