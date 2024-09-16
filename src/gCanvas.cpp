/*
* gCanvas.cpp
*
*  Created on: September 4, 2024
*      Author: Kadir Semih Arslan && Mehmet Furkan Utlu
*/


#include "gCanvas.h"
#include "menuCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {
    setupGame();
	setupMap();
	setupGoal();
	setupScore();
	setupBall();
	setupPuds();
	setupMaphitbox();
	setupGoalPostsLight();
	setupPauseMenu();
	setupGoalEvent();
	setupOptionsMenu();
	setupGameEndPanel();
	setupGameMode();
}
void gCanvas::update() {
	if(gamestate == GAME_START) {
		updateBallPosition();
		updateHitAnimating();
		updatePudAnimating();
		updateBot();
		updatePudControl();
	}

	if(gamestate == GAME_GOAL) {
		updateGoalEvent();
		updateGoalPostsLight();
	}

	if(gamestate == GAME_WAIT) waitEvent();
}

void gCanvas::draw() {
	//logo.draw((getWidth() - logo.getWidth()) / 2, (getHeight() - logo.getHeight()) / 2);
	drawMap();
	drawPuds();
	drawBall();
	drawGoal();
	drawHit();
	drawScore();
	drawGoalPostsLight();
	drawPauseMenu();
	if(gamestate == GAME_GOAL) drawGoalEvent();
	if(gamestate == GAME_WAIT) drawWaitEvent();
	if(gamestate == GAME_OPTIONS) drawOptionsMenu();
	if(gamestate == GAME_SELECT_MODE) drawGameMode();
	if(gamestate == GAME_LOSE || gamestate == GAME_WIN) drawGameEndPanel();
	//gDrawRectangle(pudleft.x, pudleft.y + 10, pudleft.w, pudleft.h - 20, false);
	//gDrawRectangle(gamelinelimitx[0], gamelinelimity[0], gamelinelimitx[1], gamelinelimity[1], true);
}

void gCanvas::keyPressed(int key) {
	//gLogi("gCanvas") << "keyPressed:" << key;
	if (key == 82) {
		resetBall();
	}
    if (key == 32) {
    	toggleBallMovement();
    }

	// Oyun oynama
	if (gamestate == GAME_START) {
		if (gamemode == MODE_PVP) {
			if (key == 265) ismovingupright = true; // Up arrow
			if (key == 264) ismovingdownright = true; // Down arrow
			if (key == 87) ismovingupleft = true; // 'W'
			if (key == 83) ismovingdownleft = true; // 'S'
		}
		else if (gamemode == MODE_PVE) {
			if (isuserleft) {
				if (key == 87) ismovingupleft = true; // 'W'
				if (key == 83) ismovingdownleft = true; // 'S'
			}
			else if (isuserright) {
				if (key == 265) ismovingupright = true; // Up arrow
				if (key == 264) ismovingdownright = true; // Down arrow
			}
		}
	}
}
void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
	if (gamestate == GAME_START) {
		if (gamemode == MODE_PVP) {
			if (key == 265) ismovingupright = false; // Up arrow
			if (key == 264) ismovingdownright = false; // Down arrow
			if (key == 87) ismovingupleft = false; // 'W'
			if (key == 83) ismovingdownleft = false; // 'S'
		}
		else if (gamemode == MODE_PVE) {
			if (isuserleft) {
				if (key == 87) ismovingupleft = false; // 'W'
				if (key == 83) ismovingdownleft = false; // 'S'
			} else if (isuserright) {
				if (key == 265) ismovingupright = false; // Up arrow
				if (key == 264) ismovingdownright = false; // Down arrow
			}
		}
	}
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
	if(gamestate != GAME_SELECT_MODE || gamestate != GAME_GOAL || gamestate != GAME_LOSE || gamestate != GAME_WIN || gamestate != GAME_WAIT) {
		if((x - pausecx) * (x - pausecx) + (y - pausecy) * (y - pausecy) <= pauseradius * pauseradius) pausebutton.hold = true;
		else pausebutton.hold = false;
	}
}

void gCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;

	// Options sliders
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(i != SLIDER_VIBRATION && sliderselected[i]) {
			if(slider[i].x >= sliderminx[i] && slider[i].x <= slidermaxx[i]) slider[i].x = x - slider[0].w / 2;
			if(slider[i].x < sliderminx[i]) slider[i].x = sliderminx[i];
			if(slider[i].x > slidermaxx[i]) slider[i].x = slidermaxx[i];
		}
	}

	// Pud Control
	if(gamestate == GAME_START) {
		if (button == MOUSEBUTTON_LEFT) {
			currentmousey = y;
			if (x < getWidth() / 2) {
				pudleft.y = currentmousey - pudleft.h / 2;

				pudleft.y = std::max(pudleft.y, 115.0f);
				pudleft.y = std::min(pudleft.y, 600 - pudleft.h);

				if (currentmousey != prevmouseyleft) {
					pudleft.velocityy = currentmousey - prevmouseyleft;
				} else {
					pudleft.velocityy = 0;
				}

				prevmouseyleft = currentmousey;
			} else {
				pudright.y = currentmousey - pudright.h / 2;

				pudright.y = std::max(pudright.y, 115.0f);
				pudright.y = std::min(pudright.y, 600 - pudright.h);
				if (currentmousey != prevmouseyright) {
					pudright.velocityy = currentmousey - prevmouseyright;
				} else {
					pudright.velocityy = 0;
				}
				prevmouseyright = currentmousey;
			}
		}
	}
}

