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
using namespace std;

int POS_X, POS_Y;
GLfloat light_pos[] = {-10.0f, 10.0f, 100.00f, 1.0f};

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
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 }; /* Red diffuse light. */
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; /* Infinite light location. */

const int MAP_SIZE = 30;
int gameMap[MAP_SIZE][MAP_SIZE] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 2, 0, 0, 1, 0, 0, 2, 0, 1, 0, 2, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, 0, 1, 0, 2, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
float xpos = 15.0 * 5.0, ypos = 0, zpos = 10.0, xrot = 0, yrot = 0, angle = 0.0;
float lastx, lasty;
float positionx[100], positiony[100], positionz[100], buildX[12], buildZ[12];
int score = 0;
double rotate_x = 0, rotate_y = 0;
GLuint texture, texture1, texture2; //the array for our texture

Model model, building, player;
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
bool checkCollision(float cubeX, float cubeY, float cubeZ, float cubeSize)
{
    // Assuming player size is 1x1x1
    if (xpos + 0.5 > cubeX - cubeSize / 2 && xpos - 0.5 < cubeX + cubeSize / 2 &&
        ypos + 0.5 > cubeY - cubeSize / 2 && ypos - 0.5 < cubeY + cubeSize / 2 &&
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
        playerY + 0.5 > buildingY - buildingSizeY / 2 &&
        playerY - 0.5 < buildingY + buildingSizeY / 2 &&
        playerZ + 0.5 > buildingZ - buildingSizeZ / 2 &&
        playerZ - 0.5 < buildingZ + buildingSizeZ / 2)
    {
        return true; // Collision detected with the building
    }
    return false; // No collision with the building
}
void updatePlayerPosition(float deltaX, float deltaY, float deltaZ)
{
    float newX = xpos + deltaX;
    float newY = ypos + deltaY;
    float newZ = zpos + deltaZ;
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
        if (checkBuildingCollision(newX, newY, newZ, buildX[i], 0.0, buildZ[i], 25.0, 0.0, 15.0))
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
}
void cube(void)
{
    for (int i = 0; i < 100; i++)
    {
        if (!checkCollision(positionx[i], 0.0, positionz[i], 1.0))
        {
            glPushMatrix();
            //glTranslated(positionx[i], 0.0, positionz[i]);
            glColor3f(1.0, 1.0, 0.0);
            model.draw(positionx[i], 0.0, positionz[i], 20.0, angle + 90.0, 90.0, 90.0);//square(); 
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
void drawBuilding(float x, float y, float z, float scale, float angleX, float angleY, float angleZ)
{
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    building.draw(x, y, z, scale, angleX, angleY, angleZ);
    glPopMatrix();
}
void drawPlayer(void)
{
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.2f);
    //player.draw(xpos, ypos, zpos, 1.0, 180.0, yrot, 0.0);
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
            if (gameMap[i][j] == 0 || gameMap[i][j] == 0)
            {
                glBindTexture(GL_TEXTURE_2D, texture);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, texture1);  // Path texture
            }

            ground();  // Draw the terrain
            glBindTexture(GL_TEXTURE_2D, texture2);
            glPopMatrix();
            if (gameMap[i][j] == 2)
            {
                drawBuilding(xTranslation, -1.0, zTranslation, 1, 0.0, 90.0, 0.0);
            }
        }
    }
}
void renderScore(void)
{
    glPushMatrix();
    glColor3f(10.0, 10.0, 10.0);
    glRasterPos3f(xpos, 1.25, zpos); // Adjust position as needed
    std::string scoreText = "Score: " + std::to_string(score);
    for (char c : scoreText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glPopMatrix();
}
void camera(void)
{
    //glRotatef(xrot, 1.0, 0.0, 0.0); // rotate our camera on the x-axis (left and right)
    //glRotatef(yrot, 0.0, 1.0, 0.0); // rotate our camera on the y-axis (up and down)
    //glTranslated(-xpos, -ypos, -zpos);
    // Calculate the position of the camera in a third-person view
    float cameraDistance = 3.0; // adjust this distance as needed
    float cameraHeight = 1.0;   // adjust this height as needed

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
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
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
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, 1.0, 1.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    //model.load("Models/MarioKart/mk_kart.obj");
    model.load("Models/Coins/Bells.obj");
    building.load("Models/Building/building.obj");
    player.load("Models/MarioKart/mk_kart.obj");

    pos_x = model.pos_x;
    pos_y = model.pos_y;
    pos_z = model.pos_z - 1.0f;

    zoom_per_scroll = -model.pos_z / 10.0f;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(pos_x, pos_y, pos_z);
    glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);
    //camera();
    drawMap();
    //drawPlayer();
    cube();
    renderScore();
    model.draw();
    //building.draw();

    player.draw();

    glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y)
{
    float moveSpeed = 0.5;
    float xrotrad;
    float yrotrad;
    if (key == 'w')
    {
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        updatePlayerPosition(float(sin(yrotrad)) * moveSpeed, 0.0, float(cos(yrotrad)) * moveSpeed);
        //xpos += float(sin(yrotrad)) * moveSpeed;
        //zpos += float(cos(yrotrad)) * moveSpeed;
        //ypos -= float(sin(xrotrad)) * moveSpeed;
    }
    if (key == 's')
    {
        yrotrad = (yrot / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        updatePlayerPosition(-float(sin(yrotrad)) * moveSpeed, 0.0, -float(cos(yrotrad)) * moveSpeed);
        //xpos -= float(sin(yrotrad)) * moveSpeed;
        //zpos -= float(cos(yrotrad)) * moveSpeed;
        //ypos += float(sin(xrotrad)) * moveSpeed;
    }
    if (key == 'd')
    {
        yrotrad = ((yrot + 90) / 180 * 3.141592654f);
        xrotrad = (xrot / 180 * 3.141592654f);
        updatePlayerPosition(-float(sin(yrotrad)) * moveSpeed, 0.0, -float(cos(yrotrad)) * moveSpeed);
        //xpos -= float(sin(yrotrad)) * moveSpeed;
        //zpos -= float(cos(yrotrad)) * moveSpeed;
    }
    if (key == 'a')
    {
        yrotrad = ((yrot - 90) / 180 * 3.141592654f);
        updatePlayerPosition(-float(sin(yrotrad)) * moveSpeed, 0.0, -float(cos(yrotrad)) * moveSpeed);
        //xpos -= float(sin(yrotrad)) * moveSpeed;
        //zpos -= float(cos(yrotrad)) * moveSpeed;
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
        display();
    }
    if (key == 'k') {
        ly -= 10.0; // move the light down
        cout << "Pressed 's': Move light down" << endl;
        display();
    }
    if (key == 'j') {
        lx -= 10.0; // move the light left
        cout << "Pressed 'a': Move light left" << endl;
        display();
    }
    if (key == 'l') {
        lx += 10.0; // move the light right
        cout << "Pressed 'd': Move light right" << endl;
        display();
    }
    glutPostRedisplay();
}
void timer(int value) {
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
        } else
            is_holding_mouse = false;
    } else if (state == GLUT_UP) {
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

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glutSetOption(GLUT_MULTISAMPLE, 8);
    //POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    //POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    //glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Load Model");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    texture = LoadTexture("grass.bmp", 256, 256);
    texture1 = LoadTexture("raod.bmp", 256, 256);
    texture2 = LoadTexture("white.bmp", 256, 256);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(mouseMovement);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
