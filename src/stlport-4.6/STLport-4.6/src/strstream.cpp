// cl-profile: upstream
// cl: /O2 /G6 /GR /EHsc /MD /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_STLP_USE_STATIC_LIB /D_STLP_NO_FORCE_INSTANTIATE /Iinclude/upstream/stlport-4.6
// stlport46
// cl-arch: x87
// UPSTREAM: stlport-4.6; pristine payload SHA256 130bf8e719a79874254346a5d572e5bf573db944017325b2a99d67af26fb5e5e
// FUNCTION: BFME2 0x00602680 size=26 name=?freeze@strstreambuf@_STL@@QAEX_N@Z evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006026A0 size=26 name=?str@strstreambuf@_STL@@QAEPADXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006026C0 size=19 name=?pcount@strstreambuf@_STL@@QBEHXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006026E0 size=36 name=?alloc@Rva006026E0@@QAEPAXI@Z evidence=?:placeholder tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602710 size=33 name=?free@Rva006026E0@@QAEXPAX@Z evidence=?:placeholder tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602740 size=105 name=?_M_setup@strstreambuf@_STL@@AAEXPAD0H@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006027B0 size=26 name=?str@istrstream@_STL@@QAEPADXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006027D0 size=26 name=?freeze@ostrstream@_STL@@QAEX_N@Z evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006027F0 size=26 name=?str@ostrstream@_STL@@QAEPADXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602810 size=19 name=?pcount@ostrstream@_STL@@QBEHXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602830 size=26 name=?freeze@strstream@_STL@@QAEX_N@Z evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602850 size=19 name=?pcount@strstream@_STL@@QBEHXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602870 size=26 name=?str@strstream@_STL@@QAEPADXZ evidence=L:vendored=stlport-4.5.3 tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006028A0 size=198 name=??0strstreambuf@_STL@@QAE@H@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602970 size=188 name=??0strstreambuf@_STL@@QAE@P6APAXI@ZP6AXPAX@Z@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602A30 size=105 name=??0strstreambuf@_STL@@QAE@PADH0@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602AA0 size=163 name=??0strstreambuf@_STL@@QAE@PBDH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602B50 size=115 name=??1strstreambuf@_STL@@UAE@XZ evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602BD0 size=82 name=?underflow@strstreambuf@_STL@@MAEHXZ evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00602F80 size=333 name=?overflow@strstreambuf@_STL@@MAEHH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006030D0 size=111 name=?pbackfail@strstreambuf@_STL@@MAEHH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00603200 size=186 name=??0ostrstream@_STL@@QAE@XZ evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006032C0 size=220 name=??0ostrstream@_STL@@QAE@PADHH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006033A0 size=195 name=??0istrstream@_STL@@QAE@PAD@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00603470 size=291 name=??0istrstream@_STL@@QAE@PBD@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x006035A0 size=199 name=??0istrstream@_STL@@QAE@PADH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00603670 size=198 name=??0istrstream@_STL@@QAE@PBDH@Z evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00603740 size=177 name=??0strstream@_STL@@QAE@XZ evidence=N:ledger tier=L hypothesis:name:L
// FUNCTION: BFME2 0x00603800 size=212 name=??0strstream@_STL@@QAE@PADHH@Z evidence=N:ledger tier=L hypothesis:name:L
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

// Implementation of the classes in header <strstream>.
// WARNING: The classes defined in <strstream> are DEPRECATED.  This
// header is defined in section D.7.1 of the C++ standard, and it
// MAY BE REMOVED in a future standard revision.  You should use the
// header <sstream> instead.

# include "stlport_prefix.h"
#include <stl/_strstream.h>
#include <stl/_algobase.h>

_STLP_BEGIN_NAMESPACE

// strstreambuf constructor, destructor.

strstreambuf::strstreambuf(streamsize initial_capacity)
   :  _M_alloc_fun(0), _M_free_fun(0),
     _M_dynamic(true), _M_frozen(false), _M_constant(false)  
{
  streamsize n = (max)(initial_capacity, streamsize(16));

  char* buf = _M_alloc(n);
  if (buf) {
    setp(buf, buf + n);
    setg(buf, buf, buf);
  }
}



strstreambuf::strstreambuf(__alloc_fn alloc_f, __free_fn free_f)
  : _M_alloc_fun(alloc_f), _M_free_fun(free_f),
    _M_dynamic(true), _M_frozen(false), _M_constant(false)
{
  streamsize n = 16;

  char* buf = _M_alloc(n);
  if (buf) {
    setp(buf, buf + n);
    setg(buf, buf, buf);
  }
}

strstreambuf::strstreambuf(char* get, streamsize n, char* put)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(false)
{
  _M_setup(get, put, n);
}

