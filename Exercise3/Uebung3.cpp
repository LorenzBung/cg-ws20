// Sample code for Übung 2

#include "vec.h"
#include "mat.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
using namespace std;

// might be you have to swith to
// #include "glut.h" depending on your GLUT installation
#include <GL/gl.h>
#include <GL/freeglut.h>

////////////////////////////////////////////////////////////
//
// system relevant global variables
//

// window width and height (choose an appropriate size)
const int g_iWidth  = 400;
const int g_iHeight = 400;

// global variable to tune the timer interval
int g_iTimerMSecs;

//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... replace by your own ones
//
// some global state variables used to describe ...
float g_iPos;		// ... position and ...
float g_iPosIncr;	// ... position increment (used in display1)

CVec2i sunPos;
CVec2i planetPos;
CVec2i moonPos;
CVec4f eyePoint;
CVec4f viewDir;
CVec4f viewUp;
float planetAngle;
float moonAngle;
float fFocus;
const float planetAngleIncrement = 2.0 * M_PI / 180;
const float moonAngleIncrement = 4.0 * M_PI / 180;
const float fFocusIncrement = 1.0;
const float xyzIncrement = 4.0 * M_PI / 180;
const float abcIncrement = 4.0 * M_PI / 180;
const float uvwIncrement = 1.0;
float xRotAngle, yRotAngle, zRotAngle;
float aRotAngle, bRotAngle, cRotAngle;


CVec2i g_vecPos;		// same as above but in vector form ...
CVec2i g_vecPosIncr;	// (used in display2)
//
/////////////////////////////////////////////////////////////

// Init variables for the viewer
void initView() {
	float eyePointArr[4] = { 0.0, 0.0, 0.0, 1.0 };
	float viewDirArr[4] = { 0.0, 0.0, -1.0, 1.0 };
	float viewUpArr[4] = { 0.0, 1.0, 0.0, 1.0 };
	eyePoint = CVec4f(eyePointArr);
	viewDir = CVec4f(viewDirArr);
	viewUp = CVec4f(viewUpArr);
	fFocus = 50.0;
	xRotAngle = 0;
	yRotAngle = 0;
	zRotAngle = 0;
	aRotAngle = 0;
	bRotAngle = 0;
	cRotAngle = 0;
}

// function to initialize our own variables
void init () 
{

	// init timer interval
	g_iTimerMSecs = 10;

	// init variables for display1
	g_iPos     = 0;
	g_iPosIncr = 2;

	// Set default planet positions
	int sunPosArr[2] = { 0, 0 };
	int planetPosArr[2] = { 125, 0 };
	int moonPosArr[2] = { 175, 0 };
	sunPos.setData(sunPosArr);
	planetPos.setData(planetPosArr);
	moonPos.setData(moonPosArr);

	// Set angles
	planetAngle = 0.0;
	moonAngle = 0.0;

	// init variables for display2
	int aiPos    [2] = {0, 0};
	int aiPosIncr[2] = {2, 2};
	g_vecPos.setData (aiPos);
	g_vecPosIncr.setData (aiPosIncr);

	// init camera elements
	initView();
}


// function to initialize the view to ortho-projection
void initGL () 
{
	glViewport (0, 0, g_iWidth, g_iHeight);	// Establish viewing area to cover entire window.

	glMatrixMode (GL_PROJECTION);			// Start modifying the projection matrix.
	glLoadIdentity ();						// Reset project matrix.
	glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);	// Map abstract coords directly to window coords.

	// tell GL that we draw to the back buffer and
	// swap buffers when image is ready to avoid flickering
	glDrawBuffer (GL_BACK);

	// tell which color to use to clear image
	glClearColor (0,0,0,1);
}

// Eine überaus primitive Punktklasse
class Point {
public:

	Point(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
	}

	int x, y;
};

// Eine überaus primitive Farbklasse
class Color {
public:

	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	float r, g, b;
};


