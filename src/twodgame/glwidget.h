#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include <QtGui/QImage>
#include "takengine.h"
#include <vector>

// glm by default uses degrees, but that functionality
// is deprecated so GLM_FORCE_RADIANS turns off some 
// glm warnings
#define GLM_FORCE_RADIANS

using glm::vec2;
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

    private:
        GLuint loadShaders(const char* vertf, const char* fragf);
        static const GLchar* readShader(const char* filename);
        void updatePositions();

        GLuint vao;
        GLuint program;

        GLuint positionBuffer;

        GLuint projectionBuffer;
        GLint projectionLoc;

        //the projection matrix
        glm::mat4 projection;

        //the width and height
        int baseWidth;
        int baseHeight;

        //the width and height of the window
        int width;
        int height;

        TakEngine engine;

        vector<vec2> whiteRects;
        vector<vec2> blackRects;
        vector<vec2> whiteCircles;
        vector<vec2> blackCircles;
        vector<vec2> boardLines;
        vector<vec2> greyRects;
};

#endif
