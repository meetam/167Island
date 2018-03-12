//
//  Plant.hpp
//  HW4
//
//  Created by Meeta Marathe on 3/11/18.
//  Copyright © 2018 Meeta Marathe. All rights reserved.
//

#ifndef Plant_h
#define Plant_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <stack>
#include <iostream>
using namespace std;

class Plant
{
private:
    GLuint VAO, VBO;
    GLuint uProjection, uModel, uView, uColor;
    GLuint shader;
    string rule, type;
    vector<vector<glm::vec3>> lineSegments;
    glm::vec3 position, color;
    float startAngle, angleDelta, drawSize;
    int iterations;
    
public:
    Plant(string pType, GLuint shaderProgram, glm::vec3 pPosition, glm::vec3 pColor, float pStartAngle, float pAngleDelta, float pDrawSize, int pIterations);
    void draw();
    void drawLines(vector<glm::vec3> lineSegment);
    string makeFernRule(int numRules);
    string addF(int numRules);
    void makePoints();
};

#endif /* Plant_h */
