#include <cstdio>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include "imageloader.h"
#pragma comment(lib, "glew32.lib")

class Planet {
public:
	float rad, dst, orbit, orbitSpeed, axTlt, axAnimate;
	// ������, ���� �˵� ��ġ, �˵����� ���� �ӵ� ����, ���� �ӵ�
	Planet(float paramRadius, float paramDist, float paramOrbit, float paramOrbitSpeed, float paramAxTilt, float paramAxAnimate) {

		rad = paramRadius;
		dst = paramDist;
		orbit = paramOrbit;
		orbitSpeed = paramOrbitSpeed;
		axTlt = paramAxTilt;
		axAnimate = paramAxAnimate;
	}

	void generateSmallOrb(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, dst, 100.0, 100.0);
		glPopMatrix();
	}

	void drawMoon(void) {
		GLUquadricObj *qad;
		qad = gluNewQuadric();
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(dst, 0.0, 0.0);
		gluSphere(qad, rad, 20.0, 20.0);
		glPopMatrix();
	}

};
//                set start locate
// rad, dist, orbit, orbit speed, axis tilt, axAnimate 
Planet sun(5.0, 0, 0, 0, 0, 0);                  //�� 2.11
Planet mer(1.0, 7, 50, 4.74, 20, 0);		     //Mercury ����  02.11
Planet ven(1.5, 11, -80, 3.50, 177.0, 0);		 //Venus �ݼ�
Planet ear(2.0, 16, 90, 2.98, 23.44, 0);		 //Earth ����
Planet mar(1.2, 21, 80, 2.41, 25.00, 0);		 //Mars ȭ��
Planet jup(3.5, 28, 50, 1.31, 03.13, 0);		 //Jupiter ��
Planet sat(3.0, 37, 30, 0.97, 26.70, 0);		 //Saturn �伺
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	     //Uranus õ�ռ�
Planet nep(2.3, 53.6, -5, 0.54, 28.32, 0);	     //Neptune �ؿռ�
Planet plu(0.3, 59, -10, 0.47, 119.6, 0);		 //Pluto ���ռ�

Planet lun(.40, 3, -30, 5.40, 0, 0);			 //���� ��
Planet phoLun(.20, 1.8, -50, 2.30, 0, 0);		     //ȭ���� 1
Planet phoLunSec(.24, 2.4, -80, 3.60, 0, 0);		     //ȭ���� 2
Planet marLun(.24, 4, 800, 4.40, 0, 0);			 //�񼺴� 1   
Planet marLunSec(.24, 4.7, -120, 5.00, 0, 0);		     //�񼺴� 2 
Planet marLunThird(.24, 5.3, -150, 2.30, 0, 0);		     //�񼺴� 3 
Planet titLun(.75, 3.7, -180, 2.40, 0, 0);		     //�伺�� 1	
Planet nixLun(.10, 1.5, -500, 5.00, 0, 0);		     //õ�մ� 1
Planet pucLun(.26, 2.9, -214, 7.00, 0, 0);		     //�ؿմ� 1	
Planet triLun(.36, 3.2, -260, 3.40, 0, 0);		     //���մ� 1

int isAnimate = 1; //�����̽��� ����ŷ
int bigOrbIsVisible = 1; // ū �༺ �˵� ����ŷ
int smallOrbIsVisible = 1; // �޵� �˵� ����ŷ
int moonIsVisible = 1; // �� ���� ����ŷ
int chgCam = 0; // ī�޶� ���
int tagIsVisible = 0; // �� ����
int zoom = 50; // �ܰ�


float lightPos[] = { 0.0, 0.0, -75.0, 1.0 }; // ���� ��ġ
static float spotAngle = 40;                 // ���� ��
float spotDirection[] = { 1.0, 0.0, 0.0 };   // ���� ����
static float spotExponent = 1.0;
// �̹������� �ؽ��ķ� �������ִ� �Լ�
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture,
jupTexture, satTexture, uraTexture, nepTexture, pluTexture, starTexture;

