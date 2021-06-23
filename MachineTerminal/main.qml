import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    visible: true
    width: 400
    height: 600
    title: qsTr("Machine Terminal")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            visible: stackView.depth === 1 || !!stackView.currentItem.canGoBack
            text: stackView.depth > 1 ? '\u2039' : '\u2630'
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth === 1) {
                    drawer.open();
                } else {
                    stackView.pop();
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height
        MouseArea {
            anchors.fill: parent
            onClicked: drawer.close()
        }
        Text {
            anchors.margins: 5
            anchors.fill: parent
            font.family: "PT Sans Narrow,sans-serif"
            font.weight: Font.Light
            font.pixelSize: 16
            text: 'OpenAPI example by<br><font color="#ef7d00" size="5"><b>basysKom GmbH</b></font><br><p>Visit us on <a href="https://www.basyskom.com/">www.basyskom.com</a>'
        }
    }

    StackView {
        id: stackView
        initialItem: "PendingOrders.qml"
        anchors.fill: parent
    }
}
