#pragma once

#include <Arduino.h>

#include "Action.h"
#include "WiFiManager.h"

class ActionReset: public Action {
public:
    ActionReset(WiFiManager* p_wifiManager);

    virtual void executer();

private:
    WiFiManager* m_wifiManager;
};