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
	trackballTransform = glm::mat4(1.0);

	camNum = 0;
	zoom = 0.0;
	turn = 0.0;
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

	
	texturematrixLocation = glGetUniformLocation(shaderProgram,"texturematrix");
	textureLocation = glGetUniformLocation(shaderProgram,"image");

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

		camMove1 = glm::scale(glm::mat4(1.0f), glm::vec3(zoom+1, zoom+1, zoom+1)) * glm::rotate(glm::mat4(1.0f),glm::radians(turn*1.0f),glm::vec3(1,0,0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
		camMove2 = glm::scale(glm::mat4(1.0f), glm::vec3(zoom+1, zoom+1, zoom+1)) * glm::rotate(glm::mat4(1.0f),glm::radians(turn*1.0f),glm::vec3(1,0,0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));

		if(camNum == 0) {
		modelView.top() = modelView.top() * glm::lookAt(glm::vec3(0,150,0),glm::vec3(0,0,0),glm::vec3(1,0,0)) * camMove1 * trackballTransform;
		}
		else if(camNum == 1) {
			modelView.pop();
			modelView.push(glm::mat4(1.0));
			glm::mat4 cameraTransform(1.0);
			glm::mat4 objTransform(1.0);
			if(this->cameraNode != NULL){
				Node *parent = cameraNode->getParent();
				while(parent != NULL){
					cameraTransform = parent->getCameraTransform() * cameraTransform;
					objTransform = parent->getTransform() * objTransform;
					parent = parent->getParent();
				}
			}

            float x = glm::distance(cameraTransform*glm::vec4(0,0,0,1), cameraTransform*glm::vec4(1,0,0,1));
            float y = glm::distance(cameraTransform*glm::vec4(0,0,0,1), cameraTransform*glm::vec4(0,1,0,1));
            float z = glm::distance(cameraTransform*glm::vec4(0,0,0,1), cameraTransform*glm::vec4(0,0,1,1));
			glm::vec4 boundMatrix = glm::vec4(cameraNode->getMaxBounds(),0) * glm::scale(glm::mat4(1.0), glm::vec3(x,y,z));
		
			// glm::scale(glm::mat4(1.0), glm::inverse(glm::vec3(x,y,z)))
			modelView.top() = modelView.top() * glm::lookAt(glm::vec3(boundMatrix.x,boundMatrix.y,0),glm::vec3(0,0,0),glm::vec3(0,1,0)) * camMove2 *
				trackballTransform * glm::scale(glm::mat4(1.0), glm::vec3(x,y,z))
				*glm::inverse(cameraTransform);
		}
		
		getLights(modelView);

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
		
		
		
		//glUseProgram(programCopy);
	}
}

void Scenegraph::animate(float time)
{
	time = time*8;
	if(cabin != NULL) {
               cabin->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(140,75,0)) * glm::rotate(glm::mat4(1.0),cos(time),glm::vec3(0,0,1)) * glm::translate(glm::mat4(1.0),glm::vec3(-100,-75,0)));
       }

       if(chair0 != NULL) {
               chair0->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
               chair1->setAnimationTransform(glm::translate(glm::mat4(1.0),glm::vec3(cos(time*5)*10,0,0)));
               chair2->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
               chair3->setAnimationTransform(glm::rotate(glm::mat4(1.0),time,glm::vec3(0,1,0)));
       }

	TransformNode *	pole1 = static_cast<TransformNode*>(root->getNode("pole1"));
	if(pole1!=NULL){
		pole1->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8.0f,glm::vec3(0,1,0)));
	}

	
	TransformNode *	pole2 = static_cast<TransformNode*>(root->getNode("pole2"));
	if(pole2!=NULL){
		pole2->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*-8.0f,glm::vec3(0,1,0)));
	}

	TransformNode *	pole3 = static_cast<TransformNode*>(root->getNode("pole3"));
	if(pole3!=NULL){
		pole3->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8.0f,glm::vec3(0,1,0)));
	}

	TransformNode *	pole4 = static_cast<TransformNode*>(root->getNode("pole4"));
	if(pole4!=NULL){
		pole4->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*-8.0f,glm::vec3(0,1,0)));
	}
	
	TransformNode *	spinner = static_cast<TransformNode*>(root->getNode("spinner"));
	if(spinner!=NULL){
		spinner->setAnimationTransform(glm::rotate(glm::mat4(1.0f),time*8,glm::vec3(0,1,0)));
	}
}

void Scenegraph::getLights(stack<glm::mat4>& modelView){
	lights.clear();
	root->getLights(lights, modelView);
}

