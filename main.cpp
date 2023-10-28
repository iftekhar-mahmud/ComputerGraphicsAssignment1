#include<windows.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1416

struct Point
{
    float x,y;
};

int boardLength = 160;
int boardLeft = 20;
int boardRight = 180;
int boardBottom = 5;
int boardTop = 165;

struct Point centerE1 = {100, 162}; //Enemy1 Center Coordinate
struct Point centerE2 = {22, 162}; //Enemy2 Center Coordinate
struct Point centerA1 = {40, 70}; //Agent1 Center Coordinate
struct Point centerA2 = {30, 40}; //Agent2 Center Coordinate
float radiusE1 = 5; //Enemy1 Radius
float radiusE2 = 5; //Enemy2 Radius
float radiusA1 = 3; //Agent1 Radius
float radiusA2 = 3; //Agent2 Radius
float velocity = 0.02;

bool isAgentDestroyed = false;
bool isGameFinished = false;

int score = 0; //Game score
int enemy1State = 0; //Enemy state 0,1,2,3
int enemy2State = 0;
void drawCirlePerimeter(float cx, float cy, float radius, int slices)
{
    int i;
    float angle, x, y;
    glBegin(GL_LINE_LOOP);
    {
        for(i=0; i<slices; i++)
        {
            angle = ((2 * PI)/slices) * i;
            x = cx + radius * cos(angle);
            y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void drawCircleWithPolygon(float cx, float cy, float radius, int slices)
{
    int i;
    float angle, x, y;
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = ((2 * PI)/slices) * i;
            x = cx + radius * cos(angle);
            y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void drawCircleWithTriangles(float cx, float cy, float radius, int slices)
{
    int i;
    float angle1, angle2;

    glBegin(GL_TRIANGLES);
    {
        for(i=0; i<slices; i++)
        {
            angle1 = ((2*PI)/slices) * i;
            angle2 = ((2*PI)/slices) * (i+1);

            glVertex2f(cx, cy);
            glVertex2f(cx + radius * cos(angle1), cy + radius * sin(angle1));
            glVertex2f(cx + radius * cos(angle2), cy + radius * sin(angle2));
        }
    }
    glEnd();
}

void gameOverScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1, 1, 1);
    char scoreText[50];
    sprintf(scoreText, "Game Over! Your Score: %d", score);


    int textX = 50;
    int textY = 100;


    glRasterPos2i(textX, textY);
    for (int i = 0; i < strlen(scoreText); i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, scoreText[i]);
    }

    glFlush();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_LINES);
    {
        glColor3f(1,1,1);
        glVertex2d(-25,0);
        glVertex2d(25, 0);

        glVertex2d(0,-25);
        glVertex2d(0,25);
    }
    glEnd();

    //Border
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2d(20,165);
        glVertex2d(180,165);
        glVertex2d(180,5);
        glVertex2d(20,5);
    }
    glEnd();

    //Enemy
    glColor3f(1,0,0);
    glPushMatrix();
    {
        drawCircleWithPolygon(centerE1.x, centerE1.y, radiusE1, 40);
    }
    glPopMatrix();

    glColor3f(1,0,1);
    glPushMatrix();
    {
        drawCircleWithPolygon(centerE2.x, centerE2.y, radiusE2, 40);
    }
    glPopMatrix();

    //Agents
    if(!isAgentDestroyed)
    {
        glColor3f(0,1,0);
        glPushMatrix();
        {
            drawCircleWithPolygon(centerA1.x, centerA1.y, radiusA1, 40);
        }
        glPopMatrix();
    }
    if(!isAgentDestroyed)
    {
        glColor3f(0,1,1);
        glPushMatrix();
        {
            drawCircleWithPolygon(centerA2.x, centerA2.y, radiusA2, 40);
        }
        glPopMatrix();
    }

    glFlush();
}

