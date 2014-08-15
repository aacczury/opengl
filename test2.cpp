#include <cstdlib>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

void init(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin( GL_QUADS );
		glColor3f( 0.0, 1.0, 1.0 );
		glVertex3f( -0.5, -0.5, -1.0 );
		glVertex3f( -0.5, 0.5, -1.0 );
		glVertex3f( 0.5, 0.5, -2.0 );
		glVertex3f( 0.5, -0.5, -2.0 );
	glEnd();
	glutSwapBuffers();
}

void resize(int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble) w / h, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 2.0, // eye
				0.0, 0.0, 0.0, //center
				0.0, 1.0, 0.0); //up
}

void idle(void){
	/*t += dt;
	glutPostRedisplay();*/
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'q' :
		case 'Q' :
			exit(EXIT_SUCCESS);
			break;
		
		case 'r' :
		case 'R' :
			//rotate = GL_TRUE;
			glutPostRedisplay();
			break;
	}
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("第一個OpenGL程式");
	init();
    glutDisplayFunc(display);
	glutReshapeFunc(resize);
	//glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}