#include <glut.h>
#include <time.h>  

#include "GameObj.h"
#include "Bullet.h"
#include "Ship.h"
#include "GameSession.h"
#include "HumanControl.h"
#include "utility.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//-- Global Variable storing the Game Session 
GameSession Game;

//---------------------------------------------------------
//-- Glut Callback on Window Reshape.
void Reshape(GLsizei w, GLsizei h)
{ 
    glutReshapeWindow(w,h);
    glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);
	glMatrixMode(GL_MODELVIEW);
}		

//---------------------------------------------------------
//-- Glut Idle Callback .
static float lasttime;
static float dt;
void Idle(void)
{
	dt=glutGet(GLUT_ELAPSED_TIME)-lasttime;
	lasttime=glutGet(GLUT_ELAPSED_TIME);

	if(Game.m_state == GameSession::STATE_PLAY)
		//Game.Update(dt/1000.0f);// GAMESTATEUPDATE //AI OR PLAYER
		Game.Update(0.016f*Game.m_timeScale);//lock at 60
    else if(Game.m_state == GameSession::STATE_STEP)
    {
        Game.Update(0.016f);
        Game.m_state = GameSession::STATE_STEPWAIT;
    }

	glutPostRedisplay();
}

//---------------------------------------------------------
//-- Glut Callback on Window visibility.
void Visible(int vis)
{
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(Idle);
	else
		glutIdleFunc(NULL);
}

//---------------------------------------------------------
//-- Glut Callback to Draw everything.
void Display(void)
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Game.Draw();
    glFinish();
	glutSwapBuffers();
}

//---------------------------------------------------------
int main(int argc, char** argv)
{ 
	glutInit(&argc, argv);
	Game.m_screenW=1024;
	Game.m_screenH=1024;
	glutInitWindowSize(Game.m_screenW,Game.m_screenH);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutCreateWindow("AIsteroids");

	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutVisibilityFunc(Visible);
	
	//sets up the keyboard functions
	glutKeyboardFunc(Key);	
	glutKeyboardUpFunc(KeyUp);	
	glutSpecialFunc(SpecialKey);	
	glutSpecialUpFunc(SpecialKeyUp);	

	glInitColorArray();
	srand(time(NULL));	
	Game.StartGame();
	glutMainLoop();
  return 0;             
}