void gCanvas::mousePressed(int x, int y, int button) {
//	gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
	// Pause Button
	if(gamestate != GAME_SELECT_MODE && gamestate != GAME_GOAL && gamestate != GAME_LOSE && gamestate != GAME_WIN && gamestate != GAME_WAIT) {
		if(x > (pausebutton.x - (pausebutton.w * 1.25f)) && x < (pausebutton.x + pausebutton.w + (pausebutton.w * 1.25f)) && y > (pausebutton.y - (pausebutton.w * 1.25f)) && y < (pausebutton.y + pausebutton.h + (pausebutton.w * 1.25f))) {
			pausebutton.hold = true;
			soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
		}
	}
	gDrawRectangle((pausebutton.x - (pausebutton.w * 1.25f)), (pausebutton.x + pausebutton.w + (pausebutton.w * 1.25f)), (pausebutton.y - (pausebutton.w * 1.25f)) , (pausebutton.y + pausebutton.h + (pausebutton.w * 1.25f)), true);
	//
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + (buttoncoordinategroup[i].h / 2))) {
			buttoncoordinategroup[i].hold = true;
			soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
		}
	}

	// Game End Buttons
	if(gamestate == GAME_LOSE || gamestate == GAME_WIN) {
		for(int i = 0; i < BUTTON_COUNT - 1; i++) {
			if(x > (buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2)) && x < (((buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2))) + buttonendcoordinategroup[i].w) && y > (buttonendcoordinategroup[i].y + endboardheaderh) && y < ((buttonendcoordinategroup[i].y + endboardheaderh) + (buttonendcoordinategroup[i].h / 2))) {
				buttonendcoordinategroup[i].hold = true;
				soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
			}
		}
	}

	// Slider press control
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(gamestate == GAME_OPTIONS && x > slider[i].x && x < slider[i].x + slider[i].w && y > slider[i].y && y < slider[i].y + slider[i].h){
			sliderselected[i] = true;
			soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
		}
	}

	// Option button
	if(x > opbutton[ACCEPT_BUTTON].x && x < (opbutton[ACCEPT_BUTTON].x + opbutton[ACCEPT_BUTTON].w) && y > opbutton[ACCEPT_BUTTON].y && y < (opbutton[ACCEPT_BUTTON].y + opbutton[ACCEPT_BUTTON].h)) {
		opbutton[ACCEPT_BUTTON].state = true;
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}

	// Mode panel button
	if(gamestate == GAME_SELECT_MODE) {
		for(int i = 0; i < maxplayernum; i++) {
			if(gamestate == GAME_SELECT_MODE && x > modebutton[i].x && x < (modebutton[i].x + modebutton[i].w) && y > modebutton[i].y && y < (modebutton[i].y + modebutton[i].h / 2)) {
				modebutton[i].hold = true;
				soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
			}
		}
	}
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;

	// Pause Button
	if(gamestate != GAME_SELECT_MODE && gamestate != GAME_GOAL && gamestate != GAME_LOSE && gamestate != GAME_WIN && gamestate != GAME_WAIT) {
		if(pausebutton.hold) {
			if(gamestate != GAME_PAUSE){
				changeGameState(GAME_PAUSE);
				toggleBallMovement();
			}
			else changeGameState(GAME_WAIT);
		}
	}

	// Pause Options Buttons düzenleniyor
	if(gamestate == GAME_PAUSE) {
		for(int i = 0; i < BUTTON_COUNT; i++) {
			if(buttoncoordinategroup[i].hold) {
				if(i == BUTTON_REPLAY) {
					buttoncoordinategroup[i].state = true;

					soundControl(musicvalue, SOUND_TYPE_CLOSE);

					gCanvas* replay = new gCanvas(root);
					appmanager->setCurrentCanvas(replay);
				}
				if(i == BUTTON_HOME) {
					soundControl(musicvalue, SOUND_TYPE_CLOSE);

					menuCanvas* main = new menuCanvas(root);
					appmanager->setCurrentCanvas(main);
				}
				if(i == BUTTON_OPTIONS) {
					changeGameState(GAME_OPTIONS);
				}
				buttoncoordinategroup[i].hold = false;
			}
		}
	}
	// Game End Buttons
	if(gamestate == GAME_LOSE || gamestate == GAME_WIN) {
		for(int i = 0; i < BUTTON_COUNT; i++) {
			if(buttonendcoordinategroup[i].hold) {
				if(i == BUTTON_REPLAY) {
					buttoncoordinategroup[i].state = true;
					gCanvas* replay = new gCanvas(root);
					appmanager->setCurrentCanvas(replay);
				}
				if(i == BUTTON_HOME) {
					menuCanvas* main = new menuCanvas(root);
					appmanager->setCurrentCanvas(main);
				}
			}
		}
	}

	// Slider release control
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(sliderselected[i]) {
			sliderselected[i] = false;

			if(i == SLIDER_DIFFICULTY) {
				// If you release the slider it will send its data to the database.
				difficultyvalue = normalizeSlider(sliderminx[i], slidermaxx[i], slider[i].x);
				changeDifficulty(difficultyvalue);
				updateSettingsDatabase("difficultystate", difficultyvalue);
			}
			if(i == SLIDER_MUSIC) {
				// If you release the slider it will send its data to the database.
				musicvalue = normalizeSlider(sliderminx[i], slidermaxx[i], slider[i].x);

				soundControl(musicvalue, SOUND_TYPE_SLIDER);
				updateSettingsDatabase("musicstate", musicvalue);
			}
			if(i == SLIDER_VIBRATION) {
				// If you release the slider it will check it and send its data to the database.
				if(slider[i].x <= sliderminx[i]) {
					slider[i].x = slidermaxx[i];
					vibrationvalue = 1;
				}
				else {
					slider[i].x = sliderminx[i];
					vibrationvalue = 0;
				}

				gLogi("Vibration Value ") << std::to_string(vibrationvalue);
				updateSettingsDatabase("vibrationstate", vibrationvalue);
			}
		}
	}

	// Option button

	if(gamestate == GAME_OPTIONS && opbutton[ACCEPT_BUTTON].state) {
		opbutton[ACCEPT_BUTTON].state = false;
		changeGameState(GAME_PAUSE);
	}


	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(buttoncoordinategroup[i].hold) buttoncoordinategroup[i].hold = false;
	}
	if(gamestate == GAME_OPTIONS) {
		for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
			if(opbuttonselected[i]) opbuttonselected[i] = false;
		}
	}

	// Gamemode select
	if(gamestate == GAME_SELECT_MODE) {
		for(int i = 0; i < maxplayernum; i++) {
			if(modebutton[i].hold) {
				modebutton[i].hold = false;
				if(gamemode == MODE_PVE) {
					if(i == PLAYER_LEFT) {
						// Select game mode and start pvp game.
						selectPlayerPosition(PLAYER_LEFT);
					}
					if(i == PLAYER_RIGHT) {
						// Select game mode and guide to choose position.
						selectPlayerPosition(PLAYER_RIGHT);
					}
				}
				if(gamemode == MODE_NONE) {
					if(i == MODE_PVP) {
						// Select game mode and start pvp game.
						gamemode = MODE_PVP;
						selectPlayerPosition(MODE_PVP);
					}
					if(i == MODE_PVE) {
						// Select game mode and guide to choose position.
						gamemode = MODE_PVE;
						selecttext[PLAYER_LEFT] = "Player Left";
						selecttext[PLAYER_RIGHT] = "Player Right";
					}
				}
			}
		}
	}

	// Make velocity 0
    if (button == MOUSEBUTTON_LEFT) {
        pudleft.velocityy = 0;
        pudright.velocityy = 0;
    }
}

