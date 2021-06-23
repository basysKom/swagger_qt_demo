import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Page {
    id: page
    width: 400
    height: parent ? parent.height : undefined

    property var order: ({})
    property bool canGoBack: order.status !== 'processing'

    title: (order.status === 'processing' ? '\u2941 Processing order: ' : 'Order: ') + order.id

    Image {
        id: image
        anchors.margins: 15
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        source: "drawing.svg"
        fillMode: Image.PreserveAspectFit
        Label {
            text: "240 cm"
            font.weight: Font.Light
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: parent.width / 4
        }
        Label {
            text: "500 cm"
            font.weight: Font.Light
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
        Label {
            text: order.dimensions.width + " cm"
            font.weight: Font.Light
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -parent.width / 3
        }
        Label {
            text: order.dimensions.height + " cm"
            font.weight: Font.Light
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    Text {
        anchors {
            left: parent.left
            right: parent.right
            top: image.bottom
            bottom: parent.bottom
            margins: 15
            topMargin: 30
        }
        text: 'Wood: ' + order.wood + '<br>Height: ' + order.dimensions.height + ' cm<br>Width: ' + order.dimensions.width + '<br>Length: ' + order.dimensions.length + ' cm'
    }

    function updateOrderStatus(status, callback) {

        if (callback && typeof callback === "function") {
            console.log('connecting to signal');
            function _cb(data, errNo, errMsg) {
                if (errNo) {
                    console.warn('updateOrderStatus(' + status + ') failed:', errNo, errMsg);
                }

                ErpService.updateOrderWithFinished.disconnect(_cb)
                callback(data, errNo, errMsg);
            }

            ErpService.updateOrderWithFinished.connect(_cb)
        }

        ErpService.updateOrderWith(order.id, ({status: status}));
    }

    footer: RowLayout {
        spacing: 0
        Button {
            Layout.fillWidth: true
            text: 'Start processing \u25b6'
            visible: order.status === 'pending'
            onClicked: {
                updateOrderStatus('processing', function (data, errNo, errMsg) {
                    if (errNo) {
                        return;
                    }

                    order = data;
                })
            }
        }
        Button {
            Layout.fillWidth: true
            text: 'Defer \u23cf'
            visible: order.status === 'processing'
            onClicked: {
                updateOrderStatus('deferred', function (data, errNo, errMsg) {
                    if (errNo) {
                        return;
                    }

                    page.StackView.view.pop();
                })
            }
        }
        Button {
            Layout.fillWidth: true
            text: 'Finish \u2611'
            visible: order.status === 'processing'
            onClicked: {
                updateOrderStatus('processed', function (data, errNo, errMsg) {
                    if (errNo) {
                        return;
                    }

                    page.StackView.view.pop();
                })
            }
        }
    }
}
