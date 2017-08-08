#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "tigr.h"

#define PI 3.1415

void filledCircle(Tigr *screen, int cx, int cy, int radius, TPixel color) {
	int x, y;
	for (y = -radius + 1; y < radius; y++)
		for (x = -radius + 1; x < radius; x++)
			if (x*x + y*y <= radius*radius)
				tigrPlot(screen, cx + x, cy + y, color);
}

struct coords {
	int x, y;
};

int main() {
	Tigr *screen;
	float lastFrame, ballAngle;
	struct coords paddle, mouse, ball;
	int buttons, paddleWidth, ballSpeed;

	/* initial */
	screen = tigrWindow(640, 480, "Ball Bounce", 0);
	tigrClear(screen, tigrRGB(255, 255, 255));
	tigrUpdate(screen);
	srand(time(NULL));

	/* initial positions and values */
	paddle.x = 0;
	paddle.y = 460;
	mouse.x = 0;
	mouse.y = 0;
	ball.x = 320;
	ball.y = 240;
	ballAngle = 1.0472;
	paddleWidth = 90;
	ballSpeed = 4;
	lastFrame = tigrTime();

	/* main game loop */
	while (!tigrClosed(screen)) {
		tigrClear(screen, tigrRGB(255, 255, 255));

		/* get the mouse position */
		tigrMouse(screen, &mouse.x, &mouse.y, &buttons);

		/* update the paddle to the mouse position */
		paddle.x = mouse.x - paddleWidth / 2;

		/* check for collisions with the side */
		if (ball.x - 10 < 0 || ball.x + 10 > 640) {
			ballAngle = -ballAngle;
		}

		/* check for collisions with the top */
		if (ball.y < 0) {
			ballAngle = PI - ballAngle;
		}

		/* check if the ball hit the bottom */
		if (ball.y > 480) {
			tigrClear(screen, tigrRGB(0, 0, 255));
			tigrPrint(screen, tfont, 280, 200, tigrRGB(255, 255, 255), "Game over!");
			tigrUpdate(screen);

			/* wait a bit */
			while (lastFrame < 4.0) lastFrame += tigrTime();

			/* quit the game */
			break;
		}

		/* check for collisions with the paddle */
		if ((ball.y + 13 > paddle.y) && (ball.y + 3 < paddle.y) && (ball.x + 13 < (paddle.x + paddleWidth)) && (ball.x + 13 > paddle.x)) {
			/* reflect the ball off of the paddle */
			ballAngle = PI - ballAngle;

			/* small amount of random angle */
			ballAngle += ((float)(rand() - (RAND_MAX / 2)) / (float)RAND_MAX) / 20;

			/* decrease the paddle width */
			if (paddleWidth > 20) paddleWidth -= 6;

			/* increase the ball speed */
			if (paddleWidth <= 60 && ballSpeed < 20) ballSpeed++;
		}

		/* update the ball's location */
		ball.x += sin(ballAngle) * ballSpeed;
		ball.y += cos(ballAngle) * ballSpeed;

		/* draw the paddle */
		tigrFill(screen, paddle.x, paddle.y, paddleWidth, 10, tigrRGB(255, 0, 0));

		/* draw the ball */
		filledCircle(screen, ball.x, ball.y, 13, tigrRGB(0, 0, 0));

		/* update the screen */
		tigrUpdate(screen);

		/* wait until 16.66ms have passed (60fps) */
		while (lastFrame < 0.01666) lastFrame += tigrTime();
		lastFrame = 0;
	}

	tigrFree(screen);
	return 0;
}
