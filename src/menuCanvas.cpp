/*
* mainMenu.cpp
*
*  Created on: September 6, 2024
*      Author: Kadir Semih Arslan
*/


#include <menuCanvas.h>
#include "gCanvas.h"

menuCanvas::menuCanvas(gApp *root) : gBaseCanvas(root) {
	this->root = root;
}

void menuCanvas::setup() {
	gamestate = GAME_NORMAL;
	setupBackground();
	setupButtons();
	setupLogo();
	setupGoal();
	setupOptionsMenu();
	setupGoalPostsLight();
	soundControl(musicvalue, SOUND_TYPE_STARTING);
}

void menuCanvas::update() {
}

void menuCanvas::draw() {
	drawBackground();
	drawButtons();
	drawLogo();
	drawGoal();
    drawGoalPostsLight();
	if(gamestate == BUTTON_OPTIONS) drawOptionsMenu();
}

void menuCanvas::mouseMoved(int x, int y) {
}

void menuCanvas::mousePressed(int x, int y, int button) {
	// Main menu buttons press
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + buttoncoordinategroup[i].h)) {
			if(gamestate == GAME_NORMAL) {
				buttoncoordinategroup[i].state = true;
			}
		}
	}

	// Slider press control
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(gamestate == BUTTON_OPTIONS && x > slider[i].x && x < slider[i].x + slider[i].w && y > slider[i].y && y < slider[i].y + slider[i].h){
			sliderselected[i] = true;
			soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
		}
	}

	// Option button
	if(x > opbutton[ACCEPT_BUTTON].x && x < (opbutton[ACCEPT_BUTTON].x + opbutton[ACCEPT_BUTTON].w) && y > opbutton[ACCEPT_BUTTON].y && y < (opbutton[ACCEPT_BUTTON].y + opbutton[ACCEPT_BUTTON].h)) {
		opbutton[ACCEPT_BUTTON].state = true;
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}
}

void menuCanvas::mouseReleased(int x, int y, int button) {
	// Main menu buttons release
	if(buttoncoordinategroup[BUTTON_PLAY].state == true) {
		gCanvas* main = new gCanvas(root);
		appmanager->setCurrentCanvas(main);

		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_BUTTON);
	}
	if(buttoncoordinategroup[BUTTON_OPTIONS].state == true) {
		buttoncoordinategroup[BUTTON_OPTIONS].state = false;
		gamestate = BUTTON_OPTIONS;
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}
	if(buttoncoordinategroup[BUTTON_RATE].state == true) {
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}
	if(buttoncoordinategroup[BUTTON_SHARE].state == true) {
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}
	if(buttoncoordinategroup[BUTTON_MORE_GAMES].state == true) {
		soundControl(musicvalue, SOUND_TYPE_ONHIT, SOUND_CLICK);
	}

	// Slider release control
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(sliderselected[i]) {
			sliderselected[i] = false;

			if(i == SLIDER_DIFFICULTY) {
				// If you release the slider it will send its data to the database.
				difficultyvalue = normalizeSlider(sliderminx[i], slidermaxx[i], slider[i].x);
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
				updateSettingsDatabase("vibrationstate", vibrationvalue);
			}
		}
	}

	// Option button

	if(opbutton[ACCEPT_BUTTON].state) {
		opbutton[ACCEPT_BUTTON].state = false;
		gamestate = GAME_NORMAL;
	}
}

void menuCanvas::mouseDragged(int x, int y, int button) {
	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(i != SLIDER_VIBRATION && sliderselected[i]) {
			if(slider[i].x >= sliderminx[i] && slider[i].x <= slidermaxx[i]) slider[i].x = x - slider[0].w / 2;
			if(slider[i].x < sliderminx[i]) slider[i].x = sliderminx[i];
			if(slider[i].x > slidermaxx[i]) slider[i].x = slidermaxx[i];
		}
	}
}

void menuCanvas::mouseScrolled(int x, int y) {
}

void menuCanvas::mouseEntered() {
}

void menuCanvas::mouseExited() {
}

