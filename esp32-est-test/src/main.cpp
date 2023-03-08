#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char *ssid = "Papillon";//"WK-Guest"; //"Papillon";
const char *password = "70445312";//"Trust4All!"; //"70445312";

static const char CSR[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE REQUEST-----
MIH1MIGcAgEAMDoxFzAVBgNVBAMMDmF3cy1kZXZpY2UtMDA0MQ0wCwYDVQQFEwQw
MDA0MRAwDgYDVQQKDAdXSVNlS2V5MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE
jvM8FRm+CBod/lT09JbkBGSXn1Ipadib00TPtCbDdaUwPlny+GPE8nO/QFiyrW5Z
MvEJNHQPgigWo+rRnKAelqAAMAoGCCqGSM49BAMCA0gAMEUCIQDiYGbcwIRsdj/L
Ssrwtf2WEFmQZKVjJxGRtQTqXlj1GgIgMtwmyXS+RuWZt9b+IAGYcjLER4A/GXz/
TEuNL6GXV2c=
-----END CERTIFICATE REQUEST-----)EOF";

//static const uint8_t CSR[] PROGMEM = R"EOF(-----BEGIN CERTIFICATE REQUEST-----MIHNMHYCAQAwFDESMBAGA1UEAwwJZGV2aWNlWHl6MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEb+gl/3/4JsBT/AkNeQag4SzaTCAOWwAE1NYQu6NIa5JA2eBadSeI5GSwa8XGTRtn/RS4dLp9A6s6Tugtf+8sK6AAMAoGCCqGSM49BAMCA0cAMEQCICkMPFFQm3yox9vjNwTXmfZNqsVt8KLS/861uAKub1prAiAT418WdAYB7mdgRwOMf7OTPbp9Km9+8DM7D0cQh5Zs4w==-----END CERTIFICATE REQUEST-----)EOF";

//static const uint8_t CSR[] = "MIHNMHYCAQAwFDESMBAGA1UEAwwJZGV2aWNlWHl6MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEb+gl/3/4JsBT/AkNeQag4SzaTCAOWwAE1NYQu6NIa5JA2eBadSeI5GSwa8XGTRtn/RS4dLp9A6s6Tugtf+8sK6AAMAoGCCqGSM49BAMCA0cAMEQCICkMPFFQm3yox9vjNwTXmfZNqsVt8KLS/861uAKub1prAiAT418WdAYB7mdgRwOMf7OTPbp9Km9+8DM7D0cQh5Zs4w==";

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

static const char CLI_CERT[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAOYloe5cvDoZ1RzCJDfqq0tRHrlCMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzAyMTkwODI2
MzVaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDGh02tCF7Eci95e/q0
2XF3r+vGAxZNKxKnRD6XnpGpTVkQEQrDsbm6EEaFFzaOW6gG0gOmZiWSSUdqzC9T
JVfaSWZ6pYCVS/NTWP5l14OrgkG5zOObxc1lwZTPeJlHJjrwphkpdYMJpub/kDyF
uMer7EnVXSYHSpd3ZOXkRpqU2eNXfjVESDH3Szvw814phXTrTUbkj0k8GPtjNrTo
tkDTBoeft2UVYcaeKnF6/O7vaLqjxNxeUYpE55lA34uObA8XXl29menq2z4qY9qB
hMBcsHc7BVPxIy8PrRbaaAOuhv1pySs6Hy9AO4OQczV3/I6u/B6oq4zC3S8gFkzf
s4tRAgMBAAGjYDBeMB8GA1UdIwQYMBaAFKJULs07R5ECXdrFrJ0DTdkstW1pMB0G
A1UdDgQWBBTn/dAZBOBYVJrG9NmIlv8Mje2EnjAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAOucR7r1jtshgGBOcK2OsGATm
wq0YtlOZRyM2pxrXi+0IUM3aME3ezTj/lokZqOB8y85nI7r5+c30+jgP7O4zJibe
QKiuJNSHgnPcXHZZ56ogIrfvsFDFuF5yOr/IMp4ps48SNjMHWhHdLo289lkmyq2v
OybG5AUhRZCAKLGn2KvJcoD96RE8C75lDmx5nHgiY6g/K+u+QKDO8cUxeZElXcjf
P8pW0a2u2XIH82SLAOQ8jABTwfVYGKJaKFjUufTJLtC2k/bmR+U7yEll6OkPY6Ia
4QxRLQNp58grx+M0d46IGURk4QfnK8hBZI5HJs5xwFwPiuYXyqtg4I13DkDFBw==
-----END CERTIFICATE-----
)EOF";

