#include "glwidget.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <memory>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QTextStream>
#include <QTimer>

#include "move.h"

#ifndef PI
    #define PI 3.14159265
#endif

using namespace std;
using glm::mat4;
using glm::vec3;
using glm::vec4;
using glm::inverse;
using std::string;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    setMouseTracking(true);
    baseWidth = 1600;
    baseHeight = 900;
    width = baseWidth;
    height = baseHeight;
    bottomBoardState = 0;
    engine = TakEngine(5, true);
    currentMove.x = -1;
    currentMove.y = -1;
}

GLWidget::~GLWidget() {
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    update();
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
    update();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(2.0f);

    initializeLines();

    // Create a new Vertex Array Object on the GPU which
    // saves the attribute layout of our vertices.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &colorBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    program = loadShaders(":/vert.glsl", ":/frag.glsl");

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    projectionLoc = glGetUniformLocation(program, "projection");
}

void GLWidget::initializeLines()
{
    glGenVertexArrays(1, &lineVao);
    glBindVertexArray(lineVao);

    // Create a buffer on the GPU for position data
    glGenBuffers(1, &linePositionBuffer);

    // Upload the position data to the GPU, storing
    // it in the buffer we just allocated.
    glBindBuffer(GL_ARRAY_BUFFER, linePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/line_vert.glsl", ":/line_frag.glsl");
    lineProg = program;

    // Bind the attribute "position" (defined in our vertex shader)
    // to the currently bound buffer object, which contains our
    // position data for a single triangle. This information 
    // is stored in our vertex array object.
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, linePositionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    lineProjectionLoc = glGetUniformLocation(program, "projection");
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0,0,w,h);
    width = w;
    height = h;

    if(((float)w / (float)h) < ((float)baseWidth / (float)baseHeight))
    {
        float newH = baseHeight * (1.f * w / baseWidth);
        float diff = ((float)h - newH) * (baseHeight / newH);
        projection = glm::ortho(0.f, (float)baseWidth, diff / 2 + baseHeight, diff / 2 * -1);
    }
    else
    {
        float newW = baseWidth * (1.f * h / baseHeight);
        float diff = ((float)w - newW) * (baseWidth / newW);
        projection = glm::ortho(diff / 2 * -1, diff / 2 + baseWidth, (float)baseHeight, 0.f);
    }

    glUseProgram(program);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
    glUseProgram(lineProg);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    updatePositions();
    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * points.size(), points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * colors.size(), colors.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, points.size());

    glUseProgram(lineProg);
    glBindBuffer(GL_ARRAY_BUFFER, linePositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * lines.size(), lines.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(lineVao);
    glDrawArrays(GL_LINES, 0, lines.size());
}

