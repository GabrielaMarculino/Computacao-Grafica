//g++ -o main main.cpp -lglut -lGL -lGLU -lm
//LUCAS HEIN e GABRIELA MARCULINO

#include <cstdlib>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>

#include <GL/glut.h>

using namespace std;

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define TEXT 4
#define  HEXAGON 5
#define NUMBERPRIMITIVES 5

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
int primeiroX;
int primeiroY;

//VARIAVEL QUE CONTROLA O TAMANHO DA GRID
int gridSizeControl = 0;

//COORDENADAS RASTREADAS DO MOUSE
int trackedX, trackedY;

/*VARIAVEL QUE INDICA SE OS DESENHOS SERAO ARAMADOS OU PREENCHIDOS*/
//TRUE : PREENCHIDO
//FALSE : ARAMADO
static bool currentFill = false;

/*VARIAVEL QUE INDICA SE OS DESENHOS SERAO TRACEJADOS OU CONTINUOS*/
//0xFFFF = CONTINUO
//0x00FF = TRACEJADO
static int currentPattern = 0xFFFF;

/*BANCO COM AS CORES DISPONIVEIS*/
static float COLORS[][3] = {0.0, 0.0, 0.0}; //BLACK
                       

/*COR ATUAL QUE FOI SELECIONADA*/
static float *currentColor = COLORS[0];

//TEXT TEMPORARIO PARA A PRIMITIVA TEXT QUE APARECE NO CANVA
vector<string> tempText(1);

// Point class.
class Point {
public:
    Point(int xVal, int yVal, float *cor) {
        x = xVal;
        y = yVal;
        this->cor = cor;
    }

    void drawPoint(); // Function to draw a point.
private:
    int x, y; // X and Y co-ordinates of point.
    static float size; // Size of point.
    float *cor;// COR DO PONTO
};

float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint() {
    glPointSize(size);
    glColor3fv(this->cor);
    glBegin(GL_POINTS);
    glVertex3f((float) x, (float) y, 0.0);
    glEnd();
}

// Vector of points.
vector<Point> points;

// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints() {
    // Loop through the points array drawing each point.
    pointsIterator = points.begin();
    while (pointsIterator != points.end()) {
        pointsIterator->drawPoint();
        pointsIterator++;
    }
}

// Line class.
class Line {
public:
    Line(int x1Val, int y1Val, int x2Val, int y2Val, float *cor, int pattern) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
        this->cor = cor;
        this->pattern = pattern;
    }

    void drawLine();

private:
    int x1, y1, x2, y2; // X and Y co-ordinates of endpoints.
    float *cor; //COR DA LINHA
    int pattern;//PADRAO: TRACEJADO / CONTINUA
};


// Function to draw a line.
void Line::drawLine() {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, this->pattern);
    glBegin(GL_LINES);
    glColor3fv(this->cor);
    glVertex3f((float) x1, (float) y1, 0.0);
    glVertex3f((float) x2, (float) y2, 0.0);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

// Vector of lines.
vector<Line> lines;

// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator;

// Function to draw all lines in the lines array.
void drawLines() {
    // Loop through the lines array drawing each line.
    linesIterator = lines.begin();
    while (linesIterator != lines.end()) {
        linesIterator->drawLine();
        linesIterator++;
    }
}

// Rectangle class.
class Rectangle {
public:
    Rectangle(int x1Val, int y1Val, int x2Val, int y2Val, float *cor, bool preenchido, int pattern) {
        x1 = x1Val;
        y1 = y1Val;
        x2 = x2Val;
        y2 = y2Val;
        this->cor = cor;
        mode = preenchido ? GL_FILL : GL_LINE;
        this->pattern = pattern;
    }

    void drawRectangle();

private:
    int x1, y1, x2, y2; // X and Y co-ordinates of diagonally opposite vertices.
    float *cor; //COR DO RETANGULO
    int mode; //MODO: PREENCHIDO / ARAMADO
    int pattern;//PADRAO: TRACEJADO / CONTINUO
};