int min (int a, int b) { return a>b? a: b; }
// timer callback function
void timer (int value) 
{
	///////
	// update your variables here ...
	//

	int size2 = min (g_iWidth, g_iHeight) / 2;

	// variables for display1 ...
	planetAngle += planetAngleIncrement;
	
	moonAngle += moonAngleIncrement;


	// variables for display2 ...
	if (g_vecPos(1)<=-size2 || g_vecPos(1)>=size2) g_vecPosIncr = -g_vecPosIncr; 
	g_vecPos += g_vecPosIncr;


	// the last two lines should always be
	glutPostRedisplay ();
	glutTimerFunc (g_iTimerMSecs, timer, 0);	// call timer for next iteration
}


void bhamLine(Point p1, Point p2, Color c)
{
	glBegin(GL_POINTS);
	// implement bhamLine here and use
	// glVertex2i (x, y);
	// to draw a pixel
	glColor3f(0.0, 0.0, 0.0);
	// erster Punkt
	glVertex2i(p1.x, p1.y);
	// letzter Punkt
	glVertex2i(p2.x, p2.y);
	glColor3f(c.r, c.g, c.b);

	int x, y, deltax, deltay, d, deltaNE, deltaE;
	Point oldP1 = Point(p1.x, p1.y);
	p1 = Point(0, 0);
	p2.x -= oldP1.x;
	p2.y -= oldP1.y;
	bool mirrorx = p2.x < 0;
	bool mirrory = p2.y < 0;

	if (mirrorx) {
		p2.x = -p2.x;
	}
	if (mirrory) {
		p2.y = -p2.y;
	}
	bool mirroraxis = p2.y > p2.x;
	if (mirroraxis) {
		int tmp = p2.x;
		p2.x = p2.y;
		p2.y = tmp;
	}

	deltax = p2.x - p1.x;
	deltay = p2.y - p1.y;
	deltaNE = 2 * (deltay - deltax);
	deltaE = 2 * deltay;

	x = p1.x;
	y = p1.y;
	d = 2 * (deltay - deltax);

	glVertex2i(oldP1.x, oldP1.y);
	while (x < p2.x) {
		if (d >= 0) {
			d += deltaNE;
			x++;
			y++;
		} // NE
		else {
			d += deltaE;
			x++;
		} // E
		Point drawp1 = oldP1;
		Point drawp2 = Point(x, y);
		if (mirroraxis) {
			int tmp = drawp2.x;
			drawp2.x = drawp2.y;
			drawp2.y = tmp;
		}
		if (mirrorx) {
			drawp2.x = -drawp2.x;
		}
		if (mirrory) {
			drawp2.y = -drawp2.y;
		}
		drawp2 = Point(drawp2.x + oldP1.x, drawp2.y + oldP1.y);
		glVertex2i(drawp2.x, drawp2.y);
	}
	glEnd();
}


void bhamCircle(Point p, int r, Color c) {
	glBegin(GL_POINTS);
	glColor3f(0.0, 0.0, 0.0);
	// Mittelpunkt
	glVertex2i(p.x, p.y);
	glColor3f(c.r, c.g, c.b);

	Point oldPoint = p;

	p = Point(0, 0);

	int x, y, d, deltaSE, deltaE;
	x = 0;
	y = r;
	d = 5 - 4 * r;
	glVertex2i(x + oldPoint.x, y + oldPoint.y);
	glVertex2i(x + oldPoint.x, -y + oldPoint.y);
	glVertex2i(-x + oldPoint.x, y + oldPoint.y);
	glVertex2i(-x + oldPoint.x, -y + oldPoint.y);
	glVertex2i(y + oldPoint.x, x + oldPoint.y);
	glVertex2i(y + oldPoint.x, -x + oldPoint.y);
	glVertex2i(-y + oldPoint.x, x + oldPoint.y);
	glVertex2i(-y + oldPoint.x, -x + oldPoint.y);
	while (y > x) {
		if (d >= 0) {
			deltaSE = 4 * (2 * (x - y) + 5);
			d += deltaSE;
			x++;
			y--;
		} // SE
		else {
			deltaE = 4 * (2 * x + 3);
			d += deltaE;
			x++;
		} // E
		glVertex2i(x + oldPoint.x, y + oldPoint.y);
		glVertex2i(x + oldPoint.x, -y + oldPoint.y);
		glVertex2i(-x + oldPoint.x, y + oldPoint.y);
		glVertex2i(-x + oldPoint.x, -y + oldPoint.y);
		glVertex2i(y + oldPoint.x, x + oldPoint.y);
		glVertex2i(y + oldPoint.x, -x + oldPoint.y);
		glVertex2i(-y + oldPoint.x, x + oldPoint.y);
		glVertex2i(-y + oldPoint.x, -x + oldPoint.y);
	}
	glEnd();
}

