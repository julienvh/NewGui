#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	//Setup communication, read JSON to build the menu, build the menu
	readJSON();
	comm.setup();
	buildGUI();	
}

//--------------------------------------------------------------
void ofApp::update(){
	comm.update();
	gui->update();
	dropdown->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	comm.draw();
	gui->draw();
	dropdown->draw();

	//Calculate the needed armrotations for the desired position and store them in the sliders
	inverse();
	targetx = Tx;
	targety = Ty;

	mBottom = draai1;
	mMiddle = draai2;
	angle5 = mTop;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


void ofApp::readJSON() {
	//Open json & store data
	std::ifstream test_file("data/movements.json");
	Json::Value emotions;
	test_file >> emotions;
	//Iterate and populate vector which will make up the movements dropdown menu
	for (auto itr : emotions["Emotions"]) {
		optionsNames.push_back(itr["Name"].asString());
		optionsPos.push_back(itr["Target"].asString());
		optionsColors.push_back(itr["Color"].asString());
	}
}
void ofApp::writeJSON() {
	//Open the JSON file & delete old content
	std::ofstream ofs("data/movements.json", std::ofstream::out | std::ofstream::trunc);
	Json::Value event;
	Json::Value vec(Json::arrayValue);
	//Fill the JSON value with the values of the vectors
	for (std::vector<int>::size_type i = 0; i != optionsNames.size(); i++)
	{	
		cout << optionsNames[i] << endl;
		event["Emotions"][i]["Name"] = optionsNames[i];
		event["Emotions"][i]["Target"] = optionsPos[i];
		event["Emotions"][i]["Color"] = optionsColors[i];
	}
	//Write to the JSON file
	ofs << event;
	ofs.close();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//-----------------EVERTHING ABOUT THE GUI----------------------
//--------------------------------------------------------------
//--------------------------------------------------------------

void ofApp::buildGUI() {
	//We use ofParameters to make sure the sliders are binded to the values, so with these the sliders will update when te variables are changed 
	bounds = ofRectangle(200, 330, 475, 370);
	Tx.set("Target X", 600, 200, 675);
	Ty.set("Target y", 400, 330, 700);
	mRotation.set("Rotation Motor", 100, -180, 180);
	mBottom.set("Bottom Motor", 100, -180, 180);
	mMiddle.set("Middle Motor", 100, -180, 180);
	mTop.set("Top Motor", 0, -45, 45);

	//Build all the GUI buttons and sliders, link actions to functions
	gui = new ofxDatGui(ofxDatGuiAnchor::TOP_RIGHT);
	gui->addHeader(":: Lumo, een exploratie naar mens en machine ::", false);
	gui->addToggle("RUN!", run = false);
	gui->addBreak()->setHeight(20.0f);
	gui->addFRM();
	gui->addWaveMonitor("AUDIO", 5.00, 0.50);
	gui->addBreak()->setHeight(20.0f);
	gui->add2dPad("posish", bounds);
	gui->addBreak()->setHeight(20.0f);
	gui->addColorPicker("LED color", ofColor::blue);
	gui->addSlider(Tx);
	gui->addSlider(Ty);
	gui->addSlider("Servo movement", 45, 135);
	gui->addSlider(mTop);
	gui->addSlider(mMiddle);
	gui->addSlider(mBottom);
	gui->addSlider(mRotation);
	gui->addBreak()->setHeight(40.0f);
	gui->addTextInput("NAME", valueName = "eg. SCARED");
	gui->addButton("ADD MOVEMENT");
	gui->onButtonEvent(this, &ofApp::onButtonEvent);
	gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
	gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
	gui->onSliderEvent(this, &ofApp::onSliderEvent);
	gui->onToggleEvent(this, &ofApp::onToggleEvent);
	gui->on2dPadEvent(this, &ofApp::on2dPadEvent);
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded("CLOSE LUMO CONTROLS");
	gui->getFooter()->setLabelWhenCollapsed("EXPAND LUMO CONTROLS");
	//Build the dropdown (evt from the JSON file)
	dropdown = new ofxDatGuiDropdown("SELECT MOVEMENT TO RUN", optionsNames);
	dropdown->setPosition(0, 0);
	dropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e) {
	//All button events go here
	if (e.target->is("ADD MOVEMENT")) {
		//If a movement is added, update the vector, rebuild the dropdown to include the new options, write everything to a JSON file for later use
		cout << "Adding movement:" << endl;
		cout << "New position = " + valueMovement << endl;
		cout << "Movement name = " + valueName << endl;
		optionsNames.push_back(valueName);
		optionsPos.push_back(Tx.toString() + "," + Ty.toString());
		optionsColors.push_back(sendColor);
		writeJSON();
		dropdown = new ofxDatGuiDropdown("SELECT MOVEMENT TO RUN", optionsNames);
		dropdown->setPosition(0, 0);
		dropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
	}
}
void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e) {
	//Fill the string with textinput for include new movement options
	if (e.target->is("NAME")) {
		valueName = e.text;
	}
}
void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
	//Get the color from the colorpicker and send it to the comm class
	sendColor = ofToString(e.color);
	sendColor += "#";
	comm.sendColor(sendColor);
}
void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e) {
	//Send the desired position to the kinematics class to calculate new movements.
	
	std::string str = optionsPos[e.child];
	std::vector<int> vect;
	std::stringstream ss(str);
	int i;
	while (ss >> i) {
		vect.push_back(i);
		if (ss.peek() == ',') {
			ss.ignore();
		}
	}
	Tx = vect[0];
	Ty = vect[1];

	cout << "Move to: X:" << Tx << ", Y:" << Ty << endl;


	//kinematics.moveTo(int x, int y);
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e) {
	//All slider events go here
	if (e.target->is("Servo movement")) {
		//Move the servo manually
		float f = e.value;
		comm.slideServo((int)(f + 0.5f));
	}
}
void ofApp::onToggleEvent(ofxDatGuiToggleEvent e) {
	//Check for the toggle button, run autonomous lumo programm if checked
	if (e.target->is("RUN!")) {
		if (e.checked) {
			//Run autonomous lumo
			cout << "checked" << endl;
		}
		if (!e.checked) {
			//Quit autonomous lumo
			cout << "unchecked" << endl;
		}
	}
}

