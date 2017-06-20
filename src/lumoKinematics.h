#pragma once

#ifndef lumoKinematics_hpp
#define lumoKinematics_hpp


#include <stdio.h>
#include <ofMain.h>
#include "ofxCv.h"
#include "ofxOpenCv.h"



class lumoKinematics {
public:

	void setup();
	string draw(int x, int y);
	void update();
	void inverse();
	void direct(float a, float b);
	void drawaxes();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void moveTo(int x, int y);
	void moveToComplex(int a, int b, int c, int d, int e, int f);


	int originX = 300;
	int originY = 700;


	int
		//targetx = 200, //Doelposities, halen uit mouseDragged()
		//targety = 200,

		draai1, //Dit willen we doorsturen
		draai2,
		draai3;



	float
		L1, //Lengtes van de armen
		L2,
		L3,
		L31,
		L32,

		Yc, //Posities voor het rekenen
		Xc,
		Xwc,
		Ywc,

		Xcw, //Posities voor het tekenen van de armen
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
	
	
	vector<string> movements = {"","",""};

	std::ostringstream oss;

	bool bDraw = false;
	bool nextPosition = false;
	bool bComplex = false;
	int arrayComplex[6];
	int complex = 1;

	int testar[6];

	bool finished = false;

	int x1, x2, x3, y1, y2, y3;
	int complexTimer = -1;
	bool bComplexRun = false;


};

#endif /* lumoKinematics_hpp */