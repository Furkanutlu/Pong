/*
* mainMenu.h
*
*  Created on: September 6, 2024
*      Author: Kadir Semih Arslan
*/

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include <gBaseCanvas.h>
#include "gApp.h"

class menuCanvas : public gBaseCanvas {
public:
	menuCanvas(gApp* root);
	virtual ~menuCanvas() = default;

	void setup();
	void update();
	void draw();

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();

private:
	gApp* root;

	static const int BUTTON_COUNT = 5;
	static const int GAME_NORMAL = -1, BUTTON_PLAY = 0, BUTTON_OPTIONS = 1, BUTTON_RATE = 2, BUTTON_SHARE = 3, BUTTON_MORE_GAMES = 4;
	static const int BUTTON_FRAMES = 2;
	static const int maxgoalnum = 2;
	static const int PLAYER_LEFT = 0, PLAYER_RIGHT = 1;
	static const int SLIDER_DIFFICULTY = 0, SLIDER_MUSIC = 1, SLIDER_VIBRATION = 2;

	static const int OPTIONS_COUNT = 3;
	static const int OPTIONS_BUTTON_COUNT = 2;
	static const int ACCEPT_BUTTON = 0, DECLINE_BUTTON = 1;

	int gamestate;

	void setupBackground();
	void setupButtons();
	void setupLogo();
	void setupGoal();
    void setupOptionsMenu();

	void drawBackground();
	void drawButtons();
	void drawLogo();
	void drawGoal();
    void drawOptionsMenu();

    void updateSettingsDatabase(std::string datatype, int datavalue);
    void updateSliderPosition(int whichslider, int value);
	int normalizeSlider(int minx, int maxx, int x);
	int denormalizeSlider(int minx, int maxx, int value);

	void sliderControl();
    void soundControl(bool musicstate);

	gImage background;
	int backgroundw, backgroundh;

	gImage button[BUTTON_COUNT];

	struct ButtonCoordinates {
		int x, y, w, h;
		int sx, sy, sw, sh;
		bool state;
	};

	ButtonCoordinates buttoncoordinategroup[BUTTON_COUNT];
	ButtonCoordinates opbutton[OPTIONS_BUTTON_COUNT];

	// Logo

	gImage courtroof;
	int courtroofx, courtroofy, courtroofw, courtroofh;

	gImage logo;
	int logox, logoy, logow, logoh;

	// Goal

	gImage goal[maxgoalnum];
	int goalx[maxgoalnum], goaly, goalw, goalh;

	// Options

	struct Options {
		int x, y, w, h;
		int sw, sh;
	};
	Options optionsbg[OPTIONS_COUNT];
	Options slider[OPTIONS_COUNT];
	Options sliderbg[OPTIONS_COUNT];
	Options opicon[OPTIONS_COUNT];

	gImage optionsicon[OPTIONS_COUNT];
	gImage optionsimg[OPTIONS_COUNT];
	gImage sliderbackground;

	gImage sliderimg[2];
	int sliderminx[OPTIONS_COUNT], slidermaxx[OPTIONS_COUNT];
	bool sliderselected[OPTIONS_COUNT];
	bool sliderstate[OPTIONS_COUNT];

	gFont optionsfont;
	std::string optionstext[OPTIONS_COUNT];

	gImage opbuttons[OPTIONS_BUTTON_COUNT];
	bool opbuttonselected[OPTIONS_BUTTON_COUNT];

	// Board
	gImage board;
	int boardx, boardy, boardw, boardh;
	gImage boardheader;
	int boardheaderx, boardheadery, boardheaderw, boardheaderh;

	std::string boardtext;
	gFont boardfont;

	// VFX
	bool musicstate, difficultystate, vibrationstate;
	int musicvalue, difficultyvalue, vibrationvalue;
	bool premusicstate, predifficultystate, previbrationstate;
};

#endif /* SRC_MENU_H_ */