static const char CLI_KEY[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAxodNrQhexHIveXv6tNlxd6/rxgMWTSsSp0Q+l56RqU1ZEBEK
w7G5uhBGhRc2jluoBtIDpmYlkklHaswvUyVX2klmeqWAlUvzU1j+ZdeDq4JBuczj
m8XNZcGUz3iZRyY68KYZKXWDCabm/5A8hbjHq+xJ1V0mB0qXd2Tl5EaalNnjV341
REgx90s78PNeKYV0601G5I9JPBj7Yza06LZA0waHn7dlFWHGnipxevzu72i6o8Tc
XlGKROeZQN+LjmwPF15dvZnp6ts+KmPagYTAXLB3OwVT8SMvD60W2mgDrob9ackr
Oh8vQDuDkHM1d/yOrvweqKuMwt0vIBZM37OLUQIDAQABAoIBAERmkciwPZaJkPnW
UYbF9GY8YouSvLhRwteGl9o9dApU7/6tIHxUW51TxjhMyys5QSc4bLGEOCWywbx2
JUgMcbxv1bMkGq1VwbxRyTTlkCQjeOm/QhyFhkMs80yYTJdLALuxPrIeHsvQiX47
/95Cz+jYPIga0+NnO9CTHvILsW7fF3v3TwzrtKP35nt66+v2+/E8EOOwALBax9wl
jpKztfgqrpzbpeVuLJi/pl6l8tIaRva2cAreyYe+fVRmXaOoJ45PI1KvxvL3uw1l
5Vdp+KInpfkCGZ+l5Xm+8oHDaXuh+fqMzKFzJ/7eMRMTQO8MvLQZ+fWVtm9Xezcd
mqxfDFECgYEA9/FEACrFJtNHAuNKdRLCzOrJVtjSvO63fAZ8CKnU3oL9NeYajWen
iNhLZdOpw7kqsfP1fSfDh57Q8Pk/UMPx8npFGOG34CjOY9WiKgUmEhghkVJ7R8OI
PhoZp0qrhfKmBCJeYT4UEUBJZrkt4pLXU7zN/cfk37dKsNQhzxmlxvcCgYEAzPrx
ejXTBeJAyFITq74/9YCg8ADXqbrzv5b82lFyrWnVRXxdXebpsxrx4C6wzLPCIEHp
U399ltLNocMSWiZtGvQA/QEI4miXAtDStbtHJMP89fRriulEt+nHuKhLTnHfuyTI
oI37Uaz53j5khkAZK4R+a3AtE2NNcR2kvvbgRfcCgYBqJIcP6z9rV5dWzSba56Ut
RHXdEAVnjGpQVhZTPyE6AKmVoWRMlMtL8gyVEv/iIir884tsgNFcBck83/cTBE9X
lrBt7nNhKxbv9kzt4DZkGKCGLeFI35Q7wcCptXWoajBYJL5ysri0iejiaPQTwvkb
iABHXIPGohj/ilDO8BPgEQKBgH+t1/hdyoIhumSM63/Q05oicxzMCYnJz8aJUBrV
HO9zusq7epThHrTXlc4khzAa0FKze5jtEtd9m2SlU6sfwFpHns35dQVCTL4OQRG+
A8Sc59Z0VwCB6LH+Rs941N+aAthZQzbu+ZqYelO63XbaOlNnBi/6AbnP4w8QfewN
bwllAoGADTAdtA2tE4/KllPdHwSu1ODY/xY520WYB6ZH+ULYP3c28FJdOxoskFLs
v5H1U6wD4vABrtnRTRwOfrgLR41vZS9UwhrljKXdTANupqjXa8k0hnGVghacCOXB
I8q0+Mplces3YoWazw63CWe4PXq+jauJVRVKGtJpqDN/OjtAicQ=
-----END RSA PRIVATE KEY-----
)EOF";

const char* CERT_FP = "68:7D:B1:4E:6A:FF:C1:DE:23:04:A3:0B:9E:A3:B7:EA:F1:DE:E6:3F";


