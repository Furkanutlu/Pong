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
	root->music.play();
	root->music.setPaused(!musicstate);
}

void menuCanvas::update() {
}

void menuCanvas::draw() {
	drawBackground();
	drawButtons();
	drawLogo();
	drawGoal();
	if(gamestate == BUTTON_OPTIONS) drawOptionsMenu();
}

void menuCanvas::mouseMoved(int x, int y) {
}

void menuCanvas::mousePressed(int x, int y, int button) {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + buttoncoordinategroup[i].h)) {
			if(gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_PLAY) {
				buttoncoordinategroup[i].state = true;
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_OPTIONS) {
				buttoncoordinategroup[i].state = true;
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_RATE) {
				buttoncoordinategroup[i].state = true;
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_SHARE) {
				buttoncoordinategroup[i].state = true;
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && i == BUTTON_MORE_GAMES) {
				buttoncoordinategroup[i].state = true;
			}
		}
	}
}

void menuCanvas::mouseReleased(int x, int y, int button) {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		if(buttoncoordinategroup[i].state == true) {
			buttoncoordinategroup[i].state = false;
		}

		if(x > buttoncoordinategroup[i].x && x < (buttoncoordinategroup[i].x + buttoncoordinategroup[i].w) && y > buttoncoordinategroup[i].y && y < (buttoncoordinategroup[i].y + buttoncoordinategroup[i].h)) {
			if(gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_PLAY) {
				gCanvas* main = new gCanvas(root);
				appmanager->setCurrentCanvas(main);
				if(musicstate) root->buttonsound.play();
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_OPTIONS) {
				gamestate = BUTTON_OPTIONS;
				if(musicstate) root->clicksound.play();
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_SHARE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_RATE) {
//				gamestate = BUTTON_RATE;
				if(musicstate) root->clicksound.play();
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_MORE_GAMES && i == BUTTON_SHARE) {
//				gamestate = BUTTON_SHARE;
				if(musicstate) root->clicksound.play();
			}
			if(gamestate != BUTTON_PLAY && gamestate != BUTTON_OPTIONS && gamestate != BUTTON_RATE && gamestate != BUTTON_SHARE && i == BUTTON_MORE_GAMES) {
//				gamestate = BUTTON_MORE_GAMES;
				if(musicstate) root->clicksound.play();
			}

		}
	}

	for(int i = 0; i < OPTIONS_BUTTON_COUNT; i++) {
		if(gamestate == BUTTON_OPTIONS && x > opbutton[i].x && x < (opbutton[i].x + opbutton[i].w) && y > opbutton[i].y && y < (opbutton[i].y + opbutton[i].h / 2)) {
			if(i == ACCEPT_BUTTON) {
				gLogi("Onaylandi");
			}
			if(i == DECLINE_BUTTON) {
				gLogi("Onaylanmadi");
			}

			if(musicstate) root->clicksound.play();
			gamestate = GAME_NORMAL;
		}
	}

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(gamestate == BUTTON_OPTIONS && x > slider[i].x && x < slider[i].x + slider[i].w && y > slider[i].y && y < slider[i].y + slider[i].h){
			sliderselected[i] = !sliderselected[i];
			if(sliderselected[i]) slider[i].x = slidermaxx[i];
			else slider[i].x = sliderminx[i];

			if(i == SLIDER_DIFFICULTY) {
				difficultystate = sliderselected[i];
			}
			if(i == SLIDER_MUSIC) {
				musicstate = sliderselected[i];
				root->music.setPaused(!musicstate);
			}
			if(i == SLIDER_VIBRATION) {
				vibrationstate = sliderselected[i];
			}

			if(musicstate) root->clicksound.play();
		}
	}
}

void menuCanvas::checkButtons() {
}

void menuCanvas::mouseDragged(int x, int y, int button) {
}

void menuCanvas::mouseScrolled(int x, int y) {
}

void menuCanvas::mouseEntered() {
}

void menuCanvas::mouseExited() {
}

void menuCanvas::gameManager(int gamestate) {
}

void menuCanvas::setupBackground() {
	background.loadImage("futbolassets/court_01.png");
	backgroundw = getWidth();
	backgroundh = getHeight();
}

void menuCanvas::drawBackground() {
	background.draw(0, 0, backgroundw, backgroundh);
}