Point rotateAroundCenter(Point p, float angle)
{
	int newx = (int) (p.x * cos(angle) - p.y * sin(angle));
	int newy = (int)(p.x * sin(angle) + p.y * cos(angle));
	return Point(newx, newy);
}

Point rotateAroundPoint(Point p1, Point p2, float angle)
{
	Point newP1 = Point(p1.x - p2.x, p1.y - p2.y);
	Point rotatedP1 = rotateAroundCenter(newP1, angle);
	Point finalPoint = Point(rotatedP1.x + p2.x, rotatedP1.y + p2.y);
	return finalPoint;
}

// display callback function
void display1 (void) 
{
	glClear(GL_COLOR_BUFFER_BIT);

	///////
	// display your data here ...
	//
	int sunPosArr[2], planetPosArr[2], moonPosArr[2];
	sunPos.getData(sunPosArr);
	planetPos.getData(planetPosArr);
	moonPos.getData(moonPosArr);

	// Create sun circle
	bhamCircle(Point(sunPosArr[0], sunPosArr[1]), 30, Color(1.0, 1.0, 0.0));

	// Move planet and create cirle
	Point newPlanetPos = rotateAroundCenter(Point(planetPosArr[0], planetPosArr[1]), planetAngle);
	bhamCircle(newPlanetPos, 20, Color(0.0, 1.0, 0.0));

	// Move moon and create cirle
	Point movedMoon = Point(moonPosArr[0] + (newPlanetPos.x - planetPosArr[0]), moonPosArr[1] + (newPlanetPos.y - planetPosArr[1]));
	Point newMoonPos = rotateAroundPoint(movedMoon, newPlanetPos, moonAngle);
	bhamCircle(newMoonPos, 10, Color(0.0, 1.0, 1.0));

	// In double buffer mode the last
	// two lines should always be
	glFlush();
	glutSwapBuffers(); // swap front and back buffer
}

CVec3f rotateAroundCenter3d(CVec3f p, float angle) {
	float rotMatArr[3][3] = { { cos(angle), -sin(angle), 0 },
							  { sin(angle),  cos(angle), 0 },
							  {          0,           0, 1 } };
	CMat3f rotMat(rotMatArr);
	return rotMat * p;
}

CVec3f rotateAroundPoint3d(CVec3f p1, CVec3f p2, float angle) {
	float p1Arr[3], p2Arr[3], rotatedP1Arr[3];
	p1.getData(p1Arr);
	p2.getData(p2Arr);
	float transl1MatrArr[3][3] = { { 1, 0, -p2Arr[0] },
								   { 0, 1, -p2Arr[1] },
								   { 0, 0,         1 } };
	CMat3f transl1Matr(transl1MatrArr);
	CVec3f movedPoint = transl1Matr * p1;
	CVec3f rotatedPoint = rotateAroundCenter3d(movedPoint, angle);
	float transl2MatrArr[3][3] = { { 1, 0, p2Arr[0] },
								   { 0, 1, p2Arr[1] },
								   { 0, 0,        1 } };
	CMat3f transl2Matr(transl2MatrArr);
	return transl2Matr * rotatedPoint;
}

