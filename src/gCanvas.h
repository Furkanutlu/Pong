 /*
 * gCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include <cmath>


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
	static const int PLAYER_LEFT = 0, PLAYER_RIGHT = 1;
	static const int UST_DIREK = 0, ALT_DIREK = 1;
	static const int LEFT = 0, RIGHT = 1, BOTTOM = 2, TOP = 3;

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



	void drawMap();
	void drawGoal();
	void drawBall();
	void drawHit();
	void drawPuds();

	void setupMap();
	void setupGoal();
	void setupBall();
	void setupMaphitbox();
	void setupPuds();

	void updateBallPosition();
	void updateHitAnimating();
	void updatePudAnimating();

	void checkGoal();
	bool checkPudCollision(Ball& ball, Pud& pud);
	bool checkPostCollision(Ball& ball, Post& post);
	void closestSide(Ball& ball, Pud& pud);
	void reflectBall(Ball& ball, Pud& pud); //pud icin
	void reflectBall(Ball& ball, Post& post); // direk icin

	void resetBall();
	void startHitAnimation(float x, float y);
	void startPudAnimation(int type);

	float getBallSpeed();
	float calculateAngle(int velocityx, int velocityy);

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

	float speed, angle;
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
	int pudrightframex ;

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

};

#endif /* GCANVAS_H_ */
