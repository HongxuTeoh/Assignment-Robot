#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "Assignment Robot"

bool isOrtho = true;
float up_down = 0.0;
float rotate_left_right = 0.0;

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

		else if (wParam == VK_SPACE)
			isOrtho = !isOrtho;

		else if (wParam == VK_UP)
			up_down += 0.1;
		else if (wParam == VK_DOWN)
			up_down -= 0.1;

		else if (wParam == VK_LEFT)
			rotate_left_right += 1.0;
		else if (wParam == VK_RIGHT)
			rotate_left_right -= 1.0;

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

void drawSphere(float r) {
	GLUquadricObj* sphere = NULL;			// Create a quadric obj pointer
	sphere = gluNewQuadric();				// Create a quadric obj 
	gluQuadricDrawStyle(sphere, GLU_FILL);	// Set to draw style to sphere
	gluSphere(sphere, r, 50, 50);		// Draw sphere
	gluDeleteQuadric(sphere);
}

void drawCylinder(float br, float tr, float h) {
	GLUquadricObj* cylinder = NULL;				// Create a quadric obj pointer
	cylinder = gluNewQuadric();					// Create a quadric obj 
	gluQuadricDrawStyle(cylinder, GLU_FILL);	// Set to draw style to cylinder
	gluCylinder(cylinder, br, tr, h, 30, 30);			// Draw cylinder
	gluDeleteQuadric(cylinder);
}

void drawCone(float tr, float h) {
	GLUquadricObj* cylinder = NULL;				// Create a quadric obj pointer
	cylinder = gluNewQuadric();					// Create a quadric obj 
	gluQuadricDrawStyle(cylinder, GLU_FILL);	// Set to draw style to cone
	gluCylinder(cylinder, 0, tr, h, 30, 30);				// Draw cone
	gluDeleteQuadric(cylinder);
}

void projection() {
	glMatrixMode(GL_PROJECTION);		// refer to projection matrix
	glLoadIdentity();					// reset the projection matrix
	//glTranslatef(ptx, pty, 0.0);		// translate the projection
	//glRotatef(pry, 0.0, 1.0, 0.0);		// rotation in y for projection

	if (isOrtho) {
		//Orthographic view
		glOrtho(-5.0, 5.0, -5.0, 5.0, 5.0, -5.0);
	}
	else {
		//Perspective view
		gluPerspective(30.0, 1.0, -5.0, 5.0);
		glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, -5.0);
	}
}

// robot head and neck



// robot body
void body() {
	glBegin(GL_QUADS);		// shoulder (not complete yet
	glColor3f(1.0, 1.0, 1.0);
		//top
		glVertex3f(1.0, 2.5, 1.0);
		glVertex3f(1.0, 2.5, -1.0);
		glVertex3f(-1.0, 2.5, -1.0);
		glVertex3f(-1.0, 2.5, 1.0);

		//bottom
		glVertex3f(1.0, 1.5, 1.0);
		glVertex3f(1.0, 1.5, -1.0);
		glVertex3f(-1.0, 1.5, -1.0);
		glVertex3f(-1.0, 1.5, 1.0);

		//front
		glVertex3f(-1.0, 2.5, 1.0);
		glVertex3f(1.0, 2.5, 1.0);
		glVertex3f(1.0, 1.5, 1.0);
		glVertex3f(-1.0, 1.5, 1.0);

		//back
		glVertex3f(-1.0, 2.5, -1.0);
		glVertex3f(1.0, 2.5, -1.0);
		glVertex3f(1.0, 1.5, -1.0);
		glVertex3f(-1.0, 1.5, -1.0);

		//left
		glVertex3f(-1.0, 2.5, -1.0);
		glVertex3f(-1.0, 2.5, 1.0);
		glVertex3f(-1.0, 1.5, 1.0);
		glVertex3f(-1.0, 1.5, -1.0);

		//right
		glVertex3f(1.0, 2.5, 1.0);
		glVertex3f(1.0, 2.5, -1.0);
		glVertex3f(1.0, 1.5, -1.0);
		glVertex3f(1.0, 1.5, 1.0);
	glEnd();	
}

void Hand() {
	glBegin(GL_QUADS);		// need to rotate/translate/scaled
	glColor3f(1.0, 1.0, 1.0);
		//bottom
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.2);
		glVertex3f(1.0, 0.0, 1.2);
		glVertex3f(1.0, 0.0, 0.0);

		//top
		glVertex3f(0.0, 0.3, 0.0);
		glVertex3f(0.0, 0.3, 1.2);
		glVertex3f(1.0, 0.3, 1.2);
		glVertex3f(1.0, 0.3, 1.2);

		//near (x axis = 0)
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.2);
		glVertex3f(0.0, 0.3, 1.2);
		glVertex3f(0.0, 0.3, 0.0);

		//far (x axis = 1)
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 1.2);
		glVertex3f(1.0, 0.3, 1.2);
		glVertex3f(1.0, 0.3, 0.0);

		//left (z axis = 0)
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 0.3, 0.0);
		glVertex3f(0.0, 0.3, 0.0);

		//right (z axis = 1.2)
		glVertex3f(0.0, 0.0, 1.2);
		glVertex3f(1.0, 0.0, 1.2);
		glVertex3f(1.0, 0.3, 1.2);
		glVertex3f(0.0, 0.3, 1.2);
	glEnd();

	firstFinger();		//(1st)index finger

	secondFinger();		//(2nd)middle finger

	thirdFinger();		//(3rd)ring finger

	fourthFinger();		//(4th)little finger
}

void cubeFinger(float length, float height, float width) {
	glBegin(GL_QUADS);		//bottom finger
	glColor3f(1.0, 1.0, 1.0);
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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	projection();

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(0.0, up_down, 0.0);
	glRotatef(rotate_left_right, 0.0, 1.0, 0.0);

	/*glPushMatrix();
	glTranslatef(-0.4, 0.0, -0.3);
	glScaled(3.5, 3.5, 3.5);
	head();
	glPopMatrix();*/

	glPushMatrix();
	glTranslatef(3.0, 0.0, 0.0);
	glScalef(5.0, 5.0, 5.0);
	Hand();
	cubeFinger(0.0, 0.3, 0.3);
	glPopMatrix();
	
	glPopMatrix();

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