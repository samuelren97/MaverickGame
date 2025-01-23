#include "BME.h"

BME::BME(const uint8_t& p_adresse)
: m_bme(new Adafruit_BME280())
{
    this->m_bme->begin(p_adresse);
}

float BME::temperature() const {
    return this->m_bme->readTemperature();
}

float BME::pression() const {
    return this->m_bme->readPressure() / 100.0F;
}

float BME::altitude() const {
    return this->m_bme->readAltitude(SEA_LEVEL);
}

float BME::humidite() const {
    return this->m_bme->readHumidity();
}