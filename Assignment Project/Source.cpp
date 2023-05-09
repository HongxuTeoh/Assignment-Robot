#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Assignment Robot"

bool isOrtho = true;
bool isLightOn = false;
float tx = 0.0, tz = 0.0, tSpeed = 1.0;			// Translate for the modelview
float Onear = 10.0, Ofar = -10.0;				// Ortho near and far
float Pnear = 1.0, Pfar = 21.0;					// Perspective near and far

float ptx = 0, pty = 0, ptSpeed = 1.0;			// translate for projection 
float pry = 0, prSpeed = 1.0;					// rotation for projection 
float s1radius = 3.0;

float TurnHead = 0, NHead = 0;
float movelegL = 0;
float movelegR = 0;
float movefoward = 0;
float jump = 0;
char switchjump;

float angle = 0.0;
float x2 = 0.0, y2 = 0.0;
float PI = 3.1415926;

float rotate_up_down = 0.0;
float rotate_left_right = 0.0;
float size = 1.0;

float x = 0.0, y = 0.8, z = 0.0;
float Neck[]{ 0.502,0.502,0.502 };
float Horn[]{ 2,0,0 };
float crown[] = { 1,0.8431,0 };
float posD[] = { x,y,z };	//pos(0.0,0.8,0.0) top of the sphere;
float dif[] = { 0.502,0.502,0.502 };//green color diffuse light
float Head1[] = { 0.502,0.502,0.502 };
float Waist[] = { 0.502,0.502,0.502 };
float BLACKOutline[] = { 0,0,0 };
float Outline[] = { 2,2,2 };
float LegCo[] = { 1,1,1 };
float UpperLeg[] = { 0.1176,0,0 };
float LowerLeg[] = { 0.1961,0,0 };

float BACKFEET[] = { 0.502,0.502,0.502 };
float FRONTFEET[] = { 0,0,0 };

float turn_body = 0.0;

//Step 1: Variable declaration
BITMAP BMP;				//bitmap structure
HBITMAP hBMP;			//bitmap handle

bool istexture = false;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);

		else if (wParam == VK_SPACE) {
			isOrtho = !isOrtho;
			movelegR = 0;
			movelegL = 0;
			movefoward = 0.0;
			rotate_left_right = 0.0;
			rotate_up_down = 0.0;
			size = 1.0;
		}

		else if (wParam == 'L')
			isLightOn = !isLightOn;
		else if (wParam == 'T')
			istexture = !istexture;
		else if (wParam == 'P')
			isOrtho = !isOrtho;

		else if (wParam == VK_UP)
			rotate_up_down -= 2.0;
		else if (wParam == VK_DOWN)
			rotate_up_down += 2.0;
		else if (wParam == VK_LEFT)
			rotate_left_right += 2.0;
		else if (wParam == VK_RIGHT)
			rotate_left_right -= 2.0;
		else if (wParam == VK_OEM_PLUS)
			size += 0.1;
		else if (wParam == VK_OEM_MINUS)
			size -= 0.1;

		else if (wParam == 'W')
			pty += 0.1;
		else if (wParam == 'S')
			pty -= 0.1;
		else if (wParam == 'A')
			ptx -= 0.1;
		else if (wParam == 'D')
			ptx += 0.1;
		else if (wParam == 'Q')
			pry += 1.0;
		else if (wParam == 'E')
			pry -= 1.0;

		else if (wParam == 'U')
			posD[1] += 0.1;
		else if (wParam == 'J')
			posD[1] -= 0.1;
		else if (wParam == 'H')
			posD[0] -= 0.1;
		else if (wParam == 'K')
			posD[0] += 0.1;
		else if (wParam == 'I')
			posD[2] += 0.1;
		else if (wParam == 'Y')
			posD[2] -= 0.1;

		else if (wParam == VK_OEM_COMMA)
			turn_body += 1.0;
		else if (wParam == VK_OEM_PERIOD)
			turn_body -= 1.0;

		else if (wParam == 0x31) {
			TurnHead += 0.2;

		}
		else if (wParam == 0x32) {
			TurnHead -= 0.2;
		}
		else if (wParam == 0x33) {
			NHead -= 0.2;
		}

		else if (wParam == 0x34) {
			NHead += 0.2;

		}
		else if (wParam == 0x35) {
			movelegL = 30;
			movelegR = 0;
			movefoward += 0.5;
		}
		else if (wParam == 0x36) {
			movelegR = 30;
			movelegL = 0;
			movefoward += 0.5;
		}

		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it successfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

GLuint loadTexture(LPCSTR filename) {

	//take from step 1
	GLuint texture = 0;		//texture name

	//Step 3: Initialize texture info
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	//Step 4: Assign texture to polygon. 
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	//take from step 5
	DeleteObject(hBMP);
	return texture;
}

void drawSphere(float r) {
	GLUquadricObj* sphere = NULL;			// Create a quadric obj pointer
	sphere = gluNewQuadric();				// Create a quadric obj 
	gluQuadricDrawStyle(sphere, GLU_FILL);	// Set to draw style to sphere
	gluSphere(sphere, r, 30, 30);		// Draw sphere
	gluDeleteQuadric(sphere);
}

