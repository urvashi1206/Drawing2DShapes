#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <vector>
#include <functional>
using namespace std;

float canvasSize[] = { 10.0f, 10.0f };
int rasterSize[] = { 700, 500 };

// Shape type
enum class ShapeType { POINT = 1, LINE, TRIANGLE, QUAD, POLYGON };

struct my_shape {
    std::vector<vector<float>> verts;
    ShapeType curr_shape;
    float color[3];
    float line_width;
    float point_size;
};

// Current drawing state
ShapeType currentShapeType;
//ShapeType currentShapeType = ShapeType::POINT;
std::vector<vector<float>> currentPoints;
vector<my_shape> shapes;

float color[3];
float currentPointSize;
float currentLineWidth;
float mousePos[2];

void init(void)
{
    //Initialize mouse position
    mousePos[0] = mousePos[1] = 0.0f;
    //Initialize color to black
    color[0] = color[1] = color[2] = 0.0f;
    currentLineWidth = 1.0f;
    currentPointSize = 1.0f;
}

void drawCursor()
{
    glColor3f(color[0], color[1], color[2]);
    glPointSize(currentPointSize);
    glBegin(GL_POINTS);
    glVertex2fv(mousePos);
    glEnd();
    glPointSize(1.0f);
}

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Iterate over the my_shapes vector
    for (auto& iter_shapes : shapes) {
        switch (iter_shapes.curr_shape)
        {
        case ShapeType::POINT:
            glPointSize(iter_shapes.point_size);
            glBegin(GL_POINTS);
            glColor3f(iter_shapes.color[0], iter_shapes.color[1], iter_shapes.color[2]);
            for (auto& point : iter_shapes.verts)
            {
                glVertex2f(point[0], point[1]);
            }
            glEnd();
            break;
        case ShapeType::LINE:
            glLineWidth(iter_shapes.line_width);
            glBegin(GL_LINE_STRIP);
            //glColor3f(color[0], color[1], color[2]);
            glColor3f(iter_shapes.color[0], iter_shapes.color[1], iter_shapes.color[2]);
            for (auto& point : iter_shapes.verts)
            {
                glVertex2f(point[0], point[1]);;
            }
            //glVertex2fv(mousePos);
            glEnd();
            break;
        case ShapeType::TRIANGLE:
            glBegin(GL_TRIANGLES);
            glColor3f(iter_shapes.color[0], iter_shapes.color[1], iter_shapes.color[2]);
            for (auto& point : iter_shapes.verts)
            {
                glVertex2f(point[0], point[1]);
            }
            //glVertex2fv(mousePos);
            glEnd();
            break;
        case ShapeType::QUAD:
            glBegin(GL_QUADS);
            glColor3f(iter_shapes.color[0], iter_shapes.color[1], iter_shapes.color[2]);
            glVertex2f(iter_shapes.verts[0][0], iter_shapes.verts[0][1]);
            glVertex2f(iter_shapes.verts[1][0], iter_shapes.verts[0][1]);
            glVertex2f(iter_shapes.verts[1][0], iter_shapes.verts[1][1]);
            glVertex2f(iter_shapes.verts[0][0], iter_shapes.verts[1][1]);

            //glVertex2fv(mousePos);
            glEnd();

            break;
        case ShapeType::POLYGON:
            glBegin(GL_TRIANGLES);
            glColor3f(iter_shapes.color[0], iter_shapes.color[1], iter_shapes.color[2]);
            for (int i = 0; i < iter_shapes.verts.size() - 2; i++) {
                glVertex2f(iter_shapes.verts[i][0], iter_shapes.verts[i][1]);
                glVertex2f(iter_shapes.verts[i + 1][0], iter_shapes.verts[i + 1][1]);
                glVertex2f(iter_shapes.verts[i + 2][0], iter_shapes.verts[i + 2][1]);
            }
            /*int last_index = iter_shapes.verts.size() - 1;
            glVertex2f(iter_shapes.verts[last_index - 1][0], iter_shapes.verts[last_index - 1][1]);
            glVertex2f(iter_shapes.verts[last_index][0], iter_shapes.verts[last_index][1]);
            glVertex2f(mousePos[0], mousePos[1]);*/
            glEnd();
            break;
        default:
            break;
        }
    }

    // Drawing for current incomplete shape
    switch (currentShapeType)
    {
    case ShapeType::POINT:
        glPointSize(currentPointSize);
        glBegin(GL_POINTS);
        glColor3f(color[0], color[1], color[2]);
        for (auto& point : currentPoints)
        {
            glVertex2f(point[0], point[1]);
        }
        glEnd();
        break;
    case ShapeType::LINE:
        if (currentPoints.size() == 1) {
            glLineWidth(currentLineWidth);
            glBegin(GL_LINE_STRIP);
            glColor3f(color[0], color[1], color[2]);
            for (auto& point : currentPoints)
            {
                glVertex2f(point[0], point[1]);
            }
            glVertex2fv(mousePos);
            glEnd();
        }
        break;
    case ShapeType::TRIANGLE:
        if (currentPoints.size() >= 1 && currentPoints.size() < 3)
        {
            glLineWidth(1.0f);
            glBegin(GL_LINE_STRIP);
            glColor3f(color[0], color[1], color[2]);
            for (auto& point : currentPoints)
            {
                glVertex2f(point[0], point[1]);
            }
            glVertex2fv(mousePos);
            glEnd();
        }
        else if (currentPoints.size() == 3)
        {
            glBegin(GL_TRIANGLES);
            glColor3f(color[0], color[1], color[2]);
            for (auto& point : currentPoints)
            {
                glVertex2f(point[0], point[1]);
            }
            glVertex2fv(mousePos);
            glEnd();
        }
        break;
    case ShapeType::QUAD:
        //Save current line width in atemop variable
        //Set linewidth to small
        if (currentPoints.size() == 1) {
            glLineWidth(1.0f);
            glBegin(GL_LINE_STRIP);
            glColor3f(color[0], color[1], color[2]);
            //int minx = min(currentPoints[0])
            glVertex2f(currentPoints[0][0], currentPoints[0][1]);
            glVertex2f(mousePos[0], currentPoints[0][1]);
            glVertex2f(mousePos[0], mousePos[1]);
            glVertex2f(currentPoints[0][0], mousePos[1]);
            glVertex2f(currentPoints[0][0], currentPoints[0][1]);
            //glVertex2fv(mousePos);
            glEnd();
        }
        break;
    case ShapeType::POLYGON:
        if (currentPoints.size() == 1) {
            glLineWidth(1.0f);
            glBegin(GL_LINE_STRIP);
            glColor3f(color[0], color[1], color[2]);
            //int minx = min(currentPoints[0])
            glVertex2f(currentPoints[0][0], currentPoints[0][1]);
            glVertex2f(mousePos[0], mousePos[1]);
            glEnd();
        }
        if (currentPoints.size() > 1) {
            glBegin(GL_TRIANGLES);
            glColor3f(color[0], color[1], color[2]);
            for (int i = 0; i < currentPoints.size() - 2; i++) {
                glVertex2f(currentPoints[i][0], currentPoints[i][1]);
                glVertex2f(currentPoints[i + 1][0], currentPoints[i + 1][1]);
                glVertex2f(currentPoints[i + 2][0], currentPoints[i + 2][1]);
            }
            int last_index = currentPoints.size() - 1;
            glVertex2f(currentPoints[last_index - 1][0], currentPoints[last_index - 1][1]);
            glVertex2f(currentPoints[last_index][0], currentPoints[last_index][1]);
            glVertex2f(mousePos[0], mousePos[1]);
            glEnd();
        }
        break;
    default:
        break;
    }

    drawCursor();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    rasterSize[0] = w;
    rasterSize[1] = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, canvasSize[0], 0.0, canvasSize[1]);
    glViewport(0, 0, rasterSize[0], rasterSize[1]);

    glutPostRedisplay();
}

