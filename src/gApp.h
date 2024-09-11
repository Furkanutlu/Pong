/*
* gApp.h
*
*  Created on: September 4, 2024
*      Author: Kadir Semih Arslan
*/

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"
#include "gDatabase.h"


class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	int musicstate, difficultystate, vibrationstate;

	gFmodSound music;
	gFmodSound buttonsound;
	gFmodSound clicksound;
	gFmodSound ballhitsound;
	gFmodSound whistlesound;
	gFmodSound goalsound;

	gDatabase database;
};

#endif /* GAPP_H_ */
