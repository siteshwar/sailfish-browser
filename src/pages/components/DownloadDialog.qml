/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Dmitry Rozhkov <dmitry.rozhkov@jollamobile.com>
**
****************************************************************************/

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */


import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: popup

    property QtObject webView
    property int winid
    property string name

    DialogHeader {
        acceptText: qsTr("File Download")
    }

    Label {
        id: label
        anchors.centerIn: parent
        width: parent.width - 2 * Theme.paddingLarge
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
        font {
            family: Theme.fontFamilyHeading
            pixelSize: Theme.fontSizeMedium
        }
        color: Theme.highlightColor
        opacity: 0.6
        text: qsTr("Do you want to download '" + name + "' ?")
    }

    onRejected : {
        webView.sendAsyncMessage("filepickerresponse",
                                {
                                "winid": winid,
                                "accepted": false,
                                "items": []
                                })
    }

    onAccepted: {
        webView.sendAsyncMessage("filepickerresponse",
                                {
                                "winid": winid,
                                "accepted": true,
                                "items": ["/home/nemo/Downloads/" + name]
                                })

    }
}