void drawSphereWithoutGLU(float r, float sl, float st)
{
	const float PI = 3.141592f;
	GLfloat x, y, z, sliceA, stackA;
	GLfloat radius = r;
	int sliceNo = sl, stackNo = st;

	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
	{
		glBegin(GL_LINE_STRIP);
		for (stackA = 0.0; stackA < 2 * PI; stackA += PI / stackNo)
		{
			x = radius * cos(stackA) * sin(sliceA);
			y = radius * sin(stackA) * sin(sliceA);
			z = radius * cos(sliceA);
			glVertex3f(x, y, z);
			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = radius * cos(sliceA + PI / sliceNo);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void drawSphereWithGLU(float r, float sl, float st)
{
	const float PI = 3.141592f;
	GLfloat x, y, z, sliceA, stackA;
	GLfloat radius = r;
	int sliceNo = sl, stackNo = st;

	for (sliceA = 0.0; sliceA < 2 * PI; sliceA += PI / sliceNo)
	{
		glBegin(GL_POLYGON);
		for (stackA = 0.0; stackA < 2 * PI; stackA += PI / stackNo)
		{
			x = radius * cos(stackA) * sin(sliceA);
			y = radius * sin(stackA) * sin(sliceA);
			z = radius * cos(sliceA);
			glVertex3f(x, y, z);
			x = radius * cos(stackA) * sin(sliceA + PI / stackNo);
			y = radius * sin(stackA) * sin(sliceA + PI / sliceNo);
			z = radius * cos(sliceA + PI / sliceNo);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

void drawsCylinderGLUFILL(float br, float tr, float h, float sl, float sta) {
	GLUquadricObj* sCylinder = NULL;//create a  quadric object pointer
	sCylinder = gluNewQuadric();//create a quadric object
	gluQuadricDrawStyle(sCylinder, GLU_FILL);
	gluCylinder(sCylinder, br, tr, h, sl, sta);
	gluDeleteQuadric(sCylinder);
}

void drawsCylinderGLULINE(float br, float tr, float h, float sl, float sta) {
	glLineWidth(2);
	GLUquadricObj* sCylinder = NULL;//create a  quadric object pointer
	sCylinder = gluNewQuadric();//create a quadric object
	gluQuadricDrawStyle(sCylinder, GLU_LINE);
	gluCylinder(sCylinder, br, tr, h, sl, sta);
	gluDeleteQuadric(sCylinder);
}

void drawCone(float tr, float h) {
	GLUquadricObj* cylinder = NULL;				// Create a quadric obj pointer
	cylinder = gluNewQuadric();					// Create a quadric obj 
	gluQuadricDrawStyle(cylinder, GLU_FILL);	// Set to draw style to cone
	gluCylinder(cylinder, 0, tr, h, 30, 30);				// Draw cone
	gluDeleteQuadric(cylinder);
}

void cubeFinger(float length, float height, float width) {
	glBegin(GL_QUADS);		//bottom finger
	glColor3f(0.5, 0.5, 0.5);
	//bottom
	glVertex3f(length, height - 0.3, width);
	glVertex3f(length + 0.3, height - 0.3, width);
	glVertex3f(length + 0.3, height - 0.3, width - 0.3);
	glVertex3f(length, height - 0.3, width - 0.3);

	//top
	glVertex3f(length, height - 0.3 + 0.3, width);
	glVertex3f(length + 0.3, height, width);
	glVertex3f(length + 0.3, height, width - 0.3);
	glVertex3f(length, height - 0.3 + 0.3, width - 0.3);

	//near (x axis = length)
	glVertex3f(length, height - 0.3, width);
	glVertex3f(length, height, width);
	glVertex3f(length, height, width - 0.3);
	glVertex3f(length, height - 0.3, width - 0.3);

	//far (x axis = length + 0.3)
	glVertex3f(length + 0.3, height - 0.3, width);
	glVertex3f(length + 0.3, height, width);
	glVertex3f(length + 0.3, height, width - 0.3);
	glVertex3f(length + 0.3, height - 0.3, width - 0.3);

	//left (z axis = width - 0.3)
	glVertex3f(length, height - 0.3, width - 0.3);
	glVertex3f(length + 0.3, height - 0.3, width - 0.3);
	glVertex3f(length + 0.3, height, width - 0.3);
	glVertex3f(length, height, width - 0.3);

	//right (z axis = width)
	glVertex3f(length, height - 0.3, width);
	glVertex3f(length + 0.3, height - 0.3, width);
	glVertex3f(length + height, 0.3, width);
	glVertex3f(length, height, width);
	glEnd();
	/*----------------------------------------*/

	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	//bottom
	glVertex3f(length, height - 0.3, width);
	glVertex3f(length + 0.3, height - 0.3, width);
	glVertex3f(length + 0.3, height - 0.3, width - 0.3);
	glVertex3f(length, height - 0.3, width - 0.3);
	glEnd();

	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(length, height - 0.3 + 0.3, width);
	glVertex3f(length + 0.3, height, width);
	glVertex3f(length + 0.3, height, width - 0.3);
	glVertex3f(length, height - 0.3 + 0.3, width - 0.3);
	glEnd();

	//near (x axis = length)
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(length, height - 0.3, width);
	glVertex3f(length, height, width);
	glVertex3f(length, height, width - 0.3);
	glVertex3f(length, height - 0.3, width - 0.3);
	glEnd();

	//far (x axis = length + 0.3)
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(length + 0.3, height - 0.3, width);
	glVertex3f(length + 0.3, height, width);
	glVertex3f(length + 0.3, height, width - 0.3);
	glVertex3f(length + 0.3, height - 0.3, width - 0.3);
	glEnd();
}

void firstFinger() {
	cubeFinger(1.0, 0.3, 1.2);		//bottom finger

	cubeFinger(1.3, 0.3, 1.2);		//middle finger

	cubeFinger(1.6, 0.3, 1.2);		//top finger
}

void secondFinger() {
	cubeFinger(1.0, 0.3, 0.9);		//bottom finger

	cubeFinger(1.3, 0.3, 0.9);		//middle finger

	cubeFinger(1.6, 0.3, 0.9);		//top finger
}

void thirdFinger() {
	cubeFinger(1.0, 0.3, 0.6);		//bottom finger

	cubeFinger(1.3, 0.3, 0.6);		//middle finger

	cubeFinger(1.6, 0.3, 0.6);		//top finger
}

void fourthFinger() {
	cubeFinger(1.0, 0.3, 0.3);		//bottom finger

	cubeFinger(1.3, 0.3, 0.3);		//middle finger

	cubeFinger(1.6, 0.3, 0.3);		//top finger
}

void Hand() {
	glBegin(GL_QUADS);		// need to rotate/translate/scaled
	glColor3f(0.5, 0.5, 0.5);
	//bottom
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.0, 1.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);

	//top
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.3, 1.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.3, 1.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.3, 0.0);

	//near (x axis = 0)
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0, 0.0, 1.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0, 0.3, 1.2);
	glTexCoord2f(0.0, 0.1);
	glVertex3f(0.0, 0.3, 0.0);

	//far (x axis = 1)
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, 0.0, 1.2);
	glTexCoord2f(0.0, 0.1);
	glVertex3f(1.0, 0.3, 1.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.3, 0.0);

	//left (z axis = 0)
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.3, 0.0);
	glTexCoord2f(0.0, 0.1);
	glVertex3f(0.0, 0.3, 0.0);

	//right (z axis = 1.2)
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.2);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 0.0, 1.2);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 0.3, 1.2);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0, 0.3, 1.2);
	glEnd();

	/*------------------------------------------*/
	//bottom
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.2);
	glVertex3f(1.0, 0.0, 1.2);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.0, 0.3, 1.2);
	glVertex3f(1.0, 0.3, 1.2);
	glVertex3f(1.0, 0.3, 0.0);
	glEnd();

	//near (x axis = 0)
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 1.2);
	glVertex3f(0.0, 0.3, 1.2);
	glVertex3f(0.0, 0.3, 0.0);
	glEnd();

	//far (x axis = 1)
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 1.2);
	glVertex3f(1.0, 0.3, 1.2);
	glVertex3f(1.0, 0.3, 0.0);
	glEnd();

	firstFinger();		//(1st)index finger

	secondFinger();		//(2nd)middle finger

	thirdFinger();		//(3rd)ring finger

	fourthFinger();		//(4th)little finger
}

