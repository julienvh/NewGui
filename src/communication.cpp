#include "communication.h"

//--------------------------------------------------------------
void communication::setup() {

	//Setup all the Arduino's, COM ports should be static

	std::vector<ofx::IO::SerialDeviceInfo> devicesInfo = ofx::IO::SerialDeviceUtils::listDevices();

	ofLogNotice("ofApp::setup") << "Connected Devices: ";

	for (std::size_t i = 0; i < devicesInfo.size(); ++i)
	{
		//Print the device info list, this lists all the COM ports.
		ofLogNotice("ofApp::setup") << "\t" << devicesInfo[i];
	}

	if (!devicesInfo.empty())
	{
		//Setup a device on a manually choosen COM port, if the wires are going to be switched, these will probably be switched too. 
		//Current list of COM ports -> docs.julienvanhaeren.com/Technische_Realisatie_&_Prototyping/Motoren_aansturing/Resultaat.html
		
		bool success = servo1.setup(devicesInfo[5], 9600);
		if (success)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[5];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[5];
		}
		bool success2 = rotate.setup(devicesInfo[0], 9600);

		if (success2)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[0];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[0];
		}
		bool success3 = head_motor.setup(devicesInfo[2], 9600);
		if (success3)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[2];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[2];
		}
		bool success4 = middle_motor.setup(devicesInfo[1], 9600);
		if (success4)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[1];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[1];
		}
		bool success5 = bottom_motor.setup(devicesInfo[3], 9600);
		if (success5)
		{
			ofLogNotice("ofApp::setup") << "Successfully setup " << devicesInfo[3];
		}
		else
		{
			ofLogNotice("ofApp::setup") << "Unable to setup " << devicesInfo[3];
		}


	}
	else
	{
		ofLogNotice("ofApp::setup") << "No devices connected.";
	}



	//Build the motor info list with ofxDatGui
	motorinfo->addHeader("Motor Info:", false);
	motorinfo->addLabel("Connected to " + servo1.getPortName() + " << Servo / LED / Webcam");
	motorinfo->addBreak()->setHeight(10.0f);
	motorinfo->addLabel("Connected to " + head_motor.getPortName() + " << Head motor");
	motorinfo->addBreak()->setHeight(10.0f);
	motorinfo->addLabel("Connected to " + middle_motor.getPortName() + " << Middle motor");
	motorinfo->addBreak()->setHeight(10.0f);
	motorinfo->addLabel("Connected to " + bottom_motor.getPortName() + " << Bottom motor");
	motorinfo->addBreak()->setHeight(10.0f);
	motorinfo->addLabel("Connected to " + rotate.getPortName() + " << Rotation motor");
	motorinfo->addBreak()->setHeight(10.0f);
	motorinfo->addLabel("See Console for more information.");
	motorinfo->addFooter();
	motorinfo->getFooter()->setLabelWhenExpanded("CLOSE MOTOR INFO");
	motorinfo->getFooter()->setLabelWhenCollapsed("EXPAND MOTOR INFO");


}

//--------------------------------------------------------------
void communication::update() {
	
	motorinfo->update();
	
	//Make the Arduino throw exceptions and send data back
	try
	{
		uint8_t buffer[1024];

		//1 while for each port used
		while (servo1.available() > 0)
		{
			std::size_t sz = servo1.readBytes(buffer, 1024);
			for (std::size_t i = 0; i < sz; ++i)
			{
				//buffer[i] contains printed data from the Arduino
				std::cout << buffer[i];
			}
		}
	}
	catch (const std::exception& exc)
	{
		ofLogError("ofApp::update") << exc.what();
	}

}

//--------------------------------------------------------------
void communication::draw() {	
	motorinfo->draw();
}

void communication::sendColor(string a) {
	//Send a string of text to the specified Arduino through a bytebuffer
	std::string text = a;
	ofx::IO::ByteBuffer textBuffer(text);
	servo1.writeBytes(textBuffer);
}

void communication::slideServo(int x) {
	//Send 1 byte to the specified Arduino
	servo1.writeByte(x);
}