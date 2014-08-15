#include <cstdlib>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

void init(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	
	
	GLfloat light_pos[] = {1.50, 1.00, 0.00, 1.00}; // (x, y, z, luminance) 
	GLfloat light_Ka[] = {0.00, 1.00, 0.00, 1.00}; // (R, G, B, A)
	GLfloat light_Kd[] = {1.00, 1.00, 1.00, 1.00}; // (R, G, B, A)
	GLfloat light_Ks[] = {1.00, 1.00, 1.00, 1.00}; // (R, G, B, A)
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);
	
	/*GLfloat material_Ka[] = {0.11, 0.06, 0.11, 1.00};
	GLfloat material_Kd[] = {0.43, 0.47, 0.54, 1.00};
	GLfloat material_Ks[] = {0.33, 0.33, 0.52, 1.00};
	GLfloat material_Ke[] = {0.00, 0.00, 0.00, 0.00};
	GLfloat material_Se = 10;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT, GL_SHININESS, material_Se);*/
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin( GL_QUADS );
		glColor3f( 0.0, 1.0, 1.0 );
		glVertex3f( -0.5, -0.5, -2.0 );
		glVertex3f( -0.5, 0.5, -2.0 );
		glVertex3f( 0.5, 0.5, -1.0 );
		glVertex3f( 0.5, -0.5, -1.0 );
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