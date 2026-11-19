#pragma once
#include "Common/Overridable.h"
#include "Common/AsciiString.h"
#include "GameClient/CommandButton.h"
// Target: allocation31B072=0x9C; ctor409FFA builds Overridable, name+10,
// 32 button pointers+14 and dwords+94/+98. Table83906C has one destructor.
// Eight old compiled views and canonical header agree on this physical layout.
class INI;
struct FieldParse;
class CommandSet : public Overridable
{
public:
 CommandSet(const AsciiString &,int);
 virtual ~CommandSet();
 CommandSet &operator=(const CommandSet &);
 const AsciiString &getName() const { return m_name; }
 const CommandButton *getCommandButton(int) const;
 void ResetCommandSet();
 void SetCommandButton(int,int);
 void refreshIfStale();
 static void parseCommandButton(INI *,void *,const CommandButton **,int);
 static const FieldParse m_commandSetFieldParseTable[];
 AsciiString m_name; // +10
 const CommandButton *m_command[32]; // +14
 unsigned int m_commandCount; // +94; ctor stores32, setter updates maximum
 int m_mode; // +98; ctor argument, setters distinguish1
};
typedef char CommandSetSize[(sizeof(CommandSet)==0x9C)?1:-1];
// SIZE 0x9C
