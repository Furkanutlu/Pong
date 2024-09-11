/*
* gCanvas.cpp
*
*  Created on: September 6, 2024
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
	gamestate = GAME_SELECT_MODE;
	gamemode = MODE_NONE;
	musicstate = root->musicstate;
	difficultystate = root->difficultystate;
	vibrationstate = root->vibrationstate;
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
		updateScore();
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
	drawBall();
	drawGoal();
	drawPuds();
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
	if (key == '1') {
		isuserleft = true;
		isuserright = false;
	}
	if (key == '2') {
		isuserleft = false;
		isuserright = true;
	}
	if (key == 265 || key == 87) {
	   if (isuserright) {
		   ismovingupright = true;
	   } else if (isuserleft) {
		   ismovingupleft = true;
	   }
   }
   if (key == 264 || key == 83) {
	   if (isuserright) {
		   ismovingdownright = true;
	   } else if (isuserleft) {
		   ismovingdownleft = true;
	   }
   }
}
void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
	if (key == 265 || key == 87) { // Up arrow or 'W'
		if (isuserright) {
			ismovingupright = false;
		} else if (isuserleft) {
			ismovingupleft = false;
		}
	}
	if (key == 264 || key == 83) { // Down arrow or 'S'
		if (isuserright) {
			ismovingdownright = false;
		} else if (isuserleft) {
			ismovingdownleft = false;
		}
	}
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
	if((x - pausecx) * (x - pausecx) + (y - pausecy) * (y - pausecy) <= pauseradius * pauseradius) {
		pausestate = true;
	}
	else {
		pausestate = false;
	}
}

void gCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
//	can be added for music

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
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + (buttoncoordinategroup[i].h / 2))) {
			switch(i) {
				case 0:
					buttoncoordinategroup[i].hold = true;
					break;
				case 1:
					buttoncoordinategroup[i].hold = true;
					break;
				case 2:
					buttoncoordinategroup[i].hold = true;
					break;
			}
		}
	}

	// Game End Buttons
	if(gamestate == GAME_LOSE || gamestate == GAME_WIN) {
		for(int i = 0; i < BUTTON_COUNT - 1; i++) {
			if(x > (buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2)) && x < (((buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2))) + buttonendcoordinategroup[i].w) && y > (buttonendcoordinategroup[i].y + endboardheaderh) && y < ((buttonendcoordinategroup[i].y + endboardheaderh) + (buttonendcoordinategroup[i].h / 2))) {
				if(i == BUTTON_REPLAY) {
					buttonendcoordinategroup[i].hold = true;
				}
				if(i == BUTTON_HOME) {
					buttonendcoordinategroup[i].hold = true;
				}
			}
		}
	}

	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		if(gamestate == GAME_OPTIONS && x > opbutton[i].x && x < (opbutton[i].x + opbutton[i].w) && y > opbutton[i].y && y < (opbutton[i].y + opbutton[i].h / 2)) {
			opbuttonselected[i] = true;
		}
	}

	for(int i = 0; i < maxplayernum; i++) {
		if(gamestate == GAME_SELECT_MODE && x > modebutton[i].x && x < (modebutton[i].x + modebutton[i].w) && y > modebutton[i].y && y < (modebutton[i].y + modebutton[i].h / 2)) {
			modebutton[i].selected = true;
		}
	}
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;

//----- The codes below is for control purposes.

//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;

//	if(button == 0) goalEvent(PLAYER_RIGHT);
//	else goalEvent(PLAYER_LEFT);

//	if(button == 0) gamestate = GAME_GOAL;
//	if(button == 1) gamestate = GAME_LOSE;
//	else gamestate = GAME_WIN;

//	gLogi("FrameX ") << std::to_string(score[PLAYER_LEFT] % scorenumberscolumncol) << " FrameY " << std::to_string(score[PLAYER_LEFT] / scorenumberscolumncol);

//----- The codes above is for control purposes.

	// Pause Button
	if(gamestate != GAME_SELECT_MODE && x > (pausebuttonx - (pausebuttonw * 1.25f)) && x < (pausebuttonx + pausebuttonw + (pausebuttonw * 1.25f)) && y > (pausebuttony - (pausebuttonw * 1.25f)) && y < (pausebuttony + pausebuttonh + (pausebuttonw * 1.25f))) {
		if(gamestate != GAME_PAUSE) gamestate = GAME_PAUSE;
		else gamestate = GAME_START;
		if(musicstate) root->clicksound.play();
	}

	// Other Buttons
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(gamestate == GAME_PAUSE && x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + (buttoncoordinategroup[i].h / 2))) {
			if(i == BUTTON_REPLAY) {
				if(musicstate) root->clicksound.play(); // If this code is not here, there will be no sound.
				buttoncoordinategroup[i].state = true;
				gCanvas* replay = new gCanvas(root);
				appmanager->setCurrentCanvas(replay);
				gLogi("Here");
			}
			if(i == BUTTON_HOME) {
				menuCanvas* main = new menuCanvas(root);
				appmanager->setCurrentCanvas(main);
			}
			if(i == BUTTON_OPTIONS) {
				gamestate = GAME_OPTIONS;
				if(musicstate) root->clicksound.play();
			}
		}
	}

	// Game End Buttons
	if(gamestate == GAME_LOSE || gamestate == GAME_WIN) {
		for(int i = 0; i < BUTTON_COUNT; i++) {
			if(x > (buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2)) && x < (((buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2))) + buttonendcoordinategroup[i].w) && y > (buttonendcoordinategroup[i].y + endboardheaderh) && y < ((buttonendcoordinategroup[i].y + endboardheaderh) + (buttonendcoordinategroup[i].h / 2))) {
				if(i == BUTTON_REPLAY) {
					if(musicstate) root->clicksound.play(); // If this code is not here, there will be no sound.
					buttoncoordinategroup[i].state = true;
					gCanvas* replay = new gCanvas(root);
					appmanager->setCurrentCanvas(replay);
				}
				if(i == BUTTON_HOME) {
					menuCanvas* main = new menuCanvas(root);
					appmanager->setCurrentCanvas(main);
				}

				if(musicstate) root->clicksound.play();
			}
		}
	}

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(gamestate == GAME_OPTIONS && x > slider[i].x && x < slider[i].x + slider[i].w && y > slider[i].y && y < slider[i].y + slider[i].h){
			sliderselected[i] = !sliderselected[i];
			if(sliderselected[i]) slider[i].x = slidermaxx[i];
			else slider[i].x = sliderminx[i];

			if(i == SLIDER_DIFFICULTY) {
				difficultystate = sliderselected[i];
				predifficultystate = !sliderselected[i];
			}
			if(i == SLIDER_MUSIC) {
				musicstate = sliderselected[i];
				premusicstate = !sliderselected[i];

				root->music.setPaused(!musicstate);
			}
			if(i == SLIDER_VIBRATION) {
				vibrationstate = sliderselected[i];
				previbrationstate = !sliderselected[i];
			}

			if(musicstate) root->clicksound.play();
		}
	}

	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		if(gamestate == GAME_OPTIONS && x > opbutton[i].x && x < (opbutton[i].x + opbutton[i].w) && y > opbutton[i].y && y < (opbutton[i].y + opbutton[i].h / 2)) {
			if(i == ACCEPT_BUTTON) {
				updateSettingsDatabase("difficultystate", difficultystate);
				updateSettingsDatabase("musicstate", musicstate);
				updateSettingsDatabase("vibrationstate", vibrationstate);
			}
			if(i == DECLINE_BUTTON) {
				musicstate = premusicstate;
				difficultystate = predifficultystate;
				vibrationstate = previbrationstate;
			}

			if(musicstate) root->clicksound.play();
			gamestate = GAME_PAUSE;
		}
	}

    if (button == MOUSEBUTTON_LEFT) {
        pudleft.velocityy = 0;
        pudright.velocityy = 0;
    }

	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(buttoncoordinategroup[i].hold) buttoncoordinategroup[i].hold = false;
	}
	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		if(opbuttonselected[i]) opbuttonselected[i] = false;
	}

	for(int i = 0; i < maxplayernum; i++) {
		if(gamestate == GAME_SELECT_MODE && x > modebutton[i].x && x < (modebutton[i].x + modebutton[i].w) && y > modebutton[i].y && y < (modebutton[i].y + modebutton[i].h / 2)) {
			if(gamemode == MODE_NONE) {
				if(i == 0) {
					modebutton[i].selected = false;
					gamemode = MODE_PVP;
					gamestate = GAME_START;
				}
				else {
					modebutton[i].selected = false;
					gamemode = MODE_PVE;
					gamestate = GAME_START;

					selecttext[0] = "Player Left";
					selecttext[1] = "Player Right";
				}
			}
			if(gamemode == MODE_PVE) {
				if(i == 0) {
					modebutton[i].selected = false;
					gamemode = MODE_PVP;
					gamestate = GAME_START;
				}
				else {
					modebutton[i].selected = false;
					gamemode = MODE_PVE;
					gamestate = GAME_START;
				}
			}

			selectGameMode(gamestate, 0);
			resetBall();
		}
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
	speed = 10.0f;
	maxspeed = 15.0f;
	angle = gRandom(360) * (M_PI / 180); //ilk yonu ve hizi
	ball.velocityx = cos(angle) * speed;
	ball.velocityy = sin(angle) * speed;
	savedvelocityx = ball.velocityx;
	savedvelocityy = ball.velocityy;
	ismoving = false;
	ishitanimating = false;
	ball.radius = ball.h / 2;
	ballcurrentframe = 0;
	ballframetimer = 0.0f;
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

	pudleft.w = pudleftimage.getWidth() / 5;
	pudleft.h = pudleftimage.getHeight();
	pudleft.x = 220;
	pudleft.y = (getHeight() - pudleft.h) / 2;
	pudleft.velocityy = 0;
	pudanimationactiveleft = false;

	pudright.w = pudrightimage.getWidth() / 5;
	pudright.h = pudrightimage.getHeight();
	pudright.x = 1055 - pudright.w;
	pudright.y = (getHeight() - pudright.h) / 2;
	pudright.velocityy = 0;
	pudanimationactiveright = false;
}

void gCanvas::drawMap() {
	map[randommapnum].draw(mapx, mapy, mapw, maph);
	mapcenterline.draw(mapcenterlinex, mapcenterliney, mapcenterlinew, mapcenterlineh);
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
void gCanvas::drawPuds() {
    pudleftframex = pudanimframeleft * pudleft.w;
    pudleftimage.drawSub(pudleft.x, pudleft.y, pudleftframex, 0, pudleft.w, pudleft.h);
    pudrightframex = pudanimframeright * pudright.w;
    pudrightimage.drawSub(pudright.x, pudright.y, pudrightframex, 0, pudright.w, pudright.h);
}
void gCanvas::drawGoal() {
	goal[PLAYER_LEFT].draw(goalx[PLAYER_LEFT], goaly[PLAYER_LEFT], goalw[PLAYER_LEFT], goalh[PLAYER_LEFT], 180);
	goal[PLAYER_RIGHT].draw(goalx[PLAYER_RIGHT], goaly[PLAYER_RIGHT], goalw[PLAYER_RIGHT], goalh[PLAYER_RIGHT]);
}

void gCanvas::updateBallPosition() {
	checkGoal();
	 if(ismoving){ // direkler
		 if(ball.y > ustdirek && ball.y < altdirek){
			 for (int i = 0; i < 2; i++) {
				if (checkPostCollision(ball, goalpostleft[i])) {
					reflectBall(ball, goalpostleft[i]);
				}
				if (checkPostCollision(ball, goalpostright[i])) {
					reflectBall(ball, goalpostright[i]);
				}
			}
		 }
		 else {// map sinirlari
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
		// pudlar
		if (checkPudCollision(ball, pudleft)) {
			if (!pudanimationactiveleft) {
				startPudAnimation(LEFT);
				startHitAnimation(ball.x, ball.y);
			}
			reflectBall(ball, pudleft);
		}

		if (checkPudCollision(ball, pudright)) {
			if (!pudanimationactiveright) {
				startPudAnimation(RIGHT);
				startHitAnimation(ball.x, ball.y);
			}
			reflectBall(ball, pudright);
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
    }
    else {
        ball.velocityx = savedvelocityx;
        ball.velocityy = savedvelocityy;
        ballcurrentframe = savedballframe;
        ballframetimer = savedballframetimer;
        ismoving = true;
    }
}

void gCanvas::resetBall() {
	if(musicstate) root->whistlesound.play();
    ball.x = getWidth() / 2;
    ball.y = getHeight() / 2;
	angle = gRandom(360) * (M_PI / 180);
	savedvelocityx = cos(angle) * speed;
	savedvelocityy = sin(angle) * speed;
    ball.velocityx = 0;
    ball.velocityy = 0;
    ismoving = false; // Top durdu
    startBall();
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
	if(musicstate) root->ballhitsound.play();
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

bool gCanvas::checkPudCollision(Ball &ball, Pud &pud) {
	return (ball.x + ball.radius >= pud.x && ball.x - ball.radius <= pud.x + pud.w &&
	                ball.y + ball.radius >= pud.y && ball.y - ball.radius <= pud.y + pud.h);
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

void gCanvas::updateBot() {
	bool ballinrightside = ball.x > getWidth() / 2;
	if (isuserleft) {
		if (!isuserright) {
			if(ballinrightside){
				botcentery = pudright.y + pudright.h / 2;
				ballcentery = ball.y;

				targety = ballcentery - pudright.h / 2;
				errormargin = 50.0f;

				if (ballcentery < pudright.y + errormargin) {
					pudright.y -= 5.0f;
				} else if (ballcentery > pudright.y + pudright.h - errormargin) {
					pudright.y += 5.0f;
				}

				pudright.y = std::max(pudright.y, 115.0f);
				pudright.y = std::min(pudright.y, 600 - pudright.h);
			}

		}
	}
	else if (isuserright) {
		if (!isuserleft) {
			if(!ballinrightside){
				botcentery = pudleft.y + pudleft.h / 2;
				ballcentery = ball.y;

				targety = ballcentery - pudleft.h / 2;
				errormargin = 50.0f;
				if (ballcentery < pudleft.y + errormargin) {
				pudleft.y -= 5.0f;
			} else if (ballcentery > pudleft.y + pudleft.h - errormargin) {
				pudleft.y += 5.0f;
			}
				pudleft.y = std::max(pudleft.y, 115.0f);
				pudleft.y = std::min(pudleft.y, 600 - pudleft.h);
			}
		}
	}
}

void gCanvas::updatePudControl() {
    if (ismovingupleft) {
        pudleft.y -= movespeed;
        pudleft.y = std::max(pudleft.y, 115.0f);
    }
    if (ismovingupright) {
        pudright.y -= movespeed;
        pudright.y = std::max(pudright.y, 115.0f);
    }

    if (ismovingdownleft) {
        pudleft.y += movespeed;
        pudleft.y = std::min(pudleft.y, 600 - pudleft.h);
    }
    if (ismovingdownright) {
        pudright.y += movespeed;
        pudright.y = std::min(pudright.y, 600 - pudright.h);
    }
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

void gCanvas::updateScore() {
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

void gCanvas::setupGoalPostsLight() {
	lightactive = false;

	goalpostslights.loadImage("futbolassets/goal_posts_lights.png");
	goalpostslightsrowcol = 5;
	goalpostslightscolumncol = 4;

	goalpostslightsframew = goalpostslights.getWidth() / 5;
	goalpostslightsframeh = goalpostslights.getHeight() / 4;
	goalpostslightsmaxframe = 20;
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

void gCanvas::setupGoalEvent() {
	goalimage.loadImage("futbolassets/text_goal.png");

	goalimagex = (getWidth() - goalimage.getWidth()) / 2;
	goalimagey = (getHeight() - goalimage.getHeight()) / 2;
	goalimagew = goalimage.getWidth();
	goalimageh = goalimage.getHeight();

	waitcounter = 0;
	waitnumber = WAIT_SECOND;
	goalscore = false;
}

void gCanvas::updateGoalEvent() {
	goalscore = true;
	waitcounter++;
	if(waitcounter % 60 == 0) {
		waitnumber--;
		if(waitnumber <= 0) {
			waitnumber = WAIT_SECOND;
			gamestate = GAME_WAIT;
		}
	}
}

void gCanvas::drawGoalEvent() {
	goalimage.draw(goalimagex, goalimagey, goalimagew, goalimageh);
}

void gCanvas::waitEvent() {
//	if(musicstate) root->goalsound.stop();
	waitcounter++;
	if(waitcounter % 60 == 0) {
		if(musicstate) root->clicksound.play();
		waitnumber--;
		if(waitnumber < 0) {
			gamestate = GAME_START;
			if(goalscore) {
				resetBall();
				goalscore = false;
			}
			waitnumber = WAIT_SECOND;
		}
	}
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

void gCanvas::goalEvent(int whoscored) {
//	if(musicstate) root->goalsound.play(); // Add goal song here.
	if(whoscored == PLAYER_LEFT) {
		generateGoalPostsLight(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60);
		generateGoalPostsLight(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] + goalh[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 12), 60, 60);
	}

	if(whoscored == PLAYER_RIGHT) {
		generateGoalPostsLight(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60);
		generateGoalPostsLight(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] + goalh[PLAYER_LEFT] - (goalh[PLAYER_LEFT] / 8), 60, 60);
	}
	if(score[PLAYER_LEFT] >= 9) {
		gamestate = GAME_WIN;
		boardtext = "Player 1 Win";
	}
	else if(score[PLAYER_RIGHT] >= 9) {
		gamestate = GAME_LOSE;
		boardtext = "Player 2 Win";
	}
	else gamestate = GAME_GOAL;
}

void gCanvas::setupPauseMenu() {
	button[0].loadImage("futbolassets/btn_round_again.png");
	button[1].loadImage("futbolassets/btn_round_home.png");
	button[2].loadImage("futbolassets/btn_round_options.png");
	// Pause
	pausebutton.loadImage("futbolassets/btn_pause.png");
	pausebuttonw = pausebutton.getWidth();
	pausebuttonh = pausebutton.getHeight();
	pausebuttonx = getWidth() - (pausebuttonw * 2);
	pausebuttony = pausebuttonh;
	pausecx =  pausebuttonx + (pausebuttonh / 2);
	pausecy = pausebuttony + (pausebuttonh / 2);
	pauseradius = pausebuttonw * 1.25f;

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
		buttoncoordinategroup[i].w = button[i].getWidth() / 1.5f;
		buttoncoordinategroup[i].h = button[i].getHeight() / 1.5f;
		buttoncoordinategroup[i].x = boardx + ((boardw / 2) - (buttoncoordinategroup[i].w * 1.65f)) + ((buttoncoordinategroup[i].w * i) * 1.15f);
		buttoncoordinategroup[i].y = boardy + ((boardh / 2) - (buttoncoordinategroup[i].h / 4));
		buttoncoordinategroup[i].sw = button[i].getWidth();
		buttoncoordinategroup[i].sh = button[i].getWidth();
		buttoncoordinategroup[i].sx = 0;
		buttoncoordinategroup[i].sy = button[i].getHeight() / 2;
		buttoncoordinategroup[i].hold = false;
	}
}

void gCanvas::drawPauseMenu() {
	if(pausestate) renderer->setColor(161, 102, 47);
	else renderer->setColor(181, 122, 67);
	gDrawCircle(pausecx, pausecy, pauseradius, true);
	renderer->setColor(255, 255, 255);

	if(pausestate) renderer->setColor(0, 200, 0);
	pausebutton.draw(pausebuttonx, pausebuttony, pausebuttonw, pausebuttonh);
	renderer->setColor(255, 255, 255);

	if(gamestate == GAME_PAUSE || gamestate == GAME_OPTIONS) {
		board.draw(boardx, boardy, boardw, boardh);
		boardheader.draw(boardheaderx, boardheadery, boardheaderw, boardheaderh);
		boardfont.drawText(boardtext, boardheaderx + ((boardheaderw - boardfont.getStringWidth(boardtext)) / 2) , boardheadery + ((boardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));
	}

	if(gamestate == GAME_PAUSE) {
		boardtext = "Paused";
		for(int i = 0; i < BUTTON_COUNT; i++) {
			button[i].drawSub(buttoncoordinategroup[i].x, buttoncoordinategroup[i].y, buttoncoordinategroup[i].w, buttoncoordinategroup[i].w,
						buttoncoordinategroup[i].sx, buttoncoordinategroup[i].sy * buttoncoordinategroup[i].hold, buttoncoordinategroup[i].sw, buttoncoordinategroup[i].sh);
		}
	}
}

void gCanvas::setupOptionsMenu() {
	optionsicon[0].loadImage("futbolassets/option_icon_difficulty.png");
	optionsicon[1].loadImage("futbolassets/option_icon_sound.png");
	optionsicon[2].loadImage("futbolassets/option_icon_vibrations.png");

	sliderbackground.loadImage("futbolassets/slider_bg.png");
	sliderimg[0].loadImage("futbolassets/slider_off.png");
	sliderimg[1].loadImage("futbolassets/slider_on.png");

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

	sliderselected[SLIDER_DIFFICULTY] = difficultystate;
	sliderselected[SLIDER_MUSIC] = musicstate;
	sliderselected[SLIDER_VIBRATION] = vibrationstate;

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(sliderselected[i] == true) slider[i].x = slidermaxx[i];
		else slider[i].x = sliderminx[i];
	}

	optionsfont.loadFont("FreeSansBold.ttf", 18);
	optionstext[0] = "Difficulty";
	optionstext[1] = "Music";
	optionstext[2] = "Vibration";

	opbuttons[DECLINE_BUTTON].loadImage("futbolassets/btn_round_no.png");
	opbuttons[ACCEPT_BUTTON].loadImage("futbolassets/btn_round_ok.png");

	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		opbutton[i].w = opbuttons[i].getWidth() / 3;
		opbutton[i].h = opbuttons[i].getHeight() / 3;
		opbutton[i].y = boardy + (boardh - (opbutton[i].w * 1.35f));
		opbutton[i].sw = opbuttons[i].getWidth();
		opbutton[i].sh = opbuttons[i].getHeight() / 2;
	}

	opbutton[DECLINE_BUTTON].x = boardx + ((boardw - opbutton[0].w) / 2) - (opbutton[0].w * 2);
	opbutton[ACCEPT_BUTTON].x = boardx + ((boardw - opbutton[1].w) / 2) + (opbutton[1].w * 2);
}

void gCanvas::drawOptionsMenu() {
	if(gamestate == GAME_OPTIONS) {
		boardtext = "Options";
		for(int i = 0; i < BUTTON_COUNT; i++) {
			optionsimg[i].draw(optionsbg[i].x, optionsbg[i].y, optionsbg[i].w, optionsbg[i].h);
			sliderbackground.draw(sliderbg[i].x, sliderbg[i].y, sliderbg[i].w, sliderbg[i].h);
			optionsicon[i].draw(opicon[i].x + (opicon[i].w / opicon[i].w), opicon[i].y + ((optionsbg[i].h - opicon[i].h) / 2), opicon[i].w, opicon[i].h);
			sliderimg[sliderselected[i]].drawSub(slider[i].x, slider[i].y, slider[i].w, slider[i].w, 0, 0, slider[i].sw, slider[i].sh / 2);

			renderer->setColor(0, 0, 0);
			optionsfont.drawText(optionstext[i], (optionsbg[i].x + (opicon[i].w * 2)), sliderbg[i].y + (sliderbg[i].h / 1.8f));
			renderer->setColor(255, 255, 255);
		}
		opbuttons[DECLINE_BUTTON].drawSub(opbutton[DECLINE_BUTTON].x, opbutton[DECLINE_BUTTON].y, opbutton[DECLINE_BUTTON].w, opbutton[DECLINE_BUTTON].w, 0, (opbutton[DECLINE_BUTTON].sh * opbuttonselected[DECLINE_BUTTON]), opbutton[DECLINE_BUTTON].sw, opbutton[DECLINE_BUTTON].sh);
		opbuttons[ACCEPT_BUTTON].drawSub(opbutton[ACCEPT_BUTTON].x, opbutton[ACCEPT_BUTTON].y, opbutton[ACCEPT_BUTTON].w, opbutton[ACCEPT_BUTTON].w, 0, (opbutton[ACCEPT_BUTTON].sh * opbuttonselected[ACCEPT_BUTTON]), opbutton[ACCEPT_BUTTON].sw, opbutton[ACCEPT_BUTTON].sh);
	}
}

int gCanvas::normalizeSlider(int minx, int maxx, int x) {
	return std::round((static_cast<double>(x - minx) * 100) / (maxx - minx));
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
		buttonendcoordinategroup[i].w = button[i].getWidth() / 1.5f;
		buttonendcoordinategroup[i].h = button[i].getHeight() / 1.5f;
		buttonendcoordinategroup[i].x = boardx + ((boardw / 2) - (buttoncoordinategroup[i].w * 1.65f)) + ((buttoncoordinategroup[i].w * i) * 1.15f);
		buttonendcoordinategroup[i].y = boardy + ((boardh / 2) - (buttoncoordinategroup[i].h / 4));
		buttonendcoordinategroup[i].sw = button[i].getWidth();
		buttonendcoordinategroup[i].sh = button[i].getWidth();
		buttonendcoordinategroup[i].sx = 0;
		buttonendcoordinategroup[i].sy = button[i].getHeight() / 2;
		buttonendcoordinategroup[i].hold = false;
	}
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
		button[i].drawSub(buttonendcoordinategroup[i].x + (buttonendcoordinategroup[i].w / 2), buttonendcoordinategroup[i].y + endboardheaderh, buttonendcoordinategroup[i].w, buttonendcoordinategroup[i].w,
				buttonendcoordinategroup[i].sx, buttonendcoordinategroup[i].sy * buttonendcoordinategroup[i].hold, buttonendcoordinategroup[i].sw, buttonendcoordinategroup[i].sh);
	}

	if(gamestate == GAME_LOSE) youloseimage.draw(youloseimagex, youloseimagey - (boardh / 2.5f), youloseimagew, youloseimageh);
	if(gamestate == GAME_WIN) youwinimage.draw(youwinimagex, youwinimagey - (boardh / 2.5f), youwinimagew, youwinimageh);
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
		modebutton[i].selected = false;
	}
	selecttext[PLAYER_LEFT] = "Player vs Player";
	selecttext[PLAYER_RIGHT] = "Player vs NPC";
}

void gCanvas::drawGameMode() {
	boardtext = "Select Mode";

	board.draw(boardx, boardy, boardw, boardh);
	boardheader.draw(boardheaderx, boardheadery, boardheaderw, boardheaderh);
	boardfont.drawText(boardtext, boardheaderx + ((boardheaderw - boardfont.getStringWidth(boardtext)) / 2) , boardheadery + ((boardheaderh - boardfont.getStringHeight(boardtext)) / 1.15f));

	for(int i = 0; i < maxplayernum; i++) {
		panelbutton.drawSub(modebutton[i].x, modebutton[i].y, modebutton[i].w, modebutton[i].h / 2, 0, modebutton[i].h * modebutton[i].selected, modebutton[i].sw, modebutton[i].h);

		renderer->setColor(0, 0, 0);
		selectfont.drawText(selecttext[i], modebutton[i].x + ((modebutton[i].w - selectfont.getStringWidth(selecttext[i])) / 2), modebutton[i].y + ((modebutton[i].h - selectfont.getStringHeight(selecttext[i])) / 3));
		renderer->setColor(255, 255, 255);
	}
}

void gCanvas::selectGameMode(int gamemode, int playerpos) {
    if (gamemode == MODE_PVP) {
        isuserleft = false;
        isuserright = false;
    }
    if (gamemode == MODE_PVE) {
    	isuserleft = true;
        isuserright = false;
    }
}

void gCanvas::updateSettingsDatabase(std::string datatype, int datavalue) {
	std::string updatestatement = "UPDATE optionst SET "+ datatype + " = " + gToStr(datavalue);
	root->database.execute(updatestatement);
	if(datatype == "difficultystate") root->difficultystate = datavalue;
	if(datatype == "musicstate") root->musicstate = datavalue;
	if(datatype == "vibrationstate") root->vibrationstate = datavalue;
}

void gCanvas::soundsoff() {
}
