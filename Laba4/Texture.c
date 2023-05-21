#include "Texture.h"
#include <windows.h>
#include <gl/gl.h>
#include "stb_easy_font.h"

#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"
#include <stdbool.h>

typedef struct {
    unsigned int texture;
} Texture;

Texture tex_Character;
Texture tex_Background;
Texture tex_Blocks;

void createTexture(Texture *obj, const char *path)
{
    int h_image, w_image, cnt;
    unsigned char *data = stbi_load(path, &w_image, &h_image, &cnt, 0);

    glGenTextures(1, &obj->texture);
    glBindTexture(GL_TEXTURE_2D, obj->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_image, h_image,
            0, cnt == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void renderAreaTexture(Texture *obj, int x, int y, float weight, float height, float areaWeight, float areaHeight, int n, int t, BOOL visible)
{
    if (visible)
    {
        static float vertex[8]; //вектор текстурируемого многоугольника
        static float TexCord[]= {0,0, 1,0, 1,1, 0,1}; // текстурные координаты изображения

        vertex[0]=vertex[6]=x;
        vertex[2]=vertex[4]=x + areaWeight;
        vertex[1]=vertex[3]=y;
        vertex[5]=vertex[7]=y + areaHeight;

        float vrtcoord_left = (areaWeight*n)/weight;
        float vrtcoord_right = vrtcoord_left+(areaWeight/weight);
        float vrtcoord_top = (areaHeight*t)/height;
        float vrtcoord_bottom = vrtcoord_top+(areaHeight/height);

        TexCord[1] = TexCord[3] = vrtcoord_top; // запись в вектор текстурных координат
        TexCord[5] = TexCord[7] = vrtcoord_bottom;
        TexCord[2] = TexCord[4] = vrtcoord_right;
        TexCord[0] = TexCord[6] = vrtcoord_left;

        if (obj == &tex_Character)
        {
            if (mirror == 0)
            {
                TexCord[1] = TexCord[3] = vrtcoord_top;
                TexCord[5] = TexCord[7] = vrtcoord_bottom;
                TexCord[2] = TexCord[4] = vrtcoord_right;
                TexCord[0] = TexCord[6] = vrtcoord_left;
            }
            else
            {
                TexCord[1] = TexCord[3] = vrtcoord_top; // запись в вектор текстурных координат
                TexCord[5] = TexCord[7] = vrtcoord_bottom;
                TexCord[0] = TexCord[6] = vrtcoord_right;
                TexCord[2] = TexCord[4] = vrtcoord_left;
            }
        }

        glEnable(GL_TEXTURE_2D); //разрешение использования текстуры
        glBindTexture(GL_TEXTURE_2D, obj->texture);
        glEnable(GL_ALPHA_TEST); // проверка на элементы α-канала (не обязательно)
        glAlphaFunc(GL_GREATER, 0.5); // задается типе уровня и его числовая граница

        glPushMatrix();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glColor3f(1,1,1);
            glVertexPointer(2,GL_FLOAT,0,vertex);
            glTexCoordPointer(2, GL_FLOAT, 0, TexCord);
            glDrawArrays(GL_TRIANGLE_FAN,0,4);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}

void renderFullTexture(Texture *obj, int x, int y, int weight, int height, BOOL visible)
{
    if (visible)
    {
        static float vertex[8]; //вектор текстурируемого многоугольника
        static float TexCord[]= {0,0, 1,0, 1,1, 0,1}; // текстурные координаты изображения

        vertex[0]=vertex[6]=x;
        vertex[2]=vertex[4]=x + weight;
        vertex[1]=vertex[3]=y;
        vertex[5]=vertex[7]=y + height;

        glEnable(GL_TEXTURE_2D); //разрешение использования текстуры
        glBindTexture(GL_TEXTURE_2D, obj->texture);
        glEnable(GL_ALPHA_TEST); // проверка на элементы α-канала (не обязательно)
        glAlphaFunc(GL_GREATER, 0.99); // задается типе уровня и его числовая граница

        glPushMatrix();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glColor3f(1,1,1);
            glVertexPointer(2,GL_FLOAT,0,vertex);
            glTexCoordPointer(2, GL_FLOAT, 0, TexCord);
            glDrawArrays(GL_TRIANGLE_FAN,0,4);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
}
