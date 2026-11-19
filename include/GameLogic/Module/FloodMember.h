#pragma once
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
struct FloodMember
{
 FloodMember() {
  m_controlPointOffset[0].x=0.0f;
  m_controlPointOffset[0].y=0.0f;
  m_controlPointOffset[0].z=0.0f;
  m_controlPointOffset[1].x=0.0f;
  m_controlPointOffset[1].y=0.0f;
  m_controlPointOffset[1].z=0.0f;
  m_controlPointOffset[2].x=0.0f;
  m_controlPointOffset[2].y=0.0f;
  m_controlPointOffset[2].z=0.0f;
  m_controlPointOffset[3].x=0.0f;
  m_controlPointOffset[3].y=0.0f;
  m_controlPointOffset[3].z=0.0f;
  m_speed=0.0f;
 }
 AsciiString m_templateName; // 0x00
 Coord3D m_controlPointOffset[4]; // 0x04
 float m_speed; // 0x34
};
// SIZE 0x38
