#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>

#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>

using namespace std;

GLuint texture;
	
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

GLuint LoadTextureBMP( const char *filename ){	
	GLuint texture;
	
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char * data;
	
	ifstream file(filename);
	file.read((char *)header, 54);
	
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);
	
	if (imageSize==0)    imageSize=width*height*3;
	if (dataPos==0)      dataPos=54;
	
	data = new unsigned char [imageSize];
	file.read((char *)data, imageSize);
	
	file.close();
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	return texture;
}

void renderObj(){
	ifstream inputObj("ppsh41.obj");
	string str;
	
	vector<double *> v;
	vector<double *> vt;
	vector<double *> vn;
	vector<int **> f;
	
	while(getline(inputObj, str)){
		smatch m;
		if(regex_search(str, m, regex("v "))){ // v
			string a = m.suffix().str();
			smatch b;
			v.push_back(new double[3]);
			int i = 0;
			while(regex_search (a,b,regex("[\\w.-]+"))){
				v[v.size()-1][i ++]=  stod(b[0]);
				a = b.suffix().str();
			}
		}
		
		if(regex_search(str, m, regex("vt "))){ // vt
			string a = m.suffix().str();
			smatch b;
			vt.push_back(new double[2]);
			int i = 0;
			while(regex_search (a,b,regex("[\\w.-]+"))){
				vt[vt.size()-1][i ++]=  stod(b[0]);
				a = b.suffix().str();
			}
		}
		
		if(regex_search(str, m, regex("vn "))){ // vn
			string a = m.suffix().str();
			smatch b;
			vn.push_back(new double[3]);
			int i = 0;
			while(regex_search (a,b,regex("[\\w.-]+"))){
				vn[vn.size()-1][i ++]=  stod(b[0]);
				a = b.suffix().str();
			}
		}
		
		if(regex_search(str, m, regex("f "))){ // f
			string a = m.suffix().str();
			smatch b;
			f.push_back(new int*[3]);
			for( int i = 0; i < 3; ++ i )
				f[f.size()-1][i] = new int[3];
			int i = 0;
			while(regex_search (a,b,regex("(\\d+)/?(\\d*)/?(\\d*)"))){
				f[f.size()-1][i][0] = stoi(b[1]);
				f[f.size()-1][i][1] = b[2].compare("")==0?0:stoi(b[2]);
				f[f.size()-1][i ++][2] = b[3].compare("")==0?0:stoi(b[3]);
				a = b.suffix().str();
			}
		}
	}
	
    glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, LoadTextureBMP("ppsh41.bmp"));
	for(int i = 0; i < f.size(); ++ i){
		glBegin( GL_TRIANGLES );
			glTexCoord2f(vt[f[i][0][1]-1][0], vt[f[i][0][1]-1][1]); glVertex3f( v[f[i][0][0]-1][0], v[f[i][0][0]-1][1], v[f[i][0][0]-1][2] );
			glTexCoord2f(vt[f[i][1][1]-1][0], vt[f[i][1][1]-1][1]); glVertex3f( v[f[i][1][0]-1][0], v[f[i][1][0]-1][1], v[f[i][1][0]-1][2] );
			glTexCoord2f(vt[f[i][2][1]-1][0], vt[f[i][2][1]-1][1]); glVertex3f( v[f[i][2][0]-1][0], v[f[i][2][0]-1][1], v[f[i][2][0]-1][2] );
		glEnd();
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderObj();
	glutSwapBuffers();
}

void resize(int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble) w / h, 1.0, 80.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 70.0, // eye
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
    glutCreateWindow("Åª¨úobj (Â²²¤ª©)");
	init();
    glutDisplayFunc(display);
	glutReshapeFunc(resize);
	//glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}