#include <gl/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../stb-master/stb_easy_font.h"

#include "menu.h"

#define nameLen 20

typedef struct
{
    char name[nameLen];
    float vert[8];
    char isHover,isDown;
    float buffer[50*nameLen];
    int num_quads;
    float textPosX,textPosY,textScale;
} TBtn;
TBtn *btn = 0;
int btnCnt = 0;

float mouseX,mouseY;

// функция добавления кнопки
int Menu_AddButton(char *name, float x, float y, float width, float height, float textScale)
{
    btnCnt++; //заводим счетчик кнопок
 btn = realloc(btn, sizeof(btn[0])*btnCnt); //выделяем память под нужное количество
 snprintf(btn[btnCnt-1].name, nameLen, "%s", name); //выделение памяти и запись имени
 float *vert = btn[btnCnt-1].vert; //передача координат кнопки
 vert[0]=vert[6]=x; //заполняем массив вершин
 vert[2]=vert[4]=x+width;
 vert[1]=vert[3]=y;
 vert[5]=vert[7]=y+height;
 btn[btnCnt-1].isHover=0;
 btn[btnCnt-1].isDown=0;


 TBtn *b= btn + btnCnt - 1; //записываем в буфер данные кнопки
 b->num_quads = stb_easy_font_print(0, 0, name, 0, b->buffer, sizeof(b->buffer)); // запись координат вершин элементов имени
 b->textPosX = x +(width-stb_easy_font_width(name)*textScale)/2.0;
 b->textPosY = y +(height-stb_easy_font_height(name)*textScale)/2.0;
 b->textPosY+= textScale*2;
 b->textScale = textScale;
 return btnCnt-1;
}

// процедура отображения одной кнопки
void ShowButton(int buttonId)
{
    TBtn btn1=btn[buttonId];
    glVertexPointer(2,GL_FLOAT,0,btn1.vert);//пересылка всех данных в буфер
    glEnableClientState(GL_VERTEX_ARRAY); //запись координат в VBO
        if(btn1.isDown) glColor3f(0.6,0.6,0.7); //разрешение
        else if(btn1.isHover)   glColor3f(0.5,0.5,0.6);// при нажатии и наведении меняют цвет
        else glColor3f(0.3,0.3,0.4); //цвет кнопки
        glDrawArrays(GL_TRIANGLE_FAN,0,4);//отрисовка кнопки
        glColor3f(0.6,0.6,0.7);//цвет обводки
        glLineWidth(1); //цвет обводки
        glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки
    glDisableClientState(GL_VERTEX_ARRAY);

     glPushMatrix(); //матрицу в стек
 glColor3f(0.7,0.7,0.7); //цвет текста
 glTranslatef(btn1.textPosX,btn1.textPosY,0); //перенос матрицы для отрисовки текста
 glScalef(btn1.textScale,btn1.textScale,0); //масштабирование текста
 glEnableClientState(GL_VERTEX_ARRAY); // разрешение
 glVertexPointer(2, GL_FLOAT, 16, btn1.buffer); //вектор для отрисовки
 glDrawArrays(GL_QUADS, 0, btn1.num_quads*4); //отрисовка текста
 glDisableClientState(GL_VERTEX_ARRAY);
 glPopMatrix();
}

void Menu_ShowMenu()
{
    for (int i=0;i<btnCnt;i++)
        ShowButton(i);

}


//определяет попадание по кнопке
char isCoordInButton(int buttonId, float x, float y)
{
    float *vert=btn[buttonId].vert;
    return (x>vert[0])&&(y>vert[1])&&(x<vert[4])&&(y<vert[5]);
}

//фун для обработки перемещения мышки
int Menu_MouseMove(float x, float y)
{
    mouseX = x;
    mouseY = y;
    int moveBtn = -1;
    for (int i=0;i<btnCnt;i++)
        if (isCoordInButton(i, mouseX, mouseY))
        {
            btn[i].isHover = 1;
            moveBtn = i;
        }
        else
        {
            btn[i].isHover=0;
            btn[i].isDown=0;
        }
        return moveBtn;
}

//фун для обработки нажатия
int Menu_MouseDown()
{
    int downBtn = -1;
    for (int i=0;i<btnCnt;i++)
        if (isCoordInButton(i, mouseX, mouseY))
        {
            btn[i].isDown = 1;
            downBtn = i;
        }
    return downBtn;
}


//отжатие кнопки
void Menu_MouseUp()
{
    for (int i=0; i<btnCnt;i++)
        btn[i].isDown=0;
}

char *Menu_GetButtonName(int buttonId)
{
    return btn[buttonId].name;
}



typedef struct
{
    float vertex[8];
    float bufr[1000];
} Ground;
Ground flr;

int AddGround(float x, float y, float width, float height)
{
 float *vertex = flr.vertex; //передача координат кнопки
 vertex[0]=vertex[6]=x; //заполняем массив вершин
 vertex[2]=vertex[4]=x+width;
 vertex[1]=vertex[3]=y;
 vertex[5]=vertex[7]=y+height;

}

// процедура отображения одной кнопки
void ShowGround()
{
    AddGround(0,590,1280,590);
    Ground flr1=flr;
    glVertexPointer(2,GL_FLOAT,0,flr1.vertex);//пересылка всех данных в буфер
    glEnableClientState(GL_VERTEX_ARRAY);
         glColor3f(0,0,0);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);//отрисовка кнопки
        glColor3f(0,0,0);//цвет обводки
        glLineWidth(1); //цвет обводки
        glDrawArrays(GL_LINE_LOOP,0,4); //отрисовка обводки
    glDisableClientState(GL_VERTEX_ARRAY);

     glPushMatrix(); //матрицу в стек
 glDisableClientState(GL_VERTEX_ARRAY);
 glPopMatrix();
}

//очистка
void Menu_Clear()
{
    btnCnt=0;
    free(btn);
    btn=0;
}
