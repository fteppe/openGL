#include "stdafx.h"
#include "Vec3.h"
#include <iostream>
#include <sstream>
#include "math.h"

using namespace tetraRender;

Vec3::Vec3()
{
}

Vec3::Vec3(glm::vec3 vec):glm::vec3(vec)
{
}

Vec3::Vec3(double x, double y, double z) : glm::vec3(x, y, z)
{
	
}

Vec3::~Vec3()
{
}

std::string Vec3::description()
{
	std::stringstream ss;
	ss << "x :" << x << std::endl << "y :" << y << std::endl << "z :" << z << std::endl;
	return ss.str();
}

Vec3 Vec3::add(Vec3 vector)
{
	double newX = x + vector.x;
	double newY= y + vector.y;
	double newZ = z + vector.z;

	return Vec3(newX, newY, newZ);
}

double Vec3::dot(Vec3 vector)
{
	return Vec3(x*vector.x, y*vector.y, z*vector.z).norm();
}

double Vec3::cos(Vec3 vector)
{
	return dot(vector) / (vector.norm()*norm());
}

double Vec3::norm() {
	return sqrt(x*x + y*y + z*z);
}



std::ostream& operator<<(std::ostream& os,const Vec3& vec)
{
	os << vec.x << " " << vec.y << " " << vec.z;
	return os;
}

Vec3 Vec3::multiply(double scalar)
{
	return Vec3(x*scalar, y*scalar, z*scalar);
}

Vec3 Vec3::cross(Vec3 vector)
{
	double newX = y*vector.z - z*vector.y;
	double newY = z*vector.x - x*vector.z;
	double newZ = x*vector.y - y*vector.x;
	return Vec3(newX, newY, newZ);
}

Vec3 Vec3::u()
{
	return Vec3(1, 0, 0);
}

Vec3 Vec3::v() 
{
	return Vec3(0, 1, 0);
}

Vec3 Vec3::w()
{
	return Vec3(0, 0, 1);
}

Vec3 Vec3::zero()
{
	return Vec3(0, 0, 0);
}