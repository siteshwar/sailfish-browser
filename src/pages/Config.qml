import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id : page
    anchors.fill: parent

    Connections {
        target: MozContext
        onRecvObserve: {
            if (message == "embed:allprefs") {
                console.log("Reeceived preferences");
                var allprefs = data;
                prefsListModel.clear();
                for (var i=0; i<allprefs.length; i++) {
                    console.log(JSON.stringify(allprefs[i]));
                    prefsListModel.append(allprefs[i]);
                }
            }
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            console.log("sending embedui:allprefs");
            MozContext.sendObserve("embedui:allprefs", {})
        }
        else if (status === PageStatus.Deactivating) {
            MozContext.sendObserve("embedui:saveprefs", {})
        }
    }

    function prefTypeByValue(value) {
        switch (value) {
            case 32: return "String"
            case 64: return "Integer"
            case 128: return "Boolean"
        }
    }

    function filterModel(value) {
        if (value == "") {
            prefsList.model = prefsListModel
        }
        else {
            filterListModel.clear()
            for (var i=0; i<prefsListModel.count; i++) {
                if (prefsListModel.get(i).name.search(value) != -1) {
                    console.log("Adding " + prefsListModel.get(i));
                    filterListModel.append(prefsListModel.get(i));
                }
            }
            prefsList.model = filterListModel
        }
    }

    ListModel {
        id: prefsListModel
    }

    ListModel {
        id: filterListModel
    }

    /*
    ParallelAnimation {
        id: animHide
        PropertyAnimation {
            target: page
            properties: "x"
            from: 0; to: -page.parent.width; duration: 300;
        }
        PropertyAnimation {
            target: page
            properties: "opacity"
            from: 1.0; to: 0.01; duration: 300;
        }
    }

    ParallelAnimation {
        id: animShow
        PropertyAnimation {
            target: page
            properties: "x"
            from: page.parent.width; to: 0; duration: 300;
        }
        PropertyAnimation {
            target: page
            properties: "opacity"
            from: 0.01; to: 1.0; duration: 300;
        }
    }

    Rectangle {
        id: title
        anchors.top: page.top
        anchors.left: page.left
        anchors.right: page.right
        height: 100 + filterArea.height
        color: "#dddddd"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                prefsList.hideAll()
            }
        }

        OverlayButton {
            id: back
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
            width: 60
            height: 60
            enabled: true
            iconSource: "../icons/backward.png"

            onClicked: {
                page.hide()
            }
        }

        Text {
            anchors.verticalCenter: back.verticalCenter
            anchors.left: back.right
            anchors.leftMargin: 20
            text: "Config"
            font.pixelSize: 40
        }

        InputArea {
            id: filterArea
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.top: back.bottom
            anchors.topMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 10
            inputMethodHints: Qt.ImhNoPredictiveText
            onAccepted: {
                filterModel(text)
            }
        }

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: "black"
        }
    }
    */

    SilicaListView {
        id: prefsList
        model: prefsListModel
        anchors.fill: parent

        header: Column {
            PageHeader {
                id: header
                title: "about:config"
            }

            TextField {
                width: prefsList.width

                text: ""
                placeholderText: "Filter"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase

                EnterKey.onClicked: {
                    filterModel(text);
                }
            }
        }
        /*PullDownMenu {
            MenuItem {
                text: "Add Item"
            }
        }*/

        /*        clip: true
        anchors.top: title.bottom
        anchors.left: page.left
        anchors.right: page.right
        anchors.bottom: page.bottom
        spacing: 0
        model: prefsListModel
        signal hideAll()
        signal filterName(string value)
        delegate: Item {
            id: prefDelegate
            width: parent.width
            height: visible ? content.height : 0
            property bool showMore: false

            Connections {
                target: prefsList
                onHideAll: {
                    prefValueEdit.setFocus(false)
                    showMore = false
                }
            }

            Rectangle {
                anchors.fill: parent
                anchors.margins: 2
                color: mArea.pressed ? "#d0d0d0" : (showMore ? "#e0e0e0" : "#efefef")
            }

            MouseArea {
                id: mArea
                anchors.fill: parent
                onClicked: {
                    prefsList.hideAll()
                    showMore = true
                }
            }

            Item {
                id: content
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 10
                height: showMore ? (prefName.height + (prefValueEdit.visible ? prefValueEdit.height : prefValueBool.height) + 30) : (Math.max(prefName.height, prefValue.height) + 15)

                Text {
                    id: prefName
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: showMore? parent.right : prefValue.left
                    anchors.rightMargin: 10
                    text: model.name
                    font.pixelSize: 20
                    elide: Text.ElideRight
                    wrapMode: Text.WrapAnywhere
                    color: model.modified ? "#0000ff" : "#000000"
                }

                Text {
                    id: prefValue
                    anchors.top: parent.top
                    anchors.right: parent.right
                    width: 100
                    text: model.value
                    font.pixelSize: 25
                    elide: Text.ElideRight
                    visible: !showMore
                }

                Text {
                    id: prefType
                    anchors.verticalCenter: prefValueBool.verticalCenter
                    anchors.topMargin: 10
                    anchors.right: parent.right
                    font.pixelSize: 20
                    text: prefTypeByValue(model.type)
                    visible: showMore
                }

                InputArea {
                    id: prefValueEdit
                    anchors.top: prefName.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.right: prefType.left
                    anchors.rightMargin: 10
                    text: model.value
                    visible: showMore && model.type != 128
                    inputMethodHints: (model.type == 64 ? Qt.ImhDigitsOnly : 0)
                    onAccepted: {
                        if (model.type == 64) {
                            MozContext.setPref(model.name, parseInt(text))
                        }
                        else {
                            MozContext.setPref(model.name, text)
                        }
                        prefValue.text = text
                    }
                }

                Checkbox {
                    id: prefValueBool
                    anchors.top: prefName.bottom
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.right: prefType.left
                    anchors.rightMargin: 10
                    fixedHeight: 30
                    isSwitch: true
                    text: checked ? "true" : "false"
                    visible: showMore && model.type == 128
                    onClicked: {
                        MozContext.setPref(model.name, checked)
                        prefValue.text = checked ? "true" : "false"
                    }
                }
            }
        }
        */
        delegate: Column {
            height: Theme.itemSize
            width: prefsList.width

            TextField {
                label: model.name
                text: model.value
                placeholderText: model.name
                visible: model.type != 128
                font { family: Theme.fontFamily; pixelSize: Theme.fontSizeSmall }
                inputMethodHints: (model.type == 64 ? Qt.ImhDigitsOnly : 0)
                width: parent.width

                onTextChanged: {
                    console.log("Setting " + text)
                    if (model.type == 64) {
                        MozContext.setPref(model.name, parseInt(text))
                    }
                    else {
                        MozContext.setPref(model.name, text)
                    }
                }
            }

            TextSwitch {
                text: model.name
                checked: model.value == "true"
                visible: model.type == 128
                width: parent.width
                onCheckedChanged: {
                    console.log("Setting " + checked);
                    MozContext.setPref(model.name, checked)
                }
            }
        }
    }
}