void menuCanvas::setupBackground() {

	//mapleri yukleme
	for(int i = 0; i < maxmapimages; i++){
		background[i].loadImage("futbolassets/court_0" + gToStr(i + 1) + ".png");
	}
	backgroundx = 0;
	backgroundy = 0;
	backgroundw = getWidth();
	backgroundh = getHeight();

	//random map secimi
	randommapnum = gRandom(maxmapimages);

	//Orta cizgi yukleme
	mapcenterline.loadImage("futbolassets/court_center_line.png");
	mapcenterlinew = mapcenterline.getWidth();
	mapcenterlineh = mapcenterline.getHeight() - 91;
	mapcenterlinex = (getWidth() - mapcenterlinew) / 2 ;
	mapcenterliney = (getHeight() - mapcenterlineh) / 2 - 3;
}

void menuCanvas::setupButtons() {
	button[BUTTON_PLAY].loadImage("futbolassets/btn_menu_h_start.png");
	button[BUTTON_OPTIONS].loadImage("futbolassets/btn_menu_h_options.png");
	button[BUTTON_RATE].loadImage("futbolassets/btn_menu_h_rate.png");
	button[BUTTON_SHARE].loadImage("futbolassets/btn_menu_h_share.png");
	button[BUTTON_MORE_GAMES].loadImage("futbolassets/btn_menu_h_more_games.png");

	buttoncoordinategroup[BUTTON_PLAY].w = button[BUTTON_PLAY].getWidth() / 1.5f;
	buttoncoordinategroup[BUTTON_PLAY].h = (button[BUTTON_PLAY].getHeight() / 3.0f);
	buttoncoordinategroup[BUTTON_PLAY].sw = button[BUTTON_PLAY].getWidth();
	buttoncoordinategroup[BUTTON_PLAY].sh = button[BUTTON_PLAY].getHeight() / BUTTON_FRAMES;
	buttoncoordinategroup[BUTTON_PLAY].x = (getWidth() - buttoncoordinategroup[BUTTON_PLAY].w) / 2;
	buttoncoordinategroup[BUTTON_PLAY].y = (getHeight() - buttoncoordinategroup[BUTTON_PLAY].h) / 2.0f;
	buttoncoordinategroup[BUTTON_PLAY].state = false;

	for(int i = 1; i < BUTTON_COUNT; i++) {
		buttoncoordinategroup[i].w = button[i].getWidth() / 1.5f;
		buttoncoordinategroup[i].h = button[i].getHeight() / 2;
		buttoncoordinategroup[i].sw = button[i].getWidth();
		buttoncoordinategroup[i].sh = button[i].getHeight() / BUTTON_FRAMES;
		buttoncoordinategroup[i].x = buttoncoordinategroup[BUTTON_PLAY].x + (buttoncoordinategroup[BUTTON_PLAY].sw * 1.20f) - ((buttoncoordinategroup[i].sw / 1.25f) * (i - 1));
		buttoncoordinategroup[i].y = buttoncoordinategroup[BUTTON_PLAY].y + (buttoncoordinategroup[BUTTON_PLAY].sh / 1.5f);
		buttoncoordinategroup[i].state = false;
	}
}

void menuCanvas::setupLogo() {
	courtroof.loadImage("futbolassets/court_roof.png");
	courtroofw = courtroof.getWidth();
	courtroofh = courtroof.getHeight() / 1.25f;
	courtroofx = (getWidth() - courtroofw) / 2;
	courtroofy = 0;

	logo.loadImage("futbolassets/logo.png");
	logow = logo.getWidth() / 1.25f;
	logoh = logo.getHeight() / 1.25f;
	logox = (courtroofx + courtroofw) - (courtroofw + logow ) / 2;
	logoy = courtroofy + (courtroofh / 7);
}

void menuCanvas::setupGoal() {
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
}

