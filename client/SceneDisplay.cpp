#include "SceneDisplay.h"
#include <iostream>
#include <math.h>
#include "loadShader.h"
//SceneDisplay version 0.1 implement by lzh 
//email: vliuzhihua@gmail.com
#define PI 3.1415926535898

//initiallize all static member data.
int SceneDisplay::posX = 200;
int SceneDisplay::posY = 100;
int SceneDisplay::winX = 600;
int SceneDisplay::winY = 600;
int SceneDisplay::mouseXOld = -1;
int SceneDisplay::mouseYOld = -1;
GLfloat SceneDisplay::eyeX = 0;
GLfloat SceneDisplay::eyeY = 0;
GLfloat SceneDisplay::eyeZ = 1;
GLfloat SceneDisplay::upX = 0;
GLfloat SceneDisplay::upY = 1;
GLfloat SceneDisplay::upZ = 0;
GLfloat SceneDisplay::bertaDegree = atan(1 / eyeY);
GLfloat SceneDisplay::alphaDegree = atan(eyeX / eyeZ);
GLfloat SceneDisplay::xStart = -1; 
GLfloat SceneDisplay::xEnd = 1;
GLfloat SceneDisplay::yStart = -1;
GLfloat SceneDisplay::yEnd = 1;
GLfloat SceneDisplay::zStart = -10;
GLfloat SceneDisplay::zEnd = 10;

GLint SceneDisplay::viewRotateOnOrOff = 1;
GLint SceneDisplay::shaderState = 0;
//default not use the shader, and mark it as -1 to denote not been initial yet. 
GLhandleARB SceneDisplay::vertexShader = -1;
GLhandleARB SceneDisplay::fragShader = -1;
GLhandleARB SceneDisplay::shaderPro = -1;

//class member static function pointer is need to be initialize like below.
void (*SceneDisplay::usrDisplay)() = NULL;
void (*SceneDisplay::usrKeyboardDownFunc)(unsigned char, int, int ) = NULL;
void (*SceneDisplay::usrMouseClickFunc)(int, int, int, int ) = NULL;

SceneDisplay::SceneDisplay(){
	init();
}

SceneDisplay::SceneDisplay(void (*disFunc)()){
	usrDisplay = disFunc;
	init();
}
SceneDisplay::SceneDisplay(int leftx, int topy, int width, int height){
	posX = leftx;
	posY = topy;
	winX = width;
	winY = height;
	init();
}

void SceneDisplay::start(){
	
	glutMainLoop();
}
void SceneDisplay::startTimer(){
	glutTimerFunc(200, onTimer, 1);
}
void SceneDisplay::setXYZSpan(GLfloat _xStart, GLfloat _xEnd, GLfloat _yStart, GLfloat _yEnd, GLfloat _zStart, GLfloat _zEnd){
	xStart = _xStart;
	xEnd = _xEnd;
	yStart = _yStart;
	yEnd =_yEnd;
	zStart = _zStart;
	zEnd = _zEnd;
}
void SceneDisplay::setUsrMouseClickFunc(void(*p)(int, int, int, int)){
	usrMouseClickFunc = p;
}
void SceneDisplay::setUsrKeyboardFunc(void (*p)(unsigned char, int, int)){
	usrKeyboardDownFunc = p;
}
void SceneDisplay::setUsrDisplayFunc(void (*p)()){
	usrDisplay = p;
}
//set the on/off to the view rotate function. 1 is on, 0 is off;
void SceneDisplay::setViewRotate(int state){
	viewRotateOnOrOff = state;
}
//choose to enable shader or Not.
void SceneDisplay::setShaderState(int state){

	shaderState = state;
	if(shaderState == 1 && shaderPro == -1){
		vertexShader = loadShader("shaders\\shadow.vp", GL_VERTEX_SHADER);
		fragShader = loadShader("shaders\\shadow.fp",GL_FRAGMENT_SHADER);
		shaderPro = linkToProgram(vertexShader, fragShader);
	}
}
//
void SceneDisplay::setEyePos(double x, double y, double z){
	eyeX = x;
	eyeY = y;
	eyeZ = z;
}
//set the windows position.
//this function is not worked.
void SceneDisplay::setWindowsPos(int leftx, int topy, int width, int height){
	posX = leftx;
	posY = topy;
	winX = width;
	winY = height;
}

void SceneDisplay::update(){
	draw();
}


void SceneDisplay::calNewDirAndPosInSphere(Vector &d, Point &p, GLfloat a){
	Vector tmpVec, newVec;
	Point tmpP, newP;
	GLfloat r = p.length();
	if(a == 0) return ;
	tmpVec = d * a;
	tmpP = p + tmpVec;
	newP = tmpP * (r / tmpP.length());
	//再看的时候已经不记得这是在做什么了。
	if(a < 0){
		newVec = newP * (tmpVec * newP * (1 / newP.length()) * (1 / newP.length())) - tmpVec;
	}else{
		newVec = tmpVec - newP * (tmpVec * newP * (1 / newP.length()) * (1 / newP.length()));
	}
	
	d = newVec * (1 / newVec.length());
	p = newP;

}