void animate()
{
    if(!isGameFinished)
    {
        score++;


        float factor;

        if(enemy1State == 0)
        {
            factor = 1;

            centerE1.y -= velocity;

            if(centerE1.x<boardBottom-radiusE1 ||centerE1.y<(boardBottom+radiusE1) )
            {
                enemy1State = 1;
            }
        }
        else if(enemy1State == 1)
        {
            factor = 1;
            centerE1.x += velocity;
            centerE1.y += factor*velocity;

            if(centerE1.x>boardRight-radiusE1)
            {
                enemy1State = 2;
            }
        }
        else if(enemy1State == 2)
        {
            factor = 1;
            centerE1.x -= velocity;


            if(centerE1.x < (boardLeft+radiusE1))
            {
                enemy1State = 3;
            }
        }
         else if(enemy1State == 3)
        {
            factor = 1;
            centerE1.x += velocity;
            centerE1.y += factor *velocity;

            if(centerE1.x>(boardLeft+boardRight)/2 || centerE1.y>(boardTop-radiusE1))
            {
                enemy1State = 0;
            }
        }

        if(enemy2State == 0)
        {
            factor = 1;
            centerE2.x += velocity;
            centerE2.y -= factor*velocity;

            if(centerE2.x>boardRight-radiusE2)
            {
                enemy2State = 1;
            }
        }
         else if(enemy2State == 1)
        {
            factor = 1;
            centerE2.x -= velocity;
            centerE2.y += factor*0.04;

            if(centerE2.x<(boardLeft+boardRight)/2 || centerE2.y>boardTop-radiusE2)
            {
                enemy2State = 2;
            }
        }
        else if(enemy2State == 2)
        {
            factor = 1;
            centerE2.x += velocity;
            centerE2.y -= factor*velocity;

            if(centerE2.x<(boardLeft+boardRight)/2 || centerE2.y<(boardBottom+boardTop)/2)
            {
                enemy2State = 3;
            }
        }
        else if(enemy2State == 3)
        {
            factor = 1;
            centerE2.x -= velocity;
            centerE2.y += factor*0.01;

            if(centerE2.x<boardLeft-radiusE2 || centerE2.y>boardTop-radiusE2)
            {
                enemy2State = 0;
            }
        }
        double distA1E1 = sqrt(pow((centerE1.x-centerA1.x),2) + pow((centerE1.y-centerA1.y),2));
        double distA1E2 = sqrt(pow((centerE2.x-centerA1.x),2) + pow((centerE2.y-centerA1.y),2));
        double distA2E1 = sqrt(pow((centerE1.x-centerA2.x),2) + pow((centerE1.y-centerA2.y),2));
        double distA2E2 = sqrt(pow((centerE2.x-centerA2.x),2) + pow((centerE2.y-centerA2.y),2));
        if(distA1E1 <= radiusA1+radiusE1)
        {
            isAgentDestroyed = true;
            isGameFinished = true;
        }
        if(distA1E2 <= radiusA1+radiusE2)
        {
            isAgentDestroyed = true;
            isGameFinished = true;
        }
        if(distA2E1 <= radiusA2+radiusE1)
        {
            isAgentDestroyed = true;
            isGameFinished = true;
        }
        if(distA2E2 <= radiusA2+radiusE2)
        {
            isAgentDestroyed = true;
            isGameFinished = true;
        }
    }
    /*else
    {
        printf("Score : %d\n", score);
        exit(0);
    }*/
    if (isGameFinished) {
        glutDisplayFunc(gameOverScreen); // Switch to game over screen
    }
    else {
        glutPostRedisplay(); // Continue with the game screen
    }

    //glutPostRedisplay();
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glOrtho(-2.0, 200.0, -2.0, 200.0, -25.0, 25.0);
}

void keyboard_action(unsigned char key, int x, int y)
{
    if(!isGameFinished)
    {
        switch (key)
        {
            case 'u':
                if (centerA2.y < boardTop - radiusA2)
                {
                    centerA2.y += 1;
                }
                break;
            case 'd':
                if (centerA2.y > boardBottom + radiusA2)
                {
                    centerA2.y -= 1;
                }
                break;
            case 'l':
                if (centerA2.x > boardLeft + radiusA2)
                {
                    centerA2.x -= 1;
                }
                break;
            case 'r':
                if (centerA2.x < boardRight - radiusA2)
                {
                    centerA2.x += 1;
                }
                break;
        }
    }


    glutPostRedisplay();
}

void special_action(int key, int x, int y)
{
    if(!isGameFinished)
    {
           switch (key) {
            case GLUT_KEY_LEFT:
                if (centerA1.x > boardLeft + radiusA1) {
                    centerA1.x -= 1;
                }
                break;
            case GLUT_KEY_RIGHT:
                if (centerA1.x < boardRight - radiusA1) {
                    centerA1.x += 1;
                }
                break;
            case GLUT_KEY_UP:
                if (centerA1.y < boardTop - radiusA1) {
                    centerA1.y += 1;
                }
                break;
            case GLUT_KEY_DOWN:
                if (centerA1.y > boardBottom + radiusA1) {
                    centerA1.y -= 1;
                }
                break;
        }
    }

    glutPostRedisplay();
}

int main()
{
    //Rasterization
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("2D Objects");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard_action);
    glutSpecialFunc(special_action);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