strstreambuf::strstreambuf(signed char* get, streamsize n, signed char* put)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(false)
{
  _M_setup(__REINTERPRET_CAST(char*,get), __REINTERPRET_CAST(char*,put), n);
}

strstreambuf::strstreambuf(unsigned char* get, streamsize n,
                           unsigned char* put)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(false)
{
  _M_setup(__REINTERPRET_CAST(char*,get), __REINTERPRET_CAST(char*,put), n);
}

strstreambuf::strstreambuf(const char* get, streamsize n)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(true)
{
  _M_setup(__CONST_CAST(char*,get), 0, n);
}

strstreambuf::strstreambuf(const signed char* get, streamsize n)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(true)
{
  _M_setup(__REINTERPRET_CAST(char*, __CONST_CAST(signed char*,get)), 0, n);
}

strstreambuf::strstreambuf(const unsigned char* get, streamsize n)
  : _M_alloc_fun(0), _M_free_fun(0),
    _M_dynamic(false), _M_frozen(false), _M_constant(true)
{
  _M_setup(__REINTERPRET_CAST(char*, __CONST_CAST(unsigned char*,get)), 0, n);
}

strstreambuf::~strstreambuf()
{
  if (_M_dynamic && !_M_frozen)
    _M_free(eback());    
}

void strstreambuf::freeze(bool frozenflag)
{
  if (_M_dynamic)
    _M_frozen = frozenflag;
}

char* strstreambuf::str()
{
  freeze(true);
  return eback();
}

int strstreambuf::pcount() const
{
  return int(pptr() ? pptr() - pbase() : 0);
}

strstreambuf::int_type strstreambuf::overflow(int_type c) {
  if (c == traits_type::eof())
    return traits_type::not_eof(c);

  // Try to expand the buffer.
  if (pptr() == epptr() && _M_dynamic && !_M_frozen && !_M_constant) {
    ptrdiff_t old_size = epptr() - pbase();
    ptrdiff_t new_size = (max)(2 * old_size, ptrdiff_t(1));

    char* buf = _M_alloc(new_size);
    if (buf) {
      memcpy(buf, pbase(), old_size);

      char* old_buffer = pbase();
      bool reposition_get = false;
      ptrdiff_t old_get_offset;
      if (gptr() != 0) {
        reposition_get = true;
        old_get_offset = gptr() - eback();
      }

      setp(buf, buf + new_size);
      pbump((int)old_size);

      if (reposition_get) 
        setg(buf, buf + old_get_offset, buf + (max)(old_get_offset, old_size));

      _M_free(old_buffer);
    }
  }

  if (pptr() != epptr()) {
    *pptr() = c;
    pbump(1);
    return c;
  }
  else
    return traits_type::eof();
}

strstreambuf::int_type strstreambuf::pbackfail(int_type c)
{
  if (gptr() != eback()) {
    if (c == _Traits::eof()) {
      gbump(-1);
      return _Traits::not_eof(c);
    }
    else if (c == gptr()[-1]) {
      gbump(-1);
      return c;
    }
    else if (!_M_constant) {
      gbump(-1);
      *gptr() = c;
      return c;
    }
  }

  return _Traits::eof();
}

strstreambuf::int_type strstreambuf::underflow()
{
  if (gptr() == egptr() && pptr() && pptr() > egptr())
    setg(eback(), gptr(), pptr());

  if (gptr() != egptr())
    return (unsigned char) *gptr();
  else
    return _Traits::eof();
}

basic_streambuf<char, char_traits<char> >* 
strstreambuf::setbuf(char*, streamsize)
{
  return this;
}

strstreambuf::pos_type
strstreambuf::seekoff(off_type off,
                      ios_base::seekdir dir, ios_base::openmode mode)
{
  bool do_get = false;
  bool do_put = false;

  if ((mode & (ios_base::in | ios_base::out)) ==
          (ios_base::in | ios_base::out) &&
      (dir == ios_base::beg || dir == ios_base::end))
    do_get = do_put = true;
  else if (mode & ios_base::in)
    do_get = true;
  else if (mode & ios_base::out)
    do_put = true;

  // !gptr() is here because, according to D.7.1 paragraph 4, the seekable
  // area is undefined if there is no get area.
  if ((!do_get && !do_put) || (do_put && !pptr()) || !gptr())
    return pos_type(off_type(-1));

  char* seeklow  = eback();
  char* seekhigh = epptr() ? epptr() : egptr();

  off_type newoff;
  switch(dir) {
  case ios_base::beg:
    newoff = 0;
    break;
  case ios_base::end:
    newoff = seekhigh - seeklow;
    break;
  case ios_base::cur:
    newoff = do_put ? pptr() - seeklow : gptr() - seeklow;
    break;
  default:
    return pos_type(off_type(-1));
  }

  off += newoff;
  if (off < 0 || off > seekhigh - seeklow)
    return pos_type(off_type(-1));

  if (do_put) {
    if (seeklow + off < pbase()) {
      setp(seeklow, epptr());
      pbump((int)off);
    }
    else {
      setp(pbase(), epptr());
      pbump((int)(off - (pbase() - seeklow)));
    }
  }
  if (do_get) {
    if (off <= egptr() - seeklow)
      setg(seeklow, seeklow + off, egptr());
    else if (off <= pptr() - seeklow)
      setg(seeklow, seeklow + off, pptr());
    else
      setg(seeklow, seeklow + off, epptr());
  }

  return pos_type(newoff);
}

