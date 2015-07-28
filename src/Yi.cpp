#include "Yi.h"

YiClient::YiClient(Client& c, String hostname, uint16_t port) :
_client(&c),
_camera_port(port),
_camera_hostname(hostname) {
}

bool YiClient::begin() {
    int result = 0;

    _is_begin = true;
    _token = 0;
    _processing = false;
    result = _client->connect(_camera_hostname.c_str(), _camera_port);

    if (!result) {
        _client->stop();
        return false;
    }
    return true;
}

bool YiClient::connected() {
    bool rc = _client->connected();
    if (!rc) {
        _token = 0;
        _client->stop();
    }

    return rc;
}

bool YiClient::loop() {
    if (!connected()) {
        return false;
    }

    if (!logon() && !_processing) {
        Serial.println("Create session");
        createSession();
    }

    // TODO: Checking processing timeout

    if (_client->available()) {
        StaticJsonBuffer<1000> jsonBuffer;
        while (_client->available()) {
            String line = _client->readStringUntil('\r');
            Serial.print("RECV : ");
            Serial.println(line);
            JsonObject& root = jsonBuffer.parseObject((char*) line.c_str());
            if (!root.success()) {
                Serial.println("parseObject() failed");
            } else {
                _processing = false;
                int rval = root["rval"];
                int msg_id = root["msg_id"];
                if (rval < 0) {
                    Serial.println("Command error");
                } else {
                    switch (msg_id) {
                        case 7: // 
                        case 13: // 
                            if (strcmp(root["type"], "battery") == 0 || 
                                strcmp(root["type"], "adapter") == 0) {
                                _battery_level = atol((const char *)root["param"]); 
                                Serial.print("BATTERY : ");
                                Serial.println(_battery_level);
                            }
                            break;
                        case 257: // AUTH
                            if (root["rval"] == 0) {
                                _token = root["param"];
                                Serial.println("Session created");
                                break;
                            }

                    }
                }

            }
        }
    }
}

bool YiClient::logon() {
    return _token != 0;
}

bool YiClient::sendCommand(int msg_id) { 
    return sendCommand(msg_id, NULL);
}

bool YiClient::sendCommand(int msg_id, const char * param) {
    _processing = true;
    _processing_start_time = millis();
    _last_msg_id = msg_id;
    String cmd = "";
    if (param == NULL) {
        cmd = String("") + 
                "{\"msg_id\":" + msg_id + 
                ",\"token\":" + _token + "}";
    } else {
        cmd = String("") + 
                "{\"msg_id\":" + msg_id + 
                ",\"token\":" + _token + 
                ", \"param\":\"" + param + "\"}";
    }
    
    Serial.print("SEND : ");
    Serial.println(cmd);
    _client->print(cmd);
    return true;
}

bool YiClient::createSession() {
    return sendCommand(257);
}

bool YiClient::getSetting(const char * type) {
    return sendCommand(3, type);
}

bool YiClient::getBatteryLevel() {
    return sendCommand(13);
}

bool YiClient::takePhoto() {
    return sendCommand(769);
}

bool YiClient::startRecording() {
    return sendCommand(513);
}

bool YiClient::stopRecording() {
    return sendCommand(514);
}
