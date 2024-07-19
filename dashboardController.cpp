#include "dashboardController.h"

DashboardController::DashboardController(QObject *parent,const char* port, int baudrate):
    QObject( parent )
{
    // allocate mem for property (need to deallocate at the destructor)
    serialPort = new Serial_Port(port,baudrate);
    sensor = new ODR_Interface(0x15);
    serialInterface = new Serial_Interface(serialPort, sensor);
    timer = new QTimer(this);

    // serialPort->start();
    // serialInterface->start();
    // load the lastest data
    // updateData();
    // updateValue();

    connect(timer, &QTimer::timeout, this, &DashboardController::updateValue); //just for testing ui
    // connect(timer, &QTimer::timeout, this, &DashboardController::updateData);
    timer->start(5000); // 10000 milliseconds = 10 seconds
}
DashboardController::~DashboardController(){
    timer->stop();
    // serialPort->stop();
    // serialInterface->stop();
    delete timer;
    delete serialPort;
    delete serialInterface;
    delete sensor;
}

// Get data form sensor and update to GUI

void DashboardController::updateData(){
    setTemparatureValue(sensor->getValue(ODR_Interface::TEMP));
    setHumidityValue(sensor->getValue(ODR_Interface::HUMI));
    setPm25Value(sensor->getValue(ODR_Interface::PM25));
    setPm10Value(sensor->getValue(ODR_Interface::PM10));
    setNoiseValue(sensor->getValue(ODR_Interface::NOISE));
    setAtmValue(sensor->getValue(ODR_Interface::ATM));
}
// Just for testing GUI
void DashboardController::updateValue(){
    setTemparatureValue(QRandomGenerator::global()->bounded(0, 101));
    setHumidityValue(QRandomGenerator::global()->bounded(0, 101));
    setPm25Value(QRandomGenerator::global()->bounded(0, 101));
    setPm10Value(QRandomGenerator::global()->bounded(0, 101));
    setNoiseValue(QRandomGenerator::global()->bounded(0, 101));
    setAtmValue(QRandomGenerator::global()->bounded(0, 101));
}

/*
 * setter and getter for PM2.5
 *
*/


float DashboardController::getPm25Value() const
{
    return m_pm25Value;
}

void DashboardController::setPm25Value(float newPm25Value)
{
    if (qFuzzyCompare(m_pm25Value, newPm25Value))
        return;
    m_pm25Value = newPm25Value;
    emit pm25ValueChanged();
}
/*
 * setter and getter for PM10
 *
*/

float DashboardController::getPm10Value() const
{
    return m_pm10Value;
}

void DashboardController::setPm10Value(float newPm10Value)
{
    if (qFuzzyCompare(m_pm10Value, newPm10Value))
        return;
    m_pm10Value = newPm10Value;
    emit pm10ValueChanged();
}
/*
 * setter and getter for temperature
 *
*/

float DashboardController::getTemparatureValue() const
{
    return m_temparatureValue;
}

void DashboardController::setTemparatureValue(float newTemparatureValue)
{
    if (qFuzzyCompare(m_temparatureValue, newTemparatureValue))
        return;
    m_temparatureValue = newTemparatureValue;
    emit temparatureValueChanged();
}

/*
 * setter and getter for Humidity
 *
*/

float DashboardController::getHumidityValue() const
{
    return m_humidityValue;
}

void DashboardController::setHumidityValue(float newHumidityValue)
{
    if (qFuzzyCompare(m_humidityValue, newHumidityValue))
        return;
    m_humidityValue = newHumidityValue;
    emit humidityValueChanged();
}


/*
 * setter and getter for Noise
 *
*/

float DashboardController::getNoiseValue() const
{
    return m_noiseValue;
}

void DashboardController::setNoiseValue(float newNoiseValue)
{
    if (qFuzzyCompare(m_noiseValue, newNoiseValue))
        return;
    m_noiseValue = newNoiseValue;
    emit noiseValueChanged();
}
/*
 * setter and getter for atmosphers
 *
*/

float DashboardController::getAtmValue() const
{
    return m_atmValue;
}

void DashboardController::setAtmValue(float newAtmValue)
{
    if (qFuzzyCompare(m_atmValue, newAtmValue))
        return;
    m_atmValue = newAtmValue;
    emit atmValueChanged();
}

/*
 * setter and getter for Light insensitive
 *
*/



float DashboardController::getLightValue() const
{
    return m_lightValue;
}

void DashboardController::setLightValue(float newLightValue)
{
    if (qFuzzyCompare(m_lightValue, newLightValue))
        return;
    m_lightValue = newLightValue;
    emit LightValueChanged();
}