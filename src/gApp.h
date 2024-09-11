/*
 * gApp.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GAPP_H_
#define GAPP_H_

#include "gBaseApp.h"
#include "gFmodSound.h"


class gApp : public gBaseApp {
public:
	gApp();
	gApp(int argc, char **argv);
	~gApp();

	void setup();
	void update();

	int musicon, musicoff;

	gFmodSound music;
	gFmodSound buttonsound;
	gFmodSound clicksound;
	gFmodSound ballhitsound;
	gFmodSound whistlesound;
	gFmodSound goalsound;
};

#endif /* GAPP_H_ */
