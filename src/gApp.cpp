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
	database.close();
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

	std::string statement = "SELECT * FROM optionst";
	std::string selecteddata;

	database.loadDatabase("options.db");
	database.execute(statement);


	while(database.hasSelectData()) {
		selecteddata = database.getSelectData();
	}

	int firstpipe = selecteddata.find("|");
	int secondpipe = selecteddata.find("|", firstpipe + 1);
	int thirdpipe = selecteddata.find("|", secondpipe + 1);

	musicstate = gToInt(selecteddata.substr(firstpipe + 1, 1));
	difficultystate = gToInt(selecteddata.substr(secondpipe + 1, 1));
	vibrationstate = gToInt(selecteddata.substr(thirdpipe + 1));

}

void gApp::update() {
}
