#include "Camera.h"

Camera::Camera(Player* player, Shader* shader)
{
	this->player = player;
	this->shader = shader;

	Reset();
}

void Camera::Move()
{
	float horizontalDistance = distanceFromPlayer * std::cos(pitch * M_PI / 180);
	float verticalDistance = distanceFromPlayer * std::sin(pitch * M_PI / 180);

	QVector3D playerHook = player->GetPosition() + QVector3D(0, 5, 0);
	
	float theta = player->GetRotation().toEulerAngles().y() + yaw;
	float offsetX = horizontalDistance * std::sin(theta * M_PI / 180);
	float offsetZ = horizontalDistance * std::cos(theta * M_PI / 180);

	position.setX(playerHook.x() - offsetX);
	position.setZ(playerHook.z() - offsetZ);
	position.setY(playerHook.y() + verticalDistance);

	cameraMatrix.setToIdentity();
	cameraMatrix.lookAt(
		position,
		playerHook,
		QVector3D(0, 1, 0));

	shader->LoadCameraMatrix(cameraMatrix);
	shader->LoadProjectionMatrix(projectionMatrix);
}

void Camera::ChangePitch(float mouseYAxis)
{
	pitch += mouseYAxis * 0.1f;
	pitch = std::max(10.f, pitch);
	pitch = std::min(50.f, pitch);
}

void Camera::ChangeYaw(float mouseXAxis)
{
	yaw -= mouseXAxis * 0.2f;
}

void Camera::ResetYaw()
{
	yaw = 0;
}

void Camera::SetProjection(float FOV, float width, float height,
	float nearPlane, float farPlane)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(FOV, (width) / height,
		nearPlane, farPlane);
}

void Camera::Reset()
{
	position = QVector3D();
	pitch = 25.f;
	yaw = 0.f;
	distanceFromPlayer = 5.f;
	FOV = 70.f;
}

void Camera::ChangeZoom(float mouseWheelDelta)
{
	distanceFromPlayer -= mouseWheelDelta * 0.005;
	distanceFromPlayer = std::max(3.f, distanceFromPlayer);
	distanceFromPlayer = std::min(8.f, distanceFromPlayer);
}
