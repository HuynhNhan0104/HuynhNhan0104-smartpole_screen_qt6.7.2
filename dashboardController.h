#ifndef DASHBOARDCONTROLLER_H
#define DASHBOARDCONTROLLER_H
#include <qqml.h>
#include <QObject>
#include <QTimer>
#include<QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>


#include "serial/serial_interface.h"
#include "serial/serial_port.h"
#include "serial/rs485.h"

#include <QDebug>
class DashboardController: public QObject
{
    Q_OBJECT
    // Q_PROPERTY(float Value READ getValue WRITE setValue NOTIFY ValueChanged)


    Q_PROPERTY(float pm25Value READ getPm25Value WRITE setPm25Value NOTIFY pm25ValueChanged )
    Q_PROPERTY(float pm10Value READ getPm10Value WRITE setPm10Value NOTIFY pm10ValueChanged )
    Q_PROPERTY(float temparatureValue READ getTemparatureValue WRITE setTemparatureValue NOTIFY temparatureValueChanged )
    Q_PROPERTY(float humidityValue READ getHumidityValue WRITE setHumidityValue NOTIFY humidityValueChanged )
    Q_PROPERTY(float noiseValue READ getNoiseValue WRITE setNoiseValue NOTIFY noiseValueChanged )
    Q_PROPERTY(float atmValue READ getAtmValue WRITE setAtmValue NOTIFY atmValueChanged)
    Q_PROPERTY(float lightValue READ getLightValue WRITE setLightValue NOTIFY lightValueChanged)



    // uncomment bottom line if you using c++object to your qml
    // QML_ELEMENT
public:

    enum MODE{
        TEST = 0,
        RUN
    };

    explicit DashboardController(QObject *parent = nullptr, const char* port = "/dev/ttyUSB0", int baudrate = 9600, int mode = TEST);
    ~DashboardController();


    float getPm25Value() const;
    void setPm25Value(float newPm25Value);

    float getPm10Value() const;
    void setPm10Value(float newPm10Value);

    float getTemparatureValue() const;
    void setTemparatureValue(float newTemparatureValue);

    float getHumidityValue() const;
    void setHumidityValue(float newHumidityValue);

    float getNoiseValue() const;
    void setNoiseValue(float newNoiseValue);

    float getAtmValue() const;
    void setAtmValue(float newAtmValue);

    void updateValue();
    void updateData();


    float getLightValue() const;
    void setLightValue(float newLightValue);

    QByteArray exportDataToJsonObject();

signals:
    void pm25ValueChanged();
    void pm10ValueChanged();
    void temparatureValueChanged();
    void humidityValueChanged();
    void noiseValueChanged();
    void atmValueChanged();
    void lightValueChanged();
    void publishDataToTopic(const QByteArray &message);

private:

    QTimer *timer ;
    Serial_Port* serialPort = nullptr;
    Serial_Interface* serialInterface = nullptr;
    ODR_Interface* sensor = nullptr;


    float m_pm25Value = 0;
    float m_pm10Value = 0 ;
    float m_temparatureValue = 0;
    float m_humidityValue = 0;
    float m_noiseValue = 0;
    float m_atmValue = 0;

    float m_lightValue;
};
#endif // DASHBOARDCONTROLLER_H
