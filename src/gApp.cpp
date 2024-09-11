/*
 * gApp.cpp
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#include "gApp.h"
#include "gCanvas.h"
#include "menuCanvas.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
}

void gApp::setup() {
//	gCanvas* cnv = new gCanvas(this);
//	appmanager->setCurrentCanvas(cnv);

	menuCanvas* menu = new menuCanvas(this);
	appmanager->setCurrentCanvas(menu);

	music.loadSound("stadium_crowd.mp3");
	music.setLoopType(gBaseSound::LOOPTYPE_DEFAULT);
	buttonsound.loadSound("buton_ses.mp3");
	clicksound.loadSound("click.wav");
	ballhitsound.loadSound("hitting_ball.wav");
	whistlesound.loadSound("whistle.wav");
	goalsound.loadSound("goal.mp3");
}

void gApp::update() {
}
