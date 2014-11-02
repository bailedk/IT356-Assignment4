#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <map>
#include <stack>
#include <vector>
#include <iostream>
using namespace std;
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Object.h"
#include "Node.h"
#include "Texture.h"

class Scenegraph
{    
    friend class LeafNode;
	friend class GroupNode;
	friend class TransformNode;
public:
    Scenegraph();
    ~Scenegraph();
    void makeScenegraph(Node *root);
    void initShaderProgram(GLint shaderProgram);
    void draw(stack<glm::mat4>& modelView);

	void addInstance(Object *in)
	{
		instances.push_back(in);
	}

	void addTexture(Texture *tex)
	{
		cout << "Texture " << tex->getName() << " added" << endl;
	}

	void animate(float t);

	Object *getInstance(string name)
	{
		int i;

		i = 0;
		while ((i<instances.size()) && (instances[i]->getName()!=name))
			i++;

		if (i<instances.size())
		{
			return instances[i];
		}
		else
		{
			return NULL;
		}
	}

	Texture *getTexture(string name)
	{
		cout << "Texture " << name << " looked up" << endl;
	}

private:
    Node *root;
	vector<Object *> instances;
	GLint objectColorLocation,modelviewLocation;
};

#endif // SCENEGRAPH_H