// display callback function
void display2 (void) 
{
	glClear (GL_COLOR_BUFFER_BIT);

	///////
	// display your data here ...
	//
	CVec3f sunPos3d, planetPos3d, moonPos3d;
	int sunPosArr[2], planetPosArr[2], moonPosArr[2];
	sunPos.getData(sunPosArr);
	planetPos.getData(planetPosArr);
	moonPos.getData(moonPosArr);

	// Transform 2d vectors to 3d vectors
	float sunPosArr3d[3] = { sunPosArr[0], sunPosArr[1], 1 };
	float planetPosArr3d[3] = { planetPosArr[0], planetPosArr[1], 1 };
	float moonPosArr3d[3] = { moonPosArr[0], moonPosArr[1], 1 };
	sunPos3d.setData(sunPosArr3d);
	planetPos3d.setData(planetPosArr3d);
	moonPos3d.setData(moonPosArr3d);

	// Create sun circle
	bhamCircle(Point((int)(sunPosArr[0]), (int)(sunPosArr[1])), 30, Color(1.0, 1.0, 0.0));

	// Move planet and create cirle
	float newPlanetPosArr[3];
	CVec3f newPlanetPos = rotateAroundCenter3d(planetPos3d, planetAngle);
	newPlanetPos.getData(newPlanetPosArr);
	Point newPlanetPosPoint((int)(newPlanetPosArr[0]), (int)(newPlanetPosArr[1]));
	bhamCircle(newPlanetPosPoint, 20, Color(0.0, 1.0, 0.0));

	// Move moon and create cirle
	float movedMoonPosArr[3] = { moonPosArr3d[0] + (newPlanetPosPoint.x - planetPosArr3d[0]), moonPosArr3d[1] + (newPlanetPosPoint.y - planetPosArr3d[1]), 1 };
	CVec3f movedMoonPos(movedMoonPosArr);
	CVec3f rotatedMoonPos = rotateAroundPoint3d(movedMoonPos, newPlanetPos, moonAngle);
	float rotatedMoonPosArr[3];
	rotatedMoonPos.getData(rotatedMoonPosArr);
	Point rotatedMoonPosPoint = Point((int)(rotatedMoonPosArr[0]), (int)(rotatedMoonPosArr[1]));
	//Point rotatedMoonPosPoint = Point((int)(movedMoonPosArr[0]), (int)(movedMoonPosArr[1]));
	bhamCircle(rotatedMoonPosPoint, 10, Color(0.0, 1.0, 1.0));

	// In double buffer mode the last
	// two lines should always be
	glFlush();
	glutSwapBuffers(); // swap front and back buffer
}


// ÜBUNG 3

CVec3f projectZ(float fFocus, CVec4f pSicht) {
	float pSichtArr[4];
	pSicht.getData(pSichtArr);

	float projectedx = pSichtArr[0] * (fFocus / (fFocus - pSichtArr[2]));
	float projectedy = pSichtArr[1] * (fFocus / (fFocus - pSichtArr[2]));
	float projectedPointArr[3] = { projectedx, projectedy, 1.0f };

	return CVec3f(projectedPointArr);
}

void bhamLineCVec3f(CVec3f p1, CVec3f p2, Color c) {
	float p1Arr[3];
	float p2Arr[3];
	p1.getData(p1Arr);
	p2.getData(p2Arr);
	
	Point point1(p1Arr[0], p1Arr[1]);
	Point point2(p2Arr[0], p2Arr[1]);
	bhamLine(point1, point2, c);
}

void drawProjectedZ(CVec3f points[8], Color c) {
	bhamLineCVec3f(points[0], points[1], c);
	bhamLineCVec3f(points[0], points[2], c);
	bhamLineCVec3f(points[1], points[3], c);
	bhamLineCVec3f(points[2], points[3], c);
	bhamLineCVec3f(points[4], points[5], c);
	bhamLineCVec3f(points[4], points[6], c);
	bhamLineCVec3f(points[5], points[7], c);
	bhamLineCVec3f(points[6], points[7], c);
	bhamLineCVec3f(points[0], points[4], c);
	bhamLineCVec3f(points[1], points[5], c);
	bhamLineCVec3f(points[2], points[6], c);
	bhamLineCVec3f(points[3], points[7], c);
}


