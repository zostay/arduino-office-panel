#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define OFF_AIR     0
#define ON_AIR      1
#define QUIETUDE    2

#define CONNECT_BIT 2
#define AIR_BIT     4
#define QUIET_BIT   5

const char *ssid     = "secret";
const char *password = "secret";

WiFiUDP udp;
unsigned int local_udp_port = 10101;
char incoming[1];
unsigned int signal_state = OFF_AIR;

void setup() {
    pinMode(CONNECT_BIT, OUTPUT);
    pinMode(AIR_BIT, OUTPUT);
    pinMode(QUIET_BIT, OUTPUT);

    Serial.begin(115200);
    delay(100);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    unsigned int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(50);
        Serial.print(".");
        if (++count % 60 == 0) Serial.println();
        digitalWrite(CONNECT_BIT, digitalRead(CONNECT_BIT) == HIGH ? LOW : HIGH);
    }

    Serial.println();
    Serial.println("Wifi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    udp.begin(local_udp_port);

    Serial.printf("Listening to UDP port ");
    Serial.println(local_udp_port);

    digitalWrite(CONNECT_BIT, LOW);

    digitalWrite(AIR_BIT, LOW);
    digitalWrite(QUIET_BIT, LOW);
}

void loop() {
    int packet_size = udp.parsePacket();
    if (packet_size) {
        Serial.print("Received ");
        Serial.print(packet_size);
        Serial.print(" bytes from ");
        Serial.print(udp.remoteIP().toString().c_str());
        Serial.print(" port ");
        Serial.println(udp.remotePort());

        int len = udp.read(incoming, 1);
        if (len > 0) {
            signal_state = incoming[0];

            if (signal_state == 1) {
                digitalWrite(AIR_BIT, HIGH);
                digitalWrite(QUIET_BIT, LOW);
                Serial.println("4 -> HIGH, 5 -> LOW");
            }
            else if (signal_state == 2) {
                digitalWrite(AIR_BIT, HIGH);
                digitalWrite(QUIET_BIT, HIGH);
                Serial.println("4 -> HIGH, 5 -> HIGH");
            }
            else {
                digitalWrite(AIR_BIT, LOW);
                digitalWrite(QUIET_BIT, LOW);
                Serial.println("4 -> LOW, 5 -> LOW");
                signal_state = 0;
            }

            digitalWrite(CONNECT_BIT, HIGH);
            delay(500);
            digitalWrite(CONNECT_BIT, LOW);

            Serial.print("Signal state set to ");
            Serial.println(signal_state);
        }
    }
}

// vim: ai si ci
