#include "DriverStation.h"

/*
* DATA FORMAT:
* "S|S1:|S2:|S3:|S4:|S5:|S6:|S7:|S8:|S9:|S10:;X:0|Y:0|A:0|B:0|DU:0|DD:0|DL:0|DR:0|BL:0|BR:0|A1:0|A2:0|A3:0|A4:0|A5:0|A6:0|E{0}"
* S - start of data packet
* E - end of data packet
* S1-S10 - driver station slots for data
* 0/1 - represents the buttons on an xbox controller with the order: X,Y,A,B,DU,DD,DL,DR,BL,BR
* A1-A6 - represtnts xbox controller axis values in the order: JLX,JLY,JRX,JRY,TL,TR
* | - seperates data 
* 10 total driver station data slots
* 10 buttons
* 6 axis data
* 11 | for driver station data
* {0} - disabled
* {1} - enabled
*/

DriverStation::DriverStation() {
    // TODO: initialize internal state
    data = "S|S1:|S2:|S3:|S4:|S5:|S6:|S7:|S8:|S9:|S10:;X:0|Y:0|A:0|B:0|DU:0|DD:0|DL:0|DR:0|BL:0|BR:0|A1:0|A2:0|A3:0|A4:0|A5:0|A6:0|E{0}";
    Serial.println("Driver Station Ready...");
}

void DriverStation::putInt(int data, int ID) {
    this->putData(String(data), ID);
}

void DriverStation::putFloat(float data, int ID) {
    this->putData(String(data), ID);
}

void DriverStation::putBool(boolean data, int ID) {
    String conv = (data == 0 ? "False" : "True");
    this->putData(conv, ID);
}

void DriverStation::putString(String data, int ID) {
    this->putData(data, ID);
}

String DriverStation::getData(int ID) {
    String dataBlock = "S" + String(ID) + ":";
    int blockIndex = data.indexOf(dataBlock)+3;
    if(ID > 0 && ID < 10){
        String nextBlock = "|S" + String((ID+1));
        int blockEndIndex = data.indexOf(nextBlock);
        return data.substring(blockIndex, blockEndIndex);
    }else if(ID == 10){
        int blockEndIndex = data.indexOf(";");
        return data.substring((blockIndex+1),blockEndIndex);
    }else{
        return "No Data Found on index: " + String(ID);
    }
}

String DriverStation::getDataRaw(){
    return data;
}

void DriverStation::process(String recievedData) {
    int startInd = recievedData.indexOf("S|");
    int endInd = recievedData.indexOf("}");
    if (startInd == -1 || endInd == -1 || endInd <= startInd) {
        return;
    }
    String incoming = recievedData.substring(startInd, endInd + 1);

    int incSemi = incoming.indexOf(";");
    if (incSemi == -1) {
        // Malformed — no slot/control separator found. Fall back to the old
        // adopt-wholesale behavior rather than leaving state half-updated.
        this->data = incoming;
        return;
    }

    // S1-S10 are persistent driver-station state that either side may set
    // once and never touch again (e.g. a value written in setup()). A blank
    // value from the other side means "I have no opinion on this slot right
    // now" — NOT "set this to empty" — so we merge slot-by-slot and only
    // adopt a slot when the incoming packet actually has real content for
    // it. This is what keeps a one-time setup() value from being wiped the
    // moment a reconnect/handshake packet (which may still be blank on that
    // slot) comes through.
    for (int id = 1; id <= 10; id++) {
        String incomingVal = extractSlotValue(incoming, id);
        if (incomingVal.length() == 0) continue;   // no opinion — keep current value
        this->putData(incomingVal, id);
    }

    // Buttons / axes / enable state are always fully live and DS-driven —
    // splice the fresh section in verbatim every time, same as before.
    int mySemi = this->data.indexOf(";");
    if (mySemi == -1) {
        this->data = incoming;
        return;
    }
    this->data = this->data.substring(0, mySemi) + incoming.substring(incSemi);
}

String DriverStation::extractSlotValue(const String &src, int ID) {
    if (ID < 1 || ID > 10) return "";
    String dataBlock = "S" + String(ID) + ":";
    int idx = src.indexOf(dataBlock);
    if (idx == -1) return "";
    int blockIndex = idx + dataBlock.length();
    if (ID < 10) {
        String nextBlock = "|S" + String(ID + 1) + ":";
        int blockEndIndex = src.indexOf(nextBlock, blockIndex);
        if (blockEndIndex == -1) return "";
        return src.substring(blockIndex, blockEndIndex);
    } else {
        int blockEndIndex = src.indexOf(";", blockIndex);
        if (blockEndIndex == -1) return "";
        return src.substring(blockIndex, blockEndIndex);
    }
}

void DriverStation::putData(String d, int ID){
    if(ID > 0 && ID <= 10){
        String fdata = "S" + String(ID) + ":" + getData(ID);
        String ndata = "S" + String(ID) + ":" + d;
        this->data.replace(fdata,ndata);
    }else{
        Serial.println("Invalid Data ID");
    }
}

bool DriverStation::getControllerBtn(String btn){
    String dataBlock = btn + ":";
    int startIndex = this->data.indexOf(dataBlock);
    if (startIndex == -1) return false;
    int valueIndex = startIndex + dataBlock.length();
    char btnChar = this->data.charAt(valueIndex);
    return (btnChar == '1');
}

float DriverStation::getControllerAxis(int axis){
    String dataBlock = "A" + String(axis) + ":";
    int blockIndex = data.indexOf(dataBlock)+3;
    if(axis > 0 && axis < 6){
        String nextBlock = "|A" + String((axis+1));
        int blockEndIndex = data.indexOf(nextBlock);
        return data.substring(blockIndex, blockEndIndex).toFloat();
    }else if(axis == 6){
        int blockEndIndex = data.indexOf("|E");
        return data.substring(blockIndex, blockEndIndex).toFloat();
    }else{
        return 0.0;
    }
}

bool DriverStation::getX(){
    return this->getControllerBtn("X");
}

bool DriverStation::getY(){
    return this->getControllerBtn("Y");
}

bool DriverStation::getA(){
    return this->getControllerBtn("A");
}

bool DriverStation::getB(){
    return this->getControllerBtn("B");
}

bool DriverStation::getBumperL(){
    return this->getControllerBtn("BL");
}

bool DriverStation::getBumperR(){
    return this->getControllerBtn("BR");
}

bool DriverStation::getDU(){
    return this->getControllerBtn("DU");
}

bool DriverStation::getDD(){
    return this->getControllerBtn("DD");
}

bool DriverStation::getDL(){
    return this->getControllerBtn("DL");
}

bool DriverStation::getDR(){
    return this->getControllerBtn("DR");
}

float DriverStation::getLX(){
    return this->getControllerAxis(1);
}

float DriverStation::getLY(){
    return this->getControllerAxis(2);
}

float DriverStation::getRX(){
    return this->getControllerAxis(3);
}

float DriverStation::getRY(){
    return this->getControllerAxis(4);
}

float DriverStation::getRT(){
    return this->getControllerAxis(6);
}

float DriverStation::getLT(){
    return this->getControllerAxis(5);
}

int DriverStation::getState(){
    int state = data.indexOf("{")+1;
    if(state == -1) return -1;
    if(data.charAt(state) == '0'){
        return 0;
    }if(data.charAt(state) == '1'){
        return 1;
    }
    return -1;
}