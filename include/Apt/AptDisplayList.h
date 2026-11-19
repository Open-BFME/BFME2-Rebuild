#pragma once
// SIZE 0x4: a handle to the AptDisplayListState that holds the root of the list
// (constructor 0x006F7FF0 builds the state, destructor 0x006F84F0 clears the list,
// detaches and force-deletes the root and frees the state). The remaining members
// recovered so far are listed from the map's mangled names; their bodies are
// still to be written (placeObject, mergeState and instantiateCharacter are the
// big ones).
class AptCIH;
class AptValue;
class AptPseudoDisplayList;
struct AptPseudoCIH_t;
struct AptNativeHash;
class AptRenderingContext;
struct AptCharacter; // `struct` in retail (instantiateCharacter's mangled name spells PAUAptCharacter)
struct AptControlPlaceObject2;
struct BfmeInput1279; // the map's placeholder for the by-pointer depth argument of 0x006F72C0
class EAStringC;
class AptDisplayListState;

class AptDisplayList
{
public:
    AptDisplayList();
    ~AptDisplayList();
    void clear(bool destroyAll);
    void removeClonedObject(AptCIH *clone);
    // 0x006F7230 / 0x006F7290 (the map names them bfmeProcess1279 / bfmeForwardValue1279): detach one list node from its
    // parent's hash and unlink it; the second looks the node up by depth first.
    void bfmeProcess1279(void *item);
    void bfmeForwardValue1279(void *value);
    // 0x006F72C0 (the map names it bfmeForward1279): removes the object at the depth the argument points to; the map spells
    // the parameter as a placeholder struct, the movie's RemoveObject control (AptMovie::doFrameControls) passes &control->value.
    void bfmeForward1279(BfmeInput1279 *depth);
    // 0x006F7720 (the map files it as rva006F7720Sub on AptAnimationPoolData but Apt.cpp calls it with the pool's display list as this):
    // renders/processes the list with a rendering context and a flag.
    void rva006F7720Sub(AptRenderingContext *context, int flag);
    // 0x006F8190: asserts the state exists, clears the list and frees the state (the destructor's body, then the state is nulled).
    void rva006F8190();
    // 0x006F7A30: walks the nodes and runs 0x006E2D60 on the sprite-inst-base and button ones.
    void rva006F7A30();
    // 0x006F79B0: draws the children whose instance word at +4 is negative (the map files it as rva006F79B0 with two untyped parameters:
    // the rendering context and the flag word the child render takes).
    void rva006F79B0(void *context, void *rect);
    void rva006F7AB0(int unused, void *item); // 0x006F7AB0: bfmeProcess1279(item) (the first argument is ignored)
    // 0x006F8EC0 (placeObject): applies one PlaceObject2 control under a parent CIH: a new character is instantiated at the
    // depth, a modify updates the object already there. Returns the CIH, or null when the control places nothing.
    AptCIH *placeObject(AptControlPlaceObject2 *place, AptCIH *parent);
    // 0x006F8B70 (the ledger name placeObjectNCXForm): the core place operation. A null existing CIH
    // creates the character at the depth under the parent (naming it when name is given), otherwise the existing CIH is updated; the matrix,
    // colour pair and actions are optional. Twelve parameters in retail (0x006E1F00 pushes twelve; attachMovie 0x006ED580 passes its init object last).
    AptCIH *placeObjectNCXForm(AptCIH *existing, int depth, AptCharacter *character, EAStringC *name, AptCIH *parent, int unused,
                               int clipDepth, const unsigned int *cxform, const float *matrix, void *actions, float ratio, AptValue *initObject);
    // 0x006F8E10 (the map names it bfmeSubmitColors1283 on BfmeSubmitter1283; the map's name is the ledger's; the name here is this header's): the eleven-parameter form
    // placeObject (0x006F8EC0) calls; it runs the core operation and returns its result.
    AptCIH *placeObjectWithColors(AptCIH *existing, int depth, AptCharacter *character, EAStringC *name, AptCIH *parent, int unused,
                                  int clipDepth, const unsigned int *cxform, const float *matrix, void *actions, float ratio);
    // 0x006F9410 (the ledger name): merges the state a pseudo display list built for a frame into the live list (the hash is the instance's; the flag
    // says whether the target frame is ahead of the current one).
    void mergeState(AptPseudoDisplayList *pseudo, AptNativeHash *hash, bool ahead);
    void rva006F76B0(); // 0x006F76B0 (map name kept); Apt.cpp 0x006CD080 forwards to it
    // 0x006F91F0 (the map name is kept): creates the CIH a pseudo node describes (its name, clip depth, colour pair, matrix and actions through 0x006F8E10) and stamps
    // the node's creation counter into it; 0x006F92D0 runs the character's init actions first, binds the name in the parent's hash and queues the new CIH on the pool;
    // 0x006F9350 replaces an existing CIH by the node, or (an unchanged character) only updates the colours and matrix in place.
    AptCIH *rva006F91F0(AptPseudoCIH_t *item, AptCIH *parent);
    AptCIH *AddToDisplayList(AptNativeHash *hash, AptPseudoCIH_t *item, AptCIH *parent);
    void rva006F9350(AptNativeHash *hash, AptCIH *original, AptPseudoCIH_t *item, AptCIH *parent);
    // 0x006F8220 (the map files it as rva006F8220 on the placeholder class Rva006F8220List): the debug walk of the list: every item must name the given parent (assert `0`,
    // line 0x761), a named defined item must be found in the parent's member hash (by value or by name), and a sprite item's own list is walked under it.
    void validateParents(AptCIH *pParent);

    AptDisplayListState *state; // 0x0
};

typedef char AptDisplayListSize[(sizeof(AptDisplayList) == 4) ? 1 : -1];
