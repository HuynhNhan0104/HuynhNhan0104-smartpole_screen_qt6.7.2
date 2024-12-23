import QtQuick
// import SerialObserver 1.0
import QtQuick.Layouts
import QtMultimedia

Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    visibility:"FullScreen"
    title: qsTr("Hello World")
    // color:"#1d1d35"
/*-------------------------Serial control---------------------------------*/
    // SerialObserver{
    //     id:observer
    // }



/*-------------------------mqtt control---------------------------------*/



/*-------------------------dashboard---------------------------------*/
    property int rowNumber: 1
    property int columnNumber: 6

    property double dashboardWidth: window.width
    property double dashboardHeight: window.height*0.2


    property double cellWidth: dashboardWidth/columnNumber
    property double cellHeight: dashboardHeight/rowNumber



    property var itemList: [
        {
            tagName:"Temp",
            measuredValue: DashboardController.temparatureValue,
            unit: "°C",
            minLimit: 0,
            maxLimit: 50,
            lowerThreshold: 25,
            upperThreshold: 30,
            baseColor: function(value) {
                if (value < this.lowerThreshold) return "green";
                else if (value < this.upperThreshold) return "orange";
                else return "red";
            }
        },
        {
            tagName:"Humi",
            measuredValue: DashboardController.humidityValue,
            unit: "%",
            minLimit: 0,
            maxLimit: 100,
            lowerThreshold: 50,
            upperThreshold: 80,
            baseColor: function(value) {
                if (value < this.lowerThreshold) return "green";
                else if (value < this.upperThreshold) return "orange";
                else return "red";
            }
        },
        {
            tagName:"PM2.5",
            measuredValue:
            DashboardController.pm25Value,
            unit: "µm/m3",
            minLimit: 0,
            maxLimit: 200,
            lowerThreshold: 12,
            upperThreshold: 35,
            baseColor:function(value) {
                if (value < this.lowerThreshold) return "green";
                else if (value < this.upperThreshold) return "orange";
                else return "red";
            }
        },
        {
            tagName:"PM10",
            measuredValue: DashboardController.pm10Value,
            unit: "µm/m3",
            minLimit: 0,
            maxLimit: 400,
            lowerThreshold: 55,
            upperThreshold: 155,
            baseColor: function(value) {
                if (value < this.lowerThreshold) return "green";
                else if (value < this.upperThreshold) return "orange";
                else return "red";
            }
        },
        {
            tagName:"Atm",
            measuredValue: DashboardController.atmValue,
            unit: "Kpa",
            minLimit: 0,
            maxLimit: 120,
            lowerThreshold: 40,
            upperThreshold: 105,
            baseColor: function(value) {
                if (this.lowerThreshold < value && value < this.upperThreshold) return "green";
                else return "red";
            }
        },
        {
            tagName:"Noise",
            measuredValue: DashboardController.noiseValue,
            unit: "dB",
            minLimit: 0,
            maxLimit: 140,
            lowerThreshold: 80,
            upperThreshold: 120,
            baseColor:function(value) {
                if (value < this.lowerThreshold) return "green";
                else if (value < this.upperThreshold) return "orange";
                else return "red";
            }
        }
    ]

    Rectangle{
        id: dashboard
        width: dashboardWidth
        height: dashboardHeight
        // color:"#4A148C"
        color:"black"
        GridLayout{
            rows: window.rowNumber
            columns: window.columnNumber
            columnSpacing: 2
            rowSpacing: 0

            Repeater{
                model: itemList.length
                RadialBarProgress{
                    // id: test
                    width: window.cellWidth
                    height: window.cellHeight

                    // edit here
                    title: itemList[index].tagName
                    currentValue: itemList[index].measuredValue
                    contentUnit: itemList[index].unit
                    minValue: itemList[index].minLimit
                    maxValue: itemList[index].maxLimit
                    lowerThreshold: itemList[index].lowerThreshold
                    upperThreshold: itemList[index].upperThreshold
                    baseColor: itemList[index].baseColor(itemList[index].measuredValue)


                    contentTextSize: 25
                    titleSize: 20
                    widthLine: 15

                    titleColor: "#FAFAFA"
                    backgroundColor: "#1d1d35"
                    foregroundColor: "#263238"   
                }
            }
        }

    }







    // RadialBarProgress{
    //     id: test
    //     width: 400
    //     height: 300
    //     // contentText: qsTr("50")
    //     currentValue: observer.value

    //     contentTextSize: 30
    //     contentTextColor: "red"
    //     contentUnit:"C degree"

    //     title: "Temperature"
    //     titleColor: "#FAFAFA"
    //     titleSize: 20

    //     backgroundColor: "#1d1d35"
    //     // backgroundColor: "transparent"
    //     progressLineColor:"#00ffc1"
    //     foregroundColor:"#191a2f"
    //     // startAngle: 145

    // }


/*-------------------------video---------------------------------*/

    VideoView{
        id: videoView
        width:  window.width
        height: window.height*0.8


        anchors.top: dashboard.bottom
        anchors.left: dashboard.left
        sourceLink: VideoController.link
        Timer {
            id: checkStatusTimer
            interval: 10000 // Kiểm tra mỗi giây
            repeat: true
            running: false // Timer bắt đầu ở trạng thái tắt
            onTriggered: {
                HttpHandler.sendRequest(); // Gọi request khi Timer kích hoạt
            }
        }
        onMediaStatusChanged:{
            if (videoView.mediastatus === MediaPlayer.InvalidMedia || videoView.mediastatus === MediaPlayer.EndOfMedia){
                // HttpHandler.sendRequest();
                checkStatusTimer.start();
            } else {
                checkStatusTimer.stop();
            }

        }

        // function request() {
        //     const xhr = new XMLHttpRequest()
        //     xhr.onreadystatechange = function() {
        //         if (xhr.readyState === XMLHttpRequest.HEADERS_RECEIVED) {
        //             print('HEADERS_RECEIVED')
        //         } else if(xhr.readyState === XMLHttpRequest.DONE) {
        //             print('DONE')
        //             const response = JSON.parse(xhr.responseText.toString())
        //             // Set JS object as model for listview
        //             sourceLink = response.items
        //         }
        //     }
        //     xhr.open("GET", HttpHandler.api)
        //     xhr.send()
        // }


    }



}