// robot head and neck
void Head() {

	glRotatef(NHead, x, y, z);
	glRotatef(TurnHead, x, y, z);
	glMaterialfv(GL_FRONT, GL_AMBIENT, Head1);
	glColor3ub(128, 128, 128);	//GREY COLOR
	glBegin(GL_POLYGON);		//FRONT FACE
	glVertex3f(0, 0, 0);
	glVertex3f(0.4, 0, 0);
	glVertex3f(0.4, 0.2, 0);
	glVertex3f(0, 0.2, 0);
	glEnd();

	glBegin(GL_POLYGON);       //LEFT FACE
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0.2, 0);
	glVertex3f(0, 0.2, 0.3);
	glVertex3f(0, 0, 0.3);
	glEnd();

	glBegin(GL_POLYGON);       //RIGHT FACE
	glVertex3f(0.4, 0, 0);
	glVertex3f(0.4, 0.2, 0);
	glVertex3f(0.4, 0.2, 0.3);
	glVertex3f(0.4, 0, 0.3);
	glEnd();

	glBegin(GL_POLYGON);      //BACK FACE
	glVertex3f(0, 0, 0.3);
	glVertex3f(0.4, 0, 0.3);
	glVertex3f(0.4, 0.2, 0.3);
	glVertex3f(0, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);		//TOP FACE 
	glVertex3f(0, 0.2, 0);
	glVertex3f(0.4, 0.2, 0);
	glVertex3f(0.4, 0.2, 0.3);
	glVertex3f(0, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);		//DOWN FACE
	glVertex3f(0, 0, 0);
	glVertex3f(0.4, 0, 0);
	glVertex3f(0.4, 0, 0.3);
	glVertex3f(0, 0, 0.3);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT, crown);
	glColor3ub(255, 215, 0);	//GOLD COLOR FOR CROWN
	glBegin(GL_POLYGON);		//FRONT CROWN FACE
	glVertex3f(0, 0.2, 0);
	glVertex3f(0.4, 0.2, 0);
	glVertex3f(0.4, 0.22, 0);
	glVertex3f(0, 0.22, 0);
	glEnd();

	glBegin(GL_POLYGON);		//BACK CROWN FACE
	glVertex3f(0, 0.2, 0.3);
	glVertex3f(0.4, 0.2, 0.3);
	glVertex3f(0.4, 0.22, 0.3);
	glVertex3f(0, 0.22, 0.3);
	glEnd();

	glBegin(GL_POLYGON);		//LEFT CROWN FACE
	glVertex3f(0, 0.2, 0);
	glVertex3f(0, 0.22, 0);
	glVertex3f(0, 0.22, 0.3);
	glVertex3f(0, 0.2, 0.3);
	glEnd();

	glBegin(GL_POLYGON);		//RIGHT CROWN FACE
	glVertex3f(0.4, 0.2, 0);
	glVertex3f(0.4, 0.22, 0);
	glVertex3f(0.4, 0.22, 0.3);
	glVertex3f(0.4, 0.2, 0.3);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT, BLACKOutline);
	glColor3b(0, 0, 0);			//BLACK COLOR OUTLINE
	glLineWidth(1.5);
	glBegin(GL_LINE_LOOP);		//FRONT CROWN FACE OUTLINE
	glVertex3f(0, 0.2, -0.00001);
	glVertex3f(0.4, 0.2, -0.00001);
	glVertex3f(0.4, 0.22, -0.00001);
	glVertex3f(0, 0.22, -0.00001);
	glEnd();

	glBegin(GL_LINE_LOOP);		//BACK CROWN FACE OUTLINE
	glVertex3f(0, 0.2, 0.30001);
	glVertex3f(0.4, 0.2, 0.30001);
	glVertex3f(0.4, 0.22, 0.30001);
	glVertex3f(0, 0.22, 0.30001);
	glEnd();

	glBegin(GL_LINE_LOOP);		//LEFT CROWN FACE OUTLINE
	glVertex3f(-0.00001, 0.2, 0);
	glVertex3f(-0.00001, 0.22, 0);
	glVertex3f(-0.00001, 0.22, 0.3);
	glVertex3f(-0.00001, 0.2, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);		//RIGHT CROWN FACE OUTLINE
	glVertex3f(0.40001, 0.2, 0);
	glVertex3f(0.40001, 0.22, 0);
	glVertex3f(0.40001, 0.22, 0.3);
	glVertex3f(0.40001, 0.2, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);		//FRONT FACE OUTLINE
	glVertex3f(0, 0, -0.00001);
	glVertex3f(0.4, 0, -0.00001);
	glVertex3f(0.4, 0.2, -0.00001);
	glVertex3f(0, 0.2, -0.00001);
	glEnd();

	glBegin(GL_LINE_LOOP);		//LEFT FACE OUTLINE
	glVertex3f(-0.00001, 0, 0);
	glVertex3f(-0.00001, 0.2, 0);
	glVertex3f(-0.00001, 0.2, 0.3);
	glVertex3f(-0.00001, 0, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);		//RIGHT FACE OUTLINE
	glVertex3f(0.40001, 0, 0);
	glVertex3f(0.40001, 0.2, 0);
	glVertex3f(0.40001, 0.2, 0.3);
	glVertex3f(0.40001, 0, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);		//BACK FACE OUTLINE
	glVertex3f(0, 0, 0.30001);
	glVertex3f(0.4, 0, 0.30001);
	glVertex3f(0.4, 0.2, 0.30001);
	glVertex3f(0, 0.2, 0.30001);
	glEnd();

	glColor3b(0, 0, 0);
	glBegin(GL_TRIANGLES);	//LEFT EYE	
	glVertex3f(0.07, 0.07, -0.001);
	glVertex3f(0.09, 0.12, -0.001);
	glVertex3f(0.14, 0.06, -0.001);
	glEnd();

	glBegin(GL_TRIANGLES);	//RIGHT EYE
	glVertex3f(0.33, 0.07, -0.001);
	glVertex3f(0.31, 0.12, -0.001);
	glVertex3f(0.26, 0.06, -0.001);
	glEnd();

	glMaterialfv(GL_FRONT, GL_AMBIENT, crown);
	glColor3ub(255, 215, 0);//GOLD COLOR CROWN
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0, 0.22, 0);
	glVertex3f(0.05, 0.22, 0);
	glVertex3f(0.025, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.05, 0.22, 0);
	glVertex3f(0.1, 0.22, 0);
	glVertex3f(0.075, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.1, 0.22, 0);
	glVertex3f(0.15, 0.22, 0);
	glVertex3f(0.125, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.15, 0.22, 0);
	glVertex3f(0.2, 0.22, 0);
	glVertex3f(0.175, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.2, 0.22, 0);
	glVertex3f(0.25, 0.22, 0);
	glVertex3f(0.225, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.25, 0.22, 0);
	glVertex3f(0.3, 0.22, 0);
	glVertex3f(0.275, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.3, 0.22, 0);
	glVertex3f(0.35, 0.22, 0);
	glVertex3f(0.325, 0.26, 0);
	glEnd();
	glBegin(GL_TRIANGLES);//FRONT CROWN
	glVertex3f(0.35, 0.22, 0);
	glVertex3f(0.4, 0.22, 0);
	glVertex3f(0.375, 0.26, 0);
	glEnd();

	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0);
	glVertex3f(0.4, 0.22, 0.05);
	glVertex3f(0.4, 0.26, 0.025);
	glEnd();
	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0.05);
	glVertex3f(0.4, 0.22, 0.1);
	glVertex3f(0.4, 0.26, 0.075);
	glEnd();
	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0.1);
	glVertex3f(0.4, 0.22, 0.15);
	glVertex3f(0.4, 0.26, 0.125);
	glEnd();
	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0.15);
	glVertex3f(0.4, 0.22, 0.2);
	glVertex3f(0.4, 0.26, 0.175);
	glEnd();
	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0.2);
	glVertex3f(0.4, 0.22, 0.25);
	glVertex3f(0.4, 0.26, 0.225);
	glEnd();
	glBegin(GL_TRIANGLES);//RIGHT CROWN
	glVertex3f(0.4, 0.22, 0.25);
	glVertex3f(0.4, 0.22, 0.3);
	glVertex3f(0.4, 0.26, 0.275);
	glEnd();

	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0, 0.22, 0.3);
	glVertex3f(0.05, 0.22, 0.3);
	glVertex3f(0.025, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.05, 0.22, 0.3);
	glVertex3f(0.1, 0.22, 0.3);
	glVertex3f(0.075, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.1, 0.22, 0.3);
	glVertex3f(0.15, 0.22, 0.3);
	glVertex3f(0.125, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.15, 0.22, 0.3);
	glVertex3f(0.2, 0.22, 0.3);
	glVertex3f(0.175, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.2, 0.22, 0.3);
	glVertex3f(0.25, 0.22, 0.3);
	glVertex3f(0.225, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.25, 0.22, 0.3);
	glVertex3f(0.3, 0.22, 0.3);
	glVertex3f(0.275, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.3, 0.22, 0.3);
	glVertex3f(0.35, 0.22, 0.3);
	glVertex3f(0.325, 0.26, 0.3);
	glEnd();
	glBegin(GL_TRIANGLES);//BACK CROWN
	glVertex3f(0.35, 0.22, 0.3);
	glVertex3f(0.4, 0.22, 0.3);
	glVertex3f(0.375, 0.26, 0.3);
	glEnd();

	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0);
	glVertex3f(0, 0.22, 0.05);
	glVertex3f(0, 0.26, 0.025);
	glEnd();
	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0.05);
	glVertex3f(0, 0.22, 0.1);
	glVertex3f(0, 0.26, 0.075);
	glEnd();
	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0.1);
	glVertex3f(0, 0.22, 0.15);
	glVertex3f(0, 0.26, 0.125);
	glEnd();
	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0.15);
	glVertex3f(0, 0.22, 0.2);
	glVertex3f(0, 0.26, 0.175);
	glEnd();
	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0.2);
	glVertex3f(0, 0.22, 0.25);
	glVertex3f(0, 0.26, 0.225);
	glEnd();
	glBegin(GL_TRIANGLES);//LEFT CROWN
	glVertex3f(0, 0.22, 0.25);
	glVertex3f(0, 0.22, 0.3);
	glVertex3f(0, 0.26, 0.275);
	glEnd();

	//THE HORN
	glMaterialfv(GL_FRONT, GL_AMBIENT, Horn);
	glColor3f(1, 0, 0);
	glLineWidth(3);
	glBegin(GL_LINES);//LEFT TOP HORN
	glVertex3f(0.08, 0.2, 0.15);
	glVertex3f(0.01, 0.35, 0.15);
	glEnd();
	glBegin(GL_LINES);//RIGHT TOP HORN
	glVertex3f(0.32, 0.2, 0.15);
	glVertex3f(0.39, 0.35, 0.15);
	glEnd();
	glBegin(GL_LINES);//LEFT SIDE HORN
	glVertex3f(0.4, 0.1, 0.15);
	glVertex3f(0.5, 0.25, 0.15);
	glEnd();
	glBegin(GL_LINES);//RIGHT SIDE HORN
	glVertex3f(0, 0.1, 0.15);
	glVertex3f(-0.1, 0.25, 0.15);
	glEnd();

	//NECK
	glMaterialfv(GL_FRONT, GL_AMBIENT, Neck);
	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);		//FIRST FRONT NECK
	glVertex3f(0.1, 0, 0.05);
	glVertex3f(0.3, 0, 0.05);
	glVertex3f(0.3, -0.03, 0.05);
	glVertex3f(0.1, -0.03, 0.05);
	glEnd();

	glBegin(GL_POLYGON);		//SECOND FRONT NECK
	glVertex3f(0.15, -0.03, 0.1);
	glVertex3f(0.25, -0.03, 0.1);
	glVertex3f(0.25, -0.06, 0.1);
	glVertex3f(0.15, -0.06, 0.1);
	glEnd();

	glBegin(GL_POLYGON);		//FIRST BACK NECK
	glVertex3f(0.1, 0, 0.25);
	glVertex3f(0.3, 0, 0.25);
	glVertex3f(0.3, -0.03, 0.25);
	glVertex3f(0.1, -0.03, 0.25);
	glEnd();

	glBegin(GL_POLYGON);		//SECOND BACK NECK
	glVertex3f(0.15, -0.03, 0.2);
	glVertex3f(0.25, -0.03, 0.2);
	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.15, -0.06, 0.2);
	glEnd();

	glBegin(GL_POLYGON);		//FIRST LEFT NECK
	glVertex3f(0.1, 0, 0.05);
	glVertex3f(0.1, -0.03, 0.05);
	glVertex3f(0.1, -0.03, 0.25);
	glVertex3f(0.1, 0, 0.25);
	glEnd();

	glBegin(GL_POLYGON);		//SECOND LEFT NECK
	glVertex3f(0.15, -0.03, 0.1);
	glVertex3f(0.15, -0.03, 0.2);
	glVertex3f(0.15, -0.06, 0.2);
	glVertex3f(0.15, -0.06, 0.1);
	glEnd();

	glBegin(GL_POLYGON);		//FIRST RIGHT NECK
	glVertex3f(0.3, 0, 0.05);
	glVertex3f(0.3, -0.03, 0.05);
	glVertex3f(0.3, -0.03, 0.25);
	glVertex3f(0.3, 0, 0.25);
	glEnd();

	glBegin(GL_POLYGON);		//SECOND RIGHT NECK
	glVertex3f(0.25, -0.03, 0.1);
	glVertex3f(0.25, -0.03, 0.2);
	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.25, -0.06, 0.1);
	glEnd();

	glBegin(GL_POLYGON);		//FIRST DOWN NECK
	glVertex3f(0.1, -0.03, 0.05);
	glVertex3f(0.3, -0.03, 0.05);
	glVertex3f(0.3, -0.03, 0.25);
	glVertex3f(0.1, -0.03, 0.25);
	glEnd();

	glBegin(GL_POLYGON);		//SECOND DOWN NECK
	glVertex3f(0.15, -0.06, 0.1);
	glVertex3f(0.25, -0.06, 0.1);
	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.15, -0.06, 0.2);
	glEnd();

	//NECK OUTLINE
	glMaterialfv(GL_FRONT, GL_AMBIENT, BLACKOutline);
	glLineWidth(1.5);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);

	glVertex3f(0.3, -0.03, 0.05);
	glVertex3f(0.1, -0.03, 0.05);

	glVertex3f(0.25, -0.06, 0.1);
	glVertex3f(0.15, -0.06, 0.1);

	glVertex3f(0.3, -0.03, 0.25);
	glVertex3f(0.1, -0.03, 0.25);

	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.15, -0.06, 0.2);

	glVertex3f(0.1, 0, 0.05);
	glVertex3f(0.1, -0.03, 0.05);

	glVertex3f(0.1, -0.03, 0.25);
	glVertex3f(0.1, 0, 0.25);

	glVertex3f(0.1, 0, 0.05);
	glVertex3f(0.1, 0, 0.25);

	glVertex3f(0.1, -0.03, 0.25);
	glVertex3f(0.1, -0.03, 0.05);

	glVertex3f(0.3, 0, 0.05);
	glVertex3f(0.3, -0.03, 0.05);

	glVertex3f(0.3, -0.03, 0.25);
	glVertex3f(0.3, 0, 0.25);

	glVertex3f(0.3, -0.03, 0.05);
	glVertex3f(0.3, -0.03, 0.25);

	glVertex3f(0.3, 0, 0.05);
	glVertex3f(0.3, 0, 0.25);

	glVertex3f(0.15, -0.03, 0.1);
	glVertex3f(0.15, -0.03, 0.2);

	glVertex3f(0.15, -0.06, 0.2);
	glVertex3f(0.15, -0.06, 0.1);

	glVertex3f(0.15, -0.06, 0.2);
	glVertex3f(0.15, -0.03, 0.2);

	glVertex3f(0.15, -0.03, 0.1);
	glVertex3f(0.15, -0.06, 0.1);

	glVertex3f(0.25, -0.03, 0.1);
	glVertex3f(0.25, -0.03, 0.2);

	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.25, -0.06, 0.1);

	glVertex3f(0.25, -0.06, 0.2);
	glVertex3f(0.25, -0.03, 0.2);

	glVertex3f(0.25, -0.03, 0.1);
	glVertex3f(0.25, -0.06, 0.1);

	glEnd();
}

