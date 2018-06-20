#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <set>
#include <memory>
#include "Material.h"
#include "Common.h"
#include "resource.h"

class Scene;

//Any object in the world. Goes with all the functions necessary to set coordinates, 
//rotation...
namespace tetraRender
{
	class GameObject : public Resource
	{
	public:
		GameObject();
		~GameObject();
		virtual void setPos(glm::vec3 pos);
		void setScale(glm::vec3 scale);
		void setRotation(float rotation, glm::vec3 rotationAngle);
		void addTag(RenderTag tag);
		void addChild(GameObject* child);
		void setParent(GameObject* parent);
		std::set<RenderTag> getRenderTags();
		glm::vec3 getPos();
		glm::vec3 getScale();
		glm::vec4 getRotation();
		glm::mat4 getmodelMatrix() const;
		virtual void draw(Scene& scene);
		virtual void draw(Scene& scene, std::shared_ptr<Material> mat);
		virtual tetraRender::GameObjectType getType() const;
		std::vector<GameObject*> getChildren();
		virtual GameObject* getDeepCopy();
		GameObject* getParent();
		GameObject* removeChild(GameObject * child);
		GameObject* removeFromParent();
		virtual void update();
		void copyChildren();

		static std::string getTagString(RenderTag tag);
		static RenderTag getTagEnum(std::string);
		static std::vector < std::pair < RenderTag, std::string>> initTranslation();

		static const std::vector < std::pair < RenderTag, std::string>> tagTranslation;
		static const std::string pos;
		static const std::string scale;
		static const std::string rotationAngle;
		static const std::string childrenField;


	protected:
		void updateModelMatrix();

		float rotation;
		glm::mat4 modelMatrix;
		std::vector<GameObject*> children;
		GameObject* parentNode;
		//These tags are an indication for the renderer, depending on what it needs to render, this gives a indication.
		// for now the most used tags are WORLD_OBJECT et POST_PROCESSING. But I can make the difference betwen dynamic and static objects that can be interesting for lightmaps.
		std::set<RenderTag> renderTags;

		
	};

}
