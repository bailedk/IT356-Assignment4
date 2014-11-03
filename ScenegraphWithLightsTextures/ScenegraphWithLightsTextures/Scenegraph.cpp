#include "Scenegraph.h"
#include <stack>

#include "TransformNode.h"
using namespace std;
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <sstream> 
#include <iostream>

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
	// UNCOMMENT WHEN READY
	/*
	for (map<string,Texture *>::iterator it=textures.begin();it!=textures.end();it++)
	{
		delete it->second;
	}
	*/
}

void Scenegraph::initShaderProgram(GLint shaderProgram)
{
	// hacky way to get this
	programCopy = shaderProgram;

	modelviewLocation = glGetUniformLocation(shaderProgram,"modelview");
	objectColorLocation = glGetUniformLocation(shaderProgram,"vColor");
	
   
	normalMatrixLocation = glGetUniformLocation(shaderProgram,"normalmatrix");
	numLightsLocation = glGetUniformLocation(shaderProgram,"numLights");


	mat_ambientLocation = glGetUniformLocation(shaderProgram,"material.ambient");
	mat_diffuseLocation = glGetUniformLocation(shaderProgram,"material.diffuse");
	mat_specularLocation = glGetUniformLocation(shaderProgram,"material.specular");
	mat_shininessLocation = glGetUniformLocation(shaderProgram,"material.shininess");
	
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
		getLights(modelView);
		//cout<<"Lights Size: "<<lights.size()<<endl;
		//cout << "program : " << programCopy << endl;
		for (int i=0;i<lights.size();i++)
		{
			stringstream name;

			name << "light[" << i << "].ambient";

			lightLocation[i].ambientLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].diffuse";

			lightLocation[i].diffuseLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].specular";

			lightLocation[i].specularLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			name << "light[" << i << "].position";

			lightLocation[i].positionLocation = glGetUniformLocation(programCopy,name.str().c_str());

			name.clear();//clear any bits set
			name.str(std::string());

			glUniform3fv(lightLocation[i].ambientLocation,1,glm::value_ptr(lights[i].getAmbient()));
			glUniform3fv(lightLocation[i].diffuseLocation,1,glm::value_ptr(lights[i].getDiffuse()));
			glUniform3fv(lightLocation[i].specularLocation,1,glm::value_ptr(lights[i].getSpecular()));
			glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(lights[i].getPosition()));

		}
		


	}
}

void Scenegraph::animate(float time)
{
}

void Scenegraph::getLights(stack<glm::mat4>& modelView){
	lights.clear();
	root->getLights(lights, modelView);
}

