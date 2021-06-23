#include "erpservice.h"

#include <QVariantMap>
#include <QLoggingCategory>

#include "client/OAIOrderApi.h"
#include "client/OAIOrder.h"

const QLoggingCategory logcat("ErpService");

using OpenAPI::OAIOrderApi;

ErpService::ErpService(QObject *parent) : QObject(parent)
  , m_orderApi(new OAIOrderApi())
{
    connect(m_orderApi, &OAIOrderApi::getOrderByIdSignal, this, [this](OpenAPI::OAIOrder summary){this->onGetOrderByIdReturnd(summary);});
    connect(m_orderApi, &OAIOrderApi::getOrderByIdSignalE, this, &ErpService::onGetOrderByIdReturnd);

    connect(m_orderApi, &OAIOrderApi::listOrdersSignal, this, [this](QList<OpenAPI::OAIOrder> summary){this->onListOrdersReturnd(summary);});
    connect(m_orderApi, &OAIOrderApi::listOrdersSignalE, this, &ErpService::onListOrdersReturnd);

    connect(m_orderApi, &OAIOrderApi::updateOrderWithSignal, this, [this](OpenAPI::OAIOrder summary){this->onUpdateOrderWithReturned(summary);});
    connect(m_orderApi, &OAIOrderApi::updateOrderWithSignalE, this, &ErpService::onUpdateOrderWithReturned);
}

ErpService::~ErpService() {
    m_orderApi->deleteLater();
}

void ErpService::getOrderById(qint64 order_id)
{
    qCDebug(logcat) << __FUNCTION__ << "(" << order_id << ")";
    m_orderApi->getOrderById(order_id);
}

void ErpService::listOrders(QStringList status)
{
    qCDebug(logcat) << __FUNCTION__ << "(" << status << ")";
    QList<QString> _status;
    for (int i = 0; i < status.size(); ++i) {
        _status.push_back(status[i]);
    }
    m_orderApi->listOrders(_status);
}

void ErpService::updateOrderWith(qint64 order_id, QVariantMap updates)
{
    qCDebug(logcat) << __FUNCTION__ << "(" << order_id << "," << updates << ")";
    OpenAPI::OAIPartialOrder partial;
    partial.fromJsonObject(QJsonObject::fromVariantMap(updates));
    m_orderApi->updateOrderWith(order_id, partial);
}

void ErpService::onGetOrderByIdReturnd(OpenAPI::OAIOrder summary, QNetworkReply::NetworkError error_type, QString error_str)
{
    qCDebug(logcat) << __FUNCTION__ << "( 0x" << Qt::hex << &summary << "," << error_type << "," << error_str << ")";
    if (error_type) {
        emit getOrderByIdFinished(QVariantMap(), error_type, error_str);
    } else if (!summary.isValid()) {
        qCWarning(logcat) << __FUNCTION__ << " got called w/o data object pointer and no error!";
        emit getOrderByIdFinished(QVariantMap(), -1, "Result is missing data");
    } else {
        emit getOrderByIdFinished(summary.asJsonObject().toVariantMap());
    }
}
void ErpService::onListOrdersReturnd(QList<OpenAPI::OAIOrder> summary, QNetworkReply::NetworkError error_type, QString error_str)
{
    qCDebug(logcat) << __FUNCTION__ << "( 0x" << Qt::hex << &summary << "," << error_type << "," << error_str << ")";
;    if (error_type) {
        emit listOrdersFinished(QVariantList(), error_type, error_str);
    } else if (summary.empty()) {
       qCWarning(logcat) << __FUNCTION__ << " got called w/o data object pointer and no error!";
        emit listOrdersFinished(QVariantList(), -1, "Result is missing data");
    } else {
        QVariantList orderList;
        foreach (auto order, summary) {
            orderList << order.asJsonObject();
        }
        emit listOrdersFinished(orderList);
    }
}
void ErpService::onUpdateOrderWithReturned(OpenAPI::OAIOrder summary, QNetworkReply::NetworkError error_type, QString error_str)
{
    qCDebug(logcat) << __FUNCTION__ << "( 0x" << Qt::hex << &summary << "," << error_type << "," << error_str << ")";
    if (error_type) {
        emit updateOrderWithFinished(QVariantMap(), error_type, error_str);
    } else if (!summary.isValid()) {
        qCWarning(logcat) << __FUNCTION__ << " got called w/o data object pointer and no error!";
        emit updateOrderWithFinished(QVariantMap(), -1, "Result is missing data");
    } else {
        emit updateOrderWithFinished(summary.asJsonObject().toVariantMap());
    }
}
