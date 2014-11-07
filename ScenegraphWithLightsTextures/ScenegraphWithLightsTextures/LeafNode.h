#ifndef _LEAF_H_
#define _LEAF_H_
#include "node.h"
#include "Object.h"
#include "Material.h"
#include <stack>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "Light.h"


class LeafNode : public Node
{
public:

	LeafNode(Object *instanceOf,Scenegraph *graph,string name="")
		:Node(graph,name)
	{
		this->instanceOf = instanceOf;
		//default material
		
		material.setAmbient(1.0f,0.8f,0.8f);
		material.setDiffuse(0.8f,0.8f,0.8f);
		material.setSpecular(0.8f,0.8f,0.8f);
		material.setShininess(50.0f);
		
		/*
		material.setAmbient(0.1f,0.025f,0.025f);
		material.setDiffuse(0.8f,0.2f,0.2f);
		material.setSpecular(0.8f,0.2f,0.2f);
		material.setShininess(50.0f);
		*/

		texture = NULL;

		//delete tex;
	}

	~LeafNode(void)
	{

	}

	Node *clone()
	{
		LeafNode *newclone = new LeafNode(instanceOf,scenegraph,name);
		newclone->setMaterial(material);

		return newclone;
	}

	virtual void draw(stack<glm::mat4> &modelView)
    {
		GLuint a;
        if (instanceOf!=NULL)
		{
			 //get the color
           // glm::vec4 color = material.getAmbient();
			mv = modelView.top();
            //set the color for all vertices to be drawn for this object
            //glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top()));
			glUniformMatrix4fv(scenegraph->normalMatrixLocation,1,GL_FALSE,glm::value_ptr(glm::transpose(glm::inverse(modelView.top()))));
			
			glUniform3fv(scenegraph->mat_ambientLocation,1,glm::value_ptr(material.getAmbient()));
			glUniform3fv(scenegraph->mat_diffuseLocation,1,glm::value_ptr(material.getDiffuse()));
			glUniform3fv(scenegraph->mat_specularLocation,1,glm::value_ptr(material.getSpecular()));
			glUniform1f(scenegraph->mat_shininessLocation,material.getShininess());

			if(texture != NULL) {
				glUniformMatrix4fv(scenegraph->texturematrixLocation,1,GL_FALSE,glm::value_ptr(glm::mat4(1.0f)));
				glEnable(GL_TEXTURE_2D);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,texture->getTextureID());
				glUniform1i(scenegraph->textureLocation,0);
				scenegraph->textureLocation;
			}
			else {

			}

			a = glGetError();
			instanceOf->draw();        
			a = glGetError();
		}
    }

	virtual void drawBB(stack<glm::mat4>& modelView)
	{
		if (bbDraw)
		{
			glm::mat4 bbTransform;

			bbTransform = glm::translate(glm::mat4(1.0),0.5f*(minBounds+maxBounds)) * glm::scale(glm::mat4(1.0),maxBounds-minBounds);
			glm::vec4 color = glm::vec4(1,1,1,1);
			//set the color for all vertices to be drawn for this object
			glUniform3fv(scenegraph->objectColorLocation,1,glm::value_ptr(color));
			glUniformMatrix4fv(scenegraph->modelviewLocation,1,GL_FALSE,glm::value_ptr(modelView.top() * bbTransform));
			scenegraph->getInstance("box")->draw();        		
		}


	}
	
	virtual void updateBB()
	{
		minBounds = instanceOf->getMinimumWorldBounds().xyz();
		maxBounds = instanceOf->getMaximumWorldBounds().xyz();
	}
	virtual void getLights(vector<Light>& l, stack<glm::mat4>& modelView){
	
		for(int i =0; i<lights.size();i++){
			Light light = lights[i];
			glm::vec4 newPos= lights[i].getPosition();
			light.setPosition(glm::vec3(modelView.top()*lights[i].getPosition()));
			l.push_back(light);
		}
		
	}

	glm::vec4 getColor()
	{
		return material.getAmbient();
	}

	/*
		*Set the material of each vertex in this object
		*/
	virtual void setMaterial(Material& mat)
	{
		material = mat;
	}

	/*
		* gets the material
		*/
	Material getMaterial()
	{
		return material;
	}

	void setTexture(Texture *tex)
	{
		cout << "Texture set to " << tex->getName() << endl;
		texture=tex;
	}

	virtual glm::mat4 getCameraTransform(){
		return glm::mat4(1.0);
	}

	virtual glm::mat4 getTransform(){
		return glm::mat4(1.0);
	}
	
protected:
	Object *instanceOf;
	Texture *texture;
	Material material;
};
#endif