// robot body
void Upperchest() {
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	//top
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 2.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 2.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, 2.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 2.5, 1.0);

	//bottom
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);

	//front
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 2.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 2.5, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);

	//back
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 2.5, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 2.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, -1.0);

	//left
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 2.5, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 2.5, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, -1.0);

	//right
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 2.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 2.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glEnd();
	/*----------------------------------*/
	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 2.5, 1.0);
	glVertex3f(1.0, 2.5, -1.0);
	glVertex3f(-1.0, 2.5, -1.0);
	glVertex3f(-1.0, 2.5, 1.0);
	glEnd();

	//bottom
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glEnd();

	//front
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, 2.5, 1.0);
	glVertex3f(1.0, 2.5, 1.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glEnd();

	//back
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.0, 2.5, -1.0);
	glVertex3f(1.0, 2.5, -1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glEnd();
}

void Lowerchest() {
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);

	//top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.5, 2.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.5, 2.5, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.5, 2.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.5, 2.5, 1.0);

	//bottom
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);

	//front
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.5, 2.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.5, 2.5, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);

	//back
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.5, 2.5, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.5, 2.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, -1.0);

	//left
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.5, 2.5, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.5, 2.5, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, -1.0);

	//right
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.5, 2.5, -1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.5, 2.5, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0, 1.5, -1.0);
	glEnd();
	/*----------------------------------*/
	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.5, 2.5, 1.0);
	glVertex3f(1.5, 2.5, -1.0);
	glVertex3f(-1.5, 2.5, -1.0);
	glVertex3f(-1.5, 2.5, 1.0);
	glEnd();

	//bottom
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glEnd();

	//front
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.5, 2.5, 1.0);
	glVertex3f(1.5, 2.5, 1.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glEnd();

	//back
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-1.5, 2.5, -1.0);
	glVertex3f(1.5, 2.5, -1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glEnd();
}

