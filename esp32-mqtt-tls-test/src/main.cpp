#include <Arduino.h>

// The following define sets the compiler for ESP8266 or for ESP32
// But it seems that this is not really required
//#define ESP32
//#define ESP8266

#ifdef ESP32
#include <SPIFFS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#endif

//
// GLOBAL VARIABLES
//

String ssid;
String password;
String brokerSNI;
String devID;
String devPubTopic;
String devSubTopic;

char strDeviceCert[2048];
char strDeviceKey[2048];
char strCACert[2048];

// bool EC_crypto = false;

#ifdef ESP8266
X509List* x509CACert;
X509List* x509DeviceCert;
PrivateKey* deviceKey;
#endif

// WiFiClient espClient;
WiFiClientSecure WIFIclient;

// PubSubClient client(espClient);
PubSubClient client(WIFIclient);

//
// WIFI SETUP
//
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.println(asctime(&timeinfo));
}

//
// SUBSCRIBE CALLBACK
// Print any message received for subscribed topic
//
void callback(char* topic, byte* payload, unsigned int length) {
  String sTopic = topic;
  String sCommand = sTopic.substring(sTopic.lastIndexOf("/") + 1);

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String sPayload = "";
  for (unsigned int i=0; i < length; i++) {
    sPayload += (char)payload[i];
  }
  Serial.println(sPayload);

  Serial.println("Command: " + sCommand);
}

//
// SETUP MQTT CONNECTION
//
void setMQTT()
{

// Load CA cert into trust store
#ifdef ESP8266  
  WIFIclient.setTrustAnchors(x509CACert);
#endif

#ifdef ESP32
  WIFIclient.setCACert(strCACert);
#endif

//  Enable self-signed cert support
  //  WIFIclient.allowSelfSignedCerts();

// Optional: Skip CA Validation
//    WIFIclient.setInsecure();

// Set Client certificate
#ifdef ESP32
  WIFIclient.setCertificate(strDeviceCert);
  WIFIclient.setPrivateKey(strDeviceKey);
#endif

#ifdef ESP8266
  if (deviceKey->isEC()) {
    WIFIclient.setClientECCert(x509DeviceCert, deviceKey, BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN, BR_TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256);
  } else {
    WIFIclient.setClientRSACert(x509DeviceCert, deviceKey);
  }
#endif

// Optional: Broker certificate validation
  //  WIFIclient.setFingerprint(BROKER_FINGERPRINT);

  //connect to MQTT server
  client.setServer(brokerSNI.c_str(), 8883);
// Alternative for non-TLS connections
  //  client.setServer(mqtt_server, 1883);

// Configure SUB callback
  client.setCallback(callback);
}

//
// RECONNECT TO BROKER
//
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(devID.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(devPubTopic.c_str(),"{\"status\":\"Connected!\"}");
      // ... and resubscribe
      client.subscribe(devSubTopic.c_str());
  client.setCallback(callback);
      Serial.println("Subscribed to " + devSubTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//
// Manage SPIFFS Files
//

bool loadConfig()
{
  String buf;
  File file;

  // Mount file system.
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return false;
  }

  file = SPIFFS.open("/device.cfg");
  if(!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }  
  
  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  brokerSNI = file.readStringUntil('\n');
  devID = file.readStringUntil('\n');
  devPubTopic = file.readStringUntil('\n');
  devSubTopic = file.readStringUntil('\n');

  file.close();

  Serial.println("Configuration:");
  Serial.println("+ WiFi SSID: " + ssid);
  Serial.println("+ WiFi Password: " + password);
  Serial.println("+ MQTT Broker SNI: " + brokerSNI);
  Serial.println("+ Device ID: " + devID);
  Serial.println("+ Device Publish Topic: " + devPubTopic);
  Serial.println("+ Device Subscribe Topic: " + devSubTopic);

  return true;
}

/*
bool createTLSFiles() 
{
  File file;

  // Mount file system.
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return false;
  }

  // create certificate file
  file = SPIFFS.open("/device.crt", "w");
  if (!file) {
    Serial.println("File '/device.crt' open failed.");
    return false;
  } else {
    file.print(DEVICE_CERT);
    file.close();
  }

  // create key file
  file = SPIFFS.open("/device.key", "w");
  if (!file) {
    Serial.println("File '/device.key' open failed.");
    return false;
  } else {
    file.print(DEVICE_PRIVATE);
    file.close();
  }

  // create ca file
  file = SPIFFS.open("/ca.crt", "w");
  if (!file) {
    Serial.println("File '/ca.crt' open failed.");
    return false;
  } else {
    file.print(CA_CERT);
    file.close();
  }

  return true;
}
*/

bool loadTLSFiles() 
{
  File file;
  int i;
  char c;

  if(!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }

  // Load device certificate
  file = SPIFFS.open("/device.crt", "r");
  if(!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }  
  i = 0;
  while(file.available()){
    c = file.read();
    strDeviceCert[i++] = c;
  }
  strDeviceCert[i] = 0;
  file.close();
  Serial.println("Device certificate:");
  Serial.println(strDeviceCert);

#ifdef ESP8266
  x509DeviceCert = new X509List(strDeviceCert);
#endif

  // Load device key  
  file = SPIFFS.open("/device.key", "r");
  if(!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }
  i = 0;
  while(file.available()){
    c = file.read();
    strDeviceKey[i++] = c;
  }
  strDeviceKey[i] = 0;
  file.close();
  Serial.println("Device key:");
  Serial.println(strDeviceKey);

#ifdef ESP8266
  deviceKey = new PrivateKey(strDeviceKey);
#endif

  // Load CA certificate  
  file = SPIFFS.open("/ca.crt", "r");
  if(!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }
  i = 0;
  while(file.available()){
    c = file.read();
    strCACert[i++] = c;
  }
  strCACert[i] = 0;
  file.close();
  Serial.println("CA certificate:");
  Serial.println(strCACert);

#ifdef ESP8266
  x509CACert = new X509List(strCACert);
#endif

  return true;
}


//
// BOARD SETUP
//
void setup() {
  // Default settings
  Serial.begin(115200);

  // Load config from SPIFFS
  loadConfig();

  // Setup WiFi
  setup_wifi();

  // Set time
  setClock();

  // Set MQTT Connection
//  createTLSFiles();
  loadTLSFiles();
  setMQTT();

}

// 
// MAIN PROCESS LOOP
//
void loop() {
  static long lastMsg = 0;
  String payload;
  float temperature = 0;
  float humidity = 0;

  if (!client.connected()) {
    Serial.println("Reconnect to broker");
    reconnect();
  } else {
    client.loop();
    // Only publish in 5 sec. intervals
    long now = millis();
    if (now - lastMsg > 5000) {
      lastMsg = now;
      
      temperature = 123.45f;
      humidity = 56.78f;

      payload = "{\"temperature\":" + String(temperature) + ", \"humidity\":" + String(humidity) + "}";
      client.publish(devPubTopic.c_str(), (char*) payload.c_str());
      Serial.println("PUBLISH!");
    }
  }
}