#include <windows.h>
#include <gl/gl.h>
#include "../../../stb-master/stb_easy_font.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb-master/stb_image.h"

#include <string.h>


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

unsigned int texture;
unsigned int Chrtexture;
char anim=0;
float TypeOfAnim=0;
float tick=0;

void texture_init()
{
    int twidth, theight,cnt;
    unsigned char *data = stbi_load("forest.jpg", &twidth, &theight, &cnt, 0);


    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,twidth,theight,
                                   0, cnt==4 ? GL_RGBA : GL_RGB,GL_UNSIGNED_BYTE,data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void character_init()
{
    int twidth, theight,cnt;
    unsigned char *data = stbi_load("step.png", &twidth, &theight, &cnt, 0);


    glGenTextures(1,&Chrtexture);
    glBindTexture(GL_TEXTURE_2D, Chrtexture);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,twidth,theight,
                                   0, cnt==4 ? GL_RGBA : GL_RGB,GL_UNSIGNED_BYTE,data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}


float vertex[]={-1,-1,0,1,-1,0,1,1,0,-1,1,0};
float texCoord[]={0,1, 1,1, 1,0, 0,0};

void texture_show()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D,texture);

    glColor3f(1,1,1);
    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(3, GL_FLOAT,0,vertex);
        glTexCoordPointer(2,GL_FLOAT,0,texCoord);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}


typedef struct
{
    char name[20];
    int x;
    int y;
    int width;
    int height;
    float textS;
    BOOL hover;

}TButton;

TButton btn[] = {{"Exit", 10,10,180,50,3,FALSE},
    {"<", 540,700,80,50,3,FALSE},
    {">", 640,700,80,50,3,FALSE},
    {"^", 800,700,80,50,3,FALSE},
};
int btnCnt = sizeof(btn)/sizeof(btn[0]);


void show_button(TButton btn)
{
    float vert[8];
    float buffer[1000];
    int num_quads;

    vert[0]=vert[6]=btn.x;
    vert[2]=vert[4]=btn.x+btn.width;
    vert[1]=vert[3]=btn.y;
    vert[5]=vert[7]=btn.y+btn.height;

    num_quads=stb_easy_font_print (0,0,btn.name,0,buffer,sizeof(buffer));

    float textPosX = btn.x +(btn.width-stb_easy_font_width(btn.name)*btn.textS)/2.0;
    float textPosY = btn.y +(btn.height-stb_easy_font_height(btn.name)*btn.textS)/2.0;

    glVertexPointer(2,GL_FLOAT,0,vert);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glColor3f(0.6,0.6,0.7);

    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки

    if(btn.hover) glColor3f(0.5,0.5,0.6);
    else glColor3f(0.3,0.3,0.4);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPushMatrix(); //матрицу в стек
    glColor3f(0.7,0.7,0.7); //цвет текста
    glTranslatef(textPosX,textPosY,0); //перенос матрицы для отрисовки текста
    glScalef(btn.textS,btn.textS,1); //масштабирование текста
    glEnableClientState(GL_VERTEX_ARRAY); // разрешение
    glVertexPointer(2, GL_FLOAT, 16, buffer); //вектор для отрисовки
    glDrawArrays(GL_QUADS, 0,num_quads*4); //отрисовка текста
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();

}

BOOL PointToButton(int x, int y, TButton btn)
{
    float vert[8];
    vert[0]=vert[6]=btn.x;
    vert[2]=vert[4]=btn.x+btn.width;
    vert[1]=vert[3]=btn.y;
    vert[5]=vert[7]=btn.y+btn.height;
    return (x>vert[0]) && (x<vert[4]) &&
           (y>vert[1]) && (y<vert[5]);
}

typedef struct
{
    int x;
    int y;
    int width;
    int height;

}Ground;

Ground flr[] = {0,660,1280,300};

void show_ground(Ground flr)
{
    float vert[8];
    float buffer[1000];
    int num_quads;

    vert[0]=vert[6]=flr.x;
    vert[2]=vert[4]=flr.x+flr.width;
    vert[1]=vert[3]=flr.y;
    vert[5]=vert[7]=flr.y+flr.height;

    num_quads=stb_easy_font_print (0,0,"",0,buffer,sizeof(buffer));


    glVertexPointer(2,GL_FLOAT,0,vert);

    glEnableClientState(GL_VERTEX_ARRAY);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glColor3f(0.6,0.6,0.6);

    glLineWidth(1);
    glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки

    glColor3f(0.2,0.2,0.25);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);
    glDisableClientState(GL_VERTEX_ARRAY);

}

void MouseDown()
{
    for(int i=0; i<btnCnt;i++)
    {
        if (strcmp(btn[i].name,"Exit")==0)
            PostQuitMessage(0);
        if ((strcmp(btn[i].name,"<")==0) || (strcmp(btn[i].name,"")==0))
        {
            TypeOfAnim=0;
            tick=0;
        }
        if (strcmp(btn[i].name,"^")==0)
        {
            TypeOfAnim=1;
            tick=0;
        }
        else
        {
            TypeOfAnim=2;
            tick=0;
        }
    }

}

float TextureVertex[] = {10,660,90,660,90,580,10,580};
float TextureCord[] = {0,0,0,0,0,0,0,0};

void character_show(float NumberOfSprite, float TypeOfAni)
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D,Chrtexture);

    glColor3f(0,0,0);
    glPushMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        static float spriteXsize=900; //переменные с размерами текстуры и отдельного кадра
        static float spriteYsize=240;
        static float charsizey=80;
        static float charsizex=100;
        float left=(charsizex*NumberOfSprite)/spriteXsize; //вычисление координат кадра на изображении от
        float right=left+(charsizex/spriteXsize); //номера кадра
        float top=(charsizey*TypeOfAnim)/spriteYsize;
        float bottom=top+(charsizey/spriteYsize);

        TextureCord[5]=TextureCord[7]=bottom;
        TextureCord[1]=TextureCord[3]=top;
        TextureCord[0]=TextureCord[6]=left;
        TextureCord[2]=TextureCord[4]=right;
        glVertexPointer(2,GL_FLOAT,0,TextureVertex);
        glTexCoordPointer(2,GL_FLOAT,0,TextureCord);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glPopMatrix();
}

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

        RECT rct; //создание переменной с координатами прямоуголника
        GetClientRect(hwnd, &rct); //получение текущих координат окна
        glOrtho(0, rct.right, rct.bottom, 0, -1, 1); //выставляем их как координаты окна

    texture_init();

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
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
            glClear(GL_COLOR_BUFFER_BIT);

            glLoadIdentity();

            texture_show();

            glOrtho(0, rct.right, rct.bottom, 0, 1, -1);
            show_ground(flr[0]);

            character_show(tick,TypeOfAnim);

            for(int i=0; i<btnCnt;i++)
               show_button(btn[i]);

            SwapBuffers(hDC);

            theta += 1.0f;
            tick++;
            if (tick>9)    tick=0;
            Sleep (1);
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

        case WM_LBUTTONDOWN:
           MouseDown();
        break;

        case WM_MOUSEMOVE:
            for(int i=0; i<btnCnt;i++)
                btn[i].hover=PointToButton(LOWORD(lParam),HIWORD(lParam),btn[i]);
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



