



#include "GameClient/DisplayString.h"
#include <stddef.h>
// Reference object O00358914.
// FUNCTION: BFME2 0x00358933 size=19 name=?removeLastChar@DisplayString@@UAEXXZ evidence=N:ledger
void DisplayString::removeLastChar()
{
	m_textString.removeLastChar();
	notifyTextChanged();
}

// FUNCTION: BFME2 0x00358946 size=17 name=?reset@DisplayString@@UAEXXZ evidence=N:ledger
void DisplayString::reset()
{
	m_textString.clear();
	m_font = NULL;
}

// FUNCTION: BFME2 0x00358957 size=38 name=?appendChar@DisplayString@@UAEXG@Z evidence=N:ledger
void DisplayString::appendChar(unsigned short character)
{
	m_textString.concat(character);
	notifyTextChanged();
}

// FUNCTION: BFME2 0x0035897D size=23 name=??0DisplayString@@QAE@XZ evidence=N:ledger
DisplayString::DisplayString()
{
	m_font = NULL;
	m_next = NULL;
	m_prev = NULL;
}

// FUNCTION: BFME2 0x00358994 size=57 name=??1DisplayString@@UAE@XZ evidence=N:ledger
DisplayString::~DisplayString()
{
	reset();
}

// FUNCTION: BFME2 0x003589E9 size=26 name=?appendString@DisplayString@@UAEXABVUnicodeString@@@Z evidence=N:ledger
void DisplayString::appendString(const UnicodeString &text)
{
	m_textString.concat(text);
	notifyTextChanged();
}

// FUNCTION: BFME2 0x00358A03 size=80 name=?setText@DisplayString@@UAEXVUnicodeString@@@Z evidence=N:ledger
void DisplayString::setText(UnicodeString text)
{
	if (text.compare(m_textString) != 0)
	{
		m_textString.set(text);
		notifyTextChanged();
	}
}
