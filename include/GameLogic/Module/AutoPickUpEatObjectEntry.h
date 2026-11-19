#pragma once
#include "GameLogic/ObjectFilter.h"
struct AutoPickUpEatObjectEntry
{
 AutoPickUpEatObjectEntry() : m_myHealth(0),m_targetHealth(1.0f) {}
 ~AutoPickUpEatObjectEntry();
 ObjectFilter m_filter; // 0x00
 float m_myHealth; // 0x04
 float m_targetHealth; // 0x08
};
// SIZE 0x0C