CMat4f getTransform(CVec4f viewOrigin, CVec4f viewDir, CVec4f viewUp) {
	float viewDirLen = sqrt(pow(viewDir(0), 2) + pow(viewDir(1), 2) + pow(viewDir(2), 2));
	viewDir *= (1 / viewDirLen);
	viewDir(3) = 1.0;
	float viewUpLen = sqrt(pow(viewUp(0), 2) + pow(viewUp(1), 2) + pow(viewUp(2), 2));
	viewUp *= (1 / viewUpLen);
	viewUp(3) = 1.0;

	CVec4f r3 = viewDir;
	float r1Arr[4] = { viewUp(1) * viewDir(2) - viewUp(2) * viewDir(1), viewUp(2) * viewDir(0) - viewUp(0) * viewDir(2), viewUp(0) * viewDir(1) - viewUp(1) * viewDir(0), 1.0f };
	CVec4f r1(r1Arr);
	float r2Arr[4] = { r3(1) * r1(2) - r3(2) * r1(1), r3(2) * r1(0) - r3(0) - r1(2), r3(0) * r1(1) - r3(1) * r1(0), 1.0f };
	CVec4f r2(r2Arr);

	float finalArr[4][4] = { { r1(0), r1(1), r1(2), -viewOrigin(0) },
							 { r2(0), r2(1), r2(2), -viewOrigin(1) },
							 { r3(0), r3(1), r3(2), -viewOrigin(2) },
							 {     0,     0,    0,               1 } };

	return CMat4f(finalArr);  // Da steht die Verschiebung richtig drin!
}


CMat4f getRotation(float x, float y, float z) {
	float rotXArr[4][4] = { {1,      0,       0, 0},
							{0, cos(x), -sin(x), 0},
							{0, sin(x),  cos(x), 0},
							{0,      0,       0, 1} };
	float rotYArr[4][4] = { { cos(y), 0, sin(y), 0},
							{      0, 1,      0, 0},
							{-sin(y), 0, cos(y), 0},
							{      0, 0,      0, 1} };
	float rotZArr[4][4] = { {cos(z), -sin(z), 0, 0},
							{sin(z),  cos(z), 0, 0},
							{     0,       0, 1, 0},
							{     0,       0, 0, 1} };
	return CMat4f(rotXArr) * CMat4f(rotYArr) * CMat4f(rotZArr);
}

CVec4f cameraRotation(CVec4f b, float angle, CVec4f point) {
	float translation1arr[4][4] = { { 1, 0, 0, -eyePoint(0) },
								    { 0, 1, 0, -eyePoint(1) },
								    { 0, 0, 1, -eyePoint(2) },
								    { 0, 0, 0,            1 } };
	float translation2arr[4][4] = { { 1, 0, 0, eyePoint(0) },
								    { 0, 1, 0, eyePoint(1) },
								    { 0, 0, 1, eyePoint(2) },
								    { 0, 0, 0,           1 } };
	float c = cos(angle);
	float s = sin(angle);
	float cs = 1 - c;
	float rotationArr[4][4] = { { b(0) * b(0) * cs +        c, b(0) * b(1) * cs - b(2) * s, b(0) * b(2) * cs + b(1) * s, 0 },
								{ b(0) * b(1) * cs + b(2) * s, b(1) * b(1) * cs +        c, b(1) * b(2) * cs - b(0) * s, 0 },
								{ b(0) * b(2) * cs - b(1) * s, b(1) * b(2) * cs + b(0) * s, b(2) * b(2) * cs +        c, 0 },
								{                           0,                           0,                           0, 1 } };
	CMat4f translation1(translation1arr);
	CMat4f translation2(translation2arr);
	CMat4f rotation(rotationArr);
	return translation2 * rotation * translation1 * point;
}


