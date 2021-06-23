#ifndef ERPSERVICE_H
#define ERPSERVICE_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QNetworkReply>
#include <client/OAIOrder.h>
#include <client/OAIOrderApi.h>

class ErpService : public QObject
{
    Q_OBJECT
    OpenAPI::OAIOrderApi * const m_orderApi;
public:
    explicit ErpService(QObject *parent = nullptr);
    ~ErpService();

    Q_INVOKABLE void getOrderById(qint64 order_id);
    Q_INVOKABLE void listOrders(QStringList status = QStringList());
    Q_INVOKABLE void updateOrderWith(qint64 order_id, QVariantMap updates);

signals:
    void getOrderByIdFinished(QVariantMap order, int errorNo = 0, QString errorMsg = QString());
    void listOrdersFinished(QVariantList orders, int errorNo = 0, QString errorMsg = QString());
    void updateOrderWithFinished(QVariantMap order, int errorNo = 0, QString errorMsg = QString());

private slots:
    void onGetOrderByIdReturnd(OpenAPI::OAIOrder summary, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void onListOrdersReturnd(QList<OpenAPI::OAIOrder> summary, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());
    void onUpdateOrderWithReturned(OpenAPI::OAIOrder summary, QNetworkReply::NetworkError error_type = QNetworkReply::NetworkError::NoError, QString error_str = QString());

};

#endif // ERPSERVICE_H
