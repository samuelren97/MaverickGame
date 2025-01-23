#include "ActionReset.h"

ActionReset::ActionReset(WiFiManager* p_wifiManager)
    : m_wifiManager(p_wifiManager)
{
}

void ActionReset::executer()
{
    this->m_wifiManager->resetSettings();
    ESP.restart();
}