void gCanvas::mouseScrolled(int x, int y) {
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

void gCanvas::setupMap() {
//	aspectx = (float)map[0].getWidth() / getWidth();
//	aspecty = (float)map[0].getHeight() / getHeight();
	//mapleri yukleme
	for(int i = 0; i < maxmapimages; i++){
		map[i].loadImage("futbolassets/court_0" + gToStr(i + 1) + ".png");
	}
	mapx = 0;
	mapy = 0;
	mapw = getWidth();
	maph = getHeight();

	//random map secimi
	randommapnum = gRandom(maxmapimages);

	//Orta cizgi yukleme
	mapcenterline.loadImage("futbolassets/court_center_line.png");
	mapcenterlinew = mapcenterline.getWidth();
	mapcenterlineh = mapcenterline.getHeight() - 91;
	mapcenterlinex = (getWidth() - mapcenterlinew) / 2 ;
	mapcenterliney = (getHeight() - mapcenterlineh) / 2 - 3;
}

void gCanvas::setupGoal() {
	//Kale konumlari
	goal[PLAYER_LEFT].loadImage("futbolassets/goal_posts_left.png");
	goal[PLAYER_RIGHT].loadImage("futbolassets/goal_posts_right.png");

	goalw[PLAYER_LEFT] = goal[PLAYER_LEFT].getWidth() / 1.1f;
	goalh[PLAYER_LEFT] = goal[PLAYER_LEFT].getHeight() / 1.5f;
	goalx[PLAYER_LEFT] = 30;
	goaly[PLAYER_LEFT] = 152;

	goalw[PLAYER_RIGHT] = goal[PLAYER_RIGHT].getWidth() / 1.1f;
	goalh[PLAYER_RIGHT] = goal[PLAYER_RIGHT].getHeight() / 1.5f;
	goaly[PLAYER_RIGHT] = 152;
	goalx[PLAYER_RIGHT] = 1125;

	goalpostleft[UST_DIREK] = {125, 200, 25}; // x , y, radius
	goalpostleft[ALT_DIREK] = {125, 515, 25};
	goalpostright[UST_DIREK] = {1150, 200, 25};
	goalpostright[ALT_DIREK] = {1150, 515, 25};
	ustdirek = 200;
	altdirek = 515;
}

void gCanvas::setupBall() {
	//frame ve hiz

	ballimage.loadImage("futbolassets/ball_frames.png");
	ballshadow.loadImage("futbolassets/ball_shadow.png");
	ballhit.loadImage("futbolassets/ball_hit.png");

	//baslangic

	ball.w = ballimage.getWidth() / 6;
	ball.h = ballimage.getHeight();
	ball.x = getWidth() / 2;
	ball.y = getHeight() / 2;
	ball.radius = ball.h / 2;
	speed = 8.0f;
	maxspeed = 15.0f;

	resetBall();
}

void gCanvas::setupMaphitbox() {
	gamelinelimitx[0] = 150;
	gamelinelimitx[1] = 1125;
	gamelinelimity[0] = 120;
	gamelinelimity[1] = 595;

	goalline[PLAYER_LEFT] = 100; // soldaki oyuncunun kale cizgisi
	goalline[PLAYER_RIGHT] = 1175 ; // sagdaki oyuncunun kale cizgisi

	goalystart = 195;
	goalyend = 520;
}

void gCanvas::setupPuds() {
	score[PLAYER_LEFT] = 0;
	score[PLAYER_RIGHT] = 0;
	pudleftimage.loadImage("futbolassets/pud_left.png");
	pudrightimage.loadImage("futbolassets/pud_right.png");
	float pudratio = 0.7f;
	pudleft.w = (pudleftimage.getWidth() / 5) * pudratio;
	pudleft.h = (pudleftimage.getHeight()) * pudratio;
	pudleft.x = 150;
	pudleft.y = (getHeight() - pudleft.h) / 2;
	pudleft.velocityy = 0;
	pudanimationactiveleft = false;

	pudright.w = (pudrightimage.getWidth() / 5) * pudratio;
	pudright.h = (pudrightimage.getHeight()) * pudratio;
	pudright.x = 1125 - pudright.w;
	pudright.y = (getHeight() - pudright.h) / 2;
	pudright.velocityy = 0;
	pudanimationactiveright = false;
}

void gCanvas::setupScore() {
	scorenumbers.loadImage("futbolassets/numbers_score.png");
	scorenumbersrowcol = 4;
	scorenumberscolumncol = 3;
	scorenumbersmaxframe = 10;

	scorenumbersframew = scorenumbers.getWidth() / scorenumbersrowcol;
	scorenumbersframeh = scorenumbers.getHeight() / scorenumberscolumncol;

	int scorecounter = 0;
	for(int i = 0; i < scorenumberscolumncol; i++) {
		scorenumbersframey = i * 64;
		for(int j = 0; j < scorenumbersrowcol; j++) {
			if(scorecounter < scorenumbersmaxframe) {
				scorenumbersframex = j * 64;
				scores[scorecounter].x = scorenumbersframex;
				scores[scorecounter].y = scorenumbersframey;
				scorecounter++;
			}
		}
	}
}

void gCanvas::setupGoalPostsLight() {
	lightactive = false;

	goalpostslights.loadImage("futbolassets/goal_posts_lights.png");
	goalpostslightsrowcol = 5;
	goalpostslightscolumncol = 4;

	goalpostslightsframew = goalpostslights.getWidth() / 5;
	goalpostslightsframeh = goalpostslights.getHeight() / 4;
	goalpostslightsmaxframe = 20;
}

void gCanvas::setupGoalEvent() {
	goalimage.loadImage("futbolassets/text_goal.png");

	goalimagex = (getWidth() - goalimage.getWidth()) / 2;
	goalimagey = (getHeight() - goalimage.getHeight()) / 2;
	goalimagew = goalimage.getWidth();
	goalimageh = goalimage.getHeight();

	waitcounter = 0;
	waitnumber = WAIT_SECOND;
}

void gCanvas::setupPauseMenu() {
	menubutton[0].loadImage("futbolassets/btn_round_again.png");
	menubutton[1].loadImage("futbolassets/btn_round_home.png");
	menubutton[2].loadImage("futbolassets/btn_round_options.png");

	// Pause Button
	pausebuttonimg.loadImage("futbolassets/btn_pause.png");
	pausebutton.w = pausebuttonimg.getWidth();
	pausebutton.h = pausebuttonimg.getHeight();
	pausebutton.x = getWidth() - (pausebutton.h * 2);
	pausebutton.y = pausebutton.h;
	pausebutton.hold = false;

	pausecx =  pausebutton.x + (pausebutton.h / 2);
	pausecy = pausebutton.y + (pausebutton.h / 2);
	pauseradius = pausebutton.w * 1.25f;

	// Panel
	board.loadImage("futbolassets/board.png");
	boardw = board.getWidth() / 1.4f;
	boardh = board.getHeight() / 1.4f;
	boardx = (getWidth() - boardw) / 2;
	boardy = (getHeight() - boardh) / 2;

	boardheader.loadImage("futbolassets/board_header.png");
	boardheaderw = boardheader.getWidth() / 1.5f;
	boardheaderh = boardheader.getHeight() / 1.5f;
	boardheaderx = boardx + ((boardw - boardheaderw) / 2);
	boardheadery = boardy + (boardheaderh / 1.5f);

	boardfont.loadFont("FreeSansBold.ttf", 30);
	boardtext = "Paused";


	for(int i = 0; i < BUTTON_COUNT; i++) {
		buttoncoordinategroup[i].w = menubutton[i].getWidth() / 1.5f;
		buttoncoordinategroup[i].h = menubutton[i].getHeight() / 1.5f;
		buttoncoordinategroup[i].x = boardx + ((boardw / 2) - (buttoncoordinategroup[i].w * 1.65f)) + ((buttoncoordinategroup[i].w * i) * 1.15f);
		buttoncoordinategroup[i].y = boardy + ((boardh / 2) - (buttoncoordinategroup[i].h / 4));
		buttoncoordinategroup[i].sw = menubutton[i].getWidth();
		buttoncoordinategroup[i].sh = menubutton[i].getWidth();
		buttoncoordinategroup[i].sx = 0;
		buttoncoordinategroup[i].sy = menubutton[i].getHeight() / 2;
		buttoncoordinategroup[i].hold = false;
	}
}

void gCanvas::setupOptionsMenu() {
	// Get database values from root
	musicvalue = root->musicvalue;
	difficultyvalue = root->difficultyvalue;
	vibrationvalue = root->vibrationvalue;

	changeDifficulty(difficultyvalue);

	// Panel
	board.loadImage("futbolassets/board.png");
	boardw = board.getWidth() / 1.4f;
	boardh = board.getHeight() / 1.4f;
	boardx = (getWidth() - boardw) / 2;
	boardy = (getHeight() - boardh) / 2;

	boardheader.loadImage("futbolassets/board_header.png");
	boardheaderw = boardheader.getWidth() / 1.5f;
	boardheaderh = boardheader.getHeight() / 1.5f;
	boardheaderx = boardx + ((boardw - boardheaderw) / 2);
	boardheadery = boardy + (boardheaderh / 1.5f);

	boardfont.loadFont("FreeSansBold.ttf", 30);
	boardtext = "Paused";

	optionsicon[0].loadImage("futbolassets/option_icon_difficulty.png");
	optionsicon[1].loadImage("futbolassets/option_icon_sound.png");
	optionsicon[2].loadImage("futbolassets/option_icon_vibrations.png");

	sliderbackground.loadImage("futbolassets/slider_bg.png");
	sliderimg[0].loadImage("futbolassets/slider_off.png");
	sliderimg[1].loadImage("futbolassets/slider_on.png");
	opbuttons[ACCEPT_BUTTON].loadImage("futbolassets/btn_round_ok.png");

	optionsfont.loadFont("FreeSansBold.ttf", 18);

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		optionsimg[i].loadImage("futbolassets/option_" + gToStr(i + 1) + ".png");
		optionsbg[i].w = optionsimg[i].getWidth() / 1.5f;
		optionsbg[i].h = optionsimg[i].getHeight() / 1.5f;
		optionsbg[i].x = boardx + (boardw / 8);
		optionsbg[i].y = (boardheadery + (boardheaderh * 1.25f)) + (optionsbg[i].h * 1.5f * i);

		opicon[i].w = optionsicon[0].getWidth() / 1.5f;
		opicon[i].h = optionsicon[0].getHeight() / 1.5f;
		opicon[i].x = optionsbg[i].x + (opicon[i].w / 2.2f);
		opicon[i].y = optionsbg[i].y - (opicon[i].h / 6);

		sliderbg[i].w = sliderbackground.getWidth();
		sliderbg[i].h = sliderbackground.getHeight();
		sliderbg[i].x = (optionsbg[i].x + optionsbg[i].w) + (sliderbg[i].w / 4);
		sliderbg[i].y = optionsbg[i].y - (sliderbg[i].h / 6);


		sliderminx[i] = sliderbg[i].x + (sliderbg[i].w / 6);
		slidermaxx[i] = sliderbg[i].x + sliderbg[i].w - (sliderbg[i].w / 3.5f);
		sliderselected[i] = false;

		slider[i].w = sliderimg[0].getWidth() / 2.5f;
		slider[i].h = sliderimg[0].getHeight() / 2.5f;
		slider[i].sw = sliderimg[0].getWidth();
		slider[i].sh = sliderimg[0].getHeight();
		slider[i].x = sliderminx[i];
		slider[i].y = sliderbg[i].y + ((sliderbg[i].h - (slider[i].h / 2)) / 2);
	}
	// Text
	optionstext[0] = "Difficulty";
	optionstext[1] = "Music";
	optionstext[2] = "Vibration";

	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		opbutton[i].w = opbuttons[i].getWidth() / 3;
		opbutton[i].h = opbuttons[i].getHeight() / 3;
		opbutton[i].y = boardy + (boardh - (opbutton[i].w * 1.35f));
		opbutton[i].sw = opbuttons[i].getWidth();
		opbutton[i].sh = opbuttons[i].getHeight() / 2;
	}

	// Slider
	sliderselected[SLIDER_VIBRATION] = vibrationstate;

	updateSliderPosition(SLIDER_DIFFICULTY, difficultyvalue);
	updateSliderPosition(SLIDER_MUSIC, musicvalue);
	updateSliderPosition(SLIDER_VIBRATION, vibrationvalue);

	// Panel accept button
	opbutton[ACCEPT_BUTTON].x = boardx + ((boardw - opbutton[ACCEPT_BUTTON].w) / 2);
	opbutton[ACCEPT_BUTTON].state = false;
}

