#include <windows.h>
#include <gl/gl.h>
#include <string.h>
#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb-master/stb_image.h"
#include "texture.h"
#include <math.h>


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

int textureId;
int fond;
char anim=0;
float TypeOfAnim=3;
float tick=0;

void InitMain()
{
    anim=0;
    Menu_Clear();
    Menu_AddButton("Play",440,290,400,70,3); // отступ слева, высота, длина, ширина, размер шрифта
    Menu_AddButton("Exit",440,390,400,70,3);
}

void InitTest()
{
    tick=0;
    TypeOfAnim=0;
    anim =1;
    Menu_Clear();
    Menu_AddButton("Back",10,10,180,50,3);
    Menu_AddButton("Back",10,10,180,50,3);
    Menu_AddButton("Back",10,10,180,50,3);

}

void MouseDown()
{
    int buttonId = Menu_MouseDown();
    if (buttonId<0) return;
    char *name = Menu_GetButtonName(buttonId);
    if (strcmp(name,"Exit")==0) PostQuitMessage(0);
    if (strcmp(name,"Play")==0) InitTest();
    if (strcmp(name,"Back")==0) InitMain();
}

//float TextureVertex[] = {0,420, 180,420, 180,590,0,590};
float TextureVertex[] = {0,0, 0,0, 0,0, 0,0};
float TextureCord[] = {0,0, 0,0, 0,0, 0,0};

void ShowTexture(int x, int y, float NumberOfSprite, float TypeOfAnim)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureId);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.99);
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        static float spriteXsize=900; //переменные с размерами текстуры и отдельного кадра
        static float spriteYsize=320;
        static float charsizey=80;
        static float charsizex=100;

        TextureVertex[0]=TextureVertex[6]=-30+x;
        TextureVertex[2]=TextureVertex[4]=x+charsizex+10;
        TextureVertex[1]=TextureVertex[3]=y-60;
        TextureVertex[5]=TextureVertex[7]=y+charsizey;

        float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
        float right=left+(charsizex/spriteXsize); //номера кадра
        float top=(charsizey*TypeOfAnim)/spriteYsize;
        float bottom=top+(charsizey/spriteYsize);

        TextureCord[5]=TextureCord[7]=bottom;
        TextureCord[1]=TextureCord[3]=top;
        TextureCord[2] = TextureCord[4] = right;
        TextureCord[0] = TextureCord[6] = left;

        if (GetKeyState(VK_RIGHT)<0){
            TextureCord[2] = TextureCord[4] = right;
            TextureCord[0] = TextureCord[6] = left;
        }

        if (GetKeyState(VK_LEFT)<0){
            TextureCord[0] = TextureCord[6] = right;
            TextureCord[2] = TextureCord[4] = left;
        }

        glVertexPointer(2,GL_FLOAT,0,TextureVertex);
        glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

float tvertex[]={0,0,1280,0,1280,850,0,850};
float textureCoord[]={0,0, 1,0, 1,1, 0,1};

void fond_show()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D,fond);

    glColor3f(1,1,1);
    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT,0,tvertex);
        glTexCoordPointer(2,GL_FLOAT,0,textureCoord);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}

int b = 0;
int step = 10;
int position = 10;
int jump = 0;
int show = 508;

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
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          1280,
                          850,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    Texture_LoadTexture("hero.png",&textureId,GL_REPEAT,GL_NEAREST, NULL);
    Texture_LoadTexture("fond.jpg",&fond,GL_REPEAT,GL_NEAREST, NULL);
    RECT rct;
    GetClientRect(hwnd, &rct);
    glOrtho(0, rct.right, rct.bottom, 0, 1,-1);

    InitMain();

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

            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);//цвет очистки экрана
            glClear(GL_COLOR_BUFFER_BIT);//очищение экрана. ОЧИЩАЕМ ТОЛЬКО ЦВЕТ ЭКРАНА

            if (anim)
            {
                fond_show();
                ShowGround();
                ShowTexture(position, show-jump,tick,TypeOfAnim);

            }


            Menu_ShowMenu();

            SwapBuffers(hDC);


            if (GetKeyState(VK_RIGHT)<0 && position<=1150)
            {
                jump=0;
                TypeOfAnim=0;
                if (tick < 9){
                tick++;
                }
                else tick = 0;
                position = position + step;

            }
            if (GetKeyState(VK_LEFT)<0 && position>=10)
            {
                jump=0;
                TypeOfAnim=0;
                if (tick < 9){
                tick++;
                }
                else tick = 0;
                position = position - step;

            }
            if (GetKeyState(VK_UP)<0)
            {

                TypeOfAnim=1;
                if (tick < 5){
                tick++;
                jump = jump + 20;
                if (GetKeyState(VK_RIGHT)<0 && position<=1150){
                   position+=20;
                   jump = jump + 20;
                }
                if (GetKeyState(VK_LEFT)<0 && position>=10) {
                  position-=20;
                  jump = jump + 20;
                }
                }
                else if(tick>5&&tick<=9){
                tick++;
                jump = jump;

                }
                else
                {tick = 0;
                 jump=0;

                }

            }

            if (GetKeyState(VK_DOWN)<0)
            {
                TypeOfAnim=2;
                jump=0;
                if (tick < 5){
                tick++;
                }
                else if(tick>5&&tick<=9){
                    if (GetKeyState(VK_UP)<0)
                    {
                        tick++;
                    }
                }
                else tick = 0;
            }

            int p=0;
            if(GetKeyState(VK_RIGHT)>=0&&GetKeyState(VK_LEFT)>=0&&GetKeyState(VK_UP)>=0&&GetKeyState(VK_DOWN)>=0)
            {
                TypeOfAnim=3;
                p++;
                if (p == 1){
                tick+=1;
                p=0;
                }
                jump=0;

            }

            Sleep (50);
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

        case WM_MOUSEMOVE:
            Menu_MouseMove(LOWORD(lParam),HIWORD(lParam));
        break;

        case WM_LBUTTONDOWN:
            MouseDown();
        break;

        case WM_LBUTTONUP:
            Menu_MouseUp();
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
