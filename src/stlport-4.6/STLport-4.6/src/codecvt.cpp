// cl-profile: upstream
// cl: /O2 /G6 /GR /EHsc /MD /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_STLP_USE_STATIC_LIB /D_STLP_NO_FORCE_INSTANTIATE -Iinclude/upstream/stlport-4.6
// stlport46
// cl-arch: x87
// UPSTREAM: stlport-4.6; pristine payload SHA256 33e9640eea91ad167538a22527dfaa5b4363ce774b0aadc5615a15100bed1009
// FUNCTION: BFME2 0x000174F0 size=105 name=?do_out@codecvt_wide_char@_STL@@QBEHAAHPBG1AAPBGPAD3AAPAD@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00017560 size=105 name=?do_in@codecvt_wide_char@_STL@@QBEHAAHPBD1AAPBDPAG3AAPAG@Z evidence=N:ledger tier=L hypothesis:name:L
/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */ 
# include "stlport_prefix.h"

# ifndef _STLP_NO_MBSTATE_T

#include <stl/_codecvt.h>
#include <stl/_algobase.h>

_STLP_BEGIN_NAMESPACE

//----------------------------------------------------------------------
// codecvt<char, char, mbstate_t>

codecvt<char, char, mbstate_t>::~codecvt() {}

int codecvt<char, char, mbstate_t>::do_length(const mbstate_t&,
                                              const  char* from, 
                                              const  char* end,
                                              size_t mx) const
{
  return (int)(min) ( __STATIC_CAST(size_t, (end - from)), mx);
}

int codecvt<char, char, mbstate_t>::do_max_length() const _STLP_NOTHROW
{
 return 1; 
}

bool
codecvt<char, char, mbstate_t>::do_always_noconv() const _STLP_NOTHROW
{
  return true;
}

int 
codecvt<char, char, mbstate_t>::do_encoding() const _STLP_NOTHROW
{
  return 1; 
}


codecvt_base::result 
codecvt<char, char, mbstate_t>::do_unshift(mbstate_t& /* __state */,
                                           char*      __to,
                                           char*      /* __to_limit */,
                                           char*&     __to_next) const
{ __to_next = __to; return noconv; }

codecvt_base::result 
codecvt<char, char, mbstate_t>::do_in (mbstate_t&   /* __state */ , 
                                       const char*  __from,
                                       const char*  /* __from_end */,
                                       const char*& __from_next,
                                       char*        __to,
                                       char*        /* __to_end */,
                                       char*&       __to_next) const
{ __from_next = __from; __to_next   = __to; return noconv; }

codecvt_base::result 
codecvt<char, char, mbstate_t>::do_out(mbstate_t&   /* __state */,
                                       const char*  __from,
                                       const char*  /* __from_end */,
                                       const char*& __from_next,
                                       char*        __to,
                                       char*        /* __to_limit */,
                                       char*&       __to_next) const
{ __from_next = __from; __to_next   = __to; return noconv; }


# ifndef _STLP_NO_WCHAR_T
//----------------------------------------------------------------------
// codecvt<wchar_t, char, mbstate_t>

codecvt<wchar_t, char, mbstate_t>::~codecvt() {}


codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_out(state_type&         /* state */, 
                                          const intern_type*  from,
                                          const intern_type*  from_end,
                                          const intern_type*& from_next,
                                          extern_type*        to,
                                          extern_type*        to_limit,
                                          extern_type*&       to_next) const
{
  ptrdiff_t len = (min) (from_end - from, to_limit - to);
  copy(from, from + len, to);
  from_next = from + len;
  to_next   = to   + len;
  return ok;
}

codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_in (state_type&       /* state */,
                                          const extern_type*  from,
                                          const extern_type*  from_end,
                                          const extern_type*& from_next,
                                          intern_type*        to,
                                          intern_type*        to_limit,
                                          intern_type*&       to_next) const
{
  ptrdiff_t len = (min) (from_end - from, to_limit - to);
  copy(from, from + len, to);
  from_next = from + len;
  to_next   = to   + len;
  return ok;
}

codecvt<wchar_t, char, mbstate_t>::result
codecvt<wchar_t, char, mbstate_t>::do_unshift(state_type&   /* state */,
                                              extern_type*  to, 
                                              extern_type*  ,
                                              extern_type*& to_next) const
{
  to_next = to;
  return noconv;
}

int codecvt<wchar_t, char, mbstate_t>::do_encoding() const _STLP_NOTHROW {
  return 1;
}


bool codecvt<wchar_t, char, mbstate_t>::do_always_noconv() const _STLP_NOTHROW
{
  return true;
}

int codecvt<wchar_t, char, mbstate_t>::do_length(const  state_type&,
                                                 const  extern_type* from, 
                                                 const  extern_type* end,
                                                 size_t mx) const 
{
  return (int)(min) ((size_t) (end - from), mx);
}

int codecvt<wchar_t, char, mbstate_t>::do_max_length() const _STLP_NOTHROW {
  return 1;
}
# endif /* wchar_t */

_STLP_END_NAMESPACE

# endif /* _STLP_NO_MBSTATE_T */

// Local Variables:
// mode:C++
// End:

