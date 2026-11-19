#pragma once
// Empty equality functor used with rts::hash in the retail transition table
// ctor1DCB64/dtor1DC636 (old ledger symbol and CodeView type evidence).
// ZH STLTypedefs.h supplies this template spelling. The call body is kept
// external until its shared instantiation is recovered.
namespace rts {
template<class T> struct equal_to {
    bool operator()(const T &,const T &) const;
};
}