CVec3f projectZallg(CMat4f matTransf, float fFocus, CVec4f pWelt) {
	float Rarr[4][4] = { { matTransf(0, 0), matTransf(1, 0), matTransf(2, 0), 0 },
						 { matTransf(0, 1), matTransf(1, 1), matTransf(2, 1), 0 },
						 { matTransf(0, 2), matTransf(1, 2), matTransf(2, 2), 0 },
						 {               0,               0,               0, 1 } };
	float Parr[4] = { -matTransf(0, 3), -matTransf(1, 3), -matTransf(2, 3), 1 };
	CVec4f P(Parr);
	CMat4f R(Rarr);
	float matTransfInvArr[4][4] = { { R(0, 0), R(1, 0), R(2, 0),    P(0) },
									{ R(0, 1), R(1, 1), R(2, 1),    P(1) },
									{ R(0, 2), R(1, 2), R(2, 2),    P(2) },
									{       0,       0,       0,       1 } };
	CMat4f matTransfInv(matTransfInvArr);
	CVec4f cameraPoint = pWelt * matTransfInv;
	cameraPoint(3) = 1.0;

	float viewRightArr[4] = { viewUp(1) * viewDir(2) - viewUp(2) * viewDir(1),
						      viewUp(2) * viewDir(0) - viewUp(0) * viewDir(2),
						      viewUp(0) * viewDir(1) - viewUp(1) * viewDir(0),
																		     1 };
	CVec4f viewRight(viewRightArr);
	float viewRightLen = sqrt(pow(viewRight(0), 2) + pow(viewRight(1), 2) + pow(viewRight(2), 2));
	viewRight *= (1 / viewRightLen);
	viewRight(3) = 1;

	CVec4f viewDirPoint = cameraRotation(viewDir, aRotAngle, cameraPoint);
	CVec4f viewUpPoint = cameraRotation(viewUp, bRotAngle, viewDirPoint);
	CVec4f finalPoint = cameraRotation(viewRight, cRotAngle, viewUpPoint);
	CVec4f movedFinalPoint = finalPoint - P;
	return projectZ(fFocus, movedFinalPoint);
}



void drawQuader(CVec4f quader[8], float fFocus, Color c) {
	CMat4f transform = getTransform(eyePoint, viewDir, viewUp);
	CMat4f rotMatr = getRotation(xRotAngle, yRotAngle, zRotAngle);
	transform = transform * rotMatr;  // Die hier (sollte) auch stimmen, zumindest die Translation
	CVec3f projectedQuader[8] = {};
	for (int i = 0; i < 8; i++) {
		//projectedQuader[i] = projectZ(fFocus, quader[i]);
		projectedQuader[i] = projectZallg(transform, fFocus, quader[i]);
	}
	drawProjectedZ(projectedQuader, c);
}


void display3(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	///////
	// display your data here ...
	//
	float point0Arr0[4] = {  5.0,  5.0,  0.0, 1.0 };
	float point0Arr1[4] = { 20.0,  5.0,  0.0, 1.0 };
	float point0Arr2[4] = {  5.0, 20.0,  0.0, 1.0 };
	float point0Arr3[4] = { 20.0, 20.0,  0.0, 1.0 };
	float point0Arr4[4] = {  5.0,  5.0, -7.0, 1.0 };
	float point0Arr5[4] = { 20.0,  5.0, -7.0, 1.0 };
	float point0Arr6[4] = {  5.0, 20.0, -7.0, 1.0 };
	float point0Arr7[4] = { 20.0, 20.0, -7.0, 1.0 };

	CVec4f points0[8] = {
		CVec4f(point0Arr0),
		CVec4f(point0Arr1),
		CVec4f(point0Arr2),
		CVec4f(point0Arr3),
		CVec4f(point0Arr4),
		CVec4f(point0Arr5),
		CVec4f(point0Arr6),
		CVec4f(point0Arr7)
	};

	float point1Arr0[4] = { -40.0, -40.0,   0.0, 1.0 };
	float point1Arr1[4] = { -20.0, -40.0,   0.0, 1.0 };
	float point1Arr2[4] = { -40.0, -20.0,   0.0, 1.0 };
	float point1Arr3[4] = { -20.0, -20.0,   0.0, 1.0 };
	float point1Arr4[4] = { -40.0, -40.0, -10.0, 1.0 };
	float point1Arr5[4] = { -20.0, -40.0, -10.0, 1.0 };
	float point1Arr6[4] = { -40.0, -20.0, -10.0, 1.0 };
	float point1Arr7[4] = { -20.0, -20.0, -10.0, 1.0 };

	CVec4f points1[8] = {
		CVec4f(point1Arr0),
		CVec4f(point1Arr1),
		CVec4f(point1Arr2),
		CVec4f(point1Arr3),
		CVec4f(point1Arr4),
		CVec4f(point1Arr5),
		CVec4f(point1Arr6),
		CVec4f(point1Arr7)
	};

	float point2Arr0[4] = { 50.0, -10.0,  0.0, 1.0 };
	float point2Arr1[4] = { 80.0, -10.0,  0.0, 1.0 };
	float point2Arr2[4] = { 50.0,  20.0,  0.0, 1.0 };
	float point2Arr3[4] = { 80.0,  20.0,  0.0, 1.0 };
	float point2Arr4[4] = { 50.0, -10.0, -5.0, 1.0 };
	float point2Arr5[4] = { 80.0, -10.0, -5.0, 1.0 };
	float point2Arr6[4] = { 50.0,  20.0, -5.0, 1.0 };
	float point2Arr7[4] = { 80.0,  20.0, -5.0, 1.0 };

	CVec4f points2[8] = {
		CVec4f(point2Arr0),
		CVec4f(point2Arr1),
		CVec4f(point2Arr2),
		CVec4f(point2Arr3),
		CVec4f(point2Arr4),
		CVec4f(point2Arr5),
		CVec4f(point2Arr6),
		CVec4f(point2Arr7)
	};

	drawQuader(points0, fFocus, Color(1.0, 0.0, 0.0));
	drawQuader(points1, fFocus, Color(0.0, 1.0, 0.0));
	drawQuader(points2, fFocus, Color(0.0, 1.0, 1.0));

	// In double buffer mode the last
	// two lines should always be
	glFlush();
	glutSwapBuffers(); // swap front and back buffer
}


