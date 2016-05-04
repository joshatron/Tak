#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include <QtGui/QImage>
#include "takengine.h"
#include "move.h"
#include <vector>

// glm by default uses degrees, but that functionality
// is deprecated so GLM_FORCE_RADIANS turns off some 
// glm warnings
#define GLM_FORCE_RADIANS

using glm::vec2;
using glm::vec3;
using std::vector;

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

    private:
        GLuint loadShaders(const char* vertf, const char* fragf);
        static const GLchar* readShader(const char* filename);
        void initializeLines();
        void updatePositions();

        void addRect(double topLeftX, double topLeftY, double width, double height, vec3 color);
        void addHex(double centerX, double centerY, double radius, vec3 color);

        GLuint vao;
        GLuint program;

        GLuint positionBuffer;
        GLuint colorBuffer;

        GLuint projectionBuffer;
        GLint projectionLoc;

        GLuint lineVao;
        GLuint lineProg;
        GLuint linePositionBuffer;
        GLint lineProjectionLoc;

        //the projection matrix
        glm::mat4 projection;

        //the width and height
        int baseWidth;
        int baseHeight;

        //the width and height of the window
        int width;
        int height;

        TakEngine engine;

        //drawn with GL_TRIANGLES
        vector<vec2> points;
        vector<vec2> lines;
        vector<vec3> colors;

        double squareSize;
        double boardOffsetX;
        double boardOffsetY;

        //0- blank
        //1- choose place
        //2- choose move
        int bottomBoardState;

        Move currentMove;
};

#endif
