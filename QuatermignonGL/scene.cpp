#include "common.h"
#include "Quaternion.h"
#include "Camera.h"
#include <math.h>
#include "Cube.h"

#define M_PI 3.14159265359

/** GLOBALS **/
GLfloat yaw, pitch;
GLfloat xAngle, yAngle, zAngle;
GLfloat distance = 20;
GLdouble size = 1.5;
const GLint zoom = -8;
Camera* cam = new Camera();
int isRunning = 1;

GLuint texture1;

/** INPUTS MANAGER FUNCTIONS **/
void KeyboardDown(unsigned char key, int xx, int yy)
{
    switch (key)
    {
    case 'e': // Unlock Camera
        cam->locked = (cam->locked) ? 0 : 1;
        break;
    case 'z':
        cam->deltaForward = 1;
        break;
    case 's':
        cam->deltaForward = -1;
        break;
    case 'd':
        cam->deltaStrafe = -1;
        break;
    case 'q':
        cam->deltaStrafe = 1;
        break;
	case 27:
		isRunning = 0;
		break;
    }
}
void KeyboardUp(unsigned char key, int xx, int yy)
{
    switch (key)
    {
    case 'z':
    case 's':
        cam->deltaForward = 0;
        break;
    case 'q':
    case 'd':
        cam->deltaStrafe = 0;
        break;
    }
}
void SpecialDown(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        cam->deltaForward = 1;
        break;
    case GLUT_KEY_DOWN:
        cam->deltaForward = -1;
        break;
    case GLUT_KEY_RIGHT:
        cam->deltaStrafe = -1;
        break;
    case GLUT_KEY_LEFT:
        cam->deltaStrafe = 1;
        break;
    }
}

void SpecialUp(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        cam->deltaForward = 0;
        break;
    case GLUT_KEY_RIGHT:
    case GLUT_KEY_LEFT:
        cam->deltaStrafe = 0;
        break;
    }
}

void mouseMove(int x, int y)
{
	
    // Rentres uniquement lors du clic
    cam->orienterCam(x, y);
	yaw += (x - 350) * 0.005;
	
	 
	GLfloat pitchDif = (y - 350) * 0.005f;

	if(pitch + pitchDif >= -M_PI/2 && pitch + pitchDif < M_PI / 2)
		pitch += pitchDif;
	glutWarpPointer(350, 350);
}
void mouseButton(int button, int state, int x, int y){ cam->grabCam(x, y); }

/** CAMERA MOVEMENTS **/
void computePos(int osef)
{

    cam->updatePos();
    glutTimerFunc(10, computePos, 0);
}

/** GLUT FUNCTIONS **/
void Initialize(void)
{
    glutSetCursor(GLUT_CURSOR_NONE);
	distance = 10;
	yaw = pitch = .0;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//texture1 = loadTexture("stainedglass05.jpg");


}

void Shutdown(void)
{

}

void Reshape(int x, int y)
{
    if (y == 0 || x == 0) return;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 0.6, 210.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, x, y);  //Use the whole window for rendering
}

void Update(void)
{
    xAngle += 0.001;
    yAngle += 0.001;
    zAngle += 0.001;
	if (isRunning == 0)
		glutLeaveMainLoop();
	Render();
}

void drawRepere(int size) {

	glColor3f(0.5f, 0.0, 0.0);
	for (int i = -size; i <= size; i++) {
		glBegin(GL_LINES);
		glVertex3f(i, 0, -size - 1);
		glVertex3f(i, 0, size + 1);
		glEnd();
	}

	glColor3f(0.0f, 0.5, 0.0);
	for (int i = -size; i <= size; i++) {
		glBegin(GL_LINES);
		glVertex3f(-size - 1, 0, i);
		glVertex3f(size + 1, 0, i);
		glEnd();
	}

}

void Render(void)
{
    glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	gluLookAt(
		distance * cos(pitch) * cos(yaw),
		distance * sin(pitch),
		distance * cos(pitch) * sin(yaw),
		0, 
		1, 
		0, 
		0.0f,
		1.0f,
		0.0f);


	

    glutTimerFunc(10, computePos, 0);

    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(KeyboardDown);
    glutKeyboardUpFunc(KeyboardUp);
    glutSpecialFunc(SpecialDown);
    glutSpecialUpFunc(SpecialUp);

    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);


	

	glPushMatrix();

	drawRepere(10);

	glTranslatef(0, 1, 0);

    // #Fee9fc color is the most beautifule pink in the world.
    glColor3f(0.99, 0.87, 0.97);

    // scaling transfomation 
    glScalef(1.0, 1.0, 1.0);

	Quaternion rotX(xAngle, 1.0, 2.0, 3.0);
    //Quaternion rotY(yAngle, 0.0, 1.0, 0.0);
	//Quaternion rotZ(zAngle, 0.0, 0.0, 1.0);

	Cube cubePrincipal(0,0,0, rotX);

	cubePrincipal.draw();


	/*
	glTranslatef(0, -1.0, 0.0);
	glScalef(30.0, .1f, 30.0);
	
	glColor3f(0.5f, 0.5f, 0.5f);
	glutSolidCube(1);
	*/

	glPopMatrix();

    // Flush buffers to screen
    glFlush();



}