void keyboard (unsigned char key, int x, int y) 
{
	switch (key) {
		case 'q':
		case 'Q':
			exit (0); // quit program
			break;
		case '1':
			glutDisplayFunc (display1);
			//glutPostRedisplay ();	// not needed since timer triggers redisplay
			break;
		case '2':
			glutDisplayFunc (display2);
			//glutPostRedisplay ();	// not needed since timer triggers redisplay
			break;
		case '3':
			glutDisplayFunc(display3);
			break;
		// Focal length
		case 'F':
			fFocus += fFocusIncrement;
			break;
		case 'f':
			if (fFocus > fFocusIncrement) {
				fFocus -= fFocusIncrement;
			}
			break;
		// Rotation around world xyz
		case 'X':
			xRotAngle += xyzIncrement;
			break;
		case 'Y':
			yRotAngle += xyzIncrement;
			break;
		case 'Z':
			zRotAngle += xyzIncrement;
			break;
		case 'x':
			xRotAngle -= xyzIncrement;
			break;
		case 'y':
			yRotAngle -= xyzIncrement;
			break;
		case 'z':
			zRotAngle -= xyzIncrement;
			break;
		// Rotation around view xyz
		case 'A':
			aRotAngle += abcIncrement;
			break;
		case 'a':
			aRotAngle -= abcIncrement;
			break;
		case 'B':
			bRotAngle += abcIncrement;
			break;
		case 'b':
			bRotAngle -= abcIncrement;
			break;
		case 'C':
			cRotAngle += abcIncrement;
			break;
		case 'c':
			cRotAngle -= abcIncrement;
			break;
		// Translation in world xyz
		case 'U':
			eyePoint(0) += uvwIncrement;
			break;
		case 'u':
			eyePoint(0) -= uvwIncrement;
			break;
		case 'V':
			eyePoint(1) += uvwIncrement;
			break;
		case 'v':
			eyePoint(1) -= uvwIncrement;
			break;
		case 'W':
			eyePoint(2) += uvwIncrement;
			break;
		case 'w':
			eyePoint(2) -= uvwIncrement;
			break;
		// Reset view
		case 'R':
		case 'r':
			initView();
			break;
		default:
			// do nothing ...
			break;
	};
}


int main (int argc, char **argv) 
{
	glutInit (&argc, argv);
	// we have to use double buffer to avoid flickering
	// TODO: lookup "double buffer", what is it for, how is it used ...
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow ("Übung 2");

	init ();	// init my variables first
	initGL ();	// init the GL (i.e. view settings, ...)

	// assign callbacks
	glutTimerFunc (10, timer, 0);
	glutKeyboardFunc (keyboard);
	glutDisplayFunc (display1);
	// you might want to add a resize function analog to
	// Übung1 using code similar to the initGL function ...

	// start main loop
	glutMainLoop ();

	return 0;
}
