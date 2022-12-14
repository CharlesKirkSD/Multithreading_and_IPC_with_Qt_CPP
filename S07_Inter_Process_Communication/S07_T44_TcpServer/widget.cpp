#include "widget.h"
#include "ui_widget.h"

#include <QDebug>
#include <QDataStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_socket(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Server | Data Receiver");

    m_server.listen(QHostAddress::LocalHost, 4040);

    connect(&m_server, &QTcpServer::newConnection, this, &Widget::gotConnection);
}

Widget::~Widget()
{
    // Cleanup
    if (m_socket) {
        qDebug() << "Closing socket : " << (m_socket == nullptr);
        m_socket->close();
        m_socket->deleteLater();
    }

    m_server.close();

    delete ui;
}

void Widget::gotConnection()
{
    qDebug() << "Server got new connection";
    m_socket = m_server.nextPendingConnection();
    connect(m_socket, &QTcpSocket::readyRead, this, &Widget::readData);
}

void Widget::readData()
{
    QDataStream in(m_socket);
    in.startTransaction(); // Recursive Method that loops until the entire data is ready and reads it in one go.

    QString rcvString;
    in >> rcvString;

    if (!in.commitTransaction()) {
        return; // Wait for more data
    }

    ui->textEdit->append(rcvString);
}