// Function to draw a rectangle.
void Rectangle::drawRectangle() {
    glPolygonMode(GL_FRONT_AND_BACK, this->mode);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, this->pattern);
    glColor3fv(this->cor);
    glRectf((float) x1, (float) y1, (float) x2, (float) y2);
    glDisable(GL_LINE_STIPPLE);
}

// Vector of rectangles.
vector<Rectangle> rectangles;

// Iterator to traverse a Rectangle array.
vector<Rectangle>::iterator rectanglesIterator;

// Function to draw all rectangles in the rectangles array.
void drawRectangles() {
    // Loop through the rectangles array drawing each rectangle.
    rectanglesIterator = rectangles.begin();
    while (rectanglesIterator != rectangles.end()) {
        rectanglesIterator->drawRectangle();
        rectanglesIterator++;
    }
}

// Routine to draw a bitmap character str.
void writeBitmapString(void *font, const char *string) {
    const char *c;
    for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

class Text {
public:
    Text(float X, float Y, vector<string> str, float *cor) {
        this->X = X;
        this->Y = Y;
        this->cor = cor;
        this->str = std::move(str);
    }

    void drawText();

private:
    float X, Y;
    float *cor;
    vector<string> str;
};

void Text::drawText() {

    glColor3fv(this->cor);
    for (int i = 0; i < str.size(); i++) {
        glRasterPos2f(this->X, (this->Y + 18.f) - (18.f * (float)(i + 1)));
        writeBitmapString(GLUT_BITMAP_HELVETICA_18, str[i].c_str());
    }
}

// Vector of texts.
vector<Text> texts;

// Iterator to traverse a Text array.
vector<Text>::iterator textsIterator;

// Function to draw all texts in the texts array.
void drawTexts() {
    // Loop through the texts array drawing each text.
    textsIterator = texts.begin();
    while (textsIterator != texts.end()) {
        textsIterator->drawText();
        textsIterator++;
    }
}
//DISTANCIA ENTRE DOIS PONTOS
float distance(float x1, float y1, float x2, float y2) {
    float dist;
    dist = sqrtf(powf((x1 - x2), 2) + powf((y1 - y2), 2));
    return (dist);
}

class Hexagon {
public:
    Hexagon(float d, float x, float y, float *cor, bool preenchido, int pattern) {
        this->X = x;
        this->Y = y;
        R = d;
        this->cor = cor;
        mode = preenchido ? GL_FILL : GL_LINE;
        this->pattern = pattern;
    }

    void drawHexagon();

private:
    float X, Y; // X and Y co-ordinates of center
    float R;
    float *cor; //COR DO RETANGULO
    int mode; //MODO: PREENCHIDO / ARAMADO
    int pattern;//PADRAO: TRACEJADO / CONTINUO
};

// Function to draw a Hexagon.
void Hexagon::drawHexagon() {
    float t = 0.f;
    glPolygonMode(GL_FRONT_AND_BACK, this->mode);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, this->pattern);
    glColor3fv(this->cor);
    glBegin(GL_POLYGON);
    //FORMULA DO CIRCULO SÓ QUE COM 6 VERTICES
    for (int i = 0; i < 6; ++i) {
        glVertex3f(this->X + 13 * cos(t), this->Y + R * sin(t), 0.0);
        t += 2 * M_PI / 6;
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}

// Vector of hexagons.
vector<Hexagon> hexagons;

// Iterator to traverse a hexagon array.
vector<Hexagon>::iterator hexagonsIterator;

// Function to draw all hexagons in the hexagons array.
void drawHexagons() {
    // Loop through the hexagons array drawing each hexagon.
    hexagonsIterator = hexagons.begin();
    while (hexagonsIterator != hexagons.end()) {
        hexagonsIterator->drawHexagon();
        hexagonsIterator++;
    }
}


// Function to draw line selection box in left selection area.
void drawLineSelectionBox() {
    if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.8f * (float) height, 0.1f * (float) width, 0.9f * (float) height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.8f * (float) height, 0.1f * (float) width, 0.9f * (float) height);

    // Draw line icon.
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0.025f * (float) width, 0.825f * (float) height, 0.0);
    glVertex3f(0.075f * (float) width, 0.875f * (float) height, 0.0);
    glEnd();
}