// �󺧺��϶� ��Ʈ�� �����ִ� �Լ�
void generateTag(void *font, char *string) {
	char *c;
	// ��Ʈ�� ������ŭ ���������� �� ����
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void setup(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//�̹��� �θ��� �ؽ��ĸ�
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	// �̹����� �ε� �ϰ� �޸� �Ҹ�
	Image* sta = loadBMP("stars.bmp");		starTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("sun.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturn.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptune.bmp");	nepTexture = loadTexture(nep);		delete nep;
	Image* plu = loadBMP("pluto.bmp");		pluTexture = loadTexture(plu);		delete plu;

	//���� ���õ�
	glEnable(GL_LIGHTING);
	float lightSpec[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDiffAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float ambLightSpec[] = { 0.5, 0.5, 0.5, 1.0 };


	// ���� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightSpec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDiffAndSpec);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLightSpec);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}


// �༺�� ��� ǥ�� ���ִ� �Լ�
void generateBigOrb(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.dst, 100.0, 100.0);
	glutWireTorus(0.001, ven.dst, 100.0, 100.0);
	glutWireTorus(0.001, ear.dst, 100.0, 100.0);
	glutWireTorus(0.001, mar.dst, 100.0, 100.0);
	glutWireTorus(0.001, jup.dst, 100.0, 100.0);
	glutWireTorus(0.001, sat.dst, 100.0, 100.0);
	glutWireTorus(0.001, ura.dst, 100.0, 100.0);
	glutWireTorus(0.001, nep.dst, 100.0, 100.0);
	glutWireTorus(0.001, plu.dst, 100.0, 100.0);
	glPopMatrix();
}

// display���� �Լ�
void myDisplay(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// ī�޶� ��� ����ŷ �� �κп� ���� glookat ����
	if (chgCam == 0) {
		gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	if (chgCam == 1) {
		gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	if (chgCam == 2) {
		gluLookAt(0.0, zoom, 0.00001, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	if (bigOrbIsVisible == 1) {
		generateBigOrb();
	}

	GLUquadric *quadric = gluNewQuadric();

	//��
	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(-1.2, 7.0, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Sun"); // �� �ӹ�ư
	}
	glPushMatrix();
	glRotatef(sun.axTlt, 1.0, 0.0, 0.0);
	glRotatef(sun.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1); // ���� ��ü ������
	gluSphere(quadric, sun.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//����
	glPushMatrix();           //1.0
	glRotatef(mer.orbit, 0.0, 3.0, 0.0);
	glTranslatef(mer.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Mercury");
	}
	glPushMatrix();
	glRotatef(mer.axTlt, 1.0, 0.0, 0.0);
	glRotatef(mer.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//�ݼ�
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Venus");
	}
	glPushMatrix();
	glRotatef(ven.axTlt, 1.0, 0.0, 0.0);
	glRotatef(ven.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	//����, �ޱ˵�, ��
	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Earth");
	}
	glPushMatrix();
	glRotatef(ear.axTlt, 1.0, 0.0, 0.0);
	glRotatef(ear.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ear.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		lun.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		lun.drawMoon();
	}
	glPopMatrix();

	//ȭ��, ȭ�� �˵�, ȭ����
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 3, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Mars");
	}
	glPushMatrix();
	glRotatef(mar.axTlt, 1.0, 0.0, 0.0);
	glRotatef(mar.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		phoLun.generateSmallOrb();
		phoLunSec.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		phoLun.drawMoon();
		phoLunSec.drawMoon();
	}
	glPopMatrix();

	//��, �� �� �˵�, ��
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Jupiter");
	}
	glPushMatrix();
	glRotatef(jup.axTlt, 1.0, 0.0, 0.0);
	glRotatef(jup.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		marLun.generateSmallOrb();
		marLunSec.generateSmallOrb();
		marLunThird.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		marLun.drawMoon();
		marLunSec.drawMoon();
		marLunThird.drawMoon();
	}
	glPopMatrix();

	//�伺, �伺 �˵�, �伺 ��
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Saturn");
	}
	glPushMatrix();
	glRotatef(sat.axTlt, 1.0, 0.0, 0.0);
	glRotatef(sat.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.rad, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		titLun.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		titLun.drawMoon();
	}
	glPopMatrix();
	glColor3ub(255, 255, 255);

	//õ�ռ�, õ�ռ� �˵�, õ�ռ� ��
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Uranus");
	}
	glPushMatrix();
	glRotatef(ura.axTlt, 1.0, 0.0, 0.0);
	glRotatef(ura.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		pucLun.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		pucLun.drawMoon();
	}
	glPopMatrix();

	//�ؿռ�, �ؿռ� �˵�, �ؿռ���
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 4.4, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Neptune");
	}
	glPushMatrix();
	glRotatef(nep.axTlt, 1.0, 0.0, 0.0);
	glRotatef(nep.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbIsVisible == 1) {
		triLun.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		triLun.drawMoon();
	}
	glPopMatrix();

	//���ռ�, ���ռ� �˵�, ���ռ� ��
	glPushMatrix();
	glRotatef(plu.orbit, 0.0, 1.0, 0.0);
	glTranslatef(plu.dst, 0.0, 0.0);
	if (tagIsVisible == 1) {
		glRasterPos3f(0.0, 3.0, 0.0);
		glColor3ub(255, 255, 255);
		generateTag(GLUT_BITMAP_HELVETICA_12, "Pluto");
	}
	glPushMatrix();
	glRotatef(plu.axTlt, 1.0, 0.0, 0.0);
	glRotatef(plu.axAnimate, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, plu.rad, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	// ����ŷ ���ο� ���� ��, �� �˵� �׸��� ���� ���� ���� if���鵵 ��������
	if (smallOrbIsVisible == 1) {
		nixLun.generateSmallOrb();
	}
	if (moonIsVisible == 1) {
		nixLun.drawMoon();
	}
	glPopMatrix();


	// ���ȭ��
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, starTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0);
	glVertex3f(-200, -200, -100);

	glTexCoord2f(2.0, 0.0);
	glVertex3f(200, -200, -100);

	glTexCoord2f(2.0, 2.0);
	glVertex3f(200, 200, -100);

	glTexCoord2f(-1.0, 2.0);
	glVertex3f(-200, 200, -100);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, starTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-200, -83, 200);

	glTexCoord2f(8.0, 0.0);
	glVertex3f(200, -83, 200);

	glTexCoord2f(8.0, 8.0);
	glVertex3f(200, -83, -200);

	glTexCoord2f(0.0, 8.0);
	glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glutSwapBuffers();
}


// resize �ݹ� �Ķ����
void myResize(int w, int h) {
	glViewport(0, 0, w, h);
	// ����Ʈ �޾Ƽ� ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void animate(int n) {
	// ���ǵ尪 + ����ġ �� �༺��ġ, �༺�� ���� �� ��ġ �̵���Ŵ
	if (isAnimate) {
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		jup.orbit += jup.orbitSpeed;
		sat.orbit += sat.orbitSpeed;
		ura.orbit += ura.orbitSpeed;
		nep.orbit += nep.orbitSpeed;
		plu.orbit += plu.orbitSpeed;
		lun.orbit += lun.orbitSpeed;
		phoLun.orbit += phoLun.orbitSpeed;
		phoLunSec.orbit += phoLunSec.orbitSpeed;
		marLun.orbit += marLun.orbitSpeed;
		marLunSec.orbit += marLunSec.orbitSpeed;
		marLunThird.orbit += marLunThird.orbitSpeed;
		titLun.orbit += titLun.orbitSpeed;
		nixLun.orbit += nixLun.orbitSpeed;
		pucLun.orbit += pucLun.orbitSpeed;
		triLun.orbit += triLun.orbitSpeed;

		// 360�� ��á���� �ٽ� ���󺹱�
		if (mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, phoLun, phoLunSec, marLun, marLunSec, marLunThird, titLun, nixLun, pucLun, triLun.orbit > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, phoLun, phoLunSec, marLun, marLunSec, marLunThird, titLun, nixLun, pucLun, triLun.orbit -= 360.0;
		}
		mer.axAnimate += 10.0;
		ven.axAnimate += 10.0;
		ear.axAnimate += 10.0;
		mar.axAnimate += 10.0;
		jup.axAnimate += 10.0;
		sat.axAnimate += 10.0;
		ura.axAnimate += 10.0;
		nep.axAnimate += 10.0;
		plu.axAnimate += 10.0;
		if (mer, ven, ear, mar, jup, sat, ura, nep, plu.axAnimate > 360.0) {
			mer, ven, ear, mar, jup, sat, ura, nep, plu.axAnimate -= 360.0;
		}
		glutPostRedisplay(); // ȭ�� ������Ʈ
		glutTimerFunc(1, animate, 1); // 1ms�� �ִϸ��̼� ���� 1�� isAnimate��(�÷��װ�) ��� �˵� �̵��ϰ� ��Ŵ

	}
}

// ������ ���콺 Ŭ���� ����
void mouseRightClick(int button, int state, int x, int y) {
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		exit(0);
	}
	glutPostRedisplay();
}


