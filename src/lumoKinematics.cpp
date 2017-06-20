#include "lumoKinematics.h"

void lumoKinematics::setup() {

}

void lumoKinematics::update() {

}

string lumoKinematics::draw(int x, int y) {

	ofSetColor(255, 255, 0);
	ofFill();

	//drawaxes();
	inverse();
	targetx = x;
	targety = y;
	

	std::copy(movements.begin(), movements.end() - 1, std::ostream_iterator<string>(oss, ",")); 
	oss << movements.back();
	return oss.str();

}




void lumoKinematics::drawaxes() {
	ofSetLineWidth(1);
	ofSetColor(255, 255, 255);
	ofDrawLine(originX - 400, originY, originX + 400, originY);
	ofDrawLine(originX, originY - 400, originX, originY + 400);
}

void lumoKinematics::inverse() {
	L1 = 200;
	L2 = 200;
	L31 = 30;
	L32 = 40;
	L3 = sqrt((L31*L31) + (L32*L32));

	Yc = (targety - originY)*-1;
	Xc = (originX - targetx + L3)*-1;
	Xwc = Xc;
	Ywc = Yc;

	//  Dit is eigenlijk het rekenwerk, wordt verderop vertaald naar daadwerkelijke draaiingshoeken

	D = (pow(Xwc, 2) + pow(Ywc, 2) - pow(L1, 2) - pow(L2, 2)) / (2 * L1*L2);
	theta3 = atan2((sqrt(1 - pow(D, 2))), D);
	theta2 = (atan2(Xwc, Ywc)) - atan2((sin(theta3)*L2), L1 + (cos(theta3)*L2));

	//ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 5);

	theta2 = theta2 / (3.14 / 180);
	theta2 = -90 + theta2;
	theta3 = theta3 / (3.14 / 180);
	theta3 = theta3 + theta2;


	direct(theta2, theta3);

}

void lumoKinematics::direct(float a, float b) {



	//  Nu gaan we de berekende waarden gebruiken om een representatie te tekeken met lijnen

	angle2 = a * (3.14 / 180);
	angle4 = b * (3.14 / 180);
	angle6 = angle5 * 0.01745329252;


	//  Pythagoras om te berekeken wat de posities van de lijnpunten zijn

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

	//  Lijnen tekeken


	ofSetLineWidth(10);

	ofSetColor(255, 0, 0);
	ofDrawLine(originX, originY, originX + Xc, originY + Yc);

	ofSetColor(0, 255, 0);
	ofDrawLine(originX + Xc, originY + Yc, originX + Xc + Xcw, originY + Yc + Ycw);

	ofSetColor(0, 0, 255);
	ofSetLineWidth(30);
	ofDrawLine(originX + Xc + Xcw, originY + Yc + Ycw, originX + Xc + Xcw + Xcwd, originY + Yc + Ycw + Ycwd);

	//  Hulplijnen tekeken

	ofSetColor(255, 255, 255);
	ofSetLineWidth(1);
	ofDrawLine(originX, originY, originX + Xc + Xcw + Xcwd, originY + Yc + Ycw + Ycwd);
	ofDrawLine(originX, originY, originX + Xc + Xcw, originY + Yc + Ycw);
	ofSetColor(0, 0, 0);
	ofDrawLine(originX, originY, originX - 300, originY);
	ofDrawLine(originX, originY, originX + Xc8, originY + Yc8);
	ofDrawLine(originX + Xc, originY + Yc, originX + Xc + Xcw8, originY + Yc + Ycw8);

	//  Hier kunnen/moeten restricties ingezet worden

	if ((a + 180<0) || (a + 180>120)) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("NEE", originX, originY);
	}
	if ((b - a<30) || (b - a>170)) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString("NEE", originX, originY);
	}

	ofFill();

	//  Hoeken vertalen naar hoeken op basis van de beginpositie voor de onderste arm, de andere armen op basis van de arm voor diegene. Afronden voor het verzenden via een seriele verbinding naar arduino

	draai1 = int((a + 180) + 0.5f);
	draai2 = int((b)+0.5f);
	draai3 = int((angle5)+0.5f);

	//  Hoeken tonen op scherm
	movements = { ofToString(draai1) , ofToString(draai2), ofToString(draai3) };
	


}

void lumoKinematics::mouseDragged(int x, int y, int button) {
	//targetx = x;
	//targety = y;
	//communication.sendData(draai1, draai2, draai3);

}

void lumoKinematics::mousePressed(int x, int y, int button) {
	//rust
	if (ofGetMouseX()>16 && ofGetMouseX()<81) {
		if (ofGetMouseY()>115 + 50 && ofGetMouseY()<145 + 50) {
			//moveToComplex(600,250,688,288);
			//moveTo(600, 250);
			if (!bDraw) {
				bDraw = true;
			}
		}
	}
	//schrik
	if (ofGetMouseX()>16 && ofGetMouseX()<81) {
		if (ofGetMouseY()>180 + 50 && ofGetMouseY()<210 + 50) {
			//moveTo(500, 175);
			complexTimer = -1;
			moveToComplex(600, 150, 600, 300, 600, 150);
			if (!bDraw) {
				finished = false;
				bDraw = true;
			}
		}
	}
	//interesse
	if (ofGetMouseX()>16 && ofGetMouseX()<81) {
		if (ofGetMouseY()>220 + 50 && ofGetMouseY()<250 + 50) {
			//moveTo(688, 288);
			moveTo(650, 250);
			//moveToComplex(600, 250, 688, 288);
			if (!bDraw) {
				bDraw = true;
			}
		}
	}

	//communication.mousePressed(x, y, button);
	//communication.sendData(draai1, draai2, draai3);
}

void lumoKinematics::mouseReleased(int x, int y, int button) {
	
}

void lumoKinematics::moveTo(int x, int y) {

	targetx = x;
	targety = y;

}

void lumoKinematics::moveToComplex(int a, int b, int c, int d, int e, int f) {
	testar[0] = a;
	testar[1] = b;
	testar[2] = c;
	testar[3] = d;
	testar[4] = e;
	testar[5] = f;
	bComplexRun = true;

}

