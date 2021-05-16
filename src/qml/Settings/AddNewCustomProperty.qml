/*
 * SPDX-FileCopyrightText: 2021 George Florea Bănuș <georgefb899@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import org.kde.kirigami 2.11 as Kirigami
import com.georgefb.haruna 1.0
import Haruna.Components 1.0

SettingsBasePage {
    id: root

    property bool isAction: true
    property int id: customPropsModel.rowCount() + 1

    GridLayout {
        columns: 2

        Label {
            text: i18n("Command")
            Layout.alignment: Qt.AlignRight
        }

        TextField {
            id: commandTextField

            placeholderText: "add volume +10"
            Layout.fillWidth: true
        }

        Label {
            text: i18n("OSD Message")
            Layout.alignment: Qt.AlignRight
        }

        TextField {
            id: osdMessageTextField

            enabled: typeGroup.checkedButton.optionName === "shortcut"
            placeholderText: "Filename: ${filename}"
            Layout.fillWidth: true
        }

        Label {
            text: i18n("Type")
            Layout.alignment: Qt.AlignRight | Qt.AlignTop
        }

        ButtonGroup {
            id: typeGroup

            buttons: typeGroupItems.children
        }

        Column {
            id: typeGroupItems

            spacing: Kirigami.Units.largeSpacing

            RadioButton {
                property string optionName: "shortcut"

                checked: true
                text: i18n("Keyboard shortcut")
            }

            RadioButton {
                property string optionName: "startup"

                text: i18n("Run at startup")
            }
        }

    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: i18n("&Save")
                icon.name: "document-save"
                enabled: commandTextField.text !== ""
                onClicked: {
                    if (commandTextField.text !== "") {
                        customPropsModel.saveCustomProperty("Command_" + root.id,
                                                            commandTextField.text,
                                                            osdMessageTextField.text,
                                                            typeGroup.checkedButton.optionName)
                        app.createUserShortcut("Command_" + root.id, commandTextField.text)
                        customPropsModel.getProperties()
                    }
                    applicationWindow().pageStack.replace("qrc:/CustomProperties.qml")
                }

                Layout.alignment: Qt.AlignRight
            }
        }
    }
}
