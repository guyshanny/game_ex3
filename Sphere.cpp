#include "Sphere.h"


Sphere::Sphere(glm::vec3 c, double r) : _center(c), _radius(r) { }

bool Sphere::isCollide(const Sphere & other)
{
	float a = glm::distance(_center, other._center);
	float b = _radius + other._radius;
	float c = a - b;
	return glm::abs((glm::distance(_center, other._center) - (_radius + other._radius))) < 1;
}