/*DESENHA A CAIXA DO HEXAGONO*/
void drawHexagonSelectionBox() {
    if (primitive == HEXAGON) glColor3f(1.0, 1.0, 1.0); // Highlight.
    else glColor3f(0.8, 0.8, 0.8); // No highlight.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.5f * (float) height, 0.1f * (float) width, 0.6f * (float) height);

    // Draw black boundary.
    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.5f * (float) height, 0.1f * (float) width, 0.6f * (float) height);

    // Draw hexagon icon.
    float R, x = (float) (0.0 + 0.1 * width) / 2, y = (float) ((0.5 * height) + (0.6 * height)) / 2;
    R = distance(x, y, 0.025f * (float) width, 0.525f * (float) height);
    Hexagon h(R, x, y, COLORS[0], false, 0xFFFF);
    h.drawHexagon();
    glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea() {
    glColor3f(0.6, 0.6, 0.6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glRectf(0.0, 0.0, 0.1f * (float) width, (1 - NUMBERPRIMITIVES * 0.1f) * (float) height);

    glColor3f(0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glRectf(0.0, 0.0, 0.1f * (float) width, (1 - NUMBERPRIMITIVES * 0.1f) * (float) height);

}

// Function to draw temporary point.
void drawTempPoint() {
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(pointSize);
    glBegin(GL_POINTS);
    glVertex3f((float) tempX, (float) tempY, 0.0);
    glEnd();
}

// Function to draw a grid.
void drawGrid() {
    int i;

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x5555);
    glColor3f(0.75, 0.75, 0.75);

    glBegin(GL_LINES);
    switch (gridSizeControl) {
        case -2:
            for (i = 5; i <= 49; i++) {
                glVertex3f((float) i * 0.02f * (float) width, 0.0, 0.0);
                glVertex3f((float) i * 0.02f * (float) width, (float) height, 0.0);
            }

            for (i = 1; i <= 49; i++) {
                glVertex3f(0.1f * (float) width, (float) i * 0.02f * (float) height, 0.0);
                glVertex3f((float) width, (float) i * 0.02f * (float) height, 0.0);
            }
            break;
        case -1:
            for (i = 2; i <= 24; i++) {
                glVertex3f((float) i * 0.05f * (float) width, 0.0, 0.0);
                glVertex3f((float) i * 0.05f * (float) width, (float) height, 0.0);
            }

            for (i = 1; i <= 24; i++) {
                glVertex3f(0.1f * (float) width, (float) i * 0.05f * (float) height, 0.0);
                glVertex3f((float) width, (float) i * 0.05f * (float) height, 0.0);
            }
            break;
        case 0:
            for (i = 1; i <= 9; i++) {
                glVertex3f((float) i * 0.1f * (float) width, 0.0, 0.0);
                glVertex3f((float) i * 0.1f * (float) width, (float) height, 0.0);
            }

            for (i = 1; i <= 9; i++) {
                glVertex3f(0.1f * (float) width, (float) i * 0.1f * (float) height, 0.0);
                glVertex3f((float) width, (float) i * 0.1f * (float) height, 0.0);
            }
            break;
        case 1:
            for (i = 0; i <= 4; i++) {
                glVertex3f((float) i * 0.2f * (float) width, 0.0, 0.0);
                glVertex3f((float) i * 0.2f * (float) width, (float) height, 0.0);
            }

            for (i = 1; i <= 4; i++) {
                glVertex3f(0.1f * (float) width, (float) i * 0.2f * (float) height, 0.0);
                glVertex3f((float) width, (float) i * 0.2f * (float) height, 0.0);
            }
            break;
        case 2:
            for (i = 1; i <= 3; i++) {
                glVertex3f((float) i * 0.25f * (float) width, 0.0, 0.0);
                glVertex3f((float) i * 0.25f * (float) width, (float) height, 0.0);
            }

            for (i = 1; i <= 3; i++) {
                glVertex3f(0.1f * (float) width, (float) i * 0.25f * (float) height, 0.0);
                glVertex3f((float) width, (float) i * 0.25f * (float) height, 0.0);
            }
            break;
        default:
            break;
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}



// Drawing routine.
void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(0.0, 0.0, 0.0);

    if (isGrid) drawGrid();

    //DESENHA AS PRIMITIVAS
    drawLines();
    drawHexagons();

    if (((primitive == LINE) || (primitive == HEXAGON) || (primitive == TEXT)) &&
        (pointCount == 1)) {
        drawTempPoint();

        if (primitive == LINE) {
            //DESENHAR A LINHA DE FORMA INTERATIVA DE ACORDO COM A LOCALIZAÇÃO DO MOUSE
            Line l(tempX, tempY, trackedX, trackedY, currentColor, currentPattern);
            l.drawLine();
        }  else if (primitive == HEXAGON) {
            //DESENHAR O HEXAGONO DE FORMA INTERATIVA DE ACORDO COM A LOCALIZAÇÃO DO MOUSE
            Hexagon h(distance((float) tempX, (float) tempY, (float) trackedX, (float) trackedY), (float) tempX,
                      (float) tempY, currentColor, currentFill, currentPattern);
            h.drawHexagon();
        }
        glutPostRedisplay();
    }
    
    drawInactiveArea();
    drawLineSelectionBox();
    drawHexagonSelectionBox(); //DESENHA O BOX DE SELEÇÃO DO HEXAGONO

    glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y) {
    if (y < (1 - NUMBERPRIMITIVES * 0.1) * height) primitive = INACTIVE;
    else if (y < (1 - 4 * 0.1) * height) primitive = HEXAGON;
    else if (y < (1 - 1 * 0.1) * height) primitive = LINE;
    else primitive = POINT;
}
/*LIMPA O TEMP TEXT APÓS O SEGUNDO CLICK*/
void clearText(vector<string> &t) {
    //LIMPA A STRING DENTRO DO VECTOR
    for (auto &i : t) {
        i.clear();
    }
    //LIMPA O VECTOR
    t.clear();
    t.emplace_back("");
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y) {
    
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = height - y; // Correct from mouse to OpenGL co-ordinates.

        // Click outside canvas - do nothing.
        if (x < 0 || x > width || y < 0 || y > height);

            // Click in left selection area.
        else if (x < 0.1 * width) {
            pickPrimitive(y);
            pointCount = 0;
        }
            // Click in canvas.
        else {
            if (primitive == POINT) points.emplace_back(x, y, currentColor);
            else if (primitive == LINE) {
                if (pointCount == 0) {
                    primeiroX = x;
                    primeiroY = y;

                    tempX = x;
                    tempY = y;
                    pointCount++;
                } else if(pointCount < 6) {
                    lines.emplace_back(tempX, tempY, x, y, currentColor, currentPattern);
                    tempX = x;
                    tempY = y;
                    

                    if(pointCount == 5){
                       lines.emplace_back(tempX, tempY, primeiroX, primeiroY, currentColor, currentPattern); 
                    }
                    pointCount++;
                    //pointCount = 0;
                }/*else if(pointCount == 5){
                    lines.emplace_back(tempX, tempY, x, y, currentColor, currentPattern);
                    tempX = x;
                    tempY = y;
                    pointCount++;
                    lines.emplace_back(tempX, tempY, primeiroX, primeiroY, currentColor, currentPattern);

                }*/else if(pointCount == 6){
                    
                    pointCount = 0;
                }
            } else if (primitive == HEXAGON) {
                if (pointCount == 0) {
                    
                    tempX = x;
                    tempY = y;
                    pointCount++;
                } else {
                    hexagons.emplace_back(distance((float) tempX, (float) tempY, (float) x, (float) y), (float) tempX,
                                          (float) tempY, currentColor, currentFill, currentPattern);
                    pointCount = 0;
                }
            }
        }
    }
    glutPostRedisplay();
}

