#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char *ssid = "WK-Guest"; //"Papillon";
const char *password = "Trust4All!"; //"70445312";


static const char CSR[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE REQUEST-----
MIHNMHYCAQAwFDESMBAGA1UEAwwJZGV2aWNlWHl6MFkwEwYHKoZIzj0CAQYIKoZI
zj0DAQcDQgAEb+gl/3/4JsBT/AkNeQag4SzaTCAOWwAE1NYQu6NIa5JA2eBadSeI
5GSwa8XGTRtn/RS4dLp9A6s6Tugtf+8sK6AAMAoGCCqGSM49BAMCA0cAMEQCICkM
PFFQm3yox9vjNwTXmfZNqsVt8KLS/861uAKub1prAiAT418WdAYB7mdgRwOMf7OT
Pbp9Km9+8DM7D0cQh5Zs4w==
-----END CERTIFICATE REQUEST-----
)EOF";


static const char CA_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDtTCCAp2gAwIBAgIQdrEgUnTwhYdGs/gjGvbCwDANBgkqhkiG9w0BAQsFADBt
MQswCQYDVQQGEwJDSDEQMA4GA1UEChMHV0lTZUtleTEiMCAGA1UECxMZT0lTVEUg
Rm91bmRhdGlvbiBFbmRvcnNlZDEoMCYGA1UEAxMfT0lTVEUgV0lTZUtleSBHbG9i
YWwgUm9vdCBHQiBDQTAeFw0xNDEyMDExNTAwMzJaFw0zOTEyMDExNTEwMzFaMG0x
CzAJBgNVBAYTAkNIMRAwDgYDVQQKEwdXSVNlS2V5MSIwIAYDVQQLExlPSVNURSBG
b3VuZGF0aW9uIEVuZG9yc2VkMSgwJgYDVQQDEx9PSVNURSBXSVNlS2V5IEdsb2Jh
bCBSb290IEdCIENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2Be3
HEokKtaXscriHvt9OO+Y9bI5mE4nuBFde9IllIiCFSZqGzG7qFshISvYD06fWvGx
WuR51jIjK+FTzJlFXHtPrby/h0oLS5daqPZI7H17Dc0hBt+eFf1Biki3IPShehtX
1F1Q/7pn2COZH8g/497/b1t3sWtuuMlk9+HKQUYOKXHQuSP8yYFfTvdv37+ErXNk
u7dCjmn21HYdfp2nuFeKUWdy19SouJVUQHMD9ur06/4oQnc/nSMbsrY9gBQHTC5P
99UKFg29ZkM3fiNDecNAhvVMKdqOmq0NpQSHiB6F4+lT1ZvIiwNjeOvgGUpuuy9r
M2RYk61pv48b74JIxwIDAQABo1EwTzALBgNVHQ8EBAMCAYYwDwYDVR0TAQH/BAUw
AwEB/zAdBgNVHQ4EFgQUNQ/INmNe4qPs+TtmFc5RUuORmj0wEAYJKwYBBAGCNxUB
BAMCAQAwDQYJKoZIhvcNAQELBQADggEBAEBM+4eymYGQfp3FsLAmzYh7KzKNbrgh
cViXfa43FK8+5/ea4n32cZiZBKpDdHij40lhPnOMTZTg+XHEthYOU3gf1qKHLwI5
gSk8rxWYITD+KJAAjNHhy/peyP34EEY7onhCkRd0VQreUGdNZtGn//3ZwLWoo4rO
ZvUPQ82nK1d7Y0Zqqi5S2PTt4W2tKZB4SLrhI6qjiey1q5bAtEuiHZeeevJuQHHf
aPFlTc58Bd9TZaml8LGXBHAVRgOY1NK/VLSgWH1Sb9pWJmLU2NuJMW8c8CLC02Ic
Nc1MaRVUGpCY3useX8p3x8uOPUNpnJpY0CQ73xtAln41rYHHTnG6iBM=
-----END CERTIFICATE-----
)EOF";

const char* CERT_FP = "68:7D:B1:4E:6A:FF:C1:DE:23:04:A3:0B:9E:A3:B7:EA:F1:DE:E6:3F";

HTTPClient httpClient;

// WiFiClient espClient;
WiFiClientSecure* WIFIclient;

//
// WIFI SETUP
//
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
 
  // Setup WiFi
  setup_wifi();

  WIFIclient = new WiFiClientSecure;
  WIFIclient->setInsecure();
//  WIFIclient->setCACert(CA_CERT);
//  WIFIclient->setFingerprint(CERT_FP);

//  WIFIclient->connect("wksa-est.certifyiddemo.com", 443);

//    httpClient.begin("https://wksa-est.certifyiddemo.com:443/.well-known/est/simpleenroll");
//  httpClient.begin(*WIFIclient, "https://wksa-est.certifyiddemo.com/.well-known/est/cacerts");
//  httpClient.begin(*WIFIclient,  "wksa-est.certifyiddemo.com", 443, "https://wksa-est.certifyiddemo.com/.well-known/est/cacerts", true);
//  httpClient.begin(*WIFIclient,  "https://trustcenter.certifyiddemo.com");
//  httpClient.begin(*WIFIclient, "https://wksa-est.certifyiddemo.com");
//  httpClient.begin(*WIFIclient, "https://testrfc7030.com:8443/.well-known/est/cacerts");
//  httpClient.begin(*WIFIclient, "https://www.centralbaptistchelmsford.org"); //https://httpbin.org/get");

//  httpClient.setURL("https://wksa-est.certifyiddemo.com/.well-known/est/cacerts");
  httpClient.begin(*WIFIclient,  "wksa-est.certifyiddemo.com", 443, "/.well-known/est/cacerts/"); //, true);

//    httpClient.setAuthorization("90af2231057f4505a92185e134d3fbe4", "d847f238c8e1453cadc1ddf0f6ba491d");
 
//    int statusCode = httpClient.POST((uint8_t*)CSR, 357);
  int statusCode = httpClient.GET();

  if (statusCode > 0) {

    Serial.println("Able to send HTTP request out of the board.");
      
    if(statusCode == HTTP_CODE_OK) {
      Serial.println("Server responded with HTTP status 200.");
      String payload = httpClient.getString();
      Serial.println(payload);
    }
    else {
      Serial.printf("Got HTTP status: %d", statusCode);
      String payload = httpClient.getString();
      Serial.println(payload);
    }
  }
  else {
    Serial.printf("Error occurred while sending HTTP Get: %s\n", httpClient.errorToString(statusCode).c_str());
  }

  // Release the resources used
  httpClient.end();
}
 
void loop() {

  delay(1000);
}