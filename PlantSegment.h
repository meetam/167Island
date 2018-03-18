//
//  PlantSegment.hpp
//  HW4
//
//  Created by Meeta Marathe on 3/18/18.
//  Copyright © 2018 Meeta Marathe. All rights reserved.
//

#ifndef PlantSegment_h
#define PlantSegment_h

#include <stdio.h>

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
#include <stdio.h>
#include <vector>
#include <iostream>

class PlantSegment
{
private:
    std::vector<glm::vec3> points;
    GLuint VAO, VBO;
    GLuint uProjection, uView, uModel, uColor;
    GLuint shader;
    glm::mat4 toWorld;
    glm::vec3 color;
    
public:
    PlantSegment(GLuint shaderProgram, std::vector<glm::vec3> p, glm::vec3 lineColor);
    void draw();
    void scale(glm::mat4 scaleMatrix);
};

#endif /* PlantSegment_h */