void GLWidget::updatePositions()
{
    points.clear();
    lines.clear();
    colors.clear();

    bool greyStart = true;
    bool grey = true;
    squareSize = (900 * .73) / engine.boardSize;
    boardOffsetX = (1600 - (squareSize * engine.boardSize)) / 2;
    boardOffsetY = 30;

    vec3 rectColor(.7,.7,.7);
    //left top rectangle
    addRect(50, 30, 350, 657, rectColor);
    //right top rectangle
    addRect(1200, 30, 350, 657, rectColor);
    //bottom rectangle
    addRect(50, 727, 1500, 143, rectColor);

    vec3 whiteSquare(1,1,1);
    vec3 blackSquare(.7,.7,.7);
    vec3 highlightSquare(.8,.8,1);
    vec3 whitePiece(1,1,1);
    vec3 blackPiece(0,0,0);
    for(int k = 0; k < engine.boardSize; k++)
    {
        for(int a = 0; a < engine.boardSize; a++)
        {
            //background
            if(currentMove.x == a && currentMove.y == k)
            {
                addRect(boardOffsetX + a * squareSize, boardOffsetY + k * squareSize, squareSize, squareSize, highlightSquare);
            }
            else if(grey)
            {
                addRect(boardOffsetX + a * squareSize, boardOffsetY + k * squareSize, squareSize, squareSize, blackSquare);
            }
            else
            {
                addRect(boardOffsetX + a * squareSize, boardOffsetY + k * squareSize, squareSize, squareSize, whiteSquare);
            }

            //pieces
            string pieces = engine.stringAtSpot(a, k);
            if(pieces.length() > 0)
            {
                int num = pieces.length();
                char top = pieces.at(pieces.length() - 1);

                if(num == 1)
                {
                    if(top == 'f')
                    {
                        addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .15,
                                squareSize * .7, squareSize * .7, whitePiece);
                    }
                    else if(top == 'F')
                    {
                        addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .15,
                                squareSize * .7, squareSize * .7, blackPiece);
                    }
                    else if(top == 's')
                    {
                        addRect(boardOffsetX + a * squareSize + squareSize * .4, boardOffsetY + k * squareSize + squareSize * .15,
                                squareSize * .2, squareSize * .7, whitePiece);
                    }
                    else if(top == 'S')
                    {
                        addRect(boardOffsetX + a * squareSize + squareSize * .4, boardOffsetY + k * squareSize + squareSize * .15,
                                squareSize * .2, squareSize * .7, blackPiece);
                    }
                    else if(top == 'c')
                    {
                        addHex(boardOffsetX + a * squareSize + squareSize * .5, boardOffsetY + k * squareSize + squareSize * .5,
                               squareSize * .35, whitePiece);
                    }
                    else if(top == 'C')
                    {
                        addHex(boardOffsetX + a * squareSize + squareSize * .5, boardOffsetY + k * squareSize + squareSize * .5,
                               squareSize * .35, blackPiece);
                    }
                }
                else
                {
                    if(top == 'f' || top == 'F')
                    {
                        double height = .7 * squareSize / num;
                        for(int i = 0; i < num; i++)
                        {
                            if(pieces.at(i) == 'f')
                            {
                                addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .15 + (num - 1 - i) * height,
                                        squareSize * .7, height, whitePiece);
                            }
                            else
                            {
                                addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .15 + (num - 1 - i) * height,
                                        squareSize * .7, height, blackPiece);
                            }
                        }
                    }
                    else
                    {
                        if(top == 's')
                        {
                            addRect(boardOffsetX + a * squareSize + squareSize * .4, boardOffsetY + k * squareSize + squareSize * .15,
                                    squareSize * .2, squareSize * .35, whitePiece);
                        }
                        else if(top == 'S')
                        {
                            addRect(boardOffsetX + a * squareSize + squareSize * .4, boardOffsetY + k * squareSize + squareSize * .15,
                                    squareSize * .2, squareSize * .35, blackPiece);
                        }
                        else if(top == 'c')
                        {
                            addHex(boardOffsetX + a * squareSize + squareSize * .5, boardOffsetY + k * squareSize + squareSize * .325,
                                   squareSize * .2, whitePiece);
                        }
                        else if(top == 'C')
                        {
                            addHex(boardOffsetX + a * squareSize + squareSize * .5, boardOffsetY + k * squareSize + squareSize * .325,
                                   squareSize * .2, blackPiece);
                        }
                        
                        double height = .35 * squareSize / (num - 1);
                        for(int i = 0; i < num - 1; i++)
                        {
                            if(pieces.at(i) == 'f')
                            {
                                addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .5 + (num - 2 - i) * height,
                                        squareSize * .7, height, whitePiece);
                            }
                            else
                            {
                                addRect(boardOffsetX + a * squareSize + squareSize * .15, boardOffsetY + k * squareSize + squareSize * .5 + (num - 2 - i) * height,
                                        squareSize * .7, height, blackPiece);
                            }
                        }
                    }
                }
            }

            grey = !grey;
        }
        greyStart = !greyStart;
        grey = greyStart;
    }

    //white normal pieces
    int currentX = 70;
    int currentY = 50;

    for(int k = 0; k < engine.whitePiecesLeft(); k++)
    {
        addRect(currentX, currentY, 55, 20, whitePiece);

        currentY += 25;
        if(currentY > 655)
        {
            currentX += 75;
            currentY = 50;
        }
    }

    //black normal pieces
    currentX = 1475;
    currentY = 50;

    for(int k = 0; k < engine.blackPiecesLeft(); k++)
    {
        addRect(currentX, currentY, 55, 20, blackPiece);

        currentY += 25;
        if(currentY > 655)
        {
            currentX -= 75;
            currentY = 50;
        }
    }

    //white capstone pieces
    currentX = 300;
    currentY = 75;
    double radius = 20;

    for(int k = 0; k < engine.whiteCapstonesLeft(); k++)
    {
        addHex(currentX, currentY, radius, whitePiece);

        currentY += 60;
    }

    //black capstone pieces
    currentX = 1300;
    currentY = 75;

    for(int k = 0; k < engine.blackCapstonesLeft(); k++)
    {
        addHex(currentX, currentY, radius, blackPiece);

        currentY += 60;
    }

    vec3 highlightColor(0,0,0);
    if(engine.whiteTurn())
    {
        addRect(48, 28, 4, 661, highlightColor);
        addRect(398, 28, 4, 661, highlightColor);
        addRect(48, 28, 354, 4, highlightColor);
        addRect(48, 685, 354, 4, highlightColor);
    }
    else
    {
        addRect(1198, 28, 4, 661, highlightColor);
        addRect(1548, 28, 4, 661, highlightColor);
        addRect(1198, 28, 354, 4, highlightColor);
        addRect(1198, 685, 354, 4, highlightColor);
    }
    
    //addRect(50, 727, 1500, 143, rectColor);
    //bottom board
    if(bottomBoardState == 1)
    {
        vec3 color(0,0,0);
        if(engine.whiteTurn() && !engine.firstTurns() || !engine.whiteTurn() && engine.firstTurns())
        {
            color = vec3(1,1,1);
        }

        vec3 highlightSquare(.8,.8,1);
        if(currentMove.placeType == 0)
        {
            addRect(800-60, 727+11, 120, 120, highlightSquare);
        }
        else
        {
            addRect(800-60, 727+11, 120, 120, color);
        }
        if(currentMove.placeType == 1)
        {
            addRect(800-200-20, 727+11, 40, 120, highlightSquare);
        }
        else
        {
            addRect(800-200-20, 727+11, 40, 120, color);
        }
        if(currentMove.placeType == 2)
        {
            addHex(800+200, 727+71, 70, highlightSquare);
        }
        else
        {
            addHex(800+200, 727+71, 70, color);
        }
    }
    else if(bottomBoardState == 2)
    {
        vec3 color(1,1,1);
        if(currentMove.direction.compare("n") == 0)
        {
            addRect(800-20, 727+11, 40, 40, highlightSquare);
        }
        else
        {
            addRect(800-20, 727+11, 40, 40, color);
        }
        if(currentMove.direction.compare("s") == 0)
        {
            addRect(800-20, 870-11-40, 40, 40, highlightSquare);
        }
        else
        {
            addRect(800-20, 870-11-40, 40, 40, color);
        }
        if(currentMove.direction.compare("w") == 0)
        {
            addRect(800-80, 798-20, 40, 40, highlightSquare);
        }
        else
        {
            addRect(800-80, 798-20, 40, 40, color);
        }
        if(currentMove.direction.compare("e") == 0)
        {
            addRect(800+40, 798-20, 40, 40, highlightSquare);
        }
        else
        {
            addRect(800+40, 798-20, 40, 40, color);
        }
    }
    else if(bottomBoardState == 3)
    {
        vec3 color(1,1,1);
        double height = 100 / stackLeft.length();
        double startY = 738;
        for(int k = 0; k < stackLeft.length(); k++)
        {
            if(currentNum == k + 1)
            {
                color = vec3(.8,.8,1);
            }
            else
            {
                color = vec3(1,1,1);
            }
            addRect(740, startY, 120, height, color);
            startY += height;
        }
        if(currentNum == 0)
        {
            color = vec3(.8,.8,1);
        }
        else
        {
            color = vec3(1,1,1);
        }
        addRect(730, 838, 140, 20, color);
    }
}

