#pragma once

#include "Clockwork.h"
#include <SFML/Graphics.hpp>
#include <set>
#include <unordered_map>

#define PHYSICS_DYNAMIC 2
#define PHYSICS_COLLISION 1

namespace Clockwork {

	//template<typename T> struct ActorRegister {
	//	ActorRegister(std::string name) {

	//	}
	//};

	class Scene;

	class Actor;

	///<summary>Comparison structure to sort actors by their draw layer. If the
	///actors belong to the same layer, it defaults to comparing their pointers.</summary>
	struct compareLayer {
		bool operator()(const Actor * a, const Actor * b) const;
	};

	extern "C" class CLOCKWORK_API Actor abstract : public sf::Transformable, public sf::Drawable
	{
		friend class Scene;

	public:
		Actor();
		~Actor();

		Scene* getScene() const;

		virtual void tick(const sf::Time& timestep);

		virtual void drawActor(sf::RenderTarget& target, sf::RenderStates states) const;

		///<summary>Attach this actor to a parent actor. Returns true if successful.</summary>
		///<param name='parent'>Parent to attach to</param>
		bool attachTo(Actor* parent);

		///<summary>Detach this actor from its parent</summary>
		void detach();

		///<summary>Get the immediate parent of this actor, or null if it has none</summary>
		Actor* getParent() const;

		///<summary>Get the root actor of the entire tree, or this if it is the root</summary>
		Actor* getRoot();

		///<summary>Get a set of the children of this actor.</summary>
		const std::set<Actor*, compareLayer> getChildren() const;

		///<sumamry>Tick this actor and all of its children (children tick first), then update physics if applicable</summary>
		void update(const sf::Time& timestep);

		///<summary>Draws the actor and all of its children. Children will be drawn after (on top) regardless of layer.</summary>
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		///<summary>Get the absolute transform of this actor relative to the world origin.</summary>
		sf::Transform getWorldTransform() const;

		///<summary>Whether this static or dynamic, and whether it can collide with others.
		///Use the bitmasks PHYSICS_DYNAMIC and PHYSICS_COLLISION to test individually.</summary>
		char getActorPhysics() const;

		///<summary>The user-defined mass of this actor, or zero if it has static physics.</summary>
		float getMass() const;

		///<summary>Determines when this actor will be drawn relative to others. Higher values will be drawn later and therefore on top.</summary>
		unsigned char layer = 0;

		///<summary>Determines whether to draw this actor at all. Invisible actors can still tick.</summary>
		bool visible = true;

		float getLayerZ() const;

	protected:

		///<summary>Mass of this actor. Use getMass() instead of accessing directly</summary>
		float mass;

		///<summary>Speed at which this actor is rotating in degrees/second</summary>
		float angularVelocity;

		///<summary>Forces affecting angular velocity to be integrated at the end of the tick</summary>
		float torque;

		///<summary>Velocity of the actor in pixels/second (at 1x scale)</summary>
		sf::Vector2f velocity;

		///<summary>Linear forces acting on this actor, to be integrated at the end of the tick</summary>
		sf::Vector2f forces;

		///<summary>Physics settings of this actor. Use bitmasks PHYSICS_DYNAMIC and PHYSICS_COLLISION to set</summary>
		char physics;
	private:
		Scene* scene;

		Actor* parent;

		std::set<Actor*, compareLayer> children;

		float layerZ;

		//static std::unordered_map<std::string, Actor*(*)()> actorClasses;
	};
}