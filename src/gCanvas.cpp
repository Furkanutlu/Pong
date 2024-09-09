/*
* gCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}

gCanvas::~gCanvas() {
}

void gCanvas::setup() {

	setupMap();
	setupGoal();
	setupBall();
	setupMaphitbox();
	setupPuds();
}
void gCanvas::update() {
	updateBallPosition();
	updateHitAnimating();
	updatePudAnimating();
}

void gCanvas::draw() {
	//logo.draw((getWidth() - logo.getWidth()) / 2, (getHeight() - logo.getHeight()) / 2);
	drawMap();
	drawGoal();
	drawPuds();
	drawHit();
	drawBall();
	//gDrawRectangle(pudleft.x, pudleft.y + 10, pudleft.w, pudleft.h - 20, false);
	//gDrawRectangle(gamelinelimitx[0], gamelinelimity[0], gamelinelimitx[1], gamelinelimity[1], true);
}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
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
}
void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button) {
	if (button == MOUSEBUTTON_LEFT) {
	currentmousey = y;

		        if (x < getWidth() / 2) {
		            pudleft.y = currentmousey - pudleft.h / 2;

		            pudleft.y = std::max(pudleft.y, 115.0f);
		            pudleft.y = std::min(pudleft.y, 600 - pudleft.h);

		            pudleft.velocityy = currentmousey - prevmouseyleft;

		            prevmouseyleft = currentmousey;
		        } else {
		            pudright.y = currentmousey - pudright.h / 2;

		            pudright.y = std::max(pudright.y, 115.0f);
		            pudright.y = std::min(pudright.y, 600 - pudleft.h);
		            pudright.velocityy = currentmousey - prevmouseyright;
		            prevmouseyright = currentmousey;
		        }
	    }

}

void gCanvas::mousePressed(int x, int y, int button) {
//	gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
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
	pudleftimage.loadImage("futbolassets/pud_left.png");
	pudrightimage.loadImage("futbolassets/pud_right.png");

	pudleft.w = pudleftimage.getWidth() / 5;
	pudleft.h = pudleftimage.getHeight();
	pudleft.x = 270;
	pudleft.y = (getHeight() - pudleft.h) / 2;
	pudleft.velocityy = 0;
	pudanimationactiveleft = false;

	pudright.w = pudrightimage.getWidth() / 5;
	pudright.h = pudrightimage.getHeight();
	pudright.x = 1005 - pudright.w;
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
	goal[PLAYER_LEFT].draw(goalx[PLAYER_LEFT], goaly[PLAYER_LEFT], goalw[PLAYER_LEFT], goalh[PLAYER_LEFT]);
	goal[PLAYER_RIGHT].draw(goalx[PLAYER_RIGHT], goaly[PLAYER_RIGHT], goalw[PLAYER_RIGHT], goalh[PLAYER_RIGHT]);
}


void gCanvas::updateBallPosition() {
	 checkGoal();
	 if(ismoving){

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

		 if(ball.y > ustdirek && ball.y < altdirek){
			 for (int i = 0; i < 2; i++) {
				if (checkPostCollision(ball, goalpostleft[i])) {
					reflectBall(ball, goalpostleft[i]);
				}
				if (checkPostCollision(ball, goalpostright[i])) {
					reflectBall(ball, goalpostright[i]);
			    }
			}
		 } else {
			 if((ball.x - ball.radius <= gamelinelimitx[0] || ball.x + ball.radius >= gamelinelimitx[1]) && (ball.y + ball.radius >= goalyend || ball.y - ball.radius <= goalystart)){
				 startHitAnimation(ball.x, ball.y);
				 ball.velocityx *= -1;


			 }
			 if (ball.y - ball.radius <= gamelinelimity[0] || ball.y + ball.radius >= gamelinelimity[1] ) {
				 startHitAnimation(ball.x, ball.y);
				 ball.velocityy *= -1;
				 if(ball.velocityy > 20)
				 ball.velocityy *= 0.5;
			 }
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
	        resetBall();
	    }
	    if (ball.x >= goalline[PLAYER_RIGHT] + ball.radius) {
	        score[PLAYER_LEFT]++;
	        resetBall();
	    }
}
void gCanvas::resetBall() {
    ball.x = getWidth() / 2;
    ball.y = getHeight() / 2;
	angle = gRandom(360) * (M_PI / 180);
	savedvelocityx = cos(angle) * speed;
	savedvelocityy = sin(angle) * speed;
    ball.velocityx = 0;
    ball.velocityy = 0;
    ismoving = false; // Top durdu
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
	    } else if (type == RIGHT) {
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
	        if(ball.velocityy > maxspeed)
	        	ball.velocityy = maxspeed;
	        if(ball.velocityy < -1 * maxspeed)
	        	ball.velocityy = -1 * maxspeed;
	        ball.y = (mindistance == disttop) ? pud.y - ball.radius : pud.y + pud.h + ball.radius;
	    }

	    if (mindistance == distleft || mindistance == distright) {
	        ball.velocityx *= -1;
	        if(abs(ball.velocityx) < speed)
	        	ball.velocityx *= speed / ball.velocityx;
	        ball.velocityy += pud.velocityy;
	        if(abs(ball.velocityy) > maxspeed)
	        	ball.velocityy *= maxspeed / ball.velocityy;
	        ball.x = (mindistance == distleft) ? pud.x - ball.radius : pud.x + pud.w + ball.radius;
	    }

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