void GLWidget::addRect(double topLeftX, double topLeftY, double width, double height, vec3 color)
{
    points.push_back(vec2(topLeftX, topLeftY));
    points.push_back(vec2(topLeftX, topLeftY + height));
    points.push_back(vec2(topLeftX + width, topLeftY));

    points.push_back(vec2(topLeftX + width, topLeftY + height));
    points.push_back(vec2(topLeftX, topLeftY + height));
    points.push_back(vec2(topLeftX + width, topLeftY));

    for(int k = 0; k < 6; k++)
    {
        colors.push_back(color);
    }

    lines.push_back(vec2(topLeftX, topLeftY));
    lines.push_back(vec2(topLeftX, topLeftY + height));
    lines.push_back(vec2(topLeftX, topLeftY + height));
    lines.push_back(vec2(topLeftX + width, topLeftY + height));
    lines.push_back(vec2(topLeftX + width, topLeftY + height));
    lines.push_back(vec2(topLeftX + width, topLeftY));
    lines.push_back(vec2(topLeftX + width, topLeftY));
    lines.push_back(vec2(topLeftX, topLeftY));
}

void GLWidget::addHex(double centerX, double centerY, double radius, vec3 color)
{
    for(int a = 0; a < 6; a++)
    {
        vec2 first = vec2(cos((M_PI / 3) * a + (M_PI / 3)) * radius + centerX, sin((M_PI / 3) * a + (M_PI / 3)) * radius + centerY);
        vec2 second = vec2(cos((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + centerX, sin((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + centerY);

        points.push_back(vec2(centerX, centerY));
        points.push_back(first);
        points.push_back(second);
        lines.push_back(first);
        lines.push_back(second);

        for(int k = 0; k < 3; k++)
        {
            colors.push_back(color);
        }
    }
}

// Copied from LoadShaders.cpp in the the oglpg-8th-edition.zip
// file provided by the OpenGL Programming Guide, 8th edition.
const GLchar* GLWidget::readShader(const char* filename) {
#ifdef WIN32
        FILE* infile;
        fopen_s( &infile, filename, "rb" );
#else
    FILE* infile = fopen( filename, "rb" );
#endif // WIN32

    if ( !infile ) {
#ifdef _DEBUG
        std::cerr << "Unable to open file '" << filename << "'" << std::endl;
#endif /* DEBUG */
        return NULL;
    }

    fseek( infile, 0, SEEK_END );
    int len = ftell( infile );
    fseek( infile, 0, SEEK_SET );

    GLchar* source = new GLchar[len+1];

    fread( source, 1, len, infile );
    fclose( infile );

    source[len] = 0;

    return const_cast<const GLchar*>(source);
}

GLuint GLWidget::loadShaders(const char* vertf, const char* fragf) {
    GLuint program = glCreateProgram();

    // read vertex shader from Qt resource file
    QFile vertFile(vertf);
    vertFile.open(QFile::ReadOnly | QFile::Text);
    QString vertString;
    QTextStream vertStream(&vertFile);
    vertString.append(vertStream.readAll());
    std::string vertSTLString = vertString.toStdString();

    const GLchar* vertSource = vertSTLString.c_str();

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertSource, NULL);
    glCompileShader(vertShader);
    {
        GLint compiled;
        glGetShaderiv( vertShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( vertShader, len, &len, log );
            std::cout << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, vertShader);

    // read fragment shader from Qt resource file
    QFile fragFile(fragf);
    fragFile.open(QFile::ReadOnly | QFile::Text);
    QString fragString;
    QTextStream fragStream(&fragFile);
    fragString.append(fragStream.readAll());
    std::string fragSTLString = fragString.toStdString();

    const GLchar* fragSource = fragSTLString.c_str();

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);
    {
        GLint compiled;
        glGetShaderiv( fragShader, GL_COMPILE_STATUS, &compiled );
        if ( !compiled ) {
            GLsizei len;
            glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetShaderInfoLog( fragShader, len, &len, log );
            std::cerr << "Shader compilation failed: " << log << std::endl;
            delete [] log;
        }
    }
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    {
        GLint linked;
        glGetProgramiv( program, GL_LINK_STATUS, &linked );
        if ( !linked ) {
            GLsizei len;
            glGetProgramiv( program, GL_INFO_LOG_LENGTH, &len );

            GLchar* log = new GLchar[len+1];
            glGetProgramInfoLog( program, len, &len, log );
            std::cout << "Shader linker failed: " << log << std::endl;
            delete [] log;
        }
    }

    return program;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    vec2 pt(event->x(), event->y());
    vec2 rawPt = vec2(inverse(projection) * vec4(pt.x / width * 2 - 1, 1 - pt.y / height * 2, 0, 1));

    if(bottomBoardState == 0)
    {
        if(rawPt.x >= boardOffsetX && rawPt.y >= boardOffsetY &&
           rawPt.x <= boardOffsetX + squareSize * engine.boardSize && rawPt.y <= boardOffsetY + squareSize * engine.boardSize)
        {
            currentMove.x = (rawPt.x - boardOffsetX) / squareSize;
            currentMove.y = (rawPt.y - boardOffsetY) / squareSize;
        }
        else
        {
            currentMove.x = -1;
            currentMove.y = -1;
        }
    }
    else if(bottomBoardState == 1)
    {
        if(rawPt.x >= 540 && rawPt.y >= 727 &&
           rawPt.x <= 1060 && rawPt.y <= 870)
        {
            if(rawPt.x < 670)
            {
                currentMove.placeType = 1;
            }
            else if(rawPt.x < 930)
            {
                currentMove.placeType = 0;
            }
            else
            {
                currentMove.placeType = 2;
            }
        }
        else
        {
            currentMove.placeType = -1;
        }
    }
    else if(bottomBoardState == 2)
    {
        if(rawPt.x >= 700 && rawPt.y >= 727 &&
           rawPt.x <= 900 && rawPt.y <= 870)
        {
            if(rawPt.x < 770)
            {
                currentMove.direction = "w";
            }
            else if(rawPt.x > 830)
            {
                currentMove.direction = "e";
            }
            else if(rawPt.y < 798)
            {
                currentMove.direction = "n";
            }
            else
            {
                currentMove.direction = "s";
            }
        }
        else
        {
            currentMove.direction = "";
        }
    }
    else if(bottomBoardState == 3)
    {
        if(rawPt.x >= 740 && rawPt.x <= 860 &&
           rawPt.y >= 738 && rawPt.y <= 838)
        {
            double height = 100 / stackLeft.length();
            currentNum = (rawPt.y - 738) / height + 1;
        }
        else if(rawPt.x >= 730 && rawPt.x <= 870 &&
                rawPt.y >= 838 && rawPt.y <= 858)
        {
            currentNum = 0;
        }
        else
        {
            currentNum = -1;
        }
    }

    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    vec2 pt(event->x(), event->y());
    vec2 rawPt = vec2(inverse(projection) * vec4(pt.x / width * 2 - 1, 1 - pt.y / height * 2, 0, 1));

    if(rawPt.x >= boardOffsetX && rawPt.y >= boardOffsetY &&
       rawPt.x <= boardOffsetX + squareSize * engine.boardSize && rawPt.y <= boardOffsetY + squareSize * engine.boardSize)
    {
        currentMove.x = (rawPt.x - boardOffsetX) / squareSize;
        currentMove.y = (rawPt.y - boardOffsetY) / squareSize;

        if(engine.numberAtSpot(currentMove.x, currentMove.y) == 0)
        {
            bottomBoardState = 1;
            currentMove.type = "place";
        }
        else
        {
            bottomBoardState = 2;
            currentMove.type = "move";
            currentMove.direction = "";
        }
    }

    if(bottomBoardState == 0)
    {
        if(currentMove.x >= 0 && currentMove.y >= 0)
        {
            if(engine.numberAtSpot(currentMove.x, currentMove.y) == 0)
            {
                bottomBoardState = 1;
                currentMove.type = "place";
            }
            else
            {
                bottomBoardState = 2;
                currentMove.type = "move";
                currentMove.direction = "";
            }
        }
    }
    else if(bottomBoardState == 1)
    {
        if(currentMove.placeType >= 0)
        {
            if(engine.tryMove(currentMove))
            {
                bottomBoardState = 0;
                currentMove.x = -1;
                currentMove.y = -1;
                currentMove.type = "";
                currentMove.placeType = -1;
            }
        }
    }
    else if(bottomBoardState == 2)
    {
        if(currentMove.direction.compare("") != 0)
        {
            bottomBoardState = 3;
            stackLeft = engine.stringAtSpot(currentMove.x, currentMove.y);
            distance = 0;
            currentNum = -1;
            currentMove.toLeave[0] = 0;
        }
    }
    else if(bottomBoardState == 3)
    {
        if(currentNum >= 0)
        {
            if(currentNum != 0)
            {
                currentNum = stackLeft.length() - currentNum + 1;
            }
            currentMove.toLeave[distance] = currentNum;
            distance++;
            stackLeft = stackLeft.substr(currentNum, stackLeft.length() - currentNum);
        }

        if(stackLeft.compare("") == 0)
        {
            currentMove.distance = distance;
            engine.tryMove(currentMove);
            bottomBoardState = 0;
            currentMove.x = -1;
            currentMove.type = "";
            currentMove.placeType = -1;
            currentMove.direction = "";
            currentMove.distance = 0;
        }
    }

    update();
}
