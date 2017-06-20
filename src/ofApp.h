#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "communication.h"
#include "ofxJSON.h"
#include <iostream>
#include <fstream>


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void buildGUI();
		void readJSON();
		void writeJSON();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void onButtonEvent(ofxDatGuiButtonEvent e);
		void onTextInputEvent(ofxDatGuiTextInputEvent e);
		void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
		void onDropdownEvent(ofxDatGuiDropdownEvent e);
		void onSliderEvent(ofxDatGuiSliderEvent e);
		void onToggleEvent(ofxDatGuiToggleEvent e);
		void on2dPadEvent(ofxDatGui2dPadEvent e);
		
		void inverse();
		void direct(float a, float b);

		string label = "Modus 1 / Modus 2";
		string valueMovement;
		string valueName;
		string sendColor = "255,255,255";

		ofParameter<int> Tx;
		ofParameter<int> Ty;

		ofParameter<int> mRotation;
		ofParameter<int> mBottom;
		ofParameter<int> mMiddle;
		ofParameter<int> mTop;

		bool run;

		vector<string> optionsNames;
		vector<string> optionsPos; 
		vector<string> optionsColors;
		
		ofxDatGuiDropdown* dropdown;
		ofxDatGui* gui;
		

		communication comm;

		ofxJSONElement result;
		
		ofImage base;

		ofRectangle bounds;

		string test;
		




		//Everything used for the kinematics calculating part is found below

		int originX = 300;
		int originY = 700;


		int
			draai1, //The actual rotations
			draai2,
			draai3;



		float
			L1, //Armlenghts
			L2,
			L3,
			L31,
			L32,

			Yc, //Starting positions for calculating
			Xc,
			Xwc,
			Ywc,

			Xcw, //Starting positions for drawing
			Ycw,
			Xcwd,
			Ycwd,
			Xcw8,
			Ycw8,
			Xc8,
			Yc8,

			D,
			theta2,
			theta3,

			angle2,
			angle4,
			angle5 = 0,
			angle6;


		int targety;
		int targetx;

};