void stomach() {
	glBegin(GL_QUADS);
	glColor3f(0.5, 0.5, 0.5);
	//top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, 1.5, 1.0);

	//bottom
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.75, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.75, 1.0, -1.0); 
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.75, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.75, 1.0, 1.0);

	//front
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.75, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.75, 1.0, 1.0);

	//back
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.75, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.75, 1.0, -1.0);

	//left
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.75, 1.0, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.75, 1.0, 1.0);

	//right
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0, 1.5, 1.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.75, 1.0, -1.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.75, 1.0, 1.0);
	glEnd();
	/*-----------------------------------*/
	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glEnd();

	//bottom
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.75, 1.0, 1.0);
	glVertex3f(0.75, 1.0, -1.0);
	glVertex3f(-0.75, 1.0, -1.0);
	glVertex3f(-0.75, 1.0, 1.0);
	glEnd();

	//front
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 1.5, 1.0);
	glVertex3f(-1.0, 1.5, 1.0);
	glVertex3f(-0.75, 1.0, 1.0);
	glVertex3f(0.75, 1.0, 1.0);
	glEnd();

	//back
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.5);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 1.5, -1.0);
	glVertex3f(-1.0, 1.5, -1.0);
	glVertex3f(-0.75, 1.0, -1.0);
	glVertex3f(0.75, 1.0, -1.0);
	glEnd();
}

void joint() {
	glColor3f(0.5, 0.5, 0.5);
	drawSphere(0.5);
}

void shoulder() {
	glColor3f(0.5, 0.5, 0.5);
	drawsCylinderGLUFILL(0.5, 0.4, 1.5, 10.0, 5.0);

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1.0);
	drawsCylinderGLULINE(0.5, 0.4, 1.5, 10.0, 5.0);
}

