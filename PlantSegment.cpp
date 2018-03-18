//
//  PlantSegment.cpp
//  HW4
//
//  Created by Meeta Marathe on 3/18/18.
//  Copyright © 2018 Meeta Marathe. All rights reserved.
//

#include "PlantSegment.h"
#include "Window.h"

PlantSegment::PlantSegment(GLuint shaderProgram, std::vector<glm::vec3> p, glm::vec3 lineColor)
{
    shader = shaderProgram;
    points = p;
    color = lineColor;
    
    toWorld = glm::mat4(1.0f);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PlantSegment::draw()
{
    glm::mat4 modelview = Window::V * toWorld;
    uProjection = glGetUniformLocation(shader, "projection");
    uModel = glGetUniformLocation(shader, "model");
    uView = glGetUniformLocation(shader, "view");
    uColor = glGetUniformLocation(shader, "color");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
    glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
    glUniform3f(uColor, color.x, color.y, color.z);
    
    
    // Add fog
    //glClearColor(0.5, 0.5, 0.5, 1);//gray color, same as fog color
    //glClearDepth(1);
    //glEnable(GL_DEPTH_TEST);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());
    
    //Unbind
    glBindVertexArray(0);
}

void PlantSegment::scale(glm::mat4 scaleMatrix)
{
    toWorld = scaleMatrix * toWorld;
}

