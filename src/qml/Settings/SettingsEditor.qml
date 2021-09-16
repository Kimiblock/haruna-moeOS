/*
 * SPDX-FileCopyrightText: 2020 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.10
import QtQuick.Window 2.1
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

import org.kde.kirigami 2.11 as Kirigami
import org.kde.haruna 1.0 as Haruna

Kirigami.ApplicationWindow {
    id: root

    width: Kirigami.Units.gridUnit * 45
    height: Kirigami.Units.gridUnit * 35
    title: i18n("Haruna Settings")
    visible: false
    pageStack.initialPage: "qrc:/Navigation.qml"

    Component.onCompleted: pageStack.push("qrc:/General.qml")

    Window {
        id: helpWindow

        width: Kirigami.Units.gridUnit * 40
        height: Kirigami.Units.gridUnit * 35
        title: i18n("Help")
        color: Kirigami.Theme.backgroundColor
        onVisibleChanged: info.text = app.getFileContent(applicationWindow().pageStack.currentItem.helpFile)

        Flickable {
            id: scrollView

            property int scrollStepSize: 100

            anchors.fill: parent
            contentHeight: info.height

            ScrollBar.vertical: ScrollBar {
                id: scrollbar
                policy: ScrollBar.AlwaysOn
                stepSize: scrollView.scrollStepSize/scrollView.contentHeight
            }

            MouseArea {
                anchors.fill: parent
                onWheel: {
                    if (wheel.angleDelta.y > 0) {
                        scrollbar.decrease()
                    } else {
                        scrollbar.increase()
                    }
                }
            }

            TextArea {
                id: info

                background: Rectangle {
                    color: "transparent"
                    border.color: "transparent"
                }
                width: parent.width
                color: Kirigami.Theme.textColor
                readOnly: true
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                selectByMouse: true
                rightPadding: scrollbar.width
                onLinkActivated: Qt.openUrlExternally(link)
                onHoveredLinkChanged: hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }
    }
}