void LowerBody() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Waist);
	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);  //BACK WAIST
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0.6, 0, 0);
	glVertex3f(0.6, 0.4, 0);
	glVertex3f(-0.1, 0.4, 0);
	glEnd();

	glBegin(GL_POLYGON);  //FRONT WAIST
	glVertex3f(-0.1, 0.4, 0.5);
	glVertex3f(-0.1, 0, 0.5);
	glVertex3f(0.6, 0, 0.5);
	glVertex3f(0.6, 0.4, 0.5);
	glEnd();

	glBegin(GL_POLYGON);  //LEFT WAIST
	glVertex3f(-0.1, 0.4, 0);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(-0.1, 0, 0.5);
	glVertex3f(-0.1, 0.4, 0.5);
	glEnd();

	glBegin(GL_POLYGON);  //RIGHT WAIST
	glVertex3f(0.6, 0, 0);
	glVertex3f(0.6, 0.4, 0);
	glVertex3f(0.6, 0.4, 0.5);
	glVertex3f(0.6, 0, 0.5);
	glEnd();

	glBegin(GL_POLYGON); //TOP WAIST
	glVertex3f(-0.1, 0.4, 0);
	glVertex3f(0.6, 0.4, 0);
	glVertex3f(0.6, 0.4, 0.5);
	glVertex3f(-0.1, 0.4, 0.5);
	glEnd();

	glBegin(GL_POLYGON);  //DOWN WAIST
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0.6, 0, 0);
	glVertex3f(0.6, 0, 0.5);
	glVertex3f(-0.1, 0, 0.5);
	glEnd();

	//OUTLINE FOR WAIST-------------------------------------------
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);  //BACK WAIST
	glVertex3f(-0.1, 0, 0);
	glVertex3f(0.6, 0, 0);
	glVertex3f(0.6, 0.4, 0);
	glVertex3f(-0.1, 0.4, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);  //FRONT WAIST
	glVertex3f(-0.1, 0.4, 0.5);
	glVertex3f(-0.1, 0, 0.5);
	glVertex3f(0.6, 0, 0.5);
	glVertex3f(0.6, 0.4, 0.5);
	glEnd();

	glBegin(GL_LINE_LOOP);  //LEFT WAIST
	glVertex3f(-0.1, 0.4, 0);
	glVertex3f(-0.1, 0, 0);
	glVertex3f(-0.1, 0, 0.5);
	glVertex3f(-0.1, 0.4, 0.5);
	glEnd();

	glBegin(GL_LINE_LOOP);  //RIGHT WAIST
	glVertex3f(0.6, 0, 0);
	glVertex3f(0.6, 0.4, 0);
	glVertex3f(0.6, 0.4, 0.5);
	glVertex3f(0.6, 0, 0.5);
	glEnd();
	glDisable(GL_LIGHT4);
	glPopMatrix();


	//LEG------------------------------------------------------------------
	// UPPER LEG-----------------------------------------------------------

	glMaterialfv(GL_FRONT, GL_DIFFUSE, LegCo);
	glPushMatrix();//LEFT UPPER LEG CONNECTOR
	glColor3ub(128, 128, 128);
	glTranslated(0.09, -0.05, 0.25);
	drawSphereWithGLU(0.11, 100, 100);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	drawSphereWithoutGLU(0.11, 10, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, LegCo);
	glPushMatrix();//RIGHT UPPER LEG CONNECTOR
	glTranslated(0.42, -0.05, 0.25);
	drawSphereWithGLU(0.11, 100, 100);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	drawSphereWithoutGLU(0.11, 10, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, UpperLeg);
	glPushMatrix();//LEFT UPPER LEG
	glRotated(movelegL, x, 0, 0);//move
	glColor3ub(128, 128, 128);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.09, 0.25, -0.1);
	drawsCylinderGLUFILL(0.1, 0.07, 0.8, 100, 100);
	glColor3b(0, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	drawsCylinderGLULINE(0.1, 0.07, 0.8, 15, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, UpperLeg);
	glPushMatrix();//RIGHT UPPER LEG
	glRotated(movelegR, x, 0, 0);//move
	glColor3ub(128, 128, 128);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.42, 0.25, -0.1);
	drawsCylinderGLUFILL(0.1, 0.07, 0.8, 100, 100);
	glColor3b(0, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	drawsCylinderGLULINE(0.1, 0.07, 0.8, 15, 10);
	glPopMatrix();


	//LOWER LEG------------------------------------------------------------
	glMaterialfv(GL_FRONT, GL_DIFFUSE, LegCo);
	glPushMatrix();//LEFT LOWER LEG CONNECTOR
	glRotated(movelegL, x, 0, 0);//move
	glColor3ub(128, 128, 128);
	glTranslated(0.09, -0.735, 0.25);
	drawSphereWithGLU(0.07, 100, 100);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	drawSphereWithoutGLU(0.07, 10, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, LegCo);
	glPushMatrix();//RIGHT LOWER LEG CONNECTOR
	glColor3ub(128, 128, 128);
	glRotated(movelegR, x, 0, 0);//move
	glTranslated(0.42, -0.735, 0.25);
	drawSphereWithGLU(0.07, 100, 100);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	drawSphereWithoutGLU(0.07, 10, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, LowerLeg);
	glPushMatrix();//LEFT LOWER LEG
	glRotated(movelegL, x, 0, 0);//move
	glColor3ub(128, 128, 128);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.09, 0.25, 0.77);
	drawsCylinderGLUFILL(0.07, 0.04, 0.6, 100, 100);
	glColor3b(0, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	drawsCylinderGLULINE(0.07, 0.04, 0.6, 15, 10);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, LowerLeg);
	glPushMatrix();//RIGHT LOWER LEG
	glRotated(movelegR, x, 0, 0);//move
	glColor3ub(128, 128, 128);
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.42, 0.25, 0.77);
	drawsCylinderGLUFILL(0.07, 0.04, 0.6, 100, 100);
	glColor3b(0, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	drawsCylinderGLULINE(0.07, 0.04, 0.6, 15, 10);
	glPopMatrix();

	//FEET------------------------------------------------------------
	//LEFT FEET------------------------------------------------------
	glPushMatrix();
	glRotated(movelegL, x, 0, 0);//move
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BACKFEET);
	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);//BACK
	glVertex3f(0.155, -1.369, 0.3);
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.155, -1.449, 0.3);
	glEnd();

	glBegin(GL_POLYGON);//LEFT
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.03, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//RIGHT
	glVertex3f(0.155, -1.369, 0.3);
	glVertex3f(0.155, -1.449, 0.3);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//DOWN
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.3);
	glEnd();

	glBegin(GL_POLYGON);//TOP
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.369, 0.2);
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.155, -1.369, 0.3);
	glEnd();

	//FEET OUTLINE
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	glBegin(GL_LINE_LOOP);//BACK
	glVertex3f(0.155, -1.369, 0.3);
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.155, -1.449, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);//LEFT
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.03, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//RIGHT
	glVertex3f(0.155, -1.369, 0.3);
	glVertex3f(0.155, -1.449, 0.3);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//DOWN
	glVertex3f(0.03, -1.449, 0.3);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);//TOP
	glVertex3f(0.03, -1.369, 0.3);
	glVertex3f(0.03, -1.369, 0.2);
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.155, -1.369, 0.3);
	glEnd();

	glColor3ub(128, 128, 128);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, FRONTFEET);
	glBegin(GL_POLYGON);//DOWN EXTEND;
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.1);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.155, -1.449, 0.1);
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.03, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.155, -1.449, 0.1);
	glVertex3f(0.155, -1.449, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.03, -1.369, 0.2);
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.03, -1.449, 0.2);
	glEnd();

	//OUTLINE
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	glBegin(GL_LINE_LOOP);//DOWN EXTEND;
	glColor3ub(0, 0, 0);
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.03, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.2);
	glVertex3f(0.155, -1.449, 0.1);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.155, -1.449, 0.1);
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.03, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.155, -1.369, 0.2);
	glVertex3f(0.155, -1.449, 0.1);
	glVertex3f(0.155, -1.449, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.03, -1.369, 0.2);
	glVertex3f(0.03, -1.449, 0.1);
	glVertex3f(0.03, -1.449, 0.2);
	glEnd();
	glPopMatrix();

	//RIGHT FEET------------------------------------------------------
	glPushMatrix();
	glRotated(movelegR, x, 0, 0);//move
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BACKFEET);
	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);//BACK
	glVertex3f(0.482, -1.369, 0.3);
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.482, -1.449, 0.3);
	glEnd();

	glBegin(GL_POLYGON);//LEFT
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.357, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//RIGHT
	glVertex3f(0.482, -1.369, 0.3);
	glVertex3f(0.482, -1.449, 0.3);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//DOWN
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.3);
	glEnd();

	glBegin(GL_POLYGON);//TOP
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.369, 0.2);
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.482, -1.369, 0.3);
	glEnd();

	//OUTLINE
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLACKOutline);
	glBegin(GL_LINE_LOOP);//BACK
	glColor3ub(0, 0, 0);
	glVertex3f(0.482, -1.369, 0.3);
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.482, -1.449, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);//LEFT
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.357, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//RIGHT
	glVertex3f(0.482, -1.369, 0.3);
	glVertex3f(0.482, -1.449, 0.3);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//DOWN
	glVertex3f(0.357, -1.449, 0.3);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.3);
	glEnd();

	glBegin(GL_LINE_LOOP);//TOP
	glVertex3f(0.357, -1.369, 0.3);
	glVertex3f(0.357, -1.369, 0.2);
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.482, -1.369, 0.3);
	glEnd();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, FRONTFEET);
	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);//DOWN EXTEND;
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.1);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.482, -1.449, 0.1);
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.357, -1.369, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.482, -1.449, 0.1);
	glVertex3f(0.482, -1.449, 0.2);
	glEnd();

	glBegin(GL_POLYGON);//
	glVertex3f(0.357, -1.369, 0.2);
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.357, -1.449, 0.2);
	glEnd();

	//OUTLINE
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Outline);
	glBegin(GL_LINE_LOOP);//DOWN EXTEND;
	glColor3f(0, 0, 0);
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.357, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.2);
	glVertex3f(0.482, -1.449, 0.1);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.482, -1.449, 0.1);
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.357, -1.369, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.482, -1.369, 0.2);
	glVertex3f(0.482, -1.449, 0.1);
	glVertex3f(0.482, -1.449, 0.2);
	glEnd();

	glBegin(GL_LINE_LOOP);//
	glVertex3f(0.357, -1.369, 0.2);
	glVertex3f(0.357, -1.449, 0.1);
	glVertex3f(0.357, -1.449, 0.2);
	glEnd();
	glPopMatrix();
}

