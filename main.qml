import QtQuick 2.10
import QtQuick.Window 2.10
import Qt.labs.handlers 1.0

Window {
    visible: true
    width: 640
    height: 640
    title: qsTr("Speechle")

    Image {
        id: background
        anchors.fill: parent
        source: "qrc:/app_background.png"
    }

    Image {
        width: parent.width
        height: parent.height / 4
        source: "qrc:/speechle_banner.png"

        DragHandler {
        }
    }

    ListView {
        id: messages

        height: parent.height
        interactive: true

        model: messageModel
        delegate: messageDelegate
        highlight: Image {
            source: "qrc:/textbox.png"
        }
        focus: true
    }

    Connections {
        target: speech
        onStateChanged: {
            console.log("Speech state: " + speech.state)
        }
    }

    Connections {
        target: recognition
        onListeningStarted: {
            console.log("listening started!")
        }

        onSpeechTextReady: {
            console.log("Speech text ready")
            messageModel.append({
                                    name: "receiver",
                                    message: recognition.speechText
                                })
            messages.currentIndex = messages.count - 1
            speech.say(recognition.speechText)
        }
    }

    Image {
        id: listenButton
        source: "qrc:/listenbuton.png"

        width: 200
        height: 100

        TapHandler {
            onTapped: if (!recognition.listening) {
                          recognition.start()
                      } else {
                          recognition.stop()
                      }
        }

        DragHandler {
        }
    }

    Image {
        id: editBox
        source: "qrc:/textbox.png"
        width: 200
        height: 200

        DragHandler {
        }

        TextEdit {
            id: edit
            font.pointSize: 20
            wrapMode: Text.WordWrap
            color: "red"
            text: "Welcome to zombo.com"
        }

        Text {
            font.pointSize: 40
            text: "➡️"
            color: "blue"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            TapHandler {
                onTapped: {
                    messageModel.append({
                                            name: "sender",
                                            message: edit.text
                                        })
                    messages.currentIndex = messages.count - 1
                    speech.say(edit.text)
                }
            }
        }
    }

    MessageModel {
        id: messageModel
    }

    Component {
        id: messageDelegate

        Item {
            width: 180
            height: 200

            Column {
                Text {
                    text: name
                }
                Text {
                    text: message
                }
            }
        }
    }
}
