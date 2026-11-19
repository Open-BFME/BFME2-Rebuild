// DebugStackwalk: the stack walker the Debug manager owns. Retail keeps one
// private instance (constructor 0x0003C0E0 only initialises dbghelp) and walks
// stacks through the loaded dbghelp.dll entry points.
//
// Target facts:
//  - Signature is a count plus 256 addresses: GetAddress (0x0003B9C0) rejects
//    n < 0 and n >= 0x100 and reads [this + n*4 + 4], and operator= (0x0003B9A0)
//    copies the count at +0 then that many dwords from +4.
//  - The two heap-free leaves at 0x0003BC60/0x0003BC70 are forwarders through
//    the Debug object at 0x009E0880 to its table slots +0xA0 and +0xA4, both of
//    which return a pointer (the map spells them ?get@...LeaGetter@@QBEPAXXZ).
//  - GetDbghelpHandle (0x0003C0F0) and IsOldDbghelp (0x0003C100) read the
//    module handle at 0x009E08AC and the flag at 0x009E08A8.
//  - GetSymbol's twelve-argument form ends in the bool fullPath
//    (?GetSymbol@Signature@DebugStackwalk@@SAXIPADIPAI0I10I11_N@Z).
#pragma once
#ifndef BFME2_DEBUGSTACKWALK_H
#define BFME2_DEBUGSTACKWALK_H

class Debug;

class DebugStackwalk
{
	friend class Debug;

	DebugStackwalk(const DebugStackwalk &);
	DebugStackwalk &operator=(const DebugStackwalk &);

	// Constructor and destructor stay private; the retail constructor 0x0003C0E0 only runs InitDbghelp.
	DebugStackwalk(void);
	~DebugStackwalk();

public:
	class Signature
	{
		friend class DebugStackwalk;

		enum { MAX_ADDR = 256 };

		unsigned m_numAddr;		// +0x000
		unsigned m_addr[MAX_ADDR];	// +0x004

	public:
		explicit Signature(void) : m_numAddr(0) {}
		Signature(const Signature &src);
		Signature &operator=(const Signature &src);

		unsigned Size(void) const { return m_numAddr; }
		unsigned GetAddress(int n) const;

		bool operator<(const Signature &other) const
		{
			unsigned m = m_numAddr < other.m_numAddr ? m_numAddr : other.m_numAddr;
			for (unsigned k = 0; k < m; k++)
			{
				if (m_addr[m_numAddr - k - 1] < other.m_addr[other.m_numAddr - k - 1])
					return true;
				if (m_addr[m_numAddr - k - 1] > other.m_addr[other.m_numAddr - k - 1])
					return false;
			}
			return m_numAddr < other.m_numAddr;
		}

		static void GetSymbol(unsigned addr, char *buf, unsigned bufSize);
		static void GetSymbol(unsigned addr,
				      char *bufMod, unsigned sizeMod, unsigned *relMod,
				      char *bufSym, unsigned sizeSym, unsigned *relSym,
				      char *bufFile, unsigned sizeFile, unsigned *line, unsigned *relLine,
				      bool fullPath);
	};

	static void *GetDbghelpHandle(void);
	static bool IsOldDbghelp(void);
	static int StackWalk(Signature &sig, struct _CONTEXT *ctx, bool useFallback);
};

#endif