// RASTREIA A POSIÇÃO DO MOUSE SE FOR ESCOLHIDA UMA PRIMITIVA
void mousePassiveMotion(int x, int y) {

    if(primitive != INACTIVE) {
        trackedX = x;
        trackedY = height - y;
    }
    glutPostRedisplay();
}

// Initialization routine.
void setup() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set viewing box dimensions equal to window dimensions.
    glOrtho(0.0, (float) w, 0.0, (float) h, -1.0, 1.0);

    // Pass the size of the OpenGL window to globals.
    width = w;
    height = h;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    if (key == 27) {
        exit(0);
    }
    /*INPUT DO TECLADO*/
    if (primitive == TEXT && pointCount == 1) {
        switch (key) {
            case 13: //ENTER
                tempText.emplace_back("");
                break;
            case 8: //BACKSCAPE
                if (!tempText.back().empty()) {
                    tempText.back().pop_back();
                } else {
                    if (tempText.size() > 1) tempText.pop_back();
                }
                break;
            default: //QUALQUER TECLA
                tempText.back().push_back((char) key);
                break;
        }
    }
}

// Clear the canvas and reset for fresh drawing.
void clearAll() {
    lines.clear();
    hexagons.clear();
    primitive = INACTIVE;
    pointCount = 0;
}