void Movefoward() {
	glTranslatef(0, 0, movefoward);
}
void Jump() {
	glTranslatef(0, jump, 0);
	movelegL = 30;
	movelegR = -30;

	jump += 0.01;
	if (jump > 0.4) {
		jump = 0;
		glTranslatef(0, 0, 0);
		movelegL = 0;
		movelegR = 0;
		switchjump = 'B';
	}


}

void drawRedCircle(float radius, float x, float y, float z) {
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.0, 0.0);		//red circle
	glVertex3f(x, y, z);				//origin of the circle

	for (angle = 0; angle <= 2 * PI; angle += (2 * PI) / 30.0)
	{
		x2 = x + radius * cos(angle);
		y2 = y + radius * sin(angle);
		glVertex3f(x2, y2, z);			//point on the perimeter of circle
	}

	glEnd();
}

void weapon() {
	glPushMatrix();
	glTranslatef(5.0, 3.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glColor3f(0.7529, 0.7529, 0.7529);
	drawsCylinderGLUFILL(0.1, 0.1, 8.0, 5.0, 1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0, 3.0, 0.0);

	drawRedCircle(0.3, 0.0, 3.0, -0.1);
	drawRedCircle(0.3, 0.0, 3.0, 0.1);
	glPushMatrix();
	glScalef(1.0, 1.5, 1.0);
	glBegin(GL_QUADS);
	glColor3f(0.0, 0.0, 0.5451);
	//bottom
	glVertex3f(1.0, 0.0, 0.1);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(-1.0, 0.0, -0.1);
	glVertex3f(-1.0, 0.0, 0.1);

	//top
	glVertex3f(1.0, 4.0, 0.1);
	glVertex3f(1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, 0.1);

	//front
	glVertex3f(1.0, 0.0, 0.1);
	glVertex3f(1.0, 4.0, 0.1);
	glVertex3f(-1.0, 4.0, 0.1);
	glVertex3f(-1.0, 0.0, 0.1);

	//back
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, -0.1);
	glVertex3f(-1.0, 0.0, -0.1);

	//left
	glVertex3f(-1.0, 4.0, 0.1);
	glVertex3f(-1.0, 4.0, -0.1);
	glVertex3f(-1.0, 0.0, -0.1);
	glVertex3f(-1.0, 0.0, 0.1);

	//right
	glVertex3f(1.0, 4.0, 0.1);
	glVertex3f(1.0, 4.0, -0.1);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(1.0, 0.0, 0.1);
	glEnd();
	/*-----------------------------------*/

	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	//bottom
	glVertex3f(1.0, 0.0, 0.1);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(-1.0, 0.0, -0.1);
	glVertex3f(-1.0, 0.0, 0.1);
	glEnd();

	//top
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 4.0, 0.1);
	glVertex3f(1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, 0.1);
	glEnd();

	//front
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.1);
	glVertex3f(1.0, 4.0, 0.1);
	glVertex3f(-1.0, 4.0, 0.1);
	glVertex3f(-1.0, 0.0, 0.1);
	glEnd();

	//back
	glBegin(GL_LINE_LOOP);
	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(1.0, 4.0, -0.1);
	glVertex3f(-1.0, 4.0, -0.1);
	glVertex3f(-1.0, 0.0, -0.1);
	glEnd();
	/*-----------------------------------*/

	glBegin(GL_LINE_STRIP);
	glLineWidth(2.0);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-0.3, 0.0, 0.1);
	glVertex3f(-0.3, 2.0, 0.1);
	glVertex3f(0.3, 2.0, 0.1);
	glVertex3f(0.3, 4.0, 0.1);

	glVertex3f(-0.3, 0.0, -0.1);
	glVertex3f(-0.3, 2.0, -0.1);
	glVertex3f(0.3, 2.0, -0.1);
	glVertex3f(0.3, 4.0, -0.1);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void projection() {
	glMatrixMode(GL_PROJECTION);		// refer to projection matrix
	glLoadIdentity();					// reset the projection matrix
	glTranslatef(ptx, pty, 0.0);		// translate the projection
	glRotatef(pry, 0.0, 1.0, 0.0);		// rotation in y for projection

	if (isOrtho) {
		//Orthographic view
		glOrtho(-10.0, 10.0, -10.0, 10.0, Onear, Ofar);
	}
	else {
		//Perspective view
		gluPerspective(20, 1.0, -1.0, 1.0);
		glFrustum(-10.0, 10.0, -10.0, 10.0, Pnear, Pfar);
	}
}


