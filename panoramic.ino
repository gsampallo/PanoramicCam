/*
 * Take panoramics photos with ESP32-Cam and Python
 * Change WIFI SSID and WIFIPASS for your own. You need to know what is the IP address that the module takes.
 * 
 * Github: https://github.com/gsampallo/PanoramicCam
 * Blog: www.gsampallo.com/blog/
 * Twitter @gsampallo
 * 
 */
#include <esp32cam.h>
#include <WebServer.h>
#include <WiFi.h>


const char* WIFI_SSID = "SSID";
const char* WIFI_PASS = "PASSWORD";

WebServer server(80);

const int motorPin1 = 12;
const int motorPin2 = 13;
const int motorPin3 = 15;
const int motorPin4 = 14;
int origin = 0;

int motorSpeed = 1200;   //spped
int stepCounter = 0;     //step counter
int stepsPerRev = 512;  //number of steps per lap
 
const int numSteps = 8;
const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };

static auto hiRes = esp32cam::Resolution::find(800, 600);

void serveJpg() {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("CAPTURE FAIL");
    server.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  server.setContentLength(frame->size());
  server.send(200, "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}

void handleJpgHi() {
  if (!esp32cam::Camera.changeResolution(hiRes)) {
    Serial.println("SET-HI-RES FAIL");
  }
  serveJpg();
}

void initialMove() {
  moveRight();
  delay(50);
  moveLeft();
  off();
}

void off() {
  digitalWrite(motorPin1,LOW);
  digitalWrite(motorPin2,LOW);
  digitalWrite(motorPin3,LOW);
  digitalWrite(motorPin4,LOW);  
}

void moveLeft() {
  for (int i = 0; i < 128; i++) { //tenia 256
    anticlockwise();
    delayMicroseconds(motorSpeed);
  }  
  
}

void moveRight() {
  for (int i = 0; i < 128; i++) {
    clockwise();
    delayMicroseconds(motorSpeed);
  }  

}

void moverDerecha() {
  moveRight();
  origin++;
  off();
  info();
}

void moverIzquierda() {
  moveLeft();
  origin--;
  off();
  info();
}

void info() {
  server.send(200, "text/plain", "{ \"pos\":\""+String(origin)+"\"}");
}

void setup()
{
  Serial.begin(9600);
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);  

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam");


  server.on("/cam", handleJpgHi);

  server.on("/info", info);
  server.on("/moverIzquierda", moverIzquierda);
  server.on("/moverDerecha", moverDerecha);
  
  initialMove();
  
  server.begin();
}

void loop(){
  server.handleClient();
}

//girar en sentido derecho
void clockwise() {
  Serial.println("Girando derecha");
  stepCounter++;
  if (stepCounter >= numSteps) stepCounter = 0;
  setOutput(stepCounter);
}

//izquierda
void anticlockwise() {
  Serial.println("Girando izquierda");
  stepCounter--;
  if (stepCounter < 0) stepCounter = numSteps - 1;
  setOutput(stepCounter);
}
 
void setOutput(int step) {
  digitalWrite(motorPin1, bitRead(stepsLookup[step], 0));
  digitalWrite(motorPin2, bitRead(stepsLookup[step], 1));
  digitalWrite(motorPin3, bitRead(stepsLookup[step], 2));
  digitalWrite(motorPin4, bitRead(stepsLookup[step], 3));
}