void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e) {
	//Using the 2d pad to manually manipulate the endeffector position in 2 dimensions
	Tx = e.x;
	Ty = e.y;
}


void ofApp::inverse() {
	//Calculating the needed arm rotations through an inverse kinematics algorithm
	L1 = 200;
	L2 = 200;
	L31 = 30;
	L32 = 40;
	L3 = sqrt((L31*L31) + (L32*L32));

	Yc = (targety - originY)*-1;
	Xc = (originX - targetx + L3)*-1;
	Xwc = Xc;
	Ywc = Yc;

	D = (pow(Xwc, 2) + pow(Ywc, 2) - pow(L1, 2) - pow(L2, 2)) / (2 * L1*L2);
	theta3 = atan2((sqrt(1 - pow(D, 2))), D);
	theta2 = (atan2(Xwc, Ywc)) - atan2((sin(theta3)*L2), L1 + (cos(theta3)*L2));

	theta2 = theta2 / (3.14 / 180);
	theta2 = -90 + theta2;
	theta3 = theta3 / (3.14 / 180);
	theta3 = theta3 + theta2;

	//Send the calculated theta's to another function that will draw the visual rep 
	direct(theta2, theta3);
}

void ofApp::direct(float a, float b) {

	//Get the rotation angles based on the calculated theta's and calculate the linepositions through some pythagorean math
	angle2 = a * (3.14 / 180);
	angle4 = b * (3.14 / 180);
	angle6 = angle5 * 0.01745329252;

	Yc = (sin(float(angle2))) *L1;
	Xc = (cos(float(angle2))) *L1;
	Ycw = (sin(float(angle4))) *L2;
	Xcw = (cos(float(angle4))) *L2;
	Yc8 = (sin(float(angle2))) *(L1 + 100);
	Xc8 = (cos(float(angle2))) *(L1 + 100);
	Ycw8 = (sin(float(angle4))) *(L2 + 100);
	Xcw8 = (cos(float(angle4))) *(L2 + 100);
	Ycwd = (sin(float(angle6))) *L3;
	Xcwd = (cos(float(angle6))) *L3;

	//Draw the lines based on the calculated points and draw some helplines

	ofSetLineWidth(10);

	ofSetColor(255, 0, 0);
	ofDrawLine(originX, originY, originX + Xc, originY + Yc);

	ofSetColor(0, 255, 0);
	ofDrawLine(originX + Xc, originY + Yc, originX + Xc + Xcw, originY + Yc + Ycw);

	ofSetColor(0, 0, 255);
	ofSetLineWidth(30);
	ofDrawLine(originX + Xc + Xcw, originY + Yc + Ycw, originX + Xc + Xcw + Xcwd, originY + Yc + Ycw + Ycwd);

	ofSetColor(255, 255, 255);
	ofSetLineWidth(1);
	ofDrawLine(originX, originY, originX + Xc + Xcw + Xcwd, originY + Yc + Ycw + Ycwd);
	ofDrawLine(originX, originY, originX + Xc + Xcw, originY + Yc + Ycw);
	ofSetColor(0, 0, 0);
	ofDrawLine(originX, originY, originX - 300, originY);
	ofDrawLine(originX, originY, originX + Xc8, originY + Yc8);
	ofDrawLine(originX + Xc, originY + Yc, originX + Xc + Xcw8, originY + Yc + Ycw8);

	//Room for restrictions

	if ((a + 180<0) || (a + 180>120)) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("NEE", originX, originY);
	}
	if ((b - a<30) || (b - a>170)) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("NEE", originX, originY);
	}

	ofFill();

	//Calculating the angles not based off eachother, but based off the starting position of the arm, because thats how stepper motors work
	draai1 = int((a + 180) + 0.5f);
	draai2 = int((b)+0.5f);
	draai3 = int((angle5)+0.5f);

}
