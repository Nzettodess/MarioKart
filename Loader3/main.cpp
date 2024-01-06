#include "main.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
#include <windows.h>
#include <MMSystem.h>
#include <chrono>
using namespace std;

int POS_X, POS_Y;


float pos_x, pos_y, pos_z;
float angle_x = 30.0f, angle_y = 0.0f;

int x_old = 0, y_old = 0;
int current_scroll = 5;
float zoom_per_scroll;

bool is_holding_mouse = false;
bool is_updated = false;

//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;
//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;
//light position variables
GLfloat lx = 1.0;
GLfloat ly = 1.0;
GLfloat lz = 1.0;
GLfloat lw = 1.0;
GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 }; /* Red diffuse light. */
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; /* Infinite light location. */

chrono::high_resolution_clock::time_point lastTime;
float deltaTime = 0.0f, totalTime = 10000.0f;
bool game_over = false;

const int MAP_SIZE = 40;
int gameMap[MAP_SIZE][MAP_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
float xpos = 15.0 * 5.0, ypos = -1.0, zpos = 10.0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;
float speed = 0.0, maxSpeed = 1.0, acceleration = 0.05, deceleration = 0.1;
float positionx[100], positiony[100], positionz[100], buildX[12], buildZ[12], treeX[100], treeZ[100];
int score = 0;
double rotate_x = 0, rotate_y = 0;
GLuint texture, texture1, texture2, texture3; //the array for our texture

Model model, building, player, tree, background;
GLuint LoadTexture(const char* filename, int width, int	height)
{
    GLuint texture;
    unsigned char* data;
    FILE* file;
    //The following code will read in our RAW file
    if (fopen_s(&file, filename, "rb") != 0) return 0;

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);
    glGenTextures(1, &texture); //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it’s array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //Generate the texture
    //Please be informed to switch from GL_RGB to GL_BGR_EXT for bitmap image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
    free(data); //free the texture
    return texture; //return whether it was successfull
}
void FreeTexture(GLuint texture)
{
    glDeleteTextures(1, &texture);
}
void cubepositions(void)
{
    srand(time(NULL));
    int cubeCount = 0;
    while (cubeCount < 100)
    {
        int i = rand() % MAP_SIZE;
        int j = rand() % MAP_SIZE;
        if (gameMap[i][j] == 1)
        {
            positionz[cubeCount] = j * 5.0 + 0.5; // Adjust for cube size
            positionx[cubeCount] = i * 5.0 + 0.5; // Adjust for cube size
            cubeCount++;
        }
    }
}
void buildPosition(void)
{
    float groundSize = 5.0;
    int p = 0;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            float xTranslation = i * groundSize;
            float zTranslation = j * groundSize;
            if (gameMap[i][j] == 2)
            {
                buildX[p] = xTranslation;
                buildZ[p] = zTranslation;
                p++;
            }
            if (gameMap[i][j] == 3)
            {
                treeX[p] = xTranslation;
                treeZ[p] = zTranslation;
            }
        }
    }
}
void ground(void)
{
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5, 0.0, -0.5);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.5, 0.0, -0.5);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5, 0.0, 0.5);
    glTexCoord2d(0.0, 1.0); glVertex3f(-0.5, 0.0, 0.5);
    glEnd();
}
void wall(void)
{
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2d(0.0, 0.0); glVertex3f(-0.5, 0.0, -0.5);
    glTexCoord2d(1.0, 0.0); glVertex3f(0.5, 0.0, -0.5);
    glTexCoord2d(1.0, 1.0); glVertex3f(0.5, 1.0, -0.5);
    glTexCoord2d(0.0, 1.0); glVertex3f(-0.5, 1.0, -0.5);
    glEnd();
}
bool checkCollision(float cubeX, float cubeY, float cubeZ, float cubeSize)
{
    // Assuming player size is 1x1x1
    if (xpos + 0.5 > cubeX - cubeSize / 2 && xpos - 0.5 < cubeX + cubeSize / 2 &&
        ypos + 1.0 > cubeY - cubeSize / 2 && ypos - 0.5 < cubeY + cubeSize / 2 &&
        zpos + 0.5 > cubeZ - cubeSize / 2 && zpos - 0.5 < cubeZ + cubeSize / 2)
    {
        return true; // Collision detected
    }
    return false; // No collision
}
bool checkBuildingCollision(float playerX, float playerY, float playerZ,
    float buildingX, float buildingY, float buildingZ,
    float buildingSizeX, float buildingSizeY, float buildingSizeZ)
{
    // Assuming buildings are axis-aligned boxes
    if (playerX + 0.5 > buildingX - buildingSizeX / 2 &&
        playerX - 0.5 < buildingX + buildingSizeX / 2 &&
        playerY + 1.5 > buildingY - buildingSizeY / 2 &&
        playerY - 1.5 < buildingY + buildingSizeY / 2 &&
        playerZ + 0.5 > buildingZ - buildingSizeZ / 2 &&
        playerZ - 0.5 < buildingZ + buildingSizeZ / 2)
    {
        return true; // Collision detected with the building
    }
    return false; // No collision with the building
}
void updatePlayerPosition(void)
{
    float yrotrad = ((rotate_y) / 180 * 3.141592654f);

    float moveX = speed * sin(yrotrad) + 0.0 * cos(yrotrad);
    float moveZ = speed * cos(yrotrad) - 0.0 * sin(yrotrad);

    float newX = xpos + moveX;
    float newZ = zpos + moveZ;
    float newY = ypos;
    // Check bounds to prevent going out of the map
    if (newX > (MAP_SIZE - 1) * 5.0 - 0.5) {
        newX = (MAP_SIZE - 1) * 5.0 - 0.5;
    }
    else if (newX < 0.5) {
        newX = 0.5;
    }
    //if (newY > MAX_HEIGHT) {
    //	newY = MAX_HEIGHT;
    //}
    //else if (newY < 0.5) {
    //	newY = 0.5;
    //}
    if (newZ > (MAP_SIZE - 1) * 5.0 - 0.5) {
        newZ = (MAP_SIZE - 1) * 5.0 - 0.5;
    }
    else if (newZ < 0.5) {
        newZ = 0.5;
    }

    for (int i = 0; i < 12; i++)
    {
        if (checkBuildingCollision(newX, newY, newZ, buildX[i]-10.0, 0.0, buildZ[i], 5.0*4, 0.0, 5.0*31))
        {
            // Collision detected with building i
            // Adjust player position to resolve collision
            // For simplicity, you can stop player movement in the colliding direction
            newX = xpos;
            newY = ypos;
            newZ = zpos;
        }
        if (checkBuildingCollision(newX, newY, newZ, treeX[i], 0.0, treeZ[i], 5.0, 0.0, 5.0))
        {
            // Collision detected with building i
            // Adjust player position to resolve collision
            // For simplicity, you can stop player movement in the colliding direction
            newX = xpos;
            newY = ypos;
            newZ = zpos;
        }
    }

    // Update player position with bounds checking
    xpos = newX;
    //ypos = newY;
    zpos = newZ;

    glutPostRedisplay();
}
void cube(void)
{
    for (int i = 0; i < 100; i++)
    {
        if (!checkCollision(positionx[i], 0.0, positionz[i], 1.0))
        {
            glPushMatrix();
            //glTranslated(positionx[i], 0.0, positionz[i]);
            glColor3f(1.0, 1.0, 1.0);
            model.draw(positionx[i], 0.0, positionz[i], 1.0, angle + 90.0, 90.0, 90.0);
            glPopMatrix();
        }
        else
        {
            // Cube collided, update its position
            score++;
            int k, l;
            do {
                k = rand() % MAP_SIZE;
                l = rand() % MAP_SIZE;
            } while (gameMap[k][l] != 1);

            positionz[i] = l * 5.0 + 0.5; // Adjust for cube size
            positionx[i] = k * 5.0 + 0.5; // Adjust for cube size
        }
    }
}
void drawTree(float x, float y, float z, float scale, float angleX, float angleY, float angleZ)
{
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    //GLfloat model_ambient_diffuse[] = { 0.2, 0.2, 0.2, 1.0 }; // Lower ambient and diffuse values for the model
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, model_ambient_diffuse);
    tree.draw(x, y, z, scale, angleX, angleY, angleZ);
    glPopMatrix();
}
void drawBuilding(float x, float y, float z, float scale, float angleX, float angleY, float angleZ)
{
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    GLfloat model_ambient_diffuse[] = { 0.2, 0.2, 0.2, 1.0 }; // Lower ambient and diffuse values for the model
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, model_ambient_diffuse);
    building.draw(x, y, z, scale, angleX, angleY, angleZ);
    glPopMatrix();
}
void drawPlayer(void)
{
    glPushMatrix();
    GLfloat model_ambient_diffuse[] = { 0.2, 0.2, 0.2, 1.0 }; // Lower ambient and diffuse values for the model
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, model_ambient_diffuse);
    glColor3f(1.0f, 1.0f, 1.0f);
    player.draw(xpos, ypos, zpos, 1.0, 0.0, rotate_y, 0.0);
    background.draw(xpos, ypos, zpos, 1.0, 0.0, rotate_y, 0.0);
    //player.draw();
    glPopMatrix();
}
void drawMap(void)
{
    float groundSize = 5.0;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            float xTranslation = i * groundSize;
            float zTranslation = j * groundSize;
            glPushMatrix();
            glTranslated(xTranslation, -1.0, zTranslation);
            glScaled(groundSize, 1.0f, groundSize);
            if (gameMap[i][j] == 0 || gameMap[i][j] == 3)
            {
                glBindTexture(GL_TEXTURE_2D, texture);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, texture1);  // Path texture
            }

            ground();  // Draw the terrain
            GLfloat ground_ambient_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Higher ambient and diffuse values for the ground
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ground_ambient_diffuse);
            glBindTexture(GL_TEXTURE_2D, texture2);
            glPopMatrix();
            if (gameMap[i][j] == 2)
            {
                drawBuilding(xTranslation, -1.0, zTranslation, 0.05, 0.0, 90.0, 0.0);

                //drawBuilding(xTranslation, -1.0, zTranslation, 1, 0.0, 90.0, 0.0);
            }
            if (gameMap[i][j] == 3)
            {
                drawTree(xTranslation, -1.0, zTranslation, 1.0, 0.0, 0.0, 0.0);
                //drawBuilding(xTranslation, -1.0, zTranslation, 1, 0.0, 90.0, 0.0);
            }
        }
    }
}
void drawBackground(void)
{
    glPushMatrix();
    /*glTranslated(0.0, 0.0, 0.0);
    glScaled(1000.0f, 1000.0f, 1000.0f); 
    glBindTexture(GL_TEXTURE_2D, texture3);
    wall();
    glBindTexture(GL_TEXTURE_2D, texture2);*/
    glColor3f(1.0f, 1.0f, 1.0f);
    //background.draw(xTranslation, -1.0, zTranslation, 1.0, 0.0, 0.0, 0.0);
    glPopMatrix();
}
void renderScore(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, HEIGHT, 0); // Set up orthographic projection
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(10.0, 10.0, 10.0);
    glRasterPos2f(WIDTH / 3, 20.0); // Adjust position as needed
    string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glRasterPos2f(WIDTH / 2, 20.0);
    string timeText = "Time: " + to_string(static_cast<int>(totalTime)) + "s";
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    if (game_over) {
        // Display "Game Over" message
        glRasterPos2f(WIDTH / 2.5, HEIGHT / 2); // Adjust position as needed
        string gameOverText = "Game Over! Final Score: " + std::to_string(score)
            + "\n\nPress ESC to Exit.";
        for (char c : gameOverText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void camera(void)
{
    //glRotatef(xrot, 1.0, 0.0, 0.0); // rotate our camera on the x-axis (left and right)
    //glRotatef(yrot, 0.0, 1.0, 0.0); // rotate our camera on the y-axis (up and down)
    //glTranslated(-xpos, -ypos, -zpos);
    // Calculate the position of the camera in a third-person view
    float cameraDistance = 3.0; // adjust this distance as needed
    float cameraHeight = -0.5;  // adjust this height as needed

    float xCam = xpos - cameraDistance * sin(yrot * (3.14159265 / 180.0));
    float zCam = zpos - cameraDistance * cos(yrot * (3.14159265 / 180.0));
    float yCam = ypos + cameraHeight;


    gluLookAt(xCam, yCam, zCam, xpos, ypos, zpos, 0.0, 1.0, 0.0);
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 200000.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouseMovement(int x, int y)
{
    int diffx = x - lastx;//check the difference between the current x and the last x position
    int diffy = y - lasty; //check the difference between the current y and the last y position
    lastx = x; //set lastx to the current x position
    lasty = y; //set lasty to the current y position
    xrot += (float)diffy; //set the x rot to x rot with the addition of the difference in the y position
    yrot += (float)diffx; //set the x rot to y rot with the addition of the difference in the x position
}

void init() {
    cubepositions();
    buildPosition();



    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    //glShadeModel(GL_FLAT);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT2); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT3); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT4); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT5); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT6); //enable LIGHT1, our Ambient Light
    glEnable(GL_LIGHT7); //enable LIGHT1, our Ambient Light

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, 1.0, 1.0, 200000.0);
    glMatrixMode(GL_MODELVIEW);


    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    //GLfloat material_diffuse[] = { .2, .2, .2, 1.0 };
    //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_diffuse);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    model.load("Models/Interative Object/Coins/Bells.obj");
    //building.load("Models/Building/building.obj");
    building.load("Models/Building/FTMK Building/FTMK.obj"); // need scale
    //building.load("Models/FTMK Building/1.obj"); // need scale
    player.load("Models/Mario Kart/Mario Kart by Sets/MarioKart/mk_kart.obj");
    tree.load("Models/Enviroments/Tree/Lowpoly_Tree.obj");
    background.load("Models/Background/Texture Background Sky 1365x768/untitled.obj");


    pos_x = model.pos_x;
    pos_y = model.pos_y;
    pos_z = model.pos_z - 1.0f;

    zoom_per_scroll = -model.pos_z / 10.0f;

    lastTime = std::chrono::high_resolution_clock::now();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(pos_x, pos_y, pos_z);
    glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);


    GLfloat DiffuseLight[] = { dlr, dlg, dlb };
    GLfloat AmbientLight[] = { alr, alg, alb };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
    glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
    GLfloat LightPosition[] = { lx, ly, lz, lw };
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    GLfloat Lightbuilding[] = { lx, 291, 1.0, 1.0 };
    glLightfv(GL_LIGHT2, GL_POSITION, LightPosition);


    //GLfloat global_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    


    camera();
    drawMap();
    drawPlayer();
    cube();
    drawBackground();
    renderScore();

    angle++;

    glutSwapBuffers();
    is_updated = true;
}
void keyboard(unsigned char key, int x, int y)
{
    if (game_over) {
        if (key == 27)
        {
            exit(0);
        }
    }
    else {
        if (key == 'w')
        {
            speed += acceleration;
            if (speed > maxSpeed) speed = maxSpeed;
        }
        if (key == 's')
        {
            speed -= deceleration;
            if (speed < -maxSpeed / 2) speed = -maxSpeed / 2;
        }
        if (key == 'd')
        {
            rotate_y -= 3.0;
        }
        if (key == 'a')
        {
            rotate_y += 3.0;
        }
        if (key == 27)
        {
            exit(0);
        }
        if (key == 't') {
            dlr = 1.0; // change light to white
            dlg = 1.0;
            dlb = 1.0;
            cout << "Pressed 't': white light" << endl;
            display();
        }
        if (key == 'r') {
            dlr = 1.0; // change light to red
            dlg = 0.0;
            dlb = 0.0;
            cout << "Pressed 'r': Red light" << endl;
            display();
        }
        if (key == 'g') {
            dlr = 0.0; // change light to green
            dlg = 1.0;
            dlb = 0.0;
            cout << "Pressed 'g': Green light" << endl;
            display();
        }
        if (key == 'b') {
            dlr = 0.0; // change light to blue
            dlg = 0.0;
            dlb = 1.0;
            cout << "Pressed 'b': Blue light" << endl;
            display();
        }
        if (key == 'i') {
            ly += 10.0; // move the light up
            cout << "Pressed 'w': Move light up" << endl;
            cout << "ly: " << ly << " lx: " << lx << endl;
            display();
        }
        if (key == 'k') {
            ly -= 10.0; // move the light down
            cout << "Pressed 's': Move light down" << endl;
            cout << "ly: " << ly << " lx: " << lx << endl;
            display();
        }
        if (key == 'j') {
            lx -= 10.0; // move the light left
            cout << "Pressed 'a': Move light left" << endl;
            cout << "ly: " << ly << " lx: " << lx << endl;
            display();
        }
        if (key == 'l') {
            lx += 10.0; // move the light right
            cout << "Pressed 'd': Move light right" << endl;
            cout << "ly: " << ly << " lx: " << lx << endl;
            display();
        }
        if (key == 'm') {
            alr += 0.1;
            alg += 0.1;
            alb += 0.1;

            cout << "alr: " << alr << " alg: " << alg << " alb: " << alb << endl;
            display();
        }
        if (key == 'n') {
            alr -= 0.1;
            alg -= 0.1;
            alb -= 0.1;
            cout << "alr: " << alr << " alg: " << alg << " alb: " << alb << endl;
            display();
        }
    }
    glutPostRedisplay();
}
void timer(int value) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    totalTime -= deltaTime;

    if (totalTime <= 0.0)
    {
        totalTime = 0.0;
        game_over = true;
    }
    if (is_updated) {
        is_updated = false;
        glutPostRedisplay();
    }
    glutTimerFunc(INTERVAL, timer, 0);
}