//���콺 �� �ݹ� �Լ� ���� ����(zoom �� ����)
void mouseWhel(int wheel, int direction, int x, int y) {
	if (direction > 0 && zoom < 100) {
		zoom++;
	}
	if (direction < 0 && zoom > -75) {
		zoom--;
	}
	glutPostRedisplay();
}


//Ű���� �ݹ�
void keyInput(unsigned char key, int x, int y) {

	switch (key) {
	case 27:
		exit(0);
		break;

	case ' ':
		if (isAnimate) { // �����̽��� ��������
			isAnimate = 0; // ����ŷ
		}
		else {
			isAnimate = 1;
			animate(1);
		}
		break;

	case 'o':
		if (smallOrbIsVisible) { //�� �˵� ����ŷ
			smallOrbIsVisible = 0;
		}
		else {
			smallOrbIsVisible = 1;
			glutPostRedisplay();
		}
		break;

	case 'O':
		if (bigOrbIsVisible) { //�༺�˵� ����ŷ
			bigOrbIsVisible = 0;
		}
		else {
			bigOrbIsVisible = 1;
			glutPostRedisplay();
		}
		break;
	case 'M':
	case 'm':
		if (moonIsVisible) { //�� ���� ����ŷ
			moonIsVisible = 0;
		}
		else {
			moonIsVisible = 1;
			glutPostRedisplay();
		}
		break;

	case 'L':
	case 'l':
		if (tagIsVisible) {
			tagIsVisible = 0;
		}
		else {
			tagIsVisible = 1;
			glutPostRedisplay();
		}
		break;

	case '1':
		chgCam = 0;
		glutPostRedisplay();
		break;

	case '2':
		chgCam = 1;
		glutPostRedisplay();
		break;

	case '3':
		chgCam = 2;
		glutPostRedisplay();
		break;
	}

}

void intro(void) {
	printf(
		"SPACE : ���, �Ͻ�����."               "\n"
		"ESC : ������."                        "\n"
		"�빮��O : �˵� ǥ�� �ѹ� �������� ����"    "\n"
		"�ҹ���o : �� �˵� ǥ�� �ѹ� �� ������ ����, ��������""\n"
		"m/M : ��ǥ�� �ѹ� �� ������ ����."       "\n"
		"L/l to show/hide labels"             "\n"
		"1, 2, 3 : ī�޶� ����, ����"            "\n"
		"���콺 ��ũ�� : ����, �ܾƿ�"            "\n"
	);
}

int main(int argc, char **argv) {
	intro();
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(700, 700);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("�¾��");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myResize);
	glutMouseFunc(mouseRightClick);
	glutKeyboardFunc(keyInput);
	glutMouseWheelFunc(mouseWhel);
	glewInit();

	setup();
	glutMainLoop();
}