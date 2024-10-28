import QtQuick
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Controls
Item {
    property string sourceLink
    property int mediastatus: player.mediaStatus
    signal mediaStatusChanged(int status)
    Rectangle{
        id: videoWindow
        color: "black"
        anchors.fill: parent
        MediaPlayer {
            id: player

            source: sourceLink
            videoOutput: videoOutput
            // audioOutput: AudioOutput {}
            autoPlay: true
            loops: MediaPlayer.Infinite
            // bufferSize: 50000
            // onErrorOccurred: error => console.log('Error occurred:', errorString, error)
            onMediaStatusChanged: {
                console.log("Media status changed:", player.mediaStatus);
                // mediaStatusChanged(player.mediaStatus);

                // console.log("Buffer progress: ", player.bufferProgress);
                if (mediaStatus === MediaPlayer.Loaded) {
                   videoOutput.source = player;
                   player.play();
               }
            }

            onSourceChanged: {
                player.play();
            }


        }
        VideoOutput{
            id: videoOutput
            anchors.fill: parent
        }
    }
    Component.onCompleted: {
        player.play();
        player.mediaStatusChanged.connect(mediaStatusChanged)

    }


}