void mouse(int button, int state, int x, int y) {
    is_updated = true;

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            x_old = x;
            y_old = y;
            is_holding_mouse = true;
        }
        else
            is_holding_mouse = false;
    }
    else if (state == GLUT_UP) {
        switch (button) {
        case 3:
            if (current_scroll > 0) {
                current_scroll--;
                pos_z += zoom_per_scroll;
            }
            break;
        case 4:
            if (current_scroll < 15) {
                current_scroll++;
                pos_z -= zoom_per_scroll;
            }
            break;
        }
    }
}

void motion(int x, int y) {
    if (is_holding_mouse) {
        is_updated = true;

        angle_y += (x - x_old);
        x_old = x;
        if (angle_y > 360.0f)
            angle_y -= 360.0f;
        else if (angle_y < 0.0f)
            angle_y += 360.0f;

        angle_x += (y - y_old);
        y_old = y;
        if (angle_x > 90.0f)
            angle_x = 90.0f;
        else if (angle_x < -90.0f)
            angle_x = -90.0f;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    //POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    //POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    //glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Street Kart Around FTMK by Resident Evil");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutIdleFunc(updatePlayerPosition);
    glutReshapeFunc(reshape);
    texture = LoadTexture("Models/Enviroments/Environment Textures/grass.bmp", 256, 256);
    texture1 = LoadTexture("Models/Enviroments/Environment Textures/raod.bmp", 256, 256);
    texture2 = LoadTexture("Models/Enviroments/Environment Textures/white.bmp", 256, 256);
    //texture3 = LoadTexture("Models/Background/Texture Background Galaxy 158x108/Galaxy Back.bmp", 158, 108);
    //texture3 = LoadTexture("Models/Background/Texture Background Sky 1365x768/BG Back.bmp", 158, 108);
    PlaySound(TEXT("Music/music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(mouseMovement);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    FreeTexture(texture);
    FreeTexture(texture1);
    FreeTexture(texture2);
    FreeTexture(texture3);
    return 0;
}
