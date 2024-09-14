/*
* gApp.cpp
*
*  Created on: September 4, 2024
*      Author: Kadir Semih Arslan
*/

#include "gApp.h"
#include "gCanvas.h"
#include "menuCanvas.h"


gApp::gApp() {
}

gApp::gApp(int argc, char **argv) : gBaseApp(argc, argv) {
}

gApp::~gApp() {
	database.close();
}

void gApp::setup() {
	// Change Canvas

//	gCanvas* cnv = new gCanvas(this);
//	appmanager->setCurrentCanvas(cnv);

	menuCanvas* menu = new menuCanvas(this);
	appmanager->setCurrentCanvas(menu);

	// Load musics

	music.loadSound("stadium_crowd.mp3");
	music.setLoopType(gBaseSound::LOOPTYPE_NORMAL);
	buttonsound.loadSound("buton_ses.mp3");
	clicksound.loadSound("click.wav");
	ballhitsound.loadSound("hitting_ball.wav");
	whistlesound.loadSound("whistle.wav");
	goalsound.loadSound("goal.mp3");

	// Database

	std::string statement = "SELECT * FROM optionst";
	std::string selecteddata;

	database.loadDatabase("options.db");
	database.execute(statement);

	while(database.hasSelectData()) {
		selecteddata = database.getSelectData();
	}

	int firstpipe = selecteddata.find("|", 1);
	int secondpipe = selecteddata.find("|", firstpipe + 1);
	int thirdpipe = selecteddata.find("|", secondpipe + 1);

	musicvalue = gToInt(selecteddata.substr(firstpipe + 1, secondpipe - firstpipe - 1));
	difficultyvalue = gToInt(selecteddata.substr(secondpipe + 1, thirdpipe - secondpipe - 1));
	vibrationvalue = gToInt(selecteddata.substr(thirdpipe + 1));
}

void gApp::update() {
}
