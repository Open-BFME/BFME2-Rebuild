// XferException.h: thrown on bad save data. SIZE 0x8.
// Target facts (old repo Code/GameEngine/Include/Common/XferException.h): the
// variadic constructor is 0x0060C36E; the copy constructor 0x000588C5; the
// destructor 0x0002BD30 deletes the message buffer at +0; the code is at +4.
// INIException shares the copy, assignment and destructor bodies.
#pragma once
#ifndef BFME2_XFEREXCEPTION_H
#define BFME2_XFEREXCEPTION_H

#include <new>

class XferException
{
public:
	XferException(int code, const char *format, ...);
	XferException(const XferException &that);
	~XferException() { delete[] m_message; }	// header inline: COMDATs folded with INIException's at 0x0002BD30
	XferException &operator=(const XferException &that);

	char *m_message;	// 0x0
	int m_code;			// 0x4
};
// SIZE 0x8

#endif
