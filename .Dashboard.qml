import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible: true
    width: 1200
    height: 720
    title: "Smart Parking System"
    color: "#0B1220"

    property int maxCars: 12
    property int slotCounter: 0
    property bool demoMode: false

    ListModel { id: carListModel }
    ListModel { id: historyModel }

    Column {
        anchors.fill: parent
        spacing: 20
        padding: 20

        Text {
            text: "SMART PARKING DASHBOARD"
            font.pixelSize: 30
            font.bold: true
            color: "#38BDF8"
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }

        Row {
            spacing: 15
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle {
                width: 260; height: 55; radius: 14; color: "#111827"; border.color: "#38BDF8"
                Row {
                    anchors.centerIn: parent; spacing: 10
                    Text { text: "Capacity:"; color: "white"; font.pixelSize: 16 }
                    TextField { id: capacityInput; width: 70; height: 32 }
                    Button {
                        text: "Set"
                        onClicked: {
                            if (capacityInput.text !== "") {
                                maxCars = parseInt(capacityInput.text)
                                addHistory("SET", "Capacity set to " + maxCars)
                            }
                        }
                    }
                }
            }
        }

        Row {
            spacing: 25
            anchors.horizontalCenter: parent.horizontalCenter
            Repeater {
                model: [
                    { title: "PARKED", color: "#22D3EE" },
                    { title: "QUEUE", color: "#A78BFA" },
                    { title: "AVAILABLE", color: "#34D399" }
                ]
                Rectangle {
                    width: 240; height: 120; radius: 18; color: "#111827"; border.color: modelData.color
                    Column {
                        anchors.centerIn: parent; spacing: 6
                        Text { text: modelData.title; color: "#9CA3AF" }
                        Text {
                            font.pixelSize: 26; font.bold: true; color: "white"
                            text: modelData.title === "PARKED" ? getParkedCount() + " / " + maxCars :
                                  modelData.title === "QUEUE" ? getQueueCount() : maxCars - getParkedCount()
                        }
                    }
                }
            }
        }

        Rectangle {
            width: parent.width; height: 320; radius: 20; color: "#111827"
            Grid { id: parkingGrid; columns: 6; spacing: 18; anchors.centerIn: parent }
        }

        Row {
            spacing: 15
            Button { text: "➕ Add"; onClicked: addCar() }
            Button { text: "➖ Remove"; onClicked: removeCar() }
            Button { text: "📜 History"; onClicked: historyPopup.open() }
        }
    }

    function getParkedCount() {
        var c = 0;
        for (var i = 0; i < carListModel.count; i++) if (carListModel.get(i).status === "Parked") c++;
        return c;
    }

    function getQueueCount() {
        var c = 0;
        for (var i = 0; i < carListModel.count; i++) if (carListModel.get(i).status === "Queue") c++;
        return c;
    }

    function addCar() {
        var status = getParkedCount() < maxCars ? "Parked" : "Queue"
        slotCounter++
        carListModel.append({ id: "CAR-" + slotCounter, status: status })
        if (status === "Parked") addCarVisual("CAR-" + slotCounter, slotCounter)
        addHistory("ADD", "Car " + slotCounter + " status: " + status)
    }

    function addHistory(action, details) {
        historyModel.append({ action: action, details: details, time: Qt.formatDateTime(new Date(), "hh:mm:ss") })
    }
}
