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

	cabin = static_cast<TransformNode*>(root->getNode("cabin"));
	chair0 = static_cast<TransformNode*>(root->getNode("chair0"));
	chair1 = static_cast<TransformNode*>(root->getNode("chair1"));
	chair2 = static_cast<TransformNode*>(root->getNode("chair2"));
	chair3 = static_cast<TransformNode*>(root->getNode("chair3"));
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
   /* if (root!=NULL)
    {
        root->draw(modelView);
    }
	*/
	if (root!=NULL)
	{
		//root->updateBB();
		//root->drawBB(modelView);
		
		
		getLights(modelView);
		//cout<<"Lights Size: "<<lights.size()<<endl;
		//cout << "program : " << programCopy << endl;

		//cout << "position light 0" << endl;
		//glm::vec4 pos = lights[0].getPosition();
		//cout << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;
				
		

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
		}


		glUniform1i(numLightsLocation,lights.size());

		for (int i=0;i<lights.size();i++){
			glUniform3fv(lightLocation[i].ambientLocation,1,glm::value_ptr(lights[i].getAmbient()));
			glUniform3fv(lightLocation[i].diffuseLocation,1,glm::value_ptr(lights[i].getDiffuse()));
			glUniform3fv(lightLocation[i].specularLocation,1,glm::value_ptr(lights[i].getSpecular()));
			glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(lights[i].getPosition()));

			
			glm::vec4 pos = lights[i].getPosition();
			//pos[0] = 50;
			//pos[1] = 75;
			//pos[2] = 0;
			cout << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;
			//glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(pos));
			
		}
			/*
			glm::vec4 pos = lights[0].getPosition();
			pos[0] = -100;
			pos[1] = 0;
			pos[2] = 0;
			cout << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;
			*/
			//glUniform4fv(lightLocation[i].positionLocation,1,glm::value_ptr(pos));

		if (root!=NULL)
		{
			root->draw(modelView);
		}

		if (root!=NULL)
		{
			root->updateBB();
			root->drawBB(modelView);
		}
		
		
		glUseProgram(programCopy);
	}
}

void Scenegraph::animate(float time)
{
	time = time*2;
	if(cabin != NULL) {
		//cabin->setAnimationTransform(glm::rotate(glm::mat4(1.0),time*0.4f,glm::vec3(0,1,0)));

		//cabin->setAnimationTransform(glm::rotate(glm::mat4(1.0),cos(time),glm::vec3(0,0,1)) * glm::translate(glm::mat4(1.0),glm::vec3(0,-75,0)));
		//cabin->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(cos(time*5)*50,0,0)) * glm::translate(glm::mat4(1.0),glm::vec3(0,-75,0)));
	}

	if(chair0 != NULL) {
		//chair1->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)) * chair1->getTransform());
		//chair1->setAnimationTransform(glm::rotate(glm::mat4(1.0),cos(time),glm::vec3(0,1,0)) * chair1->getTransform());

		
		//chair0->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
		chair0->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(cos(time*5)*50,0,0)));
		//chair1->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
		//chair2->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
		//chair3->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
		
	}

}

void Scenegraph::getLights(stack<glm::mat4>& modelView){
	lights.clear();
	root->getLights(lights, modelView);
}

