import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: configPage
    anchors.fill: parent

    Connections {
        target: MozContext
        onRecvObserve: {
            if (message == "embed:allprefs") {
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
            MozContext.sendObserve("embedui:allprefs", {})
        }
        else if (status === PageStatus.Deactivating) {
            MozContext.sendObserve("embedui:saveprefs", {})
        }
    }

    function filterModel(value) {
        if (value == "") {
            prefsList.model = prefsListModel
        }
        else {
            filterListModel.clear()
            for (var i=0; i<prefsListModel.count; i++) {
                if (prefsListModel.get(i).name.toLowerCase().search(value.toLowerCase()) != -1) {
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

    SilicaListView {
        id: prefsList
        model: prefsListModel
        anchors.fill: parent

        VerticalScrollDecorator { flickable: prefsList }

        header: Column {
            PageHeader {
                id: header
                title: "about:config"
            }

            SearchField {
                width: prefsList.width

                text: ""
                placeholderText: "Filter"
                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
                font { family: Theme.fontFamily; pixelSize: Theme.fontSizeMedium }

                EnterKey.onClicked: {
                    filterModel(text);
                }
            }
        }

        delegate: Column {
            height: Theme.itemSize
            width: prefsList.width

            TextField {
                label: model.name
                text: model.value
                placeholderText: model.name
                visible: model.type != 128
                font { family: Theme.fontFamily; pixelSize: Theme.fontSizeMedium }
                inputMethodHints: (model.type == 64 ? Qt.ImhDigitsOnly : 0)
                width: parent.width

                onTextChanged: {
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
                    MozContext.setPref(model.name, checked)
                }
            }
        }
    }
}
