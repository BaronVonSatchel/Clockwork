#pragma once

#include <SFML/System.hpp>
#include <cmath>
#include <ostream>

namespace Clockwork {
	//template<typename T> class Vecmath {

	//};
	///<summary>Square of the magnitude of the vector</summary>
	template<typename T> T mag2(const sf::Vector2<T>& v) {
		return v.x * v.x + v.y * v.y;
	}

	///<summary>Magnitude of the vector (might be off when using integers)</summary>
	template<typename T> T mag(const sf::Vector2<T>& v) {
		return std::sqrt(mag2(v));
	}
	
	///<summary>Square of the distance between the vectors</summary>
	template<typename T> T dist2(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
		return mag2(v1 - v2);
	}

	///<summary>Distance between the vectors (might be off when using integers)</summary>
	template<typename T> T dist(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
		return std::sqrt(dist2(v1, v2));
	}

	///<summary>Dot product of the vectors</summary>
	template<typename T> T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
		return v1.x * v2.x + v1.y + v2.y;
	}

	///<summary>Normalized vector (unit vector in same direction)</summary>
	template<typename T> sf::Vector2<T> norm(const sf::Vector2<T>& v) {
		return v / mag(v);
	}

	///<summary>2D projection of point p onto axis a</summary>
	template<typename T> sf::Vector2<T> proj(const sf::Vector2<T>& p, const sf::Vector2<T>& a) {
		return a * dot(p, a) / mag2(a);
	}

	///<summary>"2D cross product" of two vectors v1 X v2</summary>
	template<typename T> T cross(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}

	///<summary>"2D cross product" v X c where c is scalar</summary>
	template<typename T> sf::Vector2<T> cross(const sf::Vector2<T>& v, T c) {
		return sf::Vector2<T>(v.y * c, -v.x * c);
	}

	///<summary>"2D cross product" c X v where c is scalar</summary>
	template<typename T> sf::Vector2<T> cross(T c, const sf::Vector2<T>& v) {
		return cross(v, -c);
	}

	///<summary>Perpendicular distance from point p to the line passing through l1 and l2</summary>
	template<typename T> T distanceToLine(const sf::Vector2<T>& p, const sf::Vector2<T>& l1, const sf::Vector2<T>& l2) {
		return (cross(p, l2 - l1) + cross(l2, l1)) / dist(l1, l2);
	}

	///<summary>Revolve v about the origin by angle a</summary>
	template<typename T> sf::Vector2<T> revolve(const sf::Vector2<T>& v, float a) {
		a += std::atan2(v.y, v.x);
		T m = mag(v);
		return sf::Vector2<T>(m * std::cos(a), m * std::sin(a));
	}

	///<summary>Revolve v about c by angle a</summary>
	template<typename T> sf::Vector2<T> revolve(const sf::Vector2<T>& v, const sf::Vector2<T>& c, T a) {
		return revolve(v - c, a) + c;
	}

	///<summary>Midpoint between v1 and v2</summary>
	template<typename T> sf::Vector2<T> mid(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) {
		return (v1 + v2) / (T)2;
	}
}

template<typename T> std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}