strstreambuf::pos_type
strstreambuf::seekpos(pos_type pos, ios_base::openmode mode)
{
  return seekoff(pos - pos_type(off_type(0)), ios_base::beg, mode);
}


char* strstreambuf::_M_alloc(size_t n)
{
  if (_M_alloc_fun)
    return __STATIC_CAST(char*,_M_alloc_fun(n));
  else
    return new char[n];
}

void strstreambuf::_M_free(char* p)
{
  if (p)
    if (_M_free_fun)
      _M_free_fun(p);
    else
      delete[] p;
}

void strstreambuf::_M_setup(char* get, char* put, streamsize n)
{
  if (get) {
    size_t N = n > 0 ? size_t(n) : n == 0 ? strlen(get) : size_t(INT_MAX);
    
    if (put) {
      setg(get, get, put);
      setp(put, put + N);
    }
    else {
      setg(get, get, get + N);
    }
  }
}

//----------------------------------------------------------------------
// Class istrstream

istrstream::istrstream(char* s)
  : basic_istream<char, char_traits<char> >(0), _M_buf(s, 0)
{
  this->init(&_M_buf);
}

istrstream::istrstream(const char* s)
  : basic_istream<char, char_traits<char> >(0), _M_buf(s, 0)
{
  this->init(&_M_buf);
}

istrstream::istrstream(char* s, streamsize n)
  : basic_istream<char, char_traits<char> >(0), _M_buf(s, n)
{
  this->init(&_M_buf);
}

istrstream::istrstream(const char* s, streamsize n)
  : basic_istream<char, char_traits<char> >(0), _M_buf(s, n)
{
  this->init(&_M_buf);
}

istrstream::~istrstream() {}

strstreambuf* istrstream::rdbuf() const {
  return __CONST_CAST(strstreambuf*,&_M_buf);
}

char* istrstream::str() { return _M_buf.str(); }

//----------------------------------------------------------------------
// Class ostrstream

ostrstream::ostrstream()
  : basic_ostream<char, char_traits<char> >(0), _M_buf()
{
  basic_ios<char, char_traits<char> >::init(&_M_buf);
}

ostrstream::ostrstream(char* s, int n, ios_base::openmode mode)
  : basic_ostream<char, char_traits<char> >(0), 
    _M_buf(s, n, mode & ios_base::app ? s + strlen(s) : s)
{
  basic_ios<char, char_traits<char> >::init(&_M_buf);
}

ostrstream::~ostrstream() {}

strstreambuf* ostrstream::rdbuf() const 
{
  return __CONST_CAST(strstreambuf*,&_M_buf);
}

void ostrstream::freeze(bool freezeflag)
{
  _M_buf.freeze(freezeflag);
}

char* ostrstream::str()
{
  return _M_buf.str();
}

int ostrstream::pcount() const
{
  return _M_buf.pcount();
}


//----------------------------------------------------------------------
// Class strstream

strstream::strstream()
  : basic_iostream<char, char_traits<char> >(0), _M_buf()
{
  basic_ios<char, char_traits<char> >::init(&_M_buf);
}

strstream::strstream(char* s, int n, ios_base::openmode mode)
  : basic_iostream<char, char_traits<char> >(0), 
    _M_buf(s, n, mode & ios_base::app ? s + strlen(s) : s)
{
  basic_ios<char, char_traits<char> >::init(&_M_buf);
}
  
strstream::~strstream() {}

strstreambuf* strstream::rdbuf() const
{
  return __CONST_CAST(strstreambuf*,&_M_buf);
}

void strstream::freeze(bool freezeflag)
{
  _M_buf.freeze(freezeflag);
}

int strstream::pcount() const
{
  return _M_buf.pcount();
}

char* strstream::str()
{
  return _M_buf.str();
}

_STLP_END_NAMESPACE

// Local Variables:
// mode:C++
// End:
