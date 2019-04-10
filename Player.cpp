#include "Player.h"
#include <QDebug>

Player::~Player()
{
}

Player::Player(MeshCollection::ModelTexture modelTexture) : Entity(MeshCollection::ModelType::Robot,modelTexture)
{
}
