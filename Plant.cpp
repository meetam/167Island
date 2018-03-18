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
#include "PlantSegment.h"
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
    
    else if (type == "bush")
        rule = makeBushRule(iterations);
    
    else if (type == "vine")
        rule = makeVineRule(iterations);
    
    //cout << rule << endl;
    
    makePoints(); //Populate line segments
}

string Plant::makeFernRule(int numRules)
{
    if (numRules <= 0)
    {
        return "";
    }
    
    else
    {
        string rule = addF(numRules) + "[-" + makeFernRule(numRules - 1) + "][" + makeFernRule(numRules - 1) + "]" + addF(numRules) + "[-" + makeFernRule(numRules - 1) + "]+" + addF(numRules) + makeFernRule(numRules - 1);
        return rule;
    }
}

string Plant::makeBushRule(int numRules)
{
    if (numRules <= 0)
    {
        return "";
    }
    
    else
    {
        string rule = makeBushRule(numRules - 1) + makeBushRule(numRules - 1) + "FF-[-" + makeBushRule(numRules - 1) + "F+" + makeBushRule(numRules - 1) + "F+" + makeBushRule(numRules - 1) + "F]+[+" + makeBushRule(numRules - 1) + "F-" + makeBushRule(numRules - 1) + "F-" + makeBushRule(numRules - 1) + "F]";
        return rule;
    }
}

string Plant::makeVineRule(int numRules)
{
    if (numRules <= 0)
    {
        return "";
    }
    
    else
    {
        string rule = makeVineRule(numRules - 1) + "F[+" + makeVineRule(numRules - 1) + "F]" + makeVineRule(numRules - 1) + "F[-" + makeVineRule(numRules - 1) + "F]" + makeVineRule(numRules - 1) + "F";
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
            PlantSegment plantSegment (shader, lineSegment, color);
            plantSegments.push_back(plantSegment);
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
    for (int i = 0; i < plantSegments.size(); i++)
    {
        plantSegments[i].draw();
    }
}

void Plant::scale(glm::mat4 scaleMatrix)
{
    for (int i = 0; i < plantSegments.size(); i++)
    {
        plantSegments[i].scale(scaleMatrix);
    }
}
