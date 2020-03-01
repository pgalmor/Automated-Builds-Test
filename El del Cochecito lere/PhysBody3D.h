#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"

class btRigidBody;
class btQuaternion;
class Module;
class vec3;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetRotation(btQuaternion orientation);
	const vec3 GetPos() const;
	vec3 GetForward() const;
	btRigidBody* GetBody() const;

	void SetAsSensor(bool is_sensor);
	bool IsSensor();

private:
	btRigidBody* body = nullptr;

public:
	p2List<Module*> collision_listeners;
	bool is_sensor;
};

#endif // __PhysBody3D_H__