import QtQuick 2.1
import Sailfish.Silica 1.0
import Sailfish.Browser 1.0

Item {
    property WebPage webPage
    property int flickableDirection: Flickable.AutoFlickDirection
    property bool moving: true
    property bool flickingVertically: true
    property int verticalVelocity: 100000
    property bool atYBeginning: false
    property int contentHeight: webPage.contentHeight
    property bool atYEnd: false

    //signal flickingVerticallyChanged

    height: webPage.height
    width: webPage.width

    QtObject {
       id: pullUpMenu
       property bool active: false
    }

    QtObject {
       id: pullDownMenu
       property bool active: false
    }

    Connections {
        target: webPage
        onFlickingVerticallyChanged: {
            flickingVerticallyChanged()
        }
    }

    function scrollToTop() {
        webPage.sendAsyncMessage("embedui:scrollTo", {"x":0, "y":0})
    }

    function scrollToBottom() {
        webPage.sendAsyncMessage("embedui:scrollTo", {"x":0, "y":webPage.contentHeight})
    }
}
