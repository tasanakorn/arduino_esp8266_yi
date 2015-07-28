#ifndef Yi_h
#define Yi_h

#include <Arduino.h>
#include <Client.h>
#include <ArduinoJson.h>

class YiClient {
private:
    Client *_client;
    uint16_t _camera_port;
    String _camera_hostname;
    int _token = 0;
    bool _is_begin = false;
    int _last_msg_id = 0;
    bool _processing = false;
    int _processing_start_time;
    int _battery_level = 0;
public:
    YiClient(Client& c, String hostname, uint16_t port = 7878);
    bool begin();
    bool connected();
    bool logon();
    bool sendCommand(int msg_id);
    bool sendCommand(int msg_id, const char * param);
    bool createSession();
    bool getSetting(const char * param);
    bool getBatteryLevel();
    bool takePhoto();
    bool startRecording();
    bool stopRecording();
    bool loop();
};

#endif