void lighting() {
	if (isLightOn) {
		glEnable(GL_LIGHTING);	// Turn on lighting for the whole 
	}
	else {
		glDisable(GL_LIGHTING);	// Turn off lighting for the whole 
	}

	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_POSITION, posD);
	glEnable(GL_LIGHT0);
}

void display()
{
	//glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	lighting();

	projection();

	GLuint texArr[2];

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(tx, 0.0, tz);

	glPushMatrix();
	Movefoward();
	switch (switchjump)
	{
	case 'B':
		Jump();
	default:
		break;
	}
	glPopMatrix();

	glRotatef(rotate_up_down, 1.0, 0.0, 0.0);
	glRotatef(rotate_left_right, 0.0, 1.0, 0.0);
	glScalef(size, size, size);

	glPushMatrix();		//P1
	/*------------------------------------------------*/

	glPushMatrix();
	glRotatef(turn_body, 0.0, 1.0, 0.0);

	glPushMatrix();		//P8
	glTranslatef(0.0, 0.7, 0.0);
	glScalef(1.0, 1.0, 0.64);

	glPushMatrix();		//P2
	glTranslatef(-0.75, 2.55, -0.6);
	glScaled(4.0, 4.0, 4.0);
	Head();
	glPopMatrix();		//P2

	glPushMatrix();		//P3
	glTranslatef(0.0, -0.2, 0.0);
	glScalef(1.5, 1.0, 1.0);
	if (istexture) {
		texArr[0] = loadTexture("body texture.bmp");
		Upperchest();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		Upperchest();
	}
	glPopMatrix();		//P3

	glPushMatrix();		//P4
	glTranslatef(0.0, -1.2, 0.0);
	if (istexture) {
		texArr[0] = loadTexture("body texture.bmp");
		Lowerchest();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		Lowerchest();
	}
	glPopMatrix();		//P4

	glPushMatrix();		//P5
	glTranslatef(0.0, -1.2, 0.0);
	if (istexture) {
		texArr[0] = loadTexture("body texture.bmp");
		stomach();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		stomach();
	}
	glPopMatrix();		//P5

	glPushMatrix();		//P6
	glTranslatef(0.0, -1.32, 0.0);
	glScalef(0.75, 0.75, 1.0);
	if (istexture) {
		texArr[0] = loadTexture("body texture.bmp");
		stomach();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		stomach();
	}
	glPopMatrix();		//P6

	glPopMatrix();		//P8

	glPushMatrix();		//P9
	glTranslated(1.8, 2.5, 0.0);
	joint();
	glPopMatrix();		//P9
	glPushMatrix();		//P9
	glTranslated(-1.8, 2.5, 0.0);
	joint();
	glPopMatrix();		//P9

	glPushMatrix();		//P10
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(1.8, 0.0, -2.2);
	shoulder();
	glPopMatrix();		//P10
	glPushMatrix();		//P10
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(-1.8, 0.0, -2.2);
	shoulder();
	glPopMatrix();		//P10

	glPushMatrix();		//P11
	glTranslated(1.8, 0.65, 0.0);
	glScalef(0.75, 0.75, 0.75);
	joint();
	glPopMatrix();		//P11
	glPushMatrix();		//P11
	glTranslated(-1.8, 0.65, 0.0);
	glScalef(0.75, 0.75, 0.75);
	joint();
	glPopMatrix();		//P11

	glPushMatrix();		//P12
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(1.8, 0.0, -0.4);
	glScalef(0.7, 0.7, 0.7);
	shoulder();
	glPopMatrix();		//P12
	glPushMatrix();		//P12
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(-1.8, 0.0, -0.4);
	glScalef(0.7, 0.7, 0.7);
	shoulder();
	glPopMatrix();		//P12

	glPushMatrix();		//P13
	glTranslated(1.8, -0.65, 0.0);
	glScalef(0.5, 0.5, 0.5);
	joint();
	glPopMatrix();		//P13
	glPushMatrix();		//P13
	glTranslated(-1.8, -0.65, 0.0);
	glScalef(0.5, 0.5, 0.5);
	joint();
	glPopMatrix();		//P13

	glPushMatrix();		//P14
	glTranslatef(1.7, -0.9, -0.3);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	if (istexture) {
		texArr[0] = loadTexture("finger texture.bmp");
		Hand();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		Hand();
	}
	glPopMatrix();		//P14
	glPushMatrix();		//P14
	glTranslatef(-1.9, -0.9, -0.3);
	glRotatef(-90, 0.0, 0.0, 1.0);
	glScalef(0.5, 0.5, 0.5);
	if (istexture) {
		texArr[0] = loadTexture("finger texture.bmp");
		Hand();
		glDeleteTextures(1, &texArr[0]);

		//Step5: Remove texture info
		glDisable(GL_TEXTURE_2D);

	}
	else {
		Hand();
	}
	glPopMatrix();		//P14

	glPopMatrix();
	/*----------------------------------------------*/

	glPushMatrix();		//P7
	glTranslatef(-0.6, -0.87, -0.64);
	glScalef(2.5, 2.5, 2.5);
	LowerBody();
	glPopMatrix();

	jump += 0.01;
	glPopMatrix();		//P7

	glPushMatrix();
	weapon();
	glPopMatrix();

	glPopMatrix();	// P1

}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------