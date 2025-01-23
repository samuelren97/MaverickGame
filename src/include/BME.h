#pragma once

#include <Arduino.h>
#include <Adafruit_BME280.h>

class BME {
public:
    BME(const uint8_t& p_adresse);
    float temperature() const;
    float pression() const;
    float altitude() const;
    float humidite() const;
private:
    const uint16_t SEA_LEVEL = 1013.25;
    Adafruit_BME280* m_bme;
};