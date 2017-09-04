#include "BaseEntity.h"
#include "GameWorld.h"

BaseEntity::BaseEntity()
: m_bounding_radius(10)
, m_is_tagged(false)
, m_pWorld(NULL)
{

}

BaseEntity::~BaseEntity()
{

}