void menuCanvas::setupButtons() {
	button[BUTTON_PLAY].loadImage("futbolassets/btn_menu_h_start.png");
	button[BUTTON_OPTIONS].loadImage("futbolassets/btn_menu_h_options.png");
	button[BUTTON_RATE].loadImage("futbolassets/btn_menu_h_rate.png");
	button[BUTTON_SHARE].loadImage("futbolassets/btn_menu_h_share.png");
	button[BUTTON_MORE_GAMES].loadImage("futbolassets/btn_menu_h_more_games.png");

	buttoncoordinategroup[BUTTON_PLAY].w = button[BUTTON_PLAY].getWidth() / 1.5f;
	buttoncoordinategroup[BUTTON_PLAY].h = button[BUTTON_PLAY].getHeight() / 2;
	buttoncoordinategroup[BUTTON_PLAY].sw = button[BUTTON_PLAY].getWidth();
	buttoncoordinategroup[BUTTON_PLAY].sh = button[BUTTON_PLAY].getHeight() / BUTTON_FRAMES;
	buttoncoordinategroup[BUTTON_PLAY].x = (getWidth() - buttoncoordinategroup[BUTTON_PLAY].w) / 2;
	buttoncoordinategroup[BUTTON_PLAY].y = (getHeight() - buttoncoordinategroup[BUTTON_PLAY].h) / 1.75f;
	buttoncoordinategroup[BUTTON_PLAY].state = false;

	for(int i = 1; i < BUTTON_COUNT; i++) {
		buttoncoordinategroup[i].w = button[i].getWidth() / 1.5f;
		buttoncoordinategroup[i].h = button[i].getHeight() / 2;
		buttoncoordinategroup[i].sw = button[i].getWidth();
		buttoncoordinategroup[i].sh = button[i].getHeight() / BUTTON_FRAMES;
		buttoncoordinategroup[i].x = buttoncoordinategroup[BUTTON_PLAY].x + (buttoncoordinategroup[BUTTON_PLAY].sw * 1.20f) - ((buttoncoordinategroup[i].sw / 1.25f) * (i - 1));
		buttoncoordinategroup[i].y = buttoncoordinategroup[BUTTON_PLAY].y + (buttoncoordinategroup[BUTTON_PLAY].sh / 1.25f);
		buttoncoordinategroup[i].state = false;
	}
}

void menuCanvas::drawButtons() {
	for(int i = 0; i < BUTTON_COUNT; i++) {
		button[i].drawSub(buttoncoordinategroup[i].x, buttoncoordinategroup[i].y, buttoncoordinategroup[i].w, buttoncoordinategroup[i].w, 0, buttoncoordinategroup[i].sh * buttoncoordinategroup[i].state, buttoncoordinategroup[i].sw, buttoncoordinategroup[i].sh);
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

void menuCanvas::drawLogo() {
	courtroof.draw(courtroofx, courtroofy, courtroofw, courtroofh);
	logo.draw(logox, logoy, logow, logoh);
}

void menuCanvas::setupGoal() {
	//Kale konumlari
	goal[PLAYER_LEFT].loadImage("futbolassets/goal_posts_left.png");
	goal[PLAYER_RIGHT].loadImage("futbolassets/goal_posts_right.png");
	goalw = goal[PLAYER_LEFT].getWidth() / 1.45f;
	goaly = 150;

	goalh = goal[PLAYER_LEFT].getHeight() / 1.45f;

	goalx[PLAYER_LEFT] = 50;
	goalx[PLAYER_RIGHT] = 1140;
}

void menuCanvas::drawGoal() {
	goal[PLAYER_LEFT].draw(goalx[PLAYER_LEFT], goaly, goalw, goalh, 180);
	goal[PLAYER_LEFT].draw(goalx[PLAYER_RIGHT], goaly, goalw, goalh);
}

void menuCanvas::setupOptionsMenu() {
	musicstate = false;
	soundstate = false;
	difficultystate = false;
	vibrationstate = false;

	for(int i = 0; i < OPTIONS_COUNT; i++) {
		if(i == SLIDER_DIFFICULTY) sliderselected[i] = difficultystate;
		if(i == SLIDER_MUSIC) sliderselected[i] = musicstate;
		if(i == SLIDER_VIBRATION) sliderselected[i] = vibrationstate;

		if(sliderselected[i]) slider[i].x = slidermaxx[i];
		else slider[i].x = sliderminx[i];
	}

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

	optionsfont.loadFont("FreeSansBold.ttf", 18);
	optionstext[0] = "Difficulty";
	optionstext[1] = "Music";
	optionstext[2] = "Vibration";

	opbuttons[0].loadImage("futbolassets/btn_round_no.png");
	opbuttons[1].loadImage("futbolassets/btn_round_ok.png");

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
	opbuttons[DECLINE_BUTTON].drawSub(opbutton[0].x, opbutton[0].y, opbutton[0].w, opbutton[0].w, 0, 0, opbutton[0].sw, opbutton[0].sh);
	opbuttons[ACCEPT_BUTTON].drawSub(opbutton[1].x, opbutton[1].y, opbutton[1].w, opbutton[1].w, 0, 0, opbutton[1].sw, opbutton[1].sh);
}
