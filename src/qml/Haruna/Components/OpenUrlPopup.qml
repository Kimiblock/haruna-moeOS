import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Popup {
    id: root

    signal urlOpened(string url)
    property string lastUrl: ""

    modal: true

    onOpened: {
        openUrlTextField.forceActiveFocus(Qt.MouseFocusReason)
        openUrlTextField.selectAll()
    }

    RowLayout {
        anchors.fill: parent

        Label {
            text: i18nc("@info", "<a href=\"https://youtube-dl.org\">Youtube-dl</a> was not found.")
            visible: !app.hasYoutubeDl()
            onLinkActivated: Qt.openUrlExternally(link)
        }

        TextField {
            id: openUrlTextField

            text: root.lastUrl
            visible: app.hasYoutubeDl()
            Layout.preferredWidth: 400
            Layout.fillWidth: true

            Keys.onPressed: {
                switch(event.key) {
                case Qt.Key_Enter:
                case Qt.Key_Return:
                    openUrlButton.clicked()
                    return;
                case Qt.Key_Escape:
                    openUrlPopup.close()
                    return;
                }
            }
        }

        Button {
            id: openUrlButton

            visible: app.hasYoutubeDl()
            text: i18nc("@action:button", "Open")

            onClicked: {
                root.urlOpened(openUrlTextField.text)
                root.close()
                openUrlTextField.clear()
            }
        }
    }
}
