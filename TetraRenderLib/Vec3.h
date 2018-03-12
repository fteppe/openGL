#pragma once
#include <iostream>
#include <string>
#include <glm/vec3.hpp>
class Vec3 : public glm::vec3
{
public:
	Vec3();
	Vec3(glm::vec3 vec);
	Vec3(double x, double y, double z);
	~Vec3();
	Vec3 multiply(double scalar);
	Vec3 cross(Vec3 vector);
	double dot(Vec3 vector);
	double cos(Vec3 vect);
	std::string description();
	double norm();

	//These functions will give the unitary vectors of the space.
	static Vec3 u();
	static Vec3 v();
	static Vec3 w();
	static Vec3 zero();
	Vec3 add(Vec3 vector);


};

std::ostream& operator<<(std::ostream& os, const Vec3& vec);