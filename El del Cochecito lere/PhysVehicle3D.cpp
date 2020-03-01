#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Application.h"
#include "Globals.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
	Scuderia = Renault;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Grey;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());


	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cube glass(info.glass_size.x, info.glass_size.y, info.glass_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&glass.transform);
	btQuaternion q2 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset2(info.glass_offset.x, info.glass_offset.y,
	info.glass_offset.z);
	offset2 = offset2.rotate(q2.getAxis(), q2.getAngle());

	glass.transform.M[12] += offset2.getX();
	glass.transform.M[13] += offset2.getY();
	glass.transform.M[14] += offset2.getZ();

	Cube prow(info.prow_size.x, info.prow_size.y, info.prow_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&prow.transform);
	btQuaternion q3 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.prow_offset.x, info.prow_offset.y,
		info.prow_offset.z);
	offset3 = offset3.rotate(q3.getAxis(), q3.getAngle());

	prow.transform.M[12] += offset3.getX();
	prow.transform.M[13] += offset3.getY();
	prow.transform.M[14] += offset3.getZ();

	Cube aileron(info.aileron_size.x, info.aileron_size.y, info.aileron_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron.transform);
	btQuaternion q4 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset4(info.aileron_offset.x, info.aileron_offset.y,
		info.aileron_offset.z);
	offset4 = offset4.rotate(q4.getAxis(), q4.getAngle());

	aileron.transform.M[12] += offset4.getX();
	aileron.transform.M[13] += offset4.getY();
	aileron.transform.M[14] += offset4.getZ();

	Cube light(info.light_size.x, info.light_size.y, info.light_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&light.transform);
	btQuaternion q5 = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset5(info.light_offset.x, info.light_offset.y,
		info.light_offset.z);
	offset5 = offset5.rotate(q5.getAxis(), q5.getAngle());
	if(info.revolutions == true){ light.color = Red; }
	else { light.color = Green; }

	light.transform.M[12] += offset5.getX();
	light.transform.M[13] += offset5.getY();
	light.transform.M[14] += offset5.getZ();

	switch (Scuderia) 
	{
		case Renault:
			chassis.color = LightBlue; chassis.Render();
			glass.color = Yellow; glass.Render();
			prow.color = Yellow; prow.Render();
			aileron.color = NavyBlue; aileron.Render();
			break;

		case Ferrari:
			chassis.color = Red3; chassis.Render();
			glass.color = Red2; glass.Render();
			prow.color = Red2; prow.Render();
			aileron.color = Grey; aileron.Render();
			break;

		case McLaren:
			chassis.color = Orange2; chassis.Render();
			glass.color = Orange; glass.Render();
			prow.color = Orange; prow.Render();
			aileron.color = Blue; aileron.Render();
			break;

		case RedBull:
			chassis.color = NavyBlue; chassis.Render();
			glass.color = Red3; glass.Render();
			prow.color = Yellow2; prow.Render();
			aileron.color = Yellow2; aileron.Render();
			break;
	}

	light.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}

