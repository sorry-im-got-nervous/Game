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
    // X
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    // Y
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    // Z
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


void Init_Material()
{
    glEnable(GL_COLOR_MATERIAL);


    glShadeModel(GL_SMOOTH);
    GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_diffuse[] = { 0.0f, 1.0f, 1.0f, 1.0f };
    GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 32.0f };
    GLfloat material_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void drawPrism(int n, int height)
{
     const float pi = acos(-1);
    const int vn = 2*n; // количество вершин

    float vertieces[3*vn]; // Это же и нормали.

    for (int i = 0; i < n; i++) {
        float phi = 2 * pi * (float)i / (float)n;
        float c = cos(phi);
        float s = sin(phi);

        // Вершина в верхнем основании
        vertieces[6*i + 0] = c/2;
        vertieces[6*i + 1] = s/2;
        vertieces[6*i + 2] = 1.0;

        // Вершина в нижнем основании
        vertieces[6*i + 3] = c;
        vertieces[6*i + 4] = s;
        vertieces[6*i + 5] = -1.0;

    }

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertieces);
    glNormalPointer(GL_FLOAT, 0, vertieces);

    // Боковые стороны
    GLuint side_index[4];
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 4; k++) {
            side_index[k] = (2*i + k) % vn;
        }

        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, &side_index);
    }


    // Верхнее основание
    GLuint base_index[n];
    for (int k = 0; k < n; k++) {
        base_index[k] = 2*k;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);

    // Нижнее основание
    for (int k = 0; k < n; k++) {
        base_index[k] = 2*k + 1;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

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

    glEnable(GL_LIGHTING);/*äîïóñê ê îñâåùåíèþ*/
    glEnable(GL_LIGHT0);/*ðåæèì*/
    glEnable(GL_COLOR_MATERIAL); /*ìàòåðèàë*/
    glEnable(GL_NORMALIZE);/*äîñòóï ê íîðìàëè*/


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

                drawCoords();
                glScalef(0.7,0.7,0.7);

            drawChessboard(20, 0.5, -5, -5,0);

             glPushMatrix();
            glTranslatef(0.0, 0.0, 1);
            float transparency = 0;
            for (int i = 0; i < 9; i++)
            {
                glPushMatrix();
                    glRotatef(45*i, 0, 0, 1);
                    glTranslatef(4, 0, 0);
                    glColor4f(0.5,0,1,transparency);
                    drawPrism(27,2);
                glPopMatrix();
                transparency += 0.1;
            }
            glPopMatrix();

            glPushMatrix();
            glRotatef(theta,0,0,1);

            glPopMatrix();

            drawCoords();
            Init_Material();
            glPopMatrix();

            SwapBuffers(hDC);

            theta += 1.0f;
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
