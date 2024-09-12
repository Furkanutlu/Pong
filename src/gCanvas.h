 /*
 * gCanvas.h
 *
 *  Created on: September 4, 2024
 *      Author: Kadir Semih Arslan && Mehmet Furkan Utlu
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include <vector>
#include <cmath>
#include <chrono>
#include <thread>


class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);

	void showNotify();
	void hideNotify();

private:

	static const int maxmapimages = 5; // toplam map sayisi
	static const int maxgoalnum = 2;
	static const int maxplayernum = 2;
	static const int movespeed = 5;
	static const int PLAYER_LEFT = 0, PLAYER_RIGHT = 1;
	static const int UST_DIREK = 0, ALT_DIREK = 1;
	static const int LEFT = 0, RIGHT = 1, BOTTOM = 2, TOP = 3;

	static const int OPTIONS_COUNT = 3;
	static const int OPTIONS_BUTTON_COUNT = 2;
	static const int ACCEPT_BUTTON = 0, DECLINE_BUTTON = 1;

	static const int SLIDER_DIFFICULTY = 0, SLIDER_MUSIC = 1, SLIDER_VIBRATION = 2;

	const int WAIT_SECOND = 3;
	const float WAIT_NUMBER_SIZE_MULTIPLIER = 1.5f;
	static const int BUTTON_COUNT = 3;
	static const int BUTTON_REPLAY = 0, BUTTON_HOME = 1, BUTTON_OPTIONS = 2;

	const int GAME_START = 0, GAME_PAUSE = 1, GAME_GOAL = 2, GAME_WIN = 3, GAME_LOSE = 4, GAME_OPTIONS = 5, GAME_WAIT = 6, GAME_SELECT_MODE = 7;
	const int MODE_NONE = -1, MODE_PVP = 0, MODE_PVE = 1;

	int gamestate;
	int gamemode;

	struct Ball{
		float x, y, w, h;
		float velocityx, velocityy;
		float radius;
	}ball;

	struct Post{
		float x, y;
		float radius;
	}goalpostleft[2], goalpostright[2]; // Her kale icin 2 direk

    struct Pud {
        float x, y, w, h;
        float velocityy; // Pudun y eksenindeki hizi
    } pudleft, pudright;

	gApp* root;
	gImage logo;
	gImage map[maxmapimages];
	gImage goal[maxgoalnum];
	gImage mapcenterline;
	gImage ballimage;
	gImage ballshadow;
	gImage ballhit;
	gImage pudleftimage;
	gImage pudrightimage;

	void setupMap();
	void setupGoal();
	void setupBall();
	void setupMaphitbox();
	void setupPuds();
	void setupScore();
    void setupGoalPostsLight();
    void setupGoalEvent();
    void setupPauseMenu();
    void setupOptionsMenu();
	void setupGameEndPanel();
	void setupGameMode();

	void drawMap();
	void drawGoal();
	void drawBall();
	void drawHit();
	void drawPuds();
    void drawGoalEvent();
	void drawScore();
    void drawGoalPostsLight();
    void drawWaitEvent();
    void drawPauseMenu();
    void drawOptionsMenu();
	void drawGameEndPanel();
	void drawGameMode();

	void updateBallPosition();
	void updateHitAnimating();
	void updatePudAnimating();
	void updateBot();
	void updatePudControl();
	void updateScore();
    void updateGoalPostsLight();
    void updateGoalEvent();
    void updateSettingsDatabase(std::string datatype, int datavalue);
    void updateSliderPosition(int whichslider, int value);
	int normalizeSlider(int minx, int maxx, int x);
	int denormalizeSlider(int minx, int maxx, int value);


	void checkGoal();
	bool checkPudCollision(Ball& ball, Pud& pud);
	bool checkPostCollision(Ball& ball, Post& post);
	void closestSide(Ball& ball, Pud& pud);
	void reflectBall(Ball& ball, Pud& pud); //pud icin
	void reflectBall(Ball& ball, Post& post); // direk icin

	void startBall();
	void resetBall();
	void startHitAnimation(float x, float y);
	void startPudAnimation(int type);

	float getBallSpeed();
	float calculateAngle(int velocityx, int velocityy);


    void generateGoalPostsLight(int goalpostslightx, int goalpostslighty, int goalpostslightw, int goalpostslighth);
    void waitEvent();
    void goalEvent(int whoscored);
	void selectGameMode(int gamemode, int playerpos);

	void sliderControl();
    void soundControl();

	int normalizeSlider(int minx, int maxx, int x);

	int mapx, mapy, mapw, maph;
	int goalx[maxgoalnum], goaly[maxgoalnum], goalw[maxgoalnum], goalh[maxgoalnum];
	int mapcenterlinex, mapcenterliney, mapcenterlinew, mapcenterlineh;

	int gamelinelimitx[2], gamelinelimity[2];
	int goalline[maxgoalnum];
	int goalystart, goalyend;
	int score[maxplayernum];
	float dx, dy; // top ve direk arasi mesafe
	float normalvx, normalvy;// normal vektorleri
	float next_x, next_y; //topun onceki konumu ve sonraki arasi mesafe
	float distance;
	float dotproduct; // hiz vektoru ve normal vector carpimi
	int ustdirek, altdirek;
	int randommapnum;
	int shadowoffsetx;
	int shadowoffsety;
	float shadowscale;

	float speed, angle, maxspeed;
	bool ismoving;
	int savedvelocityx, savedvelocityy;
	bool ishitanimating;
	float hitw, hith;
	int hitframe;
	int hitframex;
	float hitframetimer;
	float hitframespeed;
	int hitanimx, hitanimy;
	float ballangle;
	float angleradians;
	float angledegrees;
	int ballcurrentframe;
	int ballframex;
	float ballframetimer;
	float ballframespeed;
	int savedballframe;
	float savedballframetimer;
	int pudleftframex;
	int pudrightframex;

	float pudanimtimerleft = 0.0f;
	int pudanimframeleft = 0;
	float pudanimtimerright = 0.0f;
	int pudanimframeright = 0;

	float pudanimframespeed = 0.1f;

	bool pudanimationactiveleft;
	bool pudanimationactiveright;
	bool istopbottomcollision;
	int prevmousey;
	float mindistance;
	float disttop, distbot, distleft, distright;
	float prevmouseyleft;
	float prevmouseyright;
	float currentmousey;
	bool isuserleft;
	bool isuserright;

	float botcentery, ballcentery;
	float targety;
	float errormargin;
	bool ismovingupright, ismovingupleft;
	bool ismovingdownright, ismovingdownleft;

	// Goal Lights
	bool lightactive;
	gImage goalpostslights;
	int goalpostslightsframew, goalpostslightsframeh, goalpostslightsmaxframe, goalpostslightsrowcol, goalpostslightscolumncol;

	std::vector<int> newgoalpostslight;
	std::vector<std::vector<int>> activeGoalPostsLights;

	enum goalPostsLightsEnum {
		GPL_X, GPL_Y, GPL_W, GPL_H, GPL_SX, GPL_SY, GPL_FRAMENO, GPL_COUNTER
	};

	int goalpolelefttop[2], goalpoleleftbottom[2], goalpolerighttop[2], goalpolerightbottom[2];

	// Score
	gImage scorenumbers;
	int scorenumbersframew, scorenumbersframeh, scorenumbersmaxframe, scorenumbersrowcol, scorenumberscolumncol, scorenumbersframex, scorenumbersframey;

	std::vector<int> newscorenumber;
	struct ScoreFrame {
		int x, y;
	};
	ScoreFrame scores[10];

	// After Score
	int waitcounter;
	int waitnumber;
	bool goalscore;

	// Events

	gImage goalimage;
	int goalimagex, goalimagey, goalimagew, goalimageh;
	gImage youloseimage;
	int youloseimagex, youloseimagey, youloseimagew, youloseimageh;
	gImage youwinimage;
	int youwinimagex, youwinimagey, youwinimagew, youwinimageh;

	// UI

	gImage button[BUTTON_COUNT];

	struct ButtonCoordinates {
		int x, y, w, h;
		int sx, sy, sw, sh;
		bool state;
		bool hold;
	};

	ButtonCoordinates buttoncoordinategroup[BUTTON_COUNT];
	ButtonCoordinates buttonendcoordinategroup[BUTTON_COUNT];
	ButtonCoordinates opbutton[OPTIONS_BUTTON_COUNT];

	gImage pausebutton;
	int pausebuttonx, pausebuttony, pausebuttonw, pausebuttonh, pausestate;
	int pausecx, pausecy, pauseradius;

	gImage pausebuttonbackground;
	int pausebuttonbgx, pausebuttonbgy, pausebuttonbgw, pausebuttonbgh;

	gImage board;
	int boardx, boardy, boardw, boardh;
	gImage boardheader;
	int boardheaderx, boardheadery, boardheaderw, boardheaderh;

	std::string boardtext;
	gFont boardfont;

	int pausemenubuttongap;

	// Options

	struct PANEL {
		int x, y, w, h;
		int sw, sh;
		bool selected;
	};
	PANEL optionsbg[OPTIONS_COUNT];
	PANEL slider[OPTIONS_COUNT];
	PANEL sliderbg[OPTIONS_COUNT];
	PANEL opicon[OPTIONS_COUNT];
	PANEL opbutton[OPTIONS_BUTTON_COUNT];

	gImage optionsicon[OPTIONS_COUNT];
	gImage optionsimg[OPTIONS_COUNT];
	gImage sliderbackground;

	gImage sliderimg[2];
	int sliderminx[OPTIONS_COUNT], slidermaxx[OPTIONS_COUNT];
	int musicvalue[OPTIONS_COUNT];
	bool sliderselected[OPTIONS_COUNT];

	gFont optionsfont;
	std::string optionstext[OPTIONS_COUNT];

	gImage opbuttons[OPTIONS_BUTTON_COUNT];
	bool opbuttonselected[OPTIONS_BUTTON_COUNT];

	int endboardx, endboardy, endboardw, endboardh;
	int endboardheaderx, endboardheadery, endboardheaderw, endboardheaderh;

	// Option states
	bool musicstate, difficultystate, vibrationstate;
	bool premusicstate, predifficultystate, previbrationstate;

	// Game End Panel

	// Player type
	gFont selectfont;
	gImage panelbutton;
	PANEL modebutton[maxplayernum];
	std::string selecttext[maxplayernum];
};

#endif /* GCANVAS_H_ */