void menuCanvas::setupOptionsMenu() {
	// Get database values from root
	musicvalue = root->musicvalue;
	difficultyvalue = root->difficultyvalue;
	vibrationvalue = root->vibrationvalue;

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

void menuCanvas::setupGoalPostsLight() {
	goalpostslights.loadImage("futbolassets/goal_posts_lights.png");
	goalpostslightsrowcol = 5;
	goalpostslightscolumncol = 4;

	goalpostslightsframew = goalpostslights.getWidth() / 5;
	goalpostslightsframeh = goalpostslights.getHeight() / 4;
	goalpostslightsmaxframe = 20;
}

void menuCanvas::drawBackground() {
	background[randommapnum].draw(backgroundx, backgroundy, backgroundw, backgroundh);
	mapcenterline.draw(mapcenterlinex, mapcenterliney, mapcenterlinew, mapcenterlineh);
}

void menuCanvas::drawButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		button[i].drawSub(buttoncoordinategroup[i].x, buttoncoordinategroup[i].y, buttoncoordinategroup[i].w, buttoncoordinategroup[i].h, 0, buttoncoordinategroup[i].sh * buttoncoordinategroup[i].state, buttoncoordinategroup[i].sw, buttoncoordinategroup[i].sh);
	}
}

void menuCanvas::drawLogo() {
	courtroof.draw(courtroofx, courtroofy, courtroofw, courtroofh);
	logo.draw(logox, logoy, logow, logoh);
}

void menuCanvas::drawGoal() {
	goal[PLAYER_LEFT].draw(goalx[PLAYER_LEFT], goaly[PLAYER_LEFT], goalw[PLAYER_LEFT], goalh[PLAYER_LEFT], 180);
	goal[PLAYER_RIGHT].draw(goalx[PLAYER_RIGHT], goaly[PLAYER_RIGHT], goalw[PLAYER_RIGHT], goalh[PLAYER_RIGHT]);
}

void menuCanvas::drawOptionsMenu() {
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

void menuCanvas::drawGoalPostsLight() {
	goalpostslights.drawSub(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, 90);
	goalpostslights.drawSub(goalx[PLAYER_RIGHT] + (goalw[PLAYER_RIGHT] / 3), goaly[PLAYER_RIGHT] + goalh[PLAYER_RIGHT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 12), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, 180);
	goalpostslights.drawSub(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] - ((goalpostslights.getWidth() / goalpostslightsframew) * 4), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh);
	goalpostslights.drawSub(goalx[PLAYER_LEFT] + (goalw[PLAYER_LEFT] / 12), goaly[PLAYER_LEFT] + goalh[PLAYER_LEFT] - (goalh[PLAYER_LEFT] / 8), 60, 60, 0, 0, goalpostslightsframew, goalpostslightsframeh, -90);
}

void menuCanvas::updateSliderPosition(int whichslider, int value) {
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

void menuCanvas::updateSettingsDatabase(std::string datatype, int datavalue) {
	std::string updatestatement = "UPDATE optionst SET "+ datatype + " = " + gToStr(datavalue);
	root->database.execute(updatestatement);
	if(datatype == "difficultystate") root->difficultyvalue = datavalue;
	if(datatype == "musicstate") root->musicvalue = datavalue;
	if(datatype == "vibrationstate") root->vibrationvalue = datavalue;
}

int menuCanvas::normalizeSlider(int minx, int maxx, int x) {
	return std::round((static_cast<double>(x - minx) * 100) / (maxx - minx));
}

int menuCanvas::denormalizeSlider(int minx, int maxx, int value) {
    return std::round(((static_cast<double>(value) * (maxx - minx)) / 100) + minx);
}

void menuCanvas::soundControl(int musicvalue, int type, int sound) {
	float volume = (float)musicvalue / 100;
	musicstate = musicvalue <= 0 ? true : false;

	if(type == SOUND_TYPE_STARTING) {
		root->music.setVolume(volume / 2);
		root->music.play();
		root->music.setPaused(musicstate);
	}
	if(type == SOUND_TYPE_SLIDER) {
		root->music.setVolume(volume);
		if(!(root->music.isPlaying())) root->music.play();
		root->music.setPaused(musicstate);
	}
	if(type == SOUND_TYPE_ONHIT && !musicstate) {
		if(sound == SOUND_BUTTON) {
			root->buttonsound.setVolume(volume);
			root->buttonsound.play();
		}
		if(sound == SOUND_CLICK) {
			root->clicksound.setVolume(volume);
			root->clicksound.play();
		}
	}
}
