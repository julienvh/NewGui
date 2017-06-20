#pragma once

#ifndef communication_hpp
#define communication_hpp


#include <stdio.h>
#include <ofMain.h>
#include "ofxSerial.h"
#include "ofxIO.h"
#include "ofxDatGui.h"

class communication{

public:
	void setup();
	void update();
	void draw();
	void sendColor(string a);
	void slideServo(int x);

	ofx::IO::SerialDevice servo1;
	ofx::IO::SerialDevice rotate;
	ofx::IO::SerialDevice head_motor;
	ofx::IO::SerialDevice bottom_motor;
	ofx::IO::SerialDevice middle_motor;

	ofxDatGui* motorinfo = new ofxDatGui(475, 0);

};

#endif /* communication_hpp */