// HTTP POST Request formats
//  httpClient.begin(*WIFIclient, "https://wksa-est.certifyiddemo.com/.well-known/est/cacerts");
//  httpClient.begin("https://wksa-est.certifyiddemo.com/.well-known/est/cacerts");
//  httpClient.begin(*WIFIclient,  "wksa-est.certifyiddemo.com", 443, "/.well-known/est/cacerts/", true);
//  httpClient.begin("wksa-est.certifyiddemo.com", 443, "/.well-known/est/cacerts", CA_CERT, CLI_CERT, CLI_KEY);

bool estSimpleEnroll(String strCsr, String &strCert)
{
  HTTPClient httpClient;
  WiFiClientSecure* WIFIclient;
  String payload;
  int statusCode;
  bool requestResult = false;

  WIFIclient = new WiFiClientSecure;
  WIFIclient->setInsecure();
//  WIFIclient->setCACert(CA_CERT);

  Serial.println("POST DATA:");
  Serial.println(strCsr);
  Serial.println("POST LENGTH:");
  Serial.println(strCsr.length());  

//  httpClient.begin("https://wksa-est.certifyiddemo.com/.well-known/est/simpleenroll");
  httpClient.begin(*WIFIclient,  "wksa-est.certifyiddemo.com", 443, "/.well-known/est/simpleenroll/", true);

//  httpClient.setAuthorization("90af2231057f4505a92185e134d3fbe4", "d847f238c8e1453cadc1ddf0f6ba491d");
//  httpClient.addHeader("Content-Type", "application/raw");
//  httpClient.addHeader("Host", "wksa-est.certifyiddemo.com");
//  httpClient.addHeader("Content-Length", "284");

  statusCode = httpClient.POST((uint8_t *)CSR, sizeof(CSR) - 1);

  if (statusCode > 0) {
    requestResult = (statusCode == HTTP_CODE_OK);
    Serial.printf("Got HTTP status: %d\n", statusCode);
    payload = httpClient.getString();
    Serial.println("Payload received:");
    Serial.println(payload);
  }
  else {
    Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
    requestResult = false;
  }

  // Release the resources used
  httpClient.end();
  delete WIFIclient;

  if (!requestResult)
    payload = "";

  strCert = payload;
  return requestResult;
}

bool ConvertP7toPEM(String &str)
{
  HTTPClient httpClient;
  WiFiClient* WIFIclient;
  String payload;
  int statusCode, i;
  bool requestResult = false;

  WIFIclient = new WiFiClient;
//  WIFIclient->setInsecure();
//  WIFIclient->setCACert(CA_CERT);

  payload = str;

  // Clean the input
  payload.trim();
  while((i = payload.indexOf('\n')) > 0) {
    payload.remove(i, 1);
  }
  while((i = payload.indexOf('\r')) > 0) {
    payload.remove(i, 1);
  }

  Serial.println("POST DATA:");
  Serial.println(payload);
  Serial.println("POST LENGTH:");
  Serial.println(payload.length());  

//  httpClient.begin("http://hotline.retrobyt.es:1880/opensslp7");
  httpClient.begin(*WIFIclient,  "hotline.retrobyt.es", 1880, "/opensslp7/", false);

  statusCode = httpClient.POST((uint8_t *)payload.c_str(), payload.length());

  if (statusCode > 0) {
    requestResult = (statusCode == HTTP_CODE_OK);
    Serial.printf("Got HTTP status: %d\n", statusCode);
    payload = httpClient.getString();
    Serial.println("Payload received:");
    Serial.println(payload);
  }
  else {
    Serial.printf("Error occurred while sending HTTP POST: %s\n", httpClient.errorToString(statusCode).c_str());
    requestResult = false;
  }

  // Release the resources used
  httpClient.end();
  delete WIFIclient;

  if (requestResult)
    str = payload;

  return requestResult;

}

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


void setup() {
  String payload;
  int statusCode;

  Serial.begin(115200);
 
  // Setup WiFi
  setup_wifi();
  setClock();


  if (estSimpleEnroll(CSR, payload)) {
    if (!ConvertP7toPEM(payload))
      Serial.println("Error converting P7 to PEM");    
  } else
    Serial.println("EST Request failed");

}
 
void loop() {

  delay(1000);
}