void gCanvas::setupGameEndPanel() {
	youloseimage.loadImage("futbolassets/text_lose.png");
	youwinimage.loadImage("futbolassets/text_win.png");

	youloseimagew = youloseimage.getWidth();
	youloseimageh = youloseimage.getHeight();
	youloseimagex = (getWidth() - youloseimagew) / 2;
	youloseimagey = (getHeight() - youloseimageh) / 2;

	youwinimagew = youwinimage.getWidth();
	youwinimageh = youwinimage.getHeight();
	youwinimagex = (getWidth() - youwinimagew) / 2;
	youwinimagey = (getHeight() - youwinimageh) / 2;

	endboardw = board.getWidth() / 1.4f;
	endboardh = board.getHeight() / 1.4f;
	endboardx = (getWidth() - boardw) / 2;
	endboardy = (getHeight() - boardh) / 2;

	endboardheaderw = boardheader.getWidth() / 1.5f;
	endboardheaderh = boardheader.getHeight() / 1.5f;
	endboardheaderx = boardx + ((boardw - boardheaderw) / 2);
	endboardheadery = boardy + (boardheaderh * 1.4f);

	for(int i = 0; i < BUTTON_COUNT - 1; i++) {
		buttonendcoordinategroup[i].w = menubutton[i].getWidth() / 1.5f;
		buttonendcoordinategroup[i].h = menubutton[i].getHeight() / 1.5f;
		buttonendcoordinategroup[i].x = boardx + ((boardw / 2) - (buttoncoordinategroup[i].w * 1.65f)) + ((buttoncoordinategroup[i].w * i) * 1.15f);
		buttonendcoordinategroup[i].y = boardy + ((boardh / 2) - (buttoncoordinategroup[i].h / 4));
		buttonendcoordinategroup[i].sw = menubutton[i].getWidth();
		buttonendcoordinategroup[i].sh = menubutton[i].getWidth();
		buttonendcoordinategroup[i].sx = 0;
		buttonendcoordinategroup[i].sy = menubutton[i].getHeight() / 2;
		buttonendcoordinategroup[i].hold = false;
	}
}

void gCanvas::setupGameMode() {
	panelbutton.loadImage("futbolassets/btn_continue.png");
	selectfont.loadFont("FreeSansBold.ttf", 20);
	for(int i = 0; i < maxplayernum; i++) {
		modebutton[i].w = panelbutton.getWidth() / 2;
		modebutton[i].h = panelbutton.getHeight() / 2;
		modebutton[i].x = boardx + ((boardw - modebutton[i].w) / 2);
		modebutton[i].y = (boardy + ((boardh - modebutton[i].h) / 2)) + (modebutton[i].h * i);
		modebutton[i].sw = panelbutton.getWidth();
		modebutton[i].sh = panelbutton.getHeight();
		modebutton[i].hold = false;
	}
	selecttext[PLAYER_LEFT] = "Player vs Player";
	selecttext[PLAYER_RIGHT] = "Player vs NPC";
}

void gCanvas::drawMap() {
	map[randommapnum].draw(mapx, mapy, mapw, maph);
	mapcenterline.draw(mapcenterlinex, mapcenterliney, mapcenterlinew, mapcenterlineh);
}

void gCanvas::drawPuds() {
    pudleftframex = pudanimframeleft * pudleft.w;
    pudleftimage.drawSub(pudleft.x, pudleft.y, pudleftframex, 0, pudleft.w, pudleft.h);
    pudrightframex = pudanimframeright * pudright.w;
    pudrightimage.drawSub(pudright.x, pudright.y, pudrightframex, 0, pudright.w, pudright.h);
}

void gCanvas::drawBall() {
	shadowoffsetx = 10;
	shadowoffsety = 10;
	shadowscale = 1.2f;
	ballshadow.drawSub(ball.x - ball.radius * shadowscale + shadowoffsetx,
					 ball.y - ball.radius * shadowscale + shadowoffsety,
					 0, 0, ball.w, ball.h);
	ballframex = ballcurrentframe * ball.w;
//	ballimage.drawSub(ball.x - ball.radius, ball.y - ball.radius, ballframex, 0, ball.w, ball.h);
	ballangle = calculateAngle(ball.velocityx, ball.velocityy);
	ballimage.drawSub(ball.x - ball.radius, ball.y - ball.radius, ball.w, ball.h, ballframex, 0, ball.w, ball.h, ballangle);
}

void gCanvas::drawHit() {
	hitw = ballhit.getWidth() / 6;
	hith = ballhit.getHeight();
	if(ishitanimating){
		hitframex = hitframe * hitw;
		ballhit.drawSub(hitanimx - hitw / 2, hitanimy - hith / 2, hitframex, 0, hitw, hith);
	}
}

