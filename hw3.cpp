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


void LoadTextureBMP( const char *filename ){	
	
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
	
	for(int i = 0; i < imageSize/3; ++ i){
		unsigned char tmp;
		tmp = data[i*3+2];
		data[i*3+2] = data[i*3];
		data[i*3] = tmp;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

vector<double *> v;
vector<double *> vt;
vector<double *> vn;
vector<int **> f;

void openObj(){
	ifstream inputObj("obj/ppsh41.obj");
	string str;
	
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
	LoadTextureBMP("obj/ppsh41.bmp");
}

void init(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	/*glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	GLfloat light_pos[] = {1.50, 1.00, 0.00, 1.00}; // (x, y, z, luminance) 
	GLfloat light_Ka[] = {1.00, 1.00, 1.00, 1.00}; // (R, G, B, A)
	GLfloat light_Kd[] = {1.00, 1.00, 1.00, 1.00}; // (R, G, B, A)
	GLfloat light_Ks[] = {1.00, 1.00, 1.00, 1.00}; // (R, G, B, A)
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);*/
	/*
	GLfloat material_Ka[] = {0.878431, 0.878431, 0.878431, 1.00};
	GLfloat material_Kd[] = {0.878431, 0.878431, 0.878431, 1.00};
	GLfloat material_Ks[] = {1.0, 1.0, 1.0, 1.00};
	GLfloat material_Ke[] = {0.00, 0.00, 0.00, 0.00};
	GLfloat material_Se = 10;
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT, GL_SHININESS, material_Se);*/
	
	openObj();
}

int old_rot_x=0;   //剛按下滑鼠時的視窗座標
int old_rot_y=0;

int rot_x=0;      //拖曳後的相對座標，用這決定要旋轉幾度
int rot_y=0;

int record_x=0;      //紀錄上一次旋轉的角度
int record_y=0;

unsigned int selectBuff[4];
int RenderMode = GL_RENDER;
GLdouble fAspect;

void renderObj(){
    glColor4f(0.0f,0.0f,0.0f,0.0f);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture);
				
	glInitNames();
	glPushName( 0 );
	
	glRotatef((float)rot_y+(float)record_y, 1.0, 0.0, 0.0);//以x軸當旋轉軸
	glRotatef((float)rot_x+(float)record_x, 0.0, 1.0, 0.0);//以y軸當旋轉軸
	
	
	for(int i = 0; i < f.size(); ++ i){
		glLoadName( i + v.size() + 1 );
		glBegin( GL_TRIANGLES );
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			//glTexCoord2f(vt[f[i][0][1]-1][0], vt[f[i][0][1]-1][1]);
			glVertex3f( v[f[i][0][0]-1][0], v[f[i][0][0]-1][1], v[f[i][0][0]-1][2] );
			
			//glTexCoord2f(vt[f[i][1][1]-1][0], vt[f[i][1][1]-1][1]);
			glVertex3f( v[f[i][1][0]-1][0], v[f[i][1][0]-1][1], v[f[i][1][0]-1][2] );
			
			//glTexCoord2f(vt[f[i][2][1]-1][0], vt[f[i][2][1]-1][1]);
			glVertex3f( v[f[i][2][0]-1][0], v[f[i][2][0]-1][1], v[f[i][2][0]-1][2] );
		glEnd();
	}
	
	for(int i = 0; i < v.size(); ++ i){
		glPushMatrix(); 
		glLoadName( i + 1 );
		glColor4f(0.0f, 1.0f, 1.0f, 0.0f);
		glTranslated(v[i][0], v[i][1], v[i][2]);
		glutSolidSphere(1.0, 10, 10);
		glPopMatrix();
		if( selectBuff[3] == i + 1 ){
			cout<< selectBuff[3] << ", " << i+1 << endl;
			glPushMatrix(); 
			glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
			glTranslated(v[i][0], v[i][1], v[i][2]);
			glutSolidSphere(0.5, 10, 10);
			glPopMatrix();
		}
	}
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 10.0, // eye
				0.0, 0.0, 0.0, //center
				0.0, 1.0, 0.0); //up
	
	renderObj();
	if(RenderMode == GL_RENDER)
		glutSwapBuffers();
}

void resize(int w, int h){
	fAspect = (GLdouble) w / h;

	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, fAspect, 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void PickObject(unsigned int id){
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
	gluPerspective(60.0f, fAspect, 1.0, 150.0);
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

void mousemotion(int x, int y)
{
	rot_x = x - old_rot_x;
	rot_y = y - old_rot_y;
	glutPostRedisplay();
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
    glutCreateWindow("讀取obj (簡略版)");
	init();
    glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemotion);
    glutMainLoop();
    return 0;
}