import QtQuick
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Controls
Item {
    property string sourceLink: "https://video-weaver.hkg06.hls.ttvnw.net/v1/playlist/CvgHsIJLjN9EsXPYL9ACLfDIfmNVG79T2jdHKQMTzcluxSwV7bfYIKhYMf57bgU6U_PQgG-JD5wLNTjf2HAl6dtLGvoROgcb5mTuRMT4WFu2tR4zGN1WMdAA8oqu9saf0CIqVF0ViBBTBkc3Ux0yh2Mm9KbxO_E5t-oWmMWZlQ5MGdtXnX_rdBlsGsIm-0pksPf6s6QrFSID_5fbDTmzZRlcQqeH8PB1sDty4Qj4o7Gm5q3OI1unEDPAMWtBMOZpFGrSrBD2qIdqvI6tgBvIH4dhpbM14qiD27xVf35o58tfbWj0jyDVNRYOsSOowVP5VZxVS1CH7WByjyhD9A4Ka1RNYbuHy-ntMYQ0H1Vkpjq_2qJpHBrIxzueDb3zK7ufkCy1Fx05XsrT8X9lF8akF1N7xoXedxFZSaPcS4xkemUwW928F8mXdYyz97l3jqE8LG0MsGtgspyX0t7fOZY0lbxGTSO-JcgKYtIFrQCj7v14jJnoak2pqYuMF6qxXY_rAfyhHVCiz0pyz2glYdEUooqeSrPt9EU58j2EJ7VdlNXmqrEgGaa6oy5ojdTP8iMjxHoAooC9lMKMRtzvFwryYvz6lMxDbPYfLB5qFE-VNd_T8xYwnYVzLaXa8q7HVMwgO38yqyse7zPs1T7xiqcvonsNiS67w2BZnzxnuEUvVbDkIYi6por8YmlVtYipfhZIb3v8aO5WadNjySX-fD6LeneIgTUqIgPQsAX8vM_nWkvVuDp34lGIM7usDhJRC63qWFKYXhnsNV4b1o205PKcP1EdWUo2XvoKOIm5kyB6-Lw0cKnNtlkxBHj1czi-u5uAn2olK17XdQm93rr-1Kz-ro7R_8O5rpncWva5MkXTvJ5ZPdtjNpdR5H6JtswKeZPvp2s3XgOGryBFHY189WKzrWkBq3vxa1c6ncUzQFTXUUSz6SkwKSzAiskmcYj0Llv2uUJuwWxnR4_K9GXBDYMXllFh89odlyUCR9CvOaJOLzsbFj0e2UWSVL4cUkNceuLWlqI25p-BBovdpcQm5iW6fxUmmSXJ8nLyHjTgLV9NLMAYR1Ogi9bY81K-G0h1VOsHlSjOmIbbQm7W07HRsrkOSnySXi9xyLbO5vI1dvf3_7PNKWRmNFMmonGLdB1x_tBFOFUGqoxcBshoxxDpS3tsq5CLKYfPMpCt_XpGOUr3RPetzM831j7SdyIlnk6VKrb_oTVTWBxQn31HFL7DqJUFI8eiy8bMAHc58gR3d5RxMf_uJ5ndC21KgdXveN_YI-gZCxWFWpAodpMTwk6M2aLcLHbCnmeJPKMJ8DLtN_FiTLuXfdw-txDj3trTf9CxmxZGUsAJsmdr_7OBZfQaDGvVDnPCIiVV6tkYOiABKgl1cy13ZXN0LTIw9wk.m3u8"
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
                console.log("Buffer progress: ", player.bufferProgress);
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

    }


}
