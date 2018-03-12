//
//  Plant.cpp
//  HW4
//
//  Created by Meeta Marathe on 3/11/18.
//  Copyright © 2018 Meeta Marathe. All rights reserved.
//

#include "Plant.h"
#include "Window.h"
#include "Math.h"
using namespace std;

Plant::Plant(string pType, GLuint shaderProgram, glm::vec3 pPosition, glm::vec3 pColor, float pStartAngle, float pAngleDelta, float pDrawSize, int pIterations)
{
    type = pType;
    shader = shaderProgram;
    position = pPosition;
    color = pColor;
    startAngle = pStartAngle;
    angleDelta = pAngleDelta;
    drawSize = pDrawSize;
    iterations = pIterations;
    
    if (type == "fern")
        rule = makeFernRule(iterations);
    
    //cout << rule << endl;
    
    makePoints(); //Populate points
}

string Plant::makeFernRule(int numRules)
{
    if (numRules <= 0)
    {
        return "";
    }
    
    else
    {
        string rule;
        rule += addF(numRules);
        rule += "[-";
        rule += makeFernRule(numRules - 1);
        rule += "][";
        rule += makeFernRule(numRules - 1);
        rule += "]";
        rule += addF(numRules);
        rule += "[-";
        rule += makeFernRule(numRules - 1);
        rule += "]+";
        rule += addF(numRules);
        rule += makeFernRule(numRules - 1);
        return rule;
    }
        
}

string Plant::addF(int numRules)
{
    string f = "F";
    for (int i = 0; i < numRules - 1; i++)
    {
        f += "F";
    }
    return f;
}

void Plant::makePoints()
{
    stack<pair<glm::vec3, float>> s;
    
    float angle = startAngle;
    glm::vec3 pos = position;
    vector<glm::vec3> lineSegment;
    lineSegment.push_back(pos);
    
    for (int i = 0; i < rule.length() - 1; i++)
    {
        if (rule[i] == 'F')
        {
            float xPos = pos.x + drawSize * cos(glm::radians(angle));
            float yPos = pos.y + drawSize * sin(glm::radians(angle));
            pos.x = xPos;
            pos.y = yPos;
            lineSegment.push_back(pos);
        }
        
        else if (rule[i] == '-')
        {
            angle += angleDelta;
        }
        
        else if (rule[i] == '+')
        {
            angle -= angleDelta;
        }
        
        else if (rule[i] == '[')
        {
            s.push(pair<glm::vec3, float>(pos, angle)); //Save position and angle
        }
        
        else if (rule[i] == ']')
        {
            //End of line segment
            lineSegments.push_back(lineSegment);
            lineSegment.clear();
            
            //Revert to earlier position and angle
            pos = s.top().first;
            lineSegment.push_back(pos);
            angle = s.top().second;
            s.pop();
        }
    }
}

void Plant::draw()
{
    for (int i = 0; i < lineSegments.size(); i++)
    {
        drawLines(lineSegments[i]);
    }
}

void Plant::drawLines(vector<glm::vec3> lineSegment)
{
    //Set up VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lineSegment.size() * sizeof(glm::vec3), &lineSegment[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    //Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glm::mat4 toWorld (1.0f);
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
    
    //Draw line
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_STRIP, 0, lineSegment.size());
    
    //Unbind
    glBindVertexArray(0);
}
