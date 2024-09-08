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

	struct Ball{
		float x, y, w, h;
		float velocityx, velocityy;
		float radius;
	}ball;

	struct Post{
		float x, y;
		float radius;
	}goalpostleft[2], goalpostright[2]; // Her kale icin 2 direk


	gApp* root;
	gImage logo;
	gImage map[maxmapimages];
	gImage goal[maxgoalnum];
	gImage mapcenterline;
	gImage ballimage;
	gImage ballshadow;
	gImage ballhit;



	void drawMap();
	void drawGoal();
	void drawBall();
	void drawHit();

	void setupMap();
	void setupGoal();
	void setupBall();
	void setupMaphitbox();

	void updateBallPosition();
	void updateHitAnimating();
	void checkGoal();
	bool checkPostCollision(Ball& ball, Post& post);
	void reflectBall(Ball& ball, Post& post);
	void resetBall();
	void startHitAnimation(float x, float y);

	float getBallSpeed();
	float calculateAngle(int velocityx, int velocityy);

	int mapx, mapy, mapw, maph;
	int goalx[maxgoalnum], goaly[maxgoalnum], goalw[maxgoalnum], goalh[maxgoalnum];
	int mapcenterlinex, mapcenterliney, mapcenterlinew, mapcenterlineh;
	int ballx, bally, ballw, ballh;
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
	float aspectx, aspecty;
	int shadowoffsetx;
	int shadowoffsety;
	float shadowscale;

	float speed, angle;
	bool ismoving;
	int savedvelocityx, savedvelocityy;
	int lastvelocityx, lastvelocityy;
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
};

#endif /* GCANVAS_H_ */
