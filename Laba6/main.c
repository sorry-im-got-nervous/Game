#include <windows.h>
#include <gl/gl.h>
#include <math.h>


struct SCamera {
    float x,y,z;
    float Xrot,Zrot;
};
struct SCamera camera = {0,0,2.5, 285,0};


void Camera_Apply(){
    glRotatef(-camera.Xrot,1,0,0);
    glRotatef(-camera.Zrot,0,0,1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
}

void Camera_Rotation(float xAngle, float zAngle){
    camera.Zrot+=zAngle;
    if( camera.Zrot<0) camera.Zrot +=360;
    if( camera.Zrot>360) camera.Zrot -=360;
    camera.Xrot+=xAngle;
    if( camera.Xrot<0) camera.Xrot =0;
    if( camera.Xrot<180) camera.Xrot =180;
}

void Camera_AutoMoveByMouse(int centerX, int centerY, float speed){
    POINT cur;
    POINT base = {centerX,centerY};
    GetCursorPos(&cur);
    Camera_Rotation( (base.y - cur.y)/5,(base.x - cur.x)/5);
    SetCursorPos(base.x,base.y);
}

void Camera_MoveDirectional(int forwardMove, int rightMove, float speed){

    float angle= -camera.Zrot/180*M_PI;

    if (forwardMove>0)
            angle+=rightMove>0 ? M_PI_4 :(rightMove<0 ? -M_PI_4 :0);
    if (forwardMove<0)
            angle+=M_PI + (rightMove>0 ? -M_PI_4 :(rightMove<0 ? M_PI_4 :0));
    if (forwardMove==0) {
        angle +=rightMove > 0? M_PI_2 : -M_PI_2;
        if (rightMove==0) speed=0;
    }
    if (speed!=0){
        camera.x+= sin(angle)*speed;
        camera.y+= cos(angle)*speed;
    }
}


void WndResize(int x, int y){
    glViewport(0,0,x,y);
    float k=x/(float)y;
    float sz = 0.1;
    glLoadIdentity();
    glFrustum(-k*sz, k*sz, -sz, sz, sz*2, 100);
}

void MoveCamera(){
    Camera_MoveDirectional(
        GetKeyState('W')< 0 ? 1 : GetKeyState('S')< 0 ? -1 : 0,
        GetKeyState('D')< 0 ? 1 : GetKeyState('A')< 0 ? -1 : 0, 0.1);

    Camera_AutoMoveByMouse(400,400,0.1);
}

void drawChessboard(int n, float tileSize, float posX, float posY, float PosZ){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((i + j) % 2 == 0) glColor3f(0.6f, 1.0f, 1.0f);
            else glColor3f(0.1f, 0.1f, 0.4f);
            glPushMatrix();
            glTranslatef(i * tileSize + posX, j * tileSize + posY, PosZ);
            glBegin(GL_QUADS);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(tileSize, 0.0f, 0.0f);
            glVertex3f(tileSize, tileSize, 0.0f);
            glVertex3f(0.0f, tileSize, 0.0f);
            glEnd();
            glPopMatrix();
        }
    }
}
float vertex[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};
float normal[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1};


void drawCoords() {
    glBegin(GL_LINES);
    // ось X
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // ось Y
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // ось Z
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

void Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawTile(float tileSize)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glEnable(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(tileSize, 0.0f, 0.0f);
        glVertex3f(tileSize, tileSize, 0.0f);
        glVertex3f(0.0f, tileSize, 0.0f);
        glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawCube1()
{
    glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(-1, -1, 1);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}

void DrawCube2()
{
    glPushMatrix();
        glScalef(0.3, 0.3, 0.3);
        glTranslatef(1, 2, 1);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}

void DrawCube3()
{
    glPushMatrix();
        glScalef(0.5, 0.5, 0.5);
        glTranslatef(2, -1, 1.5);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "laba6",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1250,
                          800,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1,1, -1,1, 2,80);
    glEnable(GL_DEPTH_TEST);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);/*допуск к освещению*/
    glEnable(GL_LIGHT0);/*режим*/
    glEnable(GL_COLOR_MATERIAL); /*материал*/
    glEnable(GL_NORMALIZE);/*доступ к нормали*/


    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glPushMatrix();

            if (GetForegroundWindow()==hwnd) MoveCamera();
            Camera_Apply();

            glPushMatrix();
                    glRotatef(24,0,0,1);
                    glRotatef(theta,0,1,0);
                    float position[] = {0,0,1,0};
                    glLightfv(GL_LIGHT0, GL_POSITION, position);

                    glTranslatef(0,0,40);
                    glScalef(1,1,1);
                    glColor3f(1,1,1);
                    Draw();
            glPopMatrix();

                glColor3f(0,0.5,0.5);
                DrawCube1();
                glColor3f(0.5,0,0.5);
                DrawCube2();
                glColor3f(0.5,0.5,0);
                DrawCube3();
                drawCoords();
                glScalef(0.7,0.7,0.7);

            drawChessboard(10, 0.5, -2.5, -2.5,0);

            glPopMatrix();

            SwapBuffers(hDC);
            theta++;
            Sleep (10);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
