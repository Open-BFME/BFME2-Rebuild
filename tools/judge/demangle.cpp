#include <string_view>
#include <string>
#include <iostream>
#include <cstdlib>
namespace llvm { enum MSDemangleFlags { None=0 }; char *microsoftDemangle(std::string_view, size_t*, int*, MSDemangleFlags); }
int main() { std::string s; while(std::getline(std::cin,s)) { int status=0; size_t n=0; char *r=llvm::microsoftDemangle(s,&n,&status,llvm::None); std::cout<<(status==0&&r?r:s.c_str())<<"\n"; std::free(r); } }