void gCanvas::drawGoal() {
	goal[PLAYER_LEFT].draw(goalx[PLAYER_LEFT], goaly[PLAYER_LEFT], goalw[PLAYER_LEFT], goalh[PLAYER_LEFT], 180);
	goal[PLAYER_RIGHT].draw(goalx[PLAYER_RIGHT], goaly[PLAYER_RIGHT], goalw[PLAYER_RIGHT], goalh[PLAYER_RIGHT]);
}

void gCanvas::drawScore() {
	if(score[PLAYER_LEFT] < scorenumbersmaxframe) {
		scorenumbers.drawSub(
				goalx[PLAYER_LEFT] + goalx[PLAYER_LEFT] / 4,
				goaly[PLAYER_LEFT] + (goalh[PLAYER_LEFT] / 2) - (scorenumbersframew / 2),
				scores[score[PLAYER_LEFT]].x,
				scores[score[PLAYER_LEFT]].y,
				(int)scorenumbersframew, (int)scorenumbersframeh);
	}

	if(score[PLAYER_LEFT] < scorenumbersmaxframe) {
		scorenumbers.drawSub(
				goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3.5f),
				goaly[PLAYER_LEFT] + (goalh[PLAYER_LEFT] / 2) - (scorenumbersframew / 2),
				scores[score[PLAYER_RIGHT]].x,
				scores[score[PLAYER_RIGHT]].y,
				(int)scorenumbersframew, (int)scorenumbersframeh);
	}
}

void gCanvas::drawGoalPostsLight() {
	goalpostslights.drawSub(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, 90);
	goalpostslights.drawSub(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] + goalh[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 12), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, 180);
	goalpostslights.drawSub(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh);
	goalpostslights.drawSub(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] + goalh[PLAYER_LEFT] - (goalh[PLAYER_LEFT] / 8), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, -90);

	renderer->setColor(238, 75, 43, 99);
	for(int i = 0; i < activeGoalPostsLights.size(); i++) {
		goalpostslights.drawSub(activeGoalPostsLights[i][GPL_X],
				activeGoalPostsLights[i][GPL_Y],
				activeGoalPostsLights[i][GPL_W],
				activeGoalPostsLights[i][GPL_H],
				activeGoalPostsLights[i][GPL_SX],
				activeGoalPostsLights[i][GPL_SY], goalpostslightsframew, goalpostslightsframeh);
	}
	renderer->setColor(255, 255, 255);
}

void gCanvas::drawGoalEvent() {
	goalimage.draw(goalimagex, goalimagey, goalimagew, goalimageh);
}

void gCanvas::drawWaitEvent() {
	scorenumbers.drawSub(
			(getWidth() - ((int)scorenumbersframew * WAIT_NUMBER_SIZE_MULTIPLIER)) / 2,
			(getHeight() - ((int)scorenumbersframeh * WAIT_NUMBER_SIZE_MULTIPLIER)) / 2,
			(int)scorenumbersframew * WAIT_NUMBER_SIZE_MULTIPLIER,
			(int)scorenumbersframew * WAIT_NUMBER_SIZE_MULTIPLIER,
			scores[waitnumber].x,
			scores[waitnumber].y,
			(int)scorenumbersframew, (int)scorenumbersframeh);
}