// The right button menu callback function.
void rightMenu(int id) {
    if (id == 1) {
        clearAll();
        glutPostRedisplay();
    }
    if (id == 2) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id) {
    if (id == 3) isGrid = 1;
    if (id == 4) isGrid = 0;
    if (isGrid) {
        if (id == 5) gridSizeControl < 2 ? gridSizeControl++ : gridSizeControl;
        if (id == 6) gridSizeControl > -3 ? gridSizeControl-- : gridSizeControl;
    }
    glutPostRedisplay();
}


/*OPCOES ARAMADO OU PREENCHIDO*/
void aramado_preenchido(int id) {
    if (id == 15) currentFill = false;
    if (id == 16)currentFill = true;
}

void tracejado_continuo(int id) {
    if (id == 17) currentPattern = 0x00FF;
    if (id == 18) currentPattern = 0xFFFF;
}

// Function to create menu.
void makeMenu() {
    int sub_menu, color_sub_menu, menu_aramado_preenchido, menu_tracejada;

    menu_tracejada = glutCreateMenu(tracejado_continuo);
    glutAddMenuEntry("Tracejado", 17);
    glutAddMenuEntry("Continuo", 18);

    
    glutCreateMenu(rightMenu);
    glutAddSubMenu("TRACEJADA", menu_tracejada);
    glutAddMenuEntry("Clear", 1);
    glutAddMenuEntry("Quit", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction() {
    cout << "Interaction:" << endl;
    cout << "Left click on a box on the left to select a primitive." << endl
         << "Then left click on the drawing area: once for point, twice for line, rectangle or hexagon." << endl
         << "Right click for menu options." << endl;
}

// Main routine.
int main(int argc, char **argv) {
    printInteraction();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("canvas.cpp");
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutPassiveMotionFunc(mousePassiveMotion);
    glutMouseFunc(mouseControl);

    makeMenu(); // Create menu.

    glutMainLoop();

    return 0;
}