#pragma once

#include <glm\glm.hpp>

class Sphere
{
public:
public:
	Sphere(glm::vec3 c, double r);
	~Sphere() {};

private:
	glm::vec3   _center;
	double    _radius;
};

