#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

int RenderMode = GL_RENDER;
GLdouble fAspect;

void init(void){

	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
}

int old_rot_x=0;   //剛按下滑鼠時的視窗座標
int old_rot_y=0;

int rot_x=0;      //拖曳後的相對座標，用這決定要旋轉幾度
int rot_y=0;

int record_x=0;      //紀錄上一次旋轉的角度
int record_y=0;

unsigned int selectBuff[4];

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 5.0, // eye
				0.0, 0.0, 0.0, //center
				0.0, 1.0, 0.0); //up
	
	cout << "display: " <<selectBuff[3] <<endl;
	glInitNames();
	glPushName( 0 );
	
	glRotatef((float)rot_y+(float)record_y, 1.0, 0.0, 0.0);//以x軸當旋轉軸
	glRotatef((float)rot_x+(float)record_x, 0.0, 1.0, 0.0);//以y軸當旋轉軸
	
	glPushMatrix(); 
	glLoadName( 1 );
	glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
	glTranslated(0.0,2.0,0.0);
	glutSolidSphere(0.5, 100, 100 );
	glPopMatrix();
	
	if( selectBuff[3] == 1 ){
		glPushMatrix(); 
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glTranslated(0.0,2.0,0.0);
		glutSolidSphere(0.1, 100, 100 );
		glPopMatrix();
	}

	glPushMatrix(); 
	glLoadName( 2 );
	glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
	glTranslated(-1.0,0.0,0.0);
	glutSolidSphere(0.5, 100, 100 );
	glPopMatrix();
	if( selectBuff[3] == 2 ){
		glPushMatrix(); 
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		glTranslated(-1.0,0.0,0.0);
		glutSolidSphere(0.1, 100, 100 );
		glPopMatrix();
	}
	
	glPushMatrix(); 
	glLoadName( 3 );
	glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
	glTranslated(1.0,0.0,0.0);
	glutSolidSphere(0.5, 100, 100 );
	glPopMatrix();
	if( selectBuff[3] == 3 ){
		glPushMatrix(); 
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glTranslated(1.0,0.0,0.0);
	glutSolidSphere(0.1, 100, 100 );
		glPopMatrix();
	}
	
	glPushMatrix(); 
	glLoadName( 4 );
	glPushMatrix(); 
	glBegin( GL_TRIANGLES );
		glColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	if( selectBuff[3] == 4 ){
		glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	}
		glVertex3f( 0.0, 2.0, 0.0 );
		glVertex3f( -1.0, 0.0, 0.0 );
		glVertex3f( 1.0, 0.0, 0.0 );
	glEnd();
	glPopMatrix();
	
	if(RenderMode == GL_RENDER)
		glutSwapBuffers();
}

void resize(int w, int h){
	fAspect = (GLdouble) w / h;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, fAspect, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void PickObject(unsigned int id){
	switch(id)
	{
	case 0:
		cout<<"00"<<endl;
		break;
	case 1:
		cout<<"11"<<endl;
		break;
	case 2:
		cout<<"22"<<endl;
		break;
	case 3:
		cout<<"33"<<endl;
		break;
	case 4:
		cout<<"44"<<endl;
		break;
	default :
		cout<<"QQ"<<endl;
	}
	glutPostRedisplay();
}

void ProcessSelection(int xPos, int yPos){
	int hits;                        //儲存滑鼠點選的座標到底點到了幾個物件
	int viewport[4];

	glSelectBuffer(4, selectBuff);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	RenderMode = GL_SELECT;
	glRenderMode(GL_SELECT);
	glLoadIdentity();
	gluPickMatrix(xPos, viewport[3] - yPos, 2, 2, viewport);
	gluPerspective(60.0f, fAspect, 1.0, 10.0);
	display();
	RenderMode = GL_RENDER;
	hits = glRenderMode(GL_RENDER);
	cout << hits << endl;
	for(int i = 0; i < 4; ++ i )
		cout << selectBuff[i] << " ";
	cout << endl;
	if(hits == 1 || hits == -1)
		PickObject(selectBuff[3]);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void mousebutton(int button, int state, int x, int y){
	if(!state){
		ProcessSelection(x,y);
		old_rot_x = x;
		old_rot_y = y;
	}
	else{
		record_x += x - old_rot_x;
		record_y += y - old_rot_y;
      
		rot_x = 0;   //沒有歸零會有不理想的結果
		rot_y = 0;
	}
}

void mousemotion(int x, int y){
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
}


int main(int argc, char *argv[])
{	
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("讀取obj (簡略版)");
	init();
    glutDisplayFunc(display);
	glutReshapeFunc(resize);
	//glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
    glutMainLoop();
    return 0;
}
