#include "Scenegraph.h"
#include <stack>
#include "TransformNode.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Scenegraph::Scenegraph()
{
    root = NULL;
}

void Scenegraph::makeScenegraph(Node *r)
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }
    this->root = r;
}



Scenegraph::~Scenegraph()
{
    if (root!=NULL)
    {
        delete root;
        root = NULL;
    }

	for (map<string,Texture *>::iterator it=textures.begin();it!=textures.end();it++)
	{
		delete it->second;
	}
}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
   modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
   objectColorLocation = glGetUniformLocation(shaderProgram,"vColor");
}

void Scenegraph::draw(stack<glm::mat4>& modelView)
{
    if (root!=NULL)
    {
        root->draw(modelView);
    }

	if (root!=NULL)
	{
		root->updateBB();
		root->drawBB(modelView);
	}
}

void Scenegraph::animate(float time)
{
}