bool make_new_shape() {
    if (currentPoints.size() == 1 && currentShapeType == ShapeType::POINT) {
        return true;
    }
    if (currentPoints.size() == 3 && currentShapeType == ShapeType::TRIANGLE) {
        return true;
    }
    if (currentPoints.size() == 2 && currentShapeType == ShapeType::QUAD) {
        return true;
    }
    if (currentPoints.size() == 2 && currentShapeType == ShapeType::LINE) {
        return true;
    }
    /*if (currentPoints.size() > 3 && currentShapeType == ShapeType::POLYGON) {
        return true;
    }*/
    return false;
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
        mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

        vector<float> temp = { mousePos[0], mousePos[1] };
        currentPoints.push_back(temp);
        if (make_new_shape()) {
            my_shape new_shape;
            new_shape.verts = currentPoints;
            new_shape.curr_shape = currentShapeType;
            new_shape.color[0] = color[0];
            new_shape.color[1] = color[1];
            new_shape.color[2] = color[2];
            new_shape.point_size = currentPointSize;
            new_shape.line_width = currentLineWidth;
            shapes.push_back(new_shape);
            currentPoints.clear();
            if (currentShapeType == ShapeType::LINE) {
                currentPoints.push_back(temp);
            }
        }
        if ((currentShapeType == ShapeType::LINE && currentPoints.size() == 2) ||
            (currentShapeType == ShapeType::TRIANGLE && currentPoints.size() == 3) ||
            (currentShapeType == ShapeType::QUAD && currentPoints.size() == 4) ||
            (currentShapeType == ShapeType::POLYGON && currentPoints.size() > 3)) {
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y)
{
    // mouse events are handled by OS, eventually. When using mouse in the raster window, it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
    mousePos[0] = (float)x / rasterSize[0] * canvasSize[0];
    mousePos[1] = (float)(rasterSize[1] - y) / rasterSize[1] * canvasSize[1];

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 13:
        // ENTER KEY
        if (currentShapeType == ShapeType::LINE || currentShapeType == ShapeType::POLYGON) {
            my_shape new_shape;
            new_shape.verts = currentPoints;
            new_shape.curr_shape = currentShapeType;
            new_shape.line_width = currentLineWidth;
            new_shape.color[0] = color[0];
            new_shape.color[1] = color[1];
            new_shape.color[2] = color[2];
            shapes.push_back(new_shape);
            currentPoints.clear();
            /*if (currentShapeType == ShapeType::LINE) {
                currentPoints.push_back(temp);
            }*/
        }
        break;
    }
}

