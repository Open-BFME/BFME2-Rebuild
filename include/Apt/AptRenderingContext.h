#pragma once
// SIZE 0x3C0, no base, no virtual functions. Layout evidence (retail bodies):
//   push/popColourTransform 0x0070E330/0x0070E390 copy the 32-byte colour
//   transform at +0 to and from a 16-entry stack at +0x38 whose depth is at +0x3B8;
//   push/popVertexMatrix 0x0070E4E0/0x0070E550 copy the 24-byte matrix at +0x20
//   to and from a 16-entry stack at +0x238 whose depth is at +0x3BC.
// The retail assertion text names aCXFormStack, nCXFormStack, aVertexMatrixStack,
// nVertexMatrixStack (and APT_ARRAYSIZE, so the stacks are arrays of 16).
// The two current-value members' names (curCXForm, curVertexMatrix) are the
// donor's spelling; the retail bytes only fix their offsets and widths.
// Colour transform = four multiplies then four adds (0x0070E3E0 multiplies
// the first four floats and adds the last four); the matrix is six floats.
// SIZE 0x3C0 is the end of the last member; no allocation of the whole object
// has been seen in a recovered body.
struct AptCXForm
{
    float multiply[4]; // 0x0
    float add[4];      // 0x10
};

struct AptMatrix
{
    float element[6]; // 0x0, 24 bytes
};

// A 4x4 float matrix, row by row (the 2D transforms of the display list are built into and combined in this form: only the elements 0, 1, 4, 5, 12 and 13 vary).
struct AptMatrix4
{
    float m[4][4];
};

// `class` in retail (the mangled names spell PAVAptRenderingContext); every member is public here.
class AptRenderingContext
{
public:
    AptCXForm curCXForm;             // 0x0
    AptMatrix curVertexMatrix;       // 0x20
    AptCXForm aCXFormStack[16];      // 0x38
    AptMatrix aVertexMatrixStack[16]; // 0x238
    int nCXFormStack;                // 0x3B8
    int nVertexMatrixStack;          // 0x3BC

    void pushColourTransform();
    void popColourTransform();
    void pushVertexMatrix();
    void popVertexMatrix();
    // 0x0070E3E0 (the map files it as bfmeOp1209 on the placeholder class BfmeA1209): concatenates a colour transform into the current one (the four
    // multipliers multiply, the four adders add) and notifies the host through the callback cell at VA 0x00E177A4.
    void multiplyColourTransform(const AptCXForm *other);
    // 0x0070E440 (the map files it as rva0070E440 on the placeholder class Rva0070E440): the current vertex matrix, or the default matrix (VA 0x00E180C4)
    // when nothing is pushed; asserts its out parameter (`pMatrix`, line 0x49).
    void getVertexMatrix(AptMatrix *out);
    // 0x0070E6C0 (the map files it as bfmeGoDXH on the placeholder class BfmeThingDXH; its row is in the gap unit G0070E5D0): multiplies a matrix into the
    // current vertex matrix and hands the result to the host callback at VA 0x00E177A0.
    void multiplyVertexMatrix(const AptMatrix *matrix);
    // 0x0070E6E0 (the map files it as bfmeApplyXS on the placeholder class BfmeThingXS): applies a bounds record (the second argument: the 0x50 bytes of a text instance) to a rect
    void bfmeApplyXS(void *rect, void *bounds);
};

typedef char AptRenderingContextSize[(sizeof(AptRenderingContext) == 0x3C0) ? 1 : -1];
