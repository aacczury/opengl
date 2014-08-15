#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

void init(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin( GL_QUADS );
		glColor3f( 0.0, 1.0, 1.0 );
		glVertex2f( -0.5, -0.5 );
		glVertex2f( -0.5, 0.5 );
		glVertex2f( 0.5, 0.5 );
		glVertex2f( 0.5, -0.5 );
	glEnd();
	glutSwapBuffers();
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
    glutMainLoop();
    return 0;
}