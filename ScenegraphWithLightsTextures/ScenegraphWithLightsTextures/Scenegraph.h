#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/gl.h>
#include "Object.h"
#include "Node.h"
#include "Texture.h"
#include "Light.h"

typedef struct
    {
        GLint positionLocation;
        GLint ambientLocation;
        GLint diffuseLocation;
        GLint specularLocation;
    } LightLocation;

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

	float turn;
	float zoom;
	int camNum;
	glm::mat4 trackballTransform;
	Node * cameraNode;

	void addInstance(Object *in)
	{
		instances.push_back(in);
	}

	void addTexture(Texture* tex)
	{
		cout << "Texture " << tex->getName() << " added" << endl;
		textureMap[tex->getName()] = tex;
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
		return textureMap[name];
	}
private:
    Node *root;
	vector<Object *> instances;
	vector<Light> lights;
	std::map<string,Texture *> textureMap;
	GLint objectColorLocation,modelviewLocation,normalMatrixLocation,numLightsLocation,mat_ambientLocation,
		mat_diffuseLocation,mat_specularLocation,mat_shininessLocation,texturematrixLocation,textureLocation;
	void getLights(stack<glm::mat4>& modelView);
	LightLocation lightLocation[3];
	
	GLuint programCopy;
	TransformNode * cabin;
	TransformNode * chair0;
	TransformNode * chair1;
	TransformNode * chair2;
	TransformNode * chair3;

	glm::mat4 camMove1;
	glm::mat4 camMove2;
};

#endif // SCENEGRAPH_H