void gCanvas::drawPauseMenu() {
	if(pausebutton.hold) renderer->setColor(161, 102, 47);
	else renderer->setColor(181, 122, 67);
	gDrawCircle(pausecx, pausecy, pauseradius, true);
	renderer->setColor(255, 255, 255);

	if(pausebutton.hold) renderer->setColor(0, 200, 0);
	pausebuttonimg.draw(pausebutton.x, pausebutton.y, pausebutton.w, pausebutton.h);
	renderer->setColor(255, 255, 255);

	if(gamestate == GAME_PAUSE || gamestate == GAME_OPTIONS) {
		board.draw(boardx, boardy, boardw, boardh);
		boardheader.draw(boardheaderx, boardheadery, boardheaderw, boardheaderh);
		boardfont.drawText(boardtext, boardheaderx + ((boardheaderw - boardfont.getStringWidth(boardtext)) / 2) , boardheadery + ((boardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));
	}

	if(gamestate == GAME_PAUSE) {
		boardtext = "Paused";
		for(int i = 0; i < BUTTON_COUNT; i++) {
			menubutton[i].drawSub(buttoncoordinategroup[i].x, buttoncoordinategroup[i].y, buttoncoordinategroup[i].w, buttoncoordinategroup[i].w,
						buttoncoordinategroup[i].sx, buttoncoordinategroup[i].sy * buttoncoordinategroup[i].hold, buttoncoordinategroup[i].sw, buttoncoordinategroup[i].sh);
		}
	}
}

void gCanvas::drawOptionsMenu() {
	boardtext = "Options";

	board.draw(boardx, boardy, boardw, boardh);
	boardheader.draw(boardheaderx, boardheadery, boardheaderw, boardheaderh);
	boardfont.drawText(boardtext, boardheaderx + ((boardheaderw - boardfont.getStringWidth(boardtext)) / 2) , boardheadery + ((boardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		optionsimg[i].draw(optionsbg[i].x, optionsbg[i].y, optionsbg[i].w, optionsbg[i].h);
		sliderbackground.draw(sliderbg[i].x, sliderbg[i].y, sliderbg[i].w, sliderbg[i].h);
		optionsicon[i].draw(opicon[i].x + (opicon[i].w / opicon[i].w), opicon[i].y + ((optionsbg[i].h - opicon[i].h) / 2), opicon[i].w, opicon[i].h);
		sliderimg[sliderselected[i]].drawSub(slider[i].x, slider[i].y, slider[i].w, slider[i].w, 0, 0, slider[i].sw, slider[i].sh / 2);

		renderer->setColor(0, 0, 0);
		optionsfont.drawText(optionstext[i], (optionsbg[i].x + (opicon[i].w * 2)), sliderbg[i].y + (sliderbg[i].h / 1.8f));
		renderer->setColor(255, 255, 255);
	}

	opbuttons[ACCEPT_BUTTON].drawSub(opbutton[ACCEPT_BUTTON].x, opbutton[ACCEPT_BUTTON].y, opbutton[ACCEPT_BUTTON].w, opbutton[ACCEPT_BUTTON].w, 0, opbutton[ACCEPT_BUTTON].sh * opbutton[ACCEPT_BUTTON].state, opbutton[ACCEPT_BUTTON].sw, opbutton[ACCEPT_BUTTON].sh);
}

void gCanvas::drawGameEndPanel() {
	if(score[PLAYER_LEFT] >= 9) {
		boardtext = "Player 1 Win";
	}
	else if(score[PLAYER_RIGHT] >= 9) {
		boardtext = "Player 2 Win";
	}
	board.draw(endboardx, endboardy, endboardw, endboardh);
	boardheader.draw(endboardheaderx, endboardheadery, endboardheaderw, endboardheaderh);
	boardfont.drawText(boardtext, endboardheaderx + ((endboardheaderw - boardfont.getStringWidth(boardtext)) / 2) , endboardheadery + ((endboardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));

	for(int i = 0; i < (BUTTON_COUNT - 1); i++) {
		menubutton[i].drawSub(buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2), buttonendcoordinategroup[i].y + endboardheaderh, buttonendcoordinategroup[i].w, buttonendcoordinategroup[i].w,
				buttonendcoordinategroup[i].sx, buttonendcoordinategroup[i].sy * buttonendcoordinategroup[i].hold, buttonendcoordinategroup[i].sw, buttonendcoordinategroup[i].sh);
	}

	if(gamestate == GAME_LOSE) youloseimage.draw(youloseimagex, youloseimagey - (boardh / 2.5f), youloseimagew, youloseimageh);
	if(gamestate == GAME_WIN) youwinimage.draw(youwinimagex, youwinimagey - (boardh / 2.5f), youwinimagew, youwinimageh);
}

void gCanvas::drawGameMode() {
	boardtext = "Select Mode";

	board.draw(boardx, boardy, boardw, boardh);
	boardheader.draw(boardheaderx, boardheadery, boardheaderw, boardheaderh);
	boardfont.drawText(boardtext, boardheaderx + ((boardheaderw - boardfont.getStringWidth(boardtext)) / 2) , boardheadery + ((boardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));

	for(int i = 0; i < maxplayernum; i++) {
		panelbutton.drawSub(modebutton[i].x, modebutton[i].y, modebutton[i].w, modebutton[i].h / 2, 0, modebutton[i].h * modebutton[i].hold, modebutton[i].sw, modebutton[i].h);

		renderer->setColor(0, 0, 0);
		selectfont.drawText(selecttext[i], modebutton[i].x + ((modebutton[i].w - selectfont.getStringWidth(selecttext[i])) / 2), modebutton[i].y + ((modebutton[i].h - selectfont.getStringHeight(selecttext[i])) / 3));
		renderer->setColor(255, 255, 255);
	}
}

void gCanvas::updateBallPosition() {
	 checkGoal();
	 if(ismoving){
	        // Pudlar

	        checkPudCollision(ball, pudleft);  // Sol pud
	        checkPudCollision(ball, pudright); // Sag pud

		 // direkler
		 if(ball.y > ustdirek && ball.y < altdirek){
			 for (int i = 0; i < 2; i++) {
				if (checkPostCollision(ball, goalpostleft[i])) {
					reflectBall(ball, goalpostleft[i]);
					checkPudCollision(ball, pudleft);
				}
				if (checkPostCollision(ball, goalpostright[i])) {
					reflectBall(ball, goalpostright[i]);
					checkPudCollision(ball, pudright);
			    }
			}
		 } else {// map sinirlari
			 if((ball.x - ball.radius <= gamelinelimitx[0] || ball.x + ball.radius >= gamelinelimitx[1]) && (ball.y + ball.radius >= goalyend || ball.y - ball.radius <= goalystart)){
				 startHitAnimation(ball.x, ball.y);
				 ball.velocityx *= -1;

				    if (ball.x - ball.radius <= gamelinelimitx[0])
				        ball.x = gamelinelimitx[0] + ball.radius;
				    else if (ball.x + ball.radius >= gamelinelimitx[1])
				        ball.x = gamelinelimitx[1] - ball.radius;

			 }
			 if (ball.y - ball.radius <= gamelinelimity[0] || ball.y + ball.radius >= gamelinelimity[1] ) {
				 startHitAnimation(ball.x, ball.y);
				 ball.velocityy *= -1;

				 if (ball.y - ball.radius <= gamelinelimity[0])
				         ball.y = gamelinelimity[0] + ball.radius;
				     else if (ball.y + ball.radius >= gamelinelimity[1])
				         ball.y = gamelinelimity[1] - ball.radius;
			 }
			 if(abs(ball.velocityy) > 20)
			 ball.velocityy *= 0.5;
		 }


		ball.x += ball.velocityx;
		ball.y += ball.velocityy;

		ballframespeed = getBallSpeed() * 0.05f;
		ballframetimer += ballframespeed;
		if(ballframetimer >= 1.0f){
			ballcurrentframe = (ballcurrentframe + 1) % 6;
			ballframetimer = 0.0f; // timeri sifirla
		}
	}
}

void gCanvas::updateHitAnimating() {
    if (ishitanimating) {
	   hitframetimer += hitframespeed;
	   if (hitframetimer >= 0.1f) {
		   hitframe = (hitframe + 1) % 6;
		   hitframetimer = 0.0f;
		   if (hitframe == 0) {
			   ishitanimating = false;
		   }
	   }
   }
}

void gCanvas::updatePudAnimating() {
    if (pudanimationactiveleft) {
        pudanimtimerleft += pudanimframespeed;
        if (pudanimtimerleft >= 0.1f) {
            pudanimframeleft = (pudanimframeleft + 1) % 5;
            pudanimtimerleft = 0.0f;
        }
        if (pudanimframeleft == 0) {
            pudanimationactiveleft = false;
        }
    }

    if (pudanimationactiveright) {
        pudanimtimerright += pudanimframespeed;
        if (pudanimtimerright >= 0.1f) {
            pudanimframeright = (pudanimframeright + 1) % 5;
            pudanimtimerright = 0.0f;
        }
        if (pudanimframeright == 0) {
            pudanimationactiveright = false;
        }
    }
}

void gCanvas::updateBot() {
	if (gamemode != MODE_PVE) return;

	if (difficulty == 1) {
		botSpeed = 3.0f;
		errormargin = 80.0f;
	} else if (difficulty == 2) {
		botSpeed = 5.0f;
		errormargin = 50.0f;
	} else if (difficulty == 3) { // Zor
		botSpeed = 8.0f;
		errormargin = 20.0f;
	}

	bool ballinrightside = ball.x > getWidth() / 2;

	if (isuserleft) {
		if (!isuserright) {
			if(ballinrightside) {
				botcentery = pudright.y + pudright.h / 2;
				ballcentery = ball.y;
				targety = ballcentery - pudright.h / 2;

				if (ballcentery < pudright.y + errormargin) {
					pudright.y -= botSpeed;
				} else if (ballcentery > pudright.y + pudright.h - errormargin) {
					pudright.y += botSpeed;
				}

				pudright.y = std::max(pudright.y, 115.0f);
				pudright.y = std::min(pudright.y, 600 - pudright.h);
			}
		}
	} else if (isuserright) {
		if (!isuserleft) {
			if(!ballinrightside) {
				botcentery = pudleft.y + pudleft.h / 2;
				ballcentery = ball.y;
				targety = ballcentery - pudleft.h / 2;

				if (ballcentery < pudleft.y + errormargin) {
					pudleft.y -= botSpeed;
				} else if (ballcentery > pudleft.y + pudleft.h - errormargin) {
					pudleft.y += botSpeed;
				}

				pudleft.y = std::max(pudleft.y, 115.0f);
				pudleft.y = std::min(pudleft.y, 600 - pudleft.h);
			}
		}
	}
}

void gCanvas::updatePudControl() {
    if (ismovingupleft) {
        pudleft.velocityy = -movespeed;
    } else if (ismovingdownleft) {
        pudleft.velocityy = movespeed;
    } else {
        pudleft.velocityy = 0;
    }

    if (ismovingupright) {
        pudright.velocityy = -movespeed;
    } else if (ismovingdownright) {
        pudright.velocityy = movespeed;
    } else {
        pudright.velocityy = 0;
    }

    pudleft.y += pudleft.velocityy;
    pudright.y += pudright.velocityy;

    pudleft.y = std::max(115.0f, std::min(pudleft.y, 600.0f - pudleft.h));

    pudright.y = std::max(115.0f, std::min(pudright.y, 600.0f - pudright.h));

    checkPudCollision(ball, pudleft);
    checkPudCollision(ball, pudright);
}

void gCanvas::updateGoalPostsLight() {
	for(int i = 0; i < activeGoalPostsLights.size(); i++) {
		if(activeGoalPostsLights[i][GPL_FRAMENO] < goalpostslightsmaxframe - 1) {
			activeGoalPostsLights[i][GPL_COUNTER]++;
			if(activeGoalPostsLights[i][GPL_COUNTER] % 3 == 0) {
				activeGoalPostsLights[i][GPL_COUNTER] = 0;
				activeGoalPostsLights[i][GPL_FRAMENO]++;
			}
			activeGoalPostsLights[i][GPL_SX] = (activeGoalPostsLights[i][GPL_FRAMENO] % goalpostslightsrowcol) * goalpostslightsframew;
			activeGoalPostsLights[i][GPL_SY] = (activeGoalPostsLights[i][GPL_FRAMENO] / goalpostslightscolumncol) * goalpostslightsframeh;
		} else activeGoalPostsLights.erase(activeGoalPostsLights.begin() + i);
	}
}

void gCanvas::updateGoalEvent() {
	waitcounter++;
	if(waitcounter % 60 == 0) {
		waitnumber--;
		if(waitnumber <= 0) {
			waitnumber = WAIT_SECOND;
			changeGameState(GAME_WAIT);
		}
	}
}

void gCanvas::updateSettingsDatabase(std::string datatype, int datavalue) {
	std::string updatestatement = "UPDATE optionst SET "+ datatype + " = " + gToStr(datavalue);
	root->database.execute(updatestatement);
	if(datatype == "difficultystate") root->difficultyvalue = datavalue;
	if(datatype == "musicstate") root->musicvalue = datavalue;
	if(datatype == "vibrationstate") root->vibrationvalue = datavalue;
}

void gCanvas::updateSliderPosition(int whichslider, int value) {
	if(whichslider == SLIDER_DIFFICULTY) {
		int diffx = denormalizeSlider(sliderminx[SLIDER_DIFFICULTY], slidermaxx[SLIDER_DIFFICULTY], difficultyvalue);
		slider[SLIDER_DIFFICULTY].x = diffx;

	}
	if(whichslider == SLIDER_MUSIC) {
		int musicx = denormalizeSlider(sliderminx[SLIDER_MUSIC], slidermaxx[SLIDER_MUSIC], musicvalue);
		slider[SLIDER_MUSIC].x = musicx;
	}
	if(whichslider == SLIDER_VIBRATION) {
		if(vibrationvalue == 1) slider[SLIDER_VIBRATION].x = slidermaxx[SLIDER_VIBRATION];
		else slider[SLIDER_VIBRATION].x = sliderminx[SLIDER_VIBRATION];
	}
}

int gCanvas::normalizeSlider(int minx, int maxx, int x) {
	return std::round((static_cast<double>(x - minx) * 100) / (maxx - minx));
}

int gCanvas::denormalizeSlider(int minx, int maxx, int value) {
    return std::round(((static_cast<double>(value) * (maxx - minx)) / 100) + minx);
}

float gCanvas::getBallSpeed() {
	return sqrt(ball.velocityx * ball.velocityx + ball.velocityy * ball.velocityy);
}
void gCanvas::checkGoal() {
	if (ball.x <= goalline[PLAYER_LEFT] - ball.radius) {
		score[PLAYER_RIGHT]++;
		goalEvent(PLAYER_LEFT);
	}
	if (ball.x >= goalline[PLAYER_RIGHT] + ball.radius) {
		score[PLAYER_LEFT]++;
		goalEvent(PLAYER_RIGHT);
	}
}

void gCanvas::startBall() {
    if (ismoving) {
        savedvelocityx = ball.velocityx;
        savedvelocityy = ball.velocityy;
        savedballframe = ballcurrentframe;
        savedballframetimer = ballframetimer;
        ball.velocityx = 0;
        ball.velocityy = 0;
        ismoving = false;
    } else {
        ball.velocityx = savedvelocityx;
        ball.velocityy = savedvelocityy;
        ballcurrentframe = savedballframe;
        ballframetimer = savedballframetimer;
        ismoving = true;
    }
}

void gCanvas::resetBall() {
    ball.x = getWidth() / 2;
    ball.y = getHeight() / 2;

    angle = gRandom(360) * (M_PI / 180);
    int randomdirectionx = (rand() % 2 == 0) ? -1 : 1;
    savedvelocityx = randomdirectionx * speed;
    savedvelocityy = sin(angle) * speed;
    ball.velocityx = 0;
    ball.velocityy = 0;
    savedballframe = 0;
    savedballframetimer = 0;
    ismoving = false;
}

void gCanvas::resetGame() {
	pudright.x = 1125 - pudright.w;
	pudright.y = (getHeight() - pudright.h) / 2;
	pudright.velocityy = 0;
	pudleft.x = 150;
	pudleft.y = (getHeight() - pudleft.h) / 2;
	pudleft.velocityy = 0;

	ismovingdownleft = false;
	ismovingdownright = false;
	ismovingupleft = false;
	ismovingupright = false;
}

bool gCanvas::checkPostCollision(Ball& ball, Post& post) {
	// top ve direk arasi mesafe
    dx = ball.x - post.x;
    dy = ball.y - post.y;
    distance = sqrt(dx * dx + dy * dy);

    return distance < (ball.radius + post.radius);
}


void gCanvas::reflectBall(Ball& ball, Post& post) {
	startHitAnimation(ball.x, ball.y);
	dx = ball.x - post.x;
	dy = ball.y - post.y;

	//normal vector
	normalvx = dx / distance;
	normalvy = dy / distance;
	// normal vectore gore yansima
	dotproduct = ball.velocityx * normalvx + ball.velocityy * normalvy;
	ball.velocityx -= 2 * dotproduct * normalvx;
	ball.velocityy -= 2 * dotproduct * normalvy;
}

void gCanvas::startHitAnimation(float x, float y) {
	soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_BALL_HIT);
	ishitanimating = true; //carpisma animasyonu oynatiliyor
	hitframe = 0; //baslangici sifirla
	hitframespeed = 0.03;
	hitframetimer = 0.0f; // animasyon zamanlayicisi
	hitanimx = x;
	hitanimy = y;
}

void gCanvas::startPudAnimation(int type) {
	if (type == LEFT) {
		pudanimationactiveleft = true;
		pudanimationactiveright = false;
		pudanimframeleft = 0;
		pudanimtimerleft = 0.0f;
	}
	else if (type == RIGHT) {
		pudanimationactiveright = true;
		pudanimationactiveleft = false;
		pudanimframeright = 0;
		pudanimtimerright = 0.0f;
	}
}

void gCanvas::checkPudCollision(Ball& ball, Pud& pud) {
    if (isColliding(ball, pud)) {
        float pudLeft = pud.x;
        float pudRight = pud.x + pud.w;
        float pudTop = pud.y;
        float pudBottom = pud.y + pud.h;

        if (ball.x + ball.radius <= pudLeft) {
            reflect(ball.velocityx, ball.velocityy, -1, 0);
            ball.x = pudLeft - ball.radius;
        }
        if (ball.x - ball.radius >= pudRight) {
            reflect(ball.velocityx, ball.velocityy, 1, 0);
            ball.x = pudRight + ball.radius;
        }
        if (ball.y + ball.radius <= pudTop) {
            reflect(ball.velocityx, ball.velocityy, 0, -1);
            ball.y = pudTop - ball.radius;
        }
        if (ball.y - ball.radius >= pudBottom) {
            reflect(ball.velocityx, ball.velocityy, 0, 1);
            ball.y = pudBottom + ball.radius;
        }
        ball.velocityy += pud.velocityy;
    }
}

void gCanvas::reflectBall(Ball &ball, Pud &pud) {
	closestSide(ball, pud);

	if (mindistance == disttop || mindistance == distbot) {
		ball.velocityy *= -1;
		ball.velocityy += pud.velocityy;
		ball.y = (mindistance == disttop) ? pud.y - ball.radius : pud.y + pud.h + ball.radius;
	}

	if (mindistance == distleft || mindistance == distright) {
		ball.velocityx *= -1;
		ball.velocityy += pud.velocityy;
		if(abs(ball.velocityy) > maxspeed)
		ball.velocityy *= maxspeed / ball.velocityy;
		ball.x = (mindistance == distleft) ? pud.x - ball.radius : pud.x + pud.w + ball.radius;
	}

	if(abs(ball.velocityx) < speed)
		ball.velocityx *= speed / ball.velocityx;
}

void gCanvas::closestSide(Ball &ball, Pud &pud) {
    disttop = abs(ball.y + ball.radius - pud.y);
    distbot = abs((ball.y - ball.radius) - pud.y - pud.h);
    distleft = abs(ball.x + ball.radius - pud.x);
    distright = abs(ball.x  - ball.radius - (pud.x + pud.w));

    mindistance = std::min({disttop, distbot, distleft, distright});
}

float gCanvas::calculateAngle(int velocityx, int velocityy) {
	if (velocityx == 0 && velocityy == 0) {
		return 0;
	}

	angleradians = atan2(velocityy, velocityx);
	angledegrees = angleradians * (180.0f / M_PI);

	return fmod(angledegrees + 360.0f, 360.0f);
}

void gCanvas::generateGoalPostsLight(int goalpostslightx, int goalpostslighty,
		int goalpostslightw, int goalpostslighth) {
	newgoalpostslight.push_back(goalpostslightx);
	newgoalpostslight.push_back(goalpostslighty);
	newgoalpostslight.push_back(goalpostslightw);
	newgoalpostslight.push_back(goalpostslighth);
	newgoalpostslight.push_back(0);  // Subx
	newgoalpostslight.push_back(0);  // Suby
	newgoalpostslight.push_back(0); // Frame No
	newgoalpostslight.push_back(0); // Counter

	activeGoalPostsLights.push_back(newgoalpostslight);
	newgoalpostslight.clear();
}

void gCanvas::waitEvent() {
	if(pregamestate != GAME_PAUSE) {
		resetBall();
		resetGame();
	}

	waitcounter++;
	if(waitcounter % 60 == 0) {
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
		waitnumber--;
		if(waitnumber < 0) {
			changeGameState(GAME_START);
			soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_WHISTLE);
			toggleBallMovement();
			waitnumber = WAIT_SECOND;
			waitcounter = 0;
		}
	}
}

void gCanvas::goalEvent(int whoscored) {
	soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_GOAL);
	if(whoscored == PLAYER_LEFT) {
		generateGoalPostsLight(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60);
		generateGoalPostsLight(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] + goalh[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 12), 60, 60);
	}

	if(whoscored == PLAYER_RIGHT) {
		generateGoalPostsLight(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60);
		generateGoalPostsLight(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] + goalh[PLAYER_LEFT] - (goalh[PLAYER_LEFT] / 8), 60, 60);
	}
	if(score[PLAYER_LEFT] >= 9) {
		changeGameState(GAME_WIN);
		boardtext = "Player 1 Win";
	}
	else if(score[PLAYER_RIGHT] >= 9) {
		changeGameState(GAME_LOSE);
		boardtext = "Player 2 Win";
	}
	else changeGameState(GAME_GOAL);
}

