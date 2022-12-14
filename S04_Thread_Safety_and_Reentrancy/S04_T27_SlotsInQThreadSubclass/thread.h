#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTimer>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

signals:

public slots:
    void incrementCount();

    // QThread interface
protected:
    virtual void run() override;

private:
    QTimer * m_timer = nullptr;
    int m_count;
};

#endif // THREAD_H
