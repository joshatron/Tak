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
using std::string;


GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    baseWidth = 1600;
    baseHeight = 900;
    width = baseWidth;
    height = baseHeight;
    engine = TakEngine(5, true);
    Move place;
    place.type = "place";
    place.x = 0;
    place.y = 0;
    place.placeType = 0;
    engine.tryMove(place);
    place.x = 1;
    engine.tryMove(place);
    place.x = 2;
    place.placeType = 1;
    engine.tryMove(place);
    place.x = 3;
    place.placeType = 2;
    engine.tryMove(place);
    engine.printBoard();
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

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(2.0f);

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
}

void GLWidget::updatePositions()
{
    points.clear();
    colors.clear();

    bool greyStart = true;
    bool grey = true;
    squareSize = (900 * .73) / engine.boardSize;
    boardOffsetX = (1600 - (squareSize * engine.boardSize)) / 2;
    boardOffsetY = 30;

    //left top rectangle
    points.push_back(vec2(50, 30));
    points.push_back(vec2(400, 30));
    points.push_back(vec2(50, 687));

    points.push_back(vec2(400, 687));
    points.push_back(vec2(400, 30));
    points.push_back(vec2(50, 687));

    //right top rectangle
    points.push_back(vec2(1550, 30));
    points.push_back(vec2(1200, 30));
    points.push_back(vec2(1550, 687));

    points.push_back(vec2(1200, 687));
    points.push_back(vec2(1200, 30));
    points.push_back(vec2(1550, 687));

    points.push_back(vec2(50, 727));
    points.push_back(vec2(1550, 727));
    points.push_back(vec2(50, 870));

    points.push_back(vec2(1550, 870));
    points.push_back(vec2(1550, 727));
    points.push_back(vec2(50, 870));

    for(int k = 0; k < 18; k++)
    {
        colors.push_back(vec3(.7, .7, .7));
    }

    for(int k = 0; k < engine.boardSize; k++)
    {
        for(int a = 0; a < engine.boardSize; a++)
        {
            //background
            points.push_back(vec2(boardOffsetX + (a) * squareSize, boardOffsetY + (k) * squareSize));
            points.push_back(vec2(boardOffsetX + (a + 1) * squareSize, boardOffsetY + (k) * squareSize));
            points.push_back(vec2(boardOffsetX + (a) * squareSize, boardOffsetY + (k + 1) * squareSize));

            points.push_back(vec2(boardOffsetX + (a) * squareSize, boardOffsetY + (k + 1) * squareSize));
            points.push_back(vec2(boardOffsetX + (a + 1) * squareSize, boardOffsetY + (k) * squareSize));
            points.push_back(vec2(boardOffsetX + (a + 1) * squareSize, boardOffsetY + (k + 1) * squareSize));

            for(int t = 0; t < 6; t++)
            {
                if(grey)
                {
                    colors.push_back(vec3(.7, .7, .7));
                }
                else
                {
                    colors.push_back(vec3(1,1,1));
                }
            }

            //pieces
            string pieces = engine.stringAtSpot(a, k);
            if(pieces.length() > 0)
            {
                double height = squareSize * .9;
                int num = pieces.length();
                double width = squareSize * .9;
                char top = pieces.at(pieces.length() - 1);
                if(top == 's' || top == 'S' || top == 'c' || top == 'C')
                {
                    height = squareSize * .7;
                    num--;
                    if(top == 's' || top == 'S')
                    {
                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .45), boardOffsetY + k * squareSize + (squareSize * .05)));
                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .55), boardOffsetY + k * squareSize + (squareSize * .05)));
                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .45), boardOffsetY + k * squareSize + (squareSize * .25)));

                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .55), boardOffsetY + k * squareSize + (squareSize * .25)));
                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .55), boardOffsetY + k * squareSize + (squareSize * .05)));
                        points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .45), boardOffsetY + k * squareSize + (squareSize * .25)));
                        for(int t = 0; t < 6; t++)
                        {
                            if(top == 's')
                            {
                                colors.push_back(vec3(1,1,1));
                            }
                            else
                            {
                                colors.push_back(vec3(0,0,0));
                            }
                        }
                    }
                    else
                    {
                        double currentX = boardOffsetX + a * squareSize + (squareSize * .5);
                        double currentY = boardOffsetY + k * squareSize + (squareSize * .15);
                        double radius = squareSize * .2;
                        for(int k = 0; k < engine.whiteCapstonesLeft(); k++)
                        {
                            for(int a = 0; a < 20; a++)
                            {
                                vec2 first = vec2(cos((M_PI / 3) * a + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * a + (M_PI / 3)) * radius + currentY);
                                vec2 second = vec2(cos((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentY);

                                points.push_back(vec2(currentX, currentY));
                                points.push_back(first);
                                points.push_back(second);

                                for(int k = 0; k < 3; k++)
                                {
                                    if(top == 'c')
                                    {
                                        colors.push_back(vec3(1,1,1));
                                    }
                                    else
                                    {
                                        colors.push_back(vec3(0,0,0));
                                    }
                                }
                            }

                            currentY += 60;
                        }
                    }
                }

                for(int i = 0; i < num; i++)
                {
                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .05), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i));
                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .95), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i));
                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .05), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i - height / num));

                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .95), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i - height / num));
                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .95), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i));
                    points.push_back(vec2(boardOffsetX + a * squareSize + (squareSize * .05), boardOffsetY + (k + 1) * squareSize - (squareSize * .05) * i - height / num));
                    for(int t = 0; t < 6; t++)
                    {
                        if(pieces.at(i) == 'f')
                        {
                            colors.push_back(vec3(1,1,1));
                        }
                        else
                        {
                            colors.push_back(vec3(0,0,0));
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
        points.push_back(vec2(currentX, currentY));
        points.push_back(vec2(currentX + 55, currentY));
        points.push_back(vec2(currentX, currentY + 20));

        points.push_back(vec2(currentX + 55, currentY + 20));
        points.push_back(vec2(currentX + 55, currentY));
        points.push_back(vec2(currentX, currentY + 20));

        for(int k = 0; k < 6; k++)
        {
            colors.push_back(vec3(1, 1, 1));
        }

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
        points.push_back(vec2(currentX, currentY));
        points.push_back(vec2(currentX + 55, currentY));
        points.push_back(vec2(currentX, currentY + 20));

        points.push_back(vec2(currentX + 55, currentY + 20));
        points.push_back(vec2(currentX + 55, currentY));
        points.push_back(vec2(currentX, currentY + 20));

        for(int k = 0; k < 6; k++)
        {
            colors.push_back(vec3(0, 0, 0));
        }

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
        for(int a = 0; a < 20; a++)
        {
            vec2 first = vec2(cos((M_PI / 3) * a + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * a + (M_PI / 3)) * radius + currentY);
            vec2 second = vec2(cos((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentY);

            points.push_back(vec2(currentX, currentY));
            points.push_back(first);
            points.push_back(second);

            for(int k = 0; k < 3; k++)
            {
                colors.push_back(vec3(1, 1, 1));
            }
        }

        currentY += 60;
    }

    //black capstone pieces
    currentX = 1300;
    currentY = 75;

    for(int k = 0; k < engine.blackCapstonesLeft(); k++)
    {
        for(int a = 0; a < 6; a++)
        {
            vec2 first = vec2(cos((M_PI / 3) * a + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * a + (M_PI / 3)) * radius + currentY);
            vec2 second = vec2(cos((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentX, sin((M_PI / 3) * (a + 1) + (M_PI / 3)) * radius + currentY);

            points.push_back(vec2(currentX, currentY));
            points.push_back(first);
            points.push_back(second);

            for(int k = 0; k < 3; k++)
            {
                colors.push_back(vec3(0, 0, 0));
            }
        }

        currentY += 60;
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

