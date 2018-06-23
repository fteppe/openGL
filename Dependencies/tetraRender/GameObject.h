#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
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
		void setRotation(glm::quat rotationQuaternion);
		void addTag(RenderTag tag);
		void addChild(GameObject* child);
		void addChildNoMove(GameObject* child);
		void setParent(GameObject* parent);
		std::set<RenderTag> getRenderTags();
		glm::vec3 getPos() const;
		glm::vec3 getScale() const;
		glm::quat getRotation() const;
		glm::mat4 getmodelMatrix() const;
		virtual void draw(Scene& scene);
		virtual void draw(Scene& scene, std::shared_ptr<Material> mat);
		virtual tetraRender::GameObjectType getType() const;
		std::vector<GameObject*> getChildren();		
		/// <summary>
		///	Creates a deep copy of the game object, it should be noted that the deep copy is to get a copy of the children so the there is still one owner of the childrenand the
		/// owner is the parent of the children.
		/// </summary>
		/// <returns> returns the deep copy of this</returns>
		virtual GameObject* getDeepCopy();
		GameObject* getParent();
		GameObject* removeChild(GameObject * child);
		GameObject* removeFromParent();
		virtual void update();
		Material* getMaterial();
		void setMaterial(std::shared_ptr<Material> const& mat);
		
		/// <summary>
		/// Copies the children. It has to be public so it can be used by the object making the copy. But using it if there is no copy of the parent beeing made a therefor an original
		///referenced WILL result in a memory leak, be carefull when using this.
		/// </summary>
		void copyChildren();

		static std::string getTagString(RenderTag tag);
		static RenderTag getTagEnum(std::string);
		static std::vector < std::pair < RenderTag, std::string>> initTranslation();

		static const std::vector < std::pair < RenderTag, std::string>> tagTranslation;
		static const std::string pos;
		static const std::string scale;
		static const std::string rotationAngle;
		static const std::string childrenField;
		static const std::string material;
		static const std::string typeField;

	protected:
		void updateModelMatrix();
		std::shared_ptr<Material> material_ptr;

		float rotation;
		glm::mat4 modelMatrix;
		std::vector<GameObject*> children;
		GameObject* parentNode;
		//These tags are an indication for the renderer, depending on what it needs to render, this gives a indication.
		// for now the most used tags are WORLD_OBJECT et POST_PROCESSING. But I can make the difference betwen dynamic and static objects that can be interesting for lightmaps.
		std::set<RenderTag> renderTags;

		
	};

}
