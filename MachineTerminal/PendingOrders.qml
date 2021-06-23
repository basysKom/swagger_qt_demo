import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 400
    height: parent ? parent.height : undefined
    title: "Pending orders"

    Timer{
        id: timer
        triggeredOnStart: true
        interval: 5000
        repeat: true
        onTriggered: {
            ErpService.listOrders('pending');
        }
    }

    StackView.onActivated: {
        timer.start()
    }
    StackView.onDeactivated: {
        timer.stop()
    }

    ListView {
        id: list
        currentIndex: -1
        anchors.fill: parent
        model: []

        delegate: ItemDelegate {
            text: modelData.id + ': ' + modelData.wood + ' (' + modelData.dimensions.height + 'x' + modelData.dimensions.width + 'cm)'
            width: parent.width
            highlighted: ListView.isCurrentItem
            onClicked: page.StackView.view.push("OrderDetails.qml", {order: list.model[index]})
        }
    }

    Connections {
        target: ErpService
        onListOrdersFinished: {
            orders.sort(function (a, b) {return a.id - b.id;});

            if (orders.length !== list.model.length) {
                // replace model
                list.model = orders;
                return;
            } else {
                for (var i = 0; i < orders.length; ++i) {
                    if (orders[i].id !== list.model[i].id) {
                        // replace model
                        list.model = orders;
                        return;
                    }
                }
            }
        }
    }
}
