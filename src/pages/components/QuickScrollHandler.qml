import QtQuick 2.1
import Sailfish.Silica 1.0
import Sailfish.Browser 1.0

Item {
    property WebPage webPage
    property int flickableDirection: Flickable.AutoFlickDirection
    property bool moving: webPage.moving
    property bool flickingVertically: webPage.verticalScrollDecorator.moving
    property int verticalVelocity: 100000
    property bool atYBeginning: webPage.scrollableOffset.y === 0
    property int contentHeight: webPage.contentHeight
    property bool atYEnd: (webPage.scrollableOffset.y + webPage.height) >= webPage.contentHeight

    //signal flickingVerticallyChanged

    height: webPage.height
    width: webPage.width

    Timer {
        interval: 1000
        running: true; repeat: true
        onTriggered: {
            /*console.log("webPage.verticalScrollDecorator : " + webPage.verticalScrollDecorator.position)*/
            console.log("webPage.contentHeight : " + webPage.contentHeight)
            //console.log("webPage.scrollableOffset : " + webPage.scrollableOffset)
            console.log("webPage.scrollableSize : " + webPage.scrollableSize)
            console.log("atYBeginning : " + atYBeginning)
            console.log("atYEnd : " + atYEnd)/*
            console.log("webPage.contentRect.height : " + webPage.contentRect.height)
            console.log("webPage.height : " + webPage.height)*/
        }
    }

    QtObject {
       id: pullUpMenu
       property bool active: false
    }

    QtObject {
       id: pullDownMenu
       property bool active: false
    }

    /*Connections {
        target: webPage
        onFlickingVerticallyChanged: {
            flickingVerticallyChanged()
        }
    }*/

    function scrollToTop() {
        webPage.sendAsyncMessage("embedui:scrollTo", {"x":0, "y":0})
    }

    function scrollToBottom() {
        webPage.sendAsyncMessage("embedui:scrollTo", {"x":0, "y":webPage.contentHeight})
    }
}
