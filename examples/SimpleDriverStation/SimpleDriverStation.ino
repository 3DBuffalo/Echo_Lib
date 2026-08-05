#include <EchoLib.h>

DistanceSensor sensor(3,42);
DriverStation station;
WiFiServerBridge wifi("EchoRobot", "password123", 8888);
MotorControllers motors;
MecanumDrive drive(motors,5,2,6,1);// FL, FR, BL, BR
bool start = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Ready");
  motors.reverse(2);
  motors.reverse(1);
  wifi.begin();
  station.putBool(false, 2);
  while(!wifi.getStatus()){
    Serial.println("Waiting on connection...");
    wifi.processIncoming();
  }
  Serial.println("Connection Found!");
  wifi.sendData(station.getDataRaw());
  delay(1000);
}

float data = 0;

void loop() {
  wifi.processIncoming();
  if(wifi.getStatus()){
    station.process(wifi.readData());
    drive.drive((station.getRX()*100)* 0.6, (station.getRY()*100) * 0.6, (station.getLX()*100) * 0.6);
    station.putFloat(data, 1);
    String d = station.getData(3);
    station.putString(d, 4);
    station.putBool(station.getA(), 5);
    data = sensor.getDistance();
    wifi.sendData(station.getDataRaw());
    Serial.println(station.getDataRaw());
  }
}