void SceneDisplay::calculateXYZbyDegree(GLfloat bDegree, GLfloat aDegree, GLfloat radious, GLfloat *x, GLfloat *y, GLfloat *z){
	GLint count = 0;
	if(bDegree > 0){
		while(bDegree - PI * count >= 0){
			count++;
		}
		count--;
	}else{
		while(bDegree + PI * count < 0){
			count ++;
		}
	}
	aDegree += (count % 2) * PI;
	*y = radious * cos(bDegree);
	*x = radious * abs(sin(bDegree)) * sin(aDegree);
	*z = radious * abs(sin(bDegree)) * cos(aDegree);
}
void SceneDisplay::mouseClickFunc(int button, int state, int x, int y){
	switch(button){
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
			mouseXOld = x;
			mouseYOld = y;
		}else if(state == GLUT_UP){
			mouseXOld = mouseYOld = -1;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
		printf("我不会告诉你鼠标中键不能调距离。 = =！\n");
		break;
	/*case GLUT_WHEEL_UP:
		printf("up.....\n");*/
	default:break;
	}
	if(usrMouseClickFunc != NULL){
		usrMouseClickFunc(button, state, x, y);
	}
	
}
void SceneDisplay::keyboardDownFunc(unsigned char key, int x, int y){
	switch(key){
	case 'w':
		//rad -= 1;
		//if(rad <= 10)rad = 10;
		break;
	case 's':
		//rad += 1;
		break;
	}
	
	if(usrKeyboardDownFunc != NULL){
		usrKeyboardDownFunc(key, x, y);
	}
}

void SceneDisplay::mouseMoveFunc(int x, int y){

	double rad = sqrt(pow(eyeX, 2) + pow(eyeY, 2) + pow(eyeZ, 2));

	Point eye(eyeX / rad, eyeY / rad, eyeZ / rad);
	Vector up(upX, upY, upZ);
	Vector left = eye.cross(up);

	if(mouseXOld != -1){
		GLfloat r = eye.length();

		calNewDirAndPosInSphere(left, eye, -(mouseXOld - x) * (1.0 / 360));
		up = left.cross(eye);
		calNewDirAndPosInSphere(up, eye, -(mouseYOld - y) * (1.0 / 360));
		
		eyeX = eye.pos[0] * rad;
		eyeY = eye.pos[1] * rad;
		eyeZ = eye.pos[2] * rad;
		upX = up.pos[0];
		upY = up.pos[1];
		upZ = up.pos[2];
		mouseXOld = x;
		mouseYOld = y;
		resharp(winX, winY);
		glutPostRedisplay();
	}
}



void SceneDisplay::draw(){
	//默认自带的
	glUseProgramObjectARB(0);
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, winX, winY);
	glLoadIdentity();
	//glFrustum(xStart, xEnd, yStart, yEnd, zStart, zEnd);
	glOrtho(xStart, xEnd, yStart, yEnd, zStart, zEnd);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(eyeX , eyeY , eyeZ , 0, 0, 0, upX, upY, upZ);
	//printf("eye position: %f %f %f\n", eyeX, eyeY, eyeZ);
	//gluLookAt(eyeX, eyeY, eyeZ, 0, 0, 0, upX, upY, upZ);
	//add the uniform info to shader
	if(shaderState == 1){
		glUseProgramObjectARB(shaderPro);
		glUniform4f(glGetUniformLocation(shaderPro, "eyePos"), eyeX , eyeY , eyeZ , 0);
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		
		glUniformMatrix4fv(glGetUniformLocation(shaderPro, "matrixM"), 1, 0, matrix);
	}

	//清除缓冲区的。
	glClearColor(0.1,  0.0, 0.5, 255);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	//打开光照的。
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//GLfloat light_pos[] = {5.0, 5.0, 10.0, 0.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//**************************************************
	if(usrDisplay) {
		usrDisplay();//用户自定义的显示函数的调用。
	}
	
	
	glFlush();
	return ;
}
void SceneDisplay::resharp(int x, int y){
	winX = x;
	winY = y;
	return ;
}
void SceneDisplay::onTimer(int value){
	glutPostRedisplay();
	glutTimerFunc(200, onTimer, 1);
}
void SceneDisplay::init(){
	//just a trick that i learn from <CUDA by Example>
	int c=1;
    char* dummy = "";
    glutInit( &c, &dummy);
	//**************************************************
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_SINGLE);
	glutInitWindowPosition(posX, posY);
	glutInitWindowSize(winX, winY);
	glutCreateWindow("opengl program");

	//glew must be initialized, ortherwise there will be some error when use some opengl function,such as glGenTexture
	glewInit();

	glutReshapeFunc(resharp);
	//notice: if you do not put an this pointer to quote the draw(like "this->draw"),then there will exist an error, 
	//that the argument list not matching. the reason is like the 
	//all above is bullshit.
	//there static function are needed. do you rememer the change form of the member function in an class,
	//it would been change to an global function include an addtional argument which is point to "this".so it will be 
	//argument not match.
	glutDisplayFunc(draw);

	//implement some keyboard or mouse reaction.
	glutMouseFunc(mouseClickFunc);
	glutMotionFunc(mouseMoveFunc);
	glutKeyboardFunc(keyboardDownFunc);

	//load shader file and some some complicated job was do by loadShader funciton.
	//vertexShader = loadShader("shaders\\shadow.vp", GL_VERTEX_SHADER);
	//fragShader = loadShader("shaders\\shadow.fp",GL_FRAGMENT_SHADER);
	//shaderPro = linkToProgram(vertexShader, fragShader);

	
	//****************************************
	
}

