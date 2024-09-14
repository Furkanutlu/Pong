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

	static const int SOUND_TYPE_STARTING = 0, SOUND_TYPE_SLIDER = 1, SOUND_TYPE_ONHIT = 2;
	static const int SOUND_BUTTON = 0, SOUND_CLICK = 1, SOUND_BALL_HIT = 2, SOUND_WHISTLE = 3, SOUND_GOAL = 4;

	int gamestate, pregamestate;
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

	void setupGame();
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

	/**
	 * Executes each GoalPostsLights object in the activeGoalPostsLights vector, runs their animation, and then erases them.
	 */
    void updateGoalPostsLight();

    /**
     * Acts as a countdown for the goal event. When the counter finishes, it sets the game to the GAME_WAIT state.
     */
    void updateGoalEvent();

    /**
     * Assigns the datatype string and datavalue integer to the updateStatement string in the format
     * "UPDATE options SET " + datatype + " = " + gToStr(datavalue). Then, based on the datatype, it executes
     * the statement on the database variable located in the root, updating one of the difficultyValue, musicValue,
     * or vibrationValue variables.
     */
    void updateSettingsDatabase(std::string datatype, int datavalue);

    /**
     * Controls the whichSlider and value integers using the SLIDER_DIFFICULTY, SLIDER_MUSIC, and SLIDER_VIBRATION constants.
     * Based on the desired denormalizeSlider values, it assigns an X-coordinate between minX or maxX, or assigns
     * accordingly if it's a true/false condition.
     */
    void updateSliderPosition(int whichslider, int value);

    /**
     * Returns a value between 0 and 100 based on the given minX, maxX, and X value.
     */
	int normalizeSlider(int minx, int maxx, int x);

	/**
	 * Returns an X-coordinate between minX and maxX based on the given value.
	 */
	int denormalizeSlider(int minx, int maxx, int value);

	/**
	 * Checks whether the ball crosses the goal line (left or right) using the conditions (ball.x ± ball.radius).
	 * If true, it increments the score for the appropriate side and triggers the goalEvent.
	 */
	void checkGoal();
	void checkPudCollision(Ball& ball, Pud& pud);
	bool checkPostCollision(Ball& ball, Post& post);
	void closestSide(Ball& ball, Pud& pud);
	void reflectBall(Ball& ball, Pud& pud); //pud icin
	void reflectBall(Ball& ball, Post& post); // direk icin

	/**
	 * Stops the ball if it is moving; if it is not, it resumes movement based on the saved velocity.
	 */
	void startBall();
	void resetBall();
	void startHitAnimation(float x, float y);
	void startPudAnimation(int type);

	float getBallSpeed();
	float calculateAngle(int velocityx, int velocityy);

	/**
	 * Called during goal-scoring events. If a goal has been scored, it creates an object based on the provided
	 * int goalPostLightX, int goalPostLightY, int goalPostLightW, and int goalPostLightH values,
	 * then adds it to the activeGoalPostsLights vector. Finally, it clears newGoalPostsLight.
	 */
    void generateGoalPostsLight(int goalpostslightx, int goalpostslighty, int goalpostslightw, int goalpostslighth);

    /**
     * Puts the game into a waiting state. A counter within this function runs once per second, decrementing
     * the number displayed at the center of the screen from 3 to 0. If the pre-game state is not gamePause,
     * the ball is reset. Once the counter reaches 0, the ball’s movement is resumed.
     */
    void waitEvent();

    /**
     * Triggered when the ball passes through the goal. It checks the whoScored variable to determine
     * which side scored. Then calls generateGoalPostsLight for the scoring side, causing the goal post lights
     * to flash for 1-2 seconds. After that, it checks the score. If one side reaches 9 points, the game ends;
     * otherwise, it transitions to the goal state.
     */
    void goalEvent(int whoscored);

    /**
     * Sets the player’s position based on the chosen side or game mode by assigning values to the isUserLeft
     * and isUserRight boolean variables.
     */
	void selectPlayerPosition(int playerpos = PLAYER_LEFT);

	/**
	 * Converts the value received from musicValue to a float between 0 and 1 by dividing it by 100.
	 * Then, based on the desired sound type and volume, it handles operations such as playing, stopping,
	 * and adjusting the volume of the sound track.
	 */
    void soundControl(int musicvalue, int type = 0, int sound = 0);

    /**
     * Transfers the current game state to the preGameState variable before changing the game state.
     * This allows for specific control and enables operations to be performed based on the previous game state.
     */
    void changeGameState(int gamestate);

    //
	void reflect(float& velocityX, float& velocityY, float normalX, float normalY);
	void toggleBallMovement();
	bool isColliding(Ball& ball, Pud& pud);
    //

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

    int difficulty;
    float botSpeed;


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

	// Events

	gImage goalimage;
	int goalimagex, goalimagey, goalimagew, goalimageh;
	gImage youloseimage;
	int youloseimagex, youloseimagey, youloseimagew, youloseimageh;
	gImage youwinimage;
	int youwinimagex, youwinimagey, youwinimagew, youwinimageh;

	// UI

	gImage menubutton[BUTTON_COUNT];

	struct Button {
		int x, y, w, h;
		int sx, sy, sw, sh;
		bool state;
		bool hold;
	};

	Button buttoncoordinategroup[BUTTON_COUNT];
	Button buttonendcoordinategroup[BUTTON_COUNT];
	Button opbutton[OPTIONS_BUTTON_COUNT];
	Button modebutton[maxplayernum];
	Button pausebutton;

	gImage pausebuttonimg;
	int pausecx, pausecy, pauseradius; // for circle collision control

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

	gImage optionsicon[OPTIONS_COUNT];
	gImage optionsimg[OPTIONS_COUNT];
	gImage sliderbackground;

	gImage sliderimg[2];
	int sliderminx[OPTIONS_COUNT], slidermaxx[OPTIONS_COUNT];
	bool sliderselected[OPTIONS_COUNT];

	gFont optionsfont;
	std::string optionstext[OPTIONS_COUNT];

	gImage opbuttons[OPTIONS_BUTTON_COUNT];
	bool opbuttonselected[OPTIONS_BUTTON_COUNT];

	int endboardx, endboardy, endboardw, endboardh;
	int endboardheaderx, endboardheadery, endboardheaderw, endboardheaderh;

	// Option states
	bool musicstate, difficultystate, vibrationstate;
	int musicvalue, difficultyvalue, vibrationvalue;
	bool premusicstate, predifficultystate, previbrationstate;

	// Game End Panel

	// Player type
	gFont selectfont;
	gImage panelbutton;
	std::string selecttext[maxplayernum];
};

#endif /* GCANVAS_H_ */
