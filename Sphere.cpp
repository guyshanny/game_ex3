#include "Sphere.h"


Sphere::Sphere(glm::vec3 c, double r) : _center(c), _radius(r) { }

bool Sphere::isCollide(const Sphere & other)
{
	return (glm::distance(_center, other._center) - (_radius + other._radius)) < -2.25f;
}
