#pragma once

#include <glm\glm.hpp>

class Sphere
{
public:
	Sphere() : _center(0), _radius(0) {}
	Sphere(glm::vec3 c, double r);
	~Sphere() {};

	bool isCollide(const Sphere& other);
	void setCenter(const glm::vec3& center) { _center = center; }
	void setRadius(const double& radius) { _radius = radius; }

private:
	glm::vec3   _center;
	double    _radius;
};

