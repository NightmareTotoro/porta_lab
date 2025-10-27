#pragma once

#ifndef WIFISERVICE_H
#define WIFISERVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include "Logger.h"

class WiFiService {
public:
    static void connect();
};

#endif // WIFISERVICE_H