void gCanvas::selectPlayerPosition(int playerpos) {
	if(playerpos == PLAYER_LEFT) {
    	isuserleft = true;
        isuserright = false;
	}
	if(playerpos == PLAYER_RIGHT) {
    	isuserleft = false;
        isuserright = true;
	}
	if(playerpos == MODE_PVP) {
		isuserleft = true;
		isuserright = true;
	}
	changeGameState(GAME_WAIT);
	resetBall();
}

void gCanvas::soundControl(int musicvalue, int type, int sound) {
	float volume = (float)musicvalue / 100;
	musicstate = musicvalue <= 0 ? true : false;

	if(type == SOUND_TYPE_STARTING) {
		root->music.setVolume(volume);
		root->music.play();
		root->music.setPaused(musicstate);
	}
	if(type == SOUND_TYPE_SLIDER) {
		root->music.setVolume(volume);
		if(!(root->music.isPlaying())) root->music.play();
		root->music.setPaused(musicstate);
	}
	if(type == SOUND_TYPE_ONHIT) {
		if(sound == SOUND_BUTTON) {
			root->buttonsound.setVolume(volume);
			root->buttonsound.play();
		}
		if(sound == SOUND_CLICK) {
			root->clicksound.setVolume(volume);
			root->clicksound.play();
		}
		if(sound == SOUND_GOAL) {
//			root->goalsound.setVolume(volume);
//			root->goalsound.play();

			int randomgoal = (int)gRandom(GOAL_SOUND_COUNT);

			if(randomgoal == 1) {
				root->goalsound1.setVolume(volume);
				root->goalsound1.play();
			}
			else if(randomgoal == 2) {
				root->goalsound2.setVolume(volume);
				root->goalsound2.play();
			}
			else if(randomgoal == 3) {
				root->goalsound3.setVolume(volume);
				root->goalsound3.play();
			}
		}
		if(sound == SOUND_WHISTLE) {
			root->whistlesound.setVolume(volume);
			root->whistlesound.play();
		}
		if(sound == SOUND_BALL_HIT) {
			root->ballhitsound.setVolume(volume);
			root->ballhitsound.play();
		}
	}
}


void gCanvas::setupGame() {
	gamestate = GAME_SELECT_MODE;
	gamemode = MODE_NONE;
	difficulty = 1;
    ismovingupleft = false;
    ismovingdownleft = false;
    ismovingupright = false;
    ismovingdownright = false;
	isuserleft = true;
	isuserright = true;
}

void gCanvas::changeGameState(int gamestate) {
	pregamestate = this->gamestate;
	this->gamestate = gamestate;
}

void gCanvas::reflect(float &velocityX, float &velocityY, float normalX,
		float normalY) {
    float dotProduct = velocityX * normalX + velocityY * normalY;
    velocityX -= 2 * dotProduct * normalX;
    velocityY -= 2 * dotProduct * normalY;
}

void gCanvas::toggleBallMovement() {
    if (ismoving) {
        savedvelocityx = ball.velocityx;
        savedvelocityy = ball.velocityy;
        savedballframe = ballcurrentframe;
        savedballframetimer = ballframetimer;
        ball.velocityx = 0;
        ball.velocityy = 0;
        ismoving = false;
    } else {
        ball.velocityx = savedvelocityx;
        ball.velocityy = savedvelocityy;
        ballcurrentframe = savedballframe;
        ballframetimer = savedballframetimer;
        ismoving = true;
    }
}

bool gCanvas::isColliding(Ball &ball, Pud &pud) {
    float nextX = ball.x + ball.velocityx;
    float nextY = ball.y + ball.velocityy;

    float pudLeft = pud.x;
    float pudRight = pud.x + pud.w;
    float pudTop = pud.y;
    float pudBottom = pud.y + pud.h;

    return (nextX - ball.radius < pudRight && nextX + ball.radius > pudLeft &&
            nextY - ball.radius < pudBottom && nextY + ball.radius > pudTop);
}

void gCanvas::changeDifficulty(int difficultyvalue) {
	if(difficultyvalue >= 0 && difficultyvalue < 33) difficulty = 1;
	if(difficultyvalue > 33 && difficultyvalue < 66) difficulty = 2;
	if(difficultyvalue > 66 && difficultyvalue <= 100) difficulty = 3;

}