void menu(int value)
{
    switch (value) {
    case 0: // clear
        //numOfVertices = 0;
        for (int i = 0; i < shapes.size(); i++)
        {
            shapes[i].verts.clear();
        }
        shapes.clear();
        glutPostRedisplay();
        break;
    case 1: //exit
        exit(0);
    case 2: // red
        color[0] = 1.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        //glutPostRedisplay();
        break;
    case 3: // green
        color[0] = 0.0f;
        color[1] = 1.0f;
        color[2] = 0.0f;
        //glutPostRedisplay();
        break;
    case 4: // blue
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 1.0f;
        //glutPostRedisplay();
        break;
    case 5: //black
        color[0] = 0.0f;
        color[1] = 0.0f;
        color[2] = 0.0f;
        //glutPostRedisplay();
        break;
    case 6: //point
        //numOfVertices = 0;
        //MaxVertices = 1;
        currentShapeType = ShapeType::POINT;
        currentPoints.clear();
        //glutPostRedisplay();
        break;
    case 7: //line
        //numOfVertices = 0;
        //MaxVertices = 2;
        //glutPostRedisplay();
        currentShapeType = ShapeType::LINE;
        currentPoints.clear();
        break;
    case 8: //triangle
        //numOfVertices = 0;
        //MaxVertices = 3;
        //glutPostRedisplay();
        currentShapeType = ShapeType::TRIANGLE;
        currentPoints.clear();
        break;
    case 9: //quad
        //numOfVertices = 0;
        //MaxVertices = 4;
        //glutPostRedisplay();
        currentShapeType = ShapeType::QUAD;
        currentPoints.clear();
        break;
    case 10: //polygon
        //numOfVertices = 0;
        //MaxVertices = 5;
        //glutPostRedisplay();
        currentShapeType = ShapeType::POLYGON;
        currentPoints.clear();
        break;
    case 11: //small
        currentPointSize = 1.0f;
        //glPointSize(0.0f);
        //glutPostRedisplay();
        break;
    case 12: //medium
        currentPointSize = 4.0f;
        //glPointSize(3.0f);
        //glutPostRedisplay();
        break;
    case 13: //large
        currentPointSize = 8.0f;
        //glPointSize(6.0f);
        //glutPostRedisplay();
        break;
    case 14: //small
        currentLineWidth = 1.0f;
        //glLineWidth(0.0f);
        //glutPostRedisplay();
        break;
    case 15: //medium
        currentLineWidth = 4.0f;
        //glLineWidth(3.0f);
        //glutPostRedisplay();
        break;
    case 16: //large
        currentLineWidth = 8.0f;
        //glLineWidth(6.0f);
        //glutPostRedisplay();
        break;
    default:
        break;
    }
}
void createMenu()
{
    int objectMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Point", 6);
    glutAddMenuEntry("Line", 7);
    glutAddMenuEntry("Triangle", 8);
    glutAddMenuEntry("Quad", 9);
    glutAddMenuEntry("Polygon", 10);

    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 2);
    glutAddMenuEntry("Green", 3);
    glutAddMenuEntry("Blue", 4);
    glutAddMenuEntry("Black", 5);

    int sizeMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 11);
    glutAddMenuEntry("Medium", 12);
    glutAddMenuEntry("Large", 13);

    int lineWidth = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 14);
    glutAddMenuEntry("Medium", 15);
    glutAddMenuEntry("Large", 16);

    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddSubMenu("Objects", objectMenu);
    glutAddSubMenu("Colors", colorMenu);
    glutAddSubMenu("Point Size", sizeMenu);
    glutAddSubMenu("Line Width", lineWidth);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(rasterSize[0], rasterSize[1]);
    glutCreateWindow("Drawing 2D Shapes");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    createMenu();
    glutMainLoop();
    return 0;


}