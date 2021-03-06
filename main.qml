import QtQuick 2.10
import QtQuick.Window 2.10
import Qt.labs.handlers 1.0

Window {
    visible: true
    width: 640
    height: 640
    title: qsTr("Speeechle")

    Image {
        id: background
        anchors.fill: parent
        source: "qrc:/app_background.png"
    }

    Image {
        width: parent.width
        height: parent.height / 4
        x: parent.width / 3
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
            listenButton.scale -= 0.1
            listenAnim.start()
        }

        onSpeechTextReady: {
            console.log("Speech text ready")
            messageModel.append({
                                    name: "receiver",
                                    message: recognition.speechText
                                })
            messages.currentIndex = messages.count - 1
            //speech.say(recognition.speechText)
            listenButton.scale += 0.1
            listenAnim.start()
        }
    }

    Image {
        id: listenButton
        source: "qrc:/listenbuton.png"

        x: parent.width / 3
        y: parent.height / 3

        width: 200
        height: 100

        TapHandler {
            onTapped: {
                if (!recognition.listening) {
                    recognition.start()
                } else {
                    recognition.stop()
                }
            }
        }

        NumberAnimation on scale {
            id: listenAnim
            to: 1
            duration: 200
            easing.type: Easing.InOutQuad
        }

        DragHandler {
        }
    }

    Image {
        id: editBox
        source: "qrc:/textbox.png"

        x: parent.width / 3
        y: parent.height / 2

        width: 500
        height: 200

        DragHandler {
        }

        TextEdit {
            id: edit
            font.pointSize: 30
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
                id: sendTapHandler
                onTapped: {
                    messageModel.append({
                                            name: "sender",
                                            message: edit.text
                                        })
                    messages.currentIndex = messages.count - 1
                    speech.volume = 100
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
                    font.pointSize: 25
                    text: name
                }
                Text {
                    font.pointSize: 25
                    text: message
                }
            }
        }
    }
}
