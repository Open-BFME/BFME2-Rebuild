#pragma once
// Old-decomp spellings for the target's assertion callback and break flag.
// Their reference identities still need Object VERIFIED evidence.
extern void (__cdecl *g_bfmeAptAssertAtE17734)(const char *, const char *, int);
extern int g_bfmeAptBreakOnAssertAtDDC01C;
// Retail places the assert's `int 3` as a plain instruction inside the surrounding code (the compiler keeps it where
// the source puts it and duplicates the epilogue after it); the __debugbreak intrinsic is scheduled differently.
// This is the one inline-asm break the Apt units use (gen3's ruling, 2026-11-17: allowed as this single macro in one
// Apt header, and only once the checker's header asm scan is live).
#define APT_ASSERT_BREAK() { __asm int 3 }
