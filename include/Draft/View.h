// The tactical view. TheTacticalView at VA 0x00DFEA3C is a View pointer, and W3DView derives from View.
// Target facts (code RVAs, VA 0x00BF6178 table):
//  - Snapshot-derived. The constructor 0x0025EDE6 stores the table, a 0 at +4, an id from the
//    counter at VA 0x00DBA970 at +8 and initialises the members up to +0xB0; W3DView's constructor
//    0x0008B7CF calls it and then puts a SubsystemInterface at +0xB4 (table VA 0x00BC7530), so the
//    base is 0xB4 bytes. 153 slots (VA 0x00BF6178, the table the base constructor stores).
//  - xfer 0x0025F020 (version 3) reads/writes the members listed below through the Xfer slots;
//    it restores the angle through slot 63, the pitch/position through further View slots.
//  - Slot names are those of W3DView's overrides in the map (real names), View's own rows, or the
//    xfer / zoomIn bodies (stated per slot); every other slot is declared with its table target
//    and no invented name. Pure slots are the ones whose base entry is __purecall (0x0003B810).
//  - TheTacticalView is the only View in the game; "TacticalView" in the draft list is this class.
#pragma once
#ifndef BFME2_VIEW_H
#define BFME2_VIEW_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Coord3D.h"
#include "Common/Coord2D.h"
#include "Common/ICoord3D.h"

struct CameraMarker;
struct CameraMarkerHead
{
	CameraMarker *m_first;
	CameraMarkerHead &operator=(CameraMarker *first) { m_first = first; return *this; }
};
class Drawable;
class Waypoint;
class Xfer;
struct ICoord2D;
class AsciiString;
struct IRegion2D;

// A saved camera placement (Zero Hour's ViewLocation: valid flag, position, angle, pitch, zoom; BFME 2 adds one more float).
// Size 0x20. The out-of-line constructor 0x0029B2E7 stores the byte at +0 and the floats at +4,+8,+0xC then +0x1C,+0x18,+0x14,+0x10
// (the order of a chained `a = b = c = d = 0`); InGameUI::resetCamera 0x0029B4F9 hands one to View slot 92 (getLocation).
struct ViewLocation
{
	ViewLocation();				// 0x0029B2E7 (row in InGameUI.cpp)
	bool m_valid;				// +0x00
	Coord3D m_pos;				// +0x04
	Real m_angle;				// +0x10
	Real m_pitch;				// +0x14
	Real m_zoom;				// +0x18
	Real m_unk0x1C;				// +0x1C
};

class View : public Snapshot
{
public:
	// Zero Hour's View::WorldToScreenReturn (retail's worldToScreenTriReturn 0x0008619F mangles it View::WorldToScreenReturn)
	enum WorldToScreenReturn { WTS_INSIDE_FRUSTUM = 0, WTS_OUTSIDE_FRUSTUM, WTS_INVALID };
	// Zero Hour's View::CameraShakeType (the retail mangling of shake() names it View::CameraShakeType); the values index the six ShakeXIntensity fields of GlobalData (+0xB74 .. +0xB88) in W3DView::shake 0x00087452.
	enum CameraShakeType
	{
		SHAKE_SUBTLE = 0,
		SHAKE_NORMAL,
		SHAKE_STRONG,
		SHAKE_SEVERE,
		SHAKE_CINE_EXTREME,
		SHAKE_CINE_INSANE
	};
	View();	// 0x0025EDE6
	// The marker list at +0x80 (role names; the map still spells the rows as members of an address-named list class: `this` is the View).
	void exchangeLink(void **link);							// 0x0025ED7D (the ledger's name: stores the list head into *link and makes link the head, i.e. pushes a marker on the front)
	CameraMarker *findCameraMarker(const AsciiString &name) const;	// 0x0025F385: the marker with that name, or 0 (retail's `this` is the View: both read the list head at +0x80; ScriptActions 0x003BAAE4 passes TheTacticalView; the map still spells it find@CameraMarkerList)
	void clearCameraMarkers();									// 0x0025F30D: pops and destroys every marker (the map still spells it clear@CameraMarkerList)
	CameraMarker *replaceCameraMarker(CameraMarker *marker);	// 0x0025F342 (swaps in the marker with the same name, returns the old one or 0)
	void removeCameraMarker(const CameraMarker *marker);		// 0x0025F3AE (unlinks and deletes the marker with the same name)

	virtual ~View();							// slot 0, deleting dtor 0x0025EFF1
protected:
	virtual void loadPostProcess() {}			// slot 1, the shared empty body 0x000B3FD0
public:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0008BC8D
protected:
	virtual void xfer(Xfer *xfer);			// slot 3, View::xfer 0x0025F020
public:
	virtual void init();	// slot 4, target 0x0025F00D; W3DView::init 0x0008C224 overrides it and SubsystemInterface::init (the +0xB4 table entry 0x0008DE53 is its this-adjusting thunk); Zero Hour's View::init
	virtual void reset();	// slot 5, target 0x0025F3F3; W3DView::reset 0x00085947 starts with a call to it and overrides SubsystemInterface::reset through the thunk 0x0008BD4E; Zero Hour's View::reset
	virtual void slot6();	// slot 6, target 0x0030F45F (name and signature not evidenced)
	virtual void slot7();	// slot 7, target 0x002B2420 (name and signature not evidenced)
	virtual Bool slot8();	// slot 8, target 0x0008BB0A: returns whether the zoom limits are active
	virtual Drawable * pickDrawable(const ICoord2D *screen, Bool forceAttack, Int pickType) = 0;	// slot 9, W3DView 0x0008B1E5
	virtual Int iterateDrawablesInRegion(IRegion2D *screenRegion, Bool (*callback)(Drawable *, void *), void *userData) = 0;	// slot 10, W3DView 0x00086245
	virtual void slot11();	// slot 11, target 0x000D1407 (name and signature not evidenced)
	virtual void drawDebugCircle(const Coord3D *position, Real radius, UnsignedInt color);	// slot 12, target 0x000D1407 (a shared `ret 0xC` stub: three stack arguments; role name, evidence ActiveBody 0x004BF9ED: `TheTacticalView->slot12(&pos, radius, 0xFFFF8000)` when a debug flag is set; f14 request 20:26)
	virtual void getScreenCornerWorldPointsAtZ(Coord3D *topLeft, Coord3D *topRight, Coord3D *bottomLeft, Coord3D *bottomRight, Real z);	// slot 13, View 0x0025EC22
	virtual void setWidth(Int width);	// slot 14, W3DView 0x00087B81
	virtual Int getWidth();	// slot 15, target 0x002A79A1; returns Int, added to the origin in getScreenCornerWorldPointsAtZ 0x0025EC22
	virtual void setHeight(Int height);	// slot 16, W3DView 0x00087AF4
	virtual Int getHeight();	// slot 17, target 0x0030F2C7; returns Int, added to the origin in getScreenCornerWorldPointsAtZ 0x0025EC22
	virtual void setOrigin(Int x, Int y);	// slot 18, W3DView 0x00087C55
	virtual void getOrigin(Int *x, Int *y);	// slot 19, View 0x0008BB0E
	virtual void forceRedraw() = 0;	// slot 20, W3DView 0x0008D134
	virtual void lookAt(const Coord3D *o);	// slot 21, View 0x0025EA7C; xfer 0x0025F020 calls it with the loaded position
	virtual void initHeightForMap();	// slot 22, W3DView 0x0008D807
	virtual Int scrollBy(Coord2D *delta);	// slot 23 (returns 0: View 0x0025EAD5 ends `xor eax,eax; ret 4`), target 0x0025EAD5; Zero Hour scrollBy: ScriptActions 0x003BACCA pushes the address of a stack {float,float} (f07 06:40 post)
	virtual void moveCameraTo(const Coord3D *position, CameraMarker *marker, Int milliseconds, Int shutter, Real easeIn, Real easeOut);	// slot 24, target 0x0025EEF5 (a six-argument stub, `ret 0x18`); Zero Hour's name; ScriptActions 0x003BAAE4 passes the marker found by name (View +0x80 list, lookup 0x0025F385 whose this is the View) as the second argument, 0x003BAD15 passes 0 for it and for the milliseconds
	virtual void moveCameraAlongWaypointPath(Waypoint *way, Int milliseconds, Int shutter, Real easeIn, Real easeOut, Real unk, Int mode);	// slot 25, target 0x0025EF01 (an empty body, `ret 0x1C`: seven arguments); Zero Hour's name, BFME 2 has three Reals and a trailing Int (ScriptActions 0x003BB0C7 calls it with a waypoint record, the milliseconds, 1, three scaled Reals and an Int)
	virtual void slot26(Int value);	// slot 26, target 0x0047A69C (name and signature not evidenced)
	virtual Bool slot27(Int waypointId);	// slot 27, target 0x005CB9FF (`xor al,al; ret 4`); ScriptConditions 0x003E4EA5 passes the waypoint id and tests the result (name not evidenced)
	virtual void slot28(Int value);	// slot 28, target 0x0047A69C (name and signature not evidenced)
	virtual void slot29(Int value);	// slot 29, target 0x0047A69C (name and signature not evidenced)
	virtual Bool isCameraMovementFinished();	// slot 30, target 0x0050B5C6 (`mov al,1; ret`); ScriptConditions 0x003EA9AF returns it straight; W3DView 0x00086A1E reports that no camera record is running; Zero Hour's name
	virtual void cameraModFinalZoom(Real finalZoom, Real easeIn, Real easeOut);	// slot 31, W3DView 0x000865DB
	virtual void cameraModRollingAverage(Int framesToAverage);	// slot 32, target 0x0047A69C (a `ret 4` stub); W3DView 0x0008690A clamps the argument to >= 1 and stores it at +0x2A8 (Zero Hour: rollingAverageFrames); Zero Hour's name
	virtual void cameraModFinalTimeMultiplier(Int finalMultiplier);	// slot 33, target 0x0047A69C (a `ret 4` stub); W3DView 0x00086812 stores the argument as the end time multiplier of every running camera record; Zero Hour's name
	virtual void cameraModFinalPitch(Real finalPitch, Real easeIn, Real easeOut);	// slot 34, W3DView 0x0008691F
	virtual void cameraModFreezeTime();	// slot 35, target 0x000B3FD0 (the empty body); W3DView 0x0008BC93 sets the byte at +0x23D0; Zero Hour's name
	virtual void cameraModFreezeAngle();	// slot 36, W3DView 0x00086702
	virtual void cameraModLookToward(Coord3D *pLoc);	// slot 37, W3DView 0x000890FE
	virtual void cameraModFinalLookToward(Coord3D *pLoc);	// slot 38, W3DView 0x000892F0
	virtual void cameraModFinalMoveTo(Coord3D *pLoc);	// slot 39, target 0x0047A69C; Zero Hour name, one Coord3D* (ScriptActions 0x003BAE04)
	virtual void cameraEnableSlaveMode(const AsciiString &thingTemplateName, const AsciiString &boneName);	// slot 40, target 0x0050B238; Zero Hour name; ScriptActions 0x003BCADA jmps to it with its two string arguments
	virtual void cameraDisableSlaveMode();	// slot 41, target 0x000B3FD0; Zero Hour name; ScriptActions 0x003BCAE8 jmps to it
	virtual void Add_Camera_Shake(const Coord3D &position, Real radius, Real duration, Real power);	// slot 42, W3DView 0x000875F2
	virtual void slot43();	// slot 43, target 0x000D43D0 (name and signature not evidenced)
	virtual Int getViewFilterType();	// slot 44, target 0x000D43D0 (returns 0); Zero Hour getViewFilterType: ScriptActions 0x003BBE44 compares it with 1
	virtual Bool setViewFilterMode(Int filterMode);	// slot 45, target 0x005CB9FF (one stack argument, returns 0); Zero Hour setViewFilterMode: ScriptActions 0x003BBE44/0x003BBE91 push 1 and 1
	virtual void slot46(const Coord3D *position, Int mode);	// slot 46, target 0x0050B238 (`ret 8`); ScriptActions 0x003BB7FE passes the waypoint position and 0 (name not evidenced)
	virtual Bool setViewFilter(Int filter);	// slot 47, target 0x005CB9FF; Zero Hour setViewFilter: ScriptActions 0x003BBE44 pushes 1, 0x003BBE91 pushes 6
	virtual void setFadeParameters(Int fadeFrames, Int direction);	// slot 48, W3DView 0x00085EC7
	virtual void set3DWireFrameMode(Bool enable);	// slot 49, target 0x0047A69C (a `ret 4` stub); W3DView 0x00085F05 stores it at +0x115 (Zero Hour: m_nextWireFrameEnabled); Zero Hour's set3DWireFrameMode
	virtual void resetCamera(const Coord3D *position, Int milliseconds, Real easeIn, Real easeOut);	// slot 50, target 0x001F01E8 (Zero Hour's resetCamera; evidence InGameUI::resetCamera 0x0029B4F9: [view+0xC8](&pos, 1, 0.0f, 0.0f))
	virtual void rotateCamera(Real rotations, Int milliseconds, Bool stopAtEnd, Real easeIn, Real easeOut);	// slot 51, W3DView 0x0008647C
	virtual void rotateCameraTowardObject(ObjectID id, Int milliseconds, Int holdMilliseconds, Real easeIn, Real easeOut, Real unk);	// slot 52, W3DView 0x0008652B
	virtual void rotateCameraTowardPosition(const Coord3D *pLoc, Int milliseconds, Real easeIn, Real easeOut, Bool reverseRotation);	// slot 53, W3DView 0x00088D65
	virtual Bool isTimeFrozen();	// slot 54, target 0x0047A699 (`xor al,al; ret`); W3DView 0x0008BC9B returns the byte at +0x23D0 that slot 35 sets; Zero Hour's name
	virtual Int getTimeMultiplier();	// slot 55, target 0x000910FB; W3DView 0x0008BCA2 returns the int at +0x23D4, which the camera-move setters copy into their time-multiplier fields; Zero Hour's name
	virtual void setTimeMultiplier(Int multiple);	// slot 56, target 0x0047A69C (a `ret 4` stub); W3DView 0x0008BCA9 stores it at +0x23D4; Zero Hour's name
	virtual void setDefaultView(Real pitch, Real angle, Real maxHeight);	// slot 57, target 0x000D1407 (ret 0xC); Zero Hour setDefaultView: ScriptActions 0x003BC22E forwards its three Reals
	virtual Real slot58();	// slot 58, target 0x002BFBF0 (name and signature not evidenced)
	virtual void slot59(Real value, Int milliseconds, Real easeIn, Real easeOut);	// slot 59, target 0x001F01E8 (`ret 0x10`); ScriptActions::doZoomCamera 0x003BAB7A (name not evidenced)
	virtual void slot60(Real value, Int milliseconds, Real easeIn, Real easeOut);	// slot 60, target 0x001F01E8 (`ret 0x10`); ScriptActions 0x003BABCB (name not evidenced)
	virtual void zoomCamera(Real finalZoom, Int milliseconds, Real easeIn, Real easeOut);	// slot 61, W3DView 0x00088FF6
	virtual void pitchCamera(Real finalPitch, Int milliseconds, Real easeIn, Real easeOut);	// slot 62, W3DView 0x0008907A
	virtual void setAngle(Real angle);	// slot 63, W3DView 0x0008D139
	virtual Real getAngle();	// slot 64, xfer 0x0025F020 reads it through slot 64 and restores it through slot 63; field +0x28
	virtual void setPitch(Real pitch);	// slot 65, W3DView 0x0008D1DD
	virtual Real getPitch();	// slot 66, W3DView target 0x00094A9C; Zero Hour's name: View::getLocation 0x0025EB43 reads the pitch through it
	virtual void slot67(Real value);	// slot 67, target 0x004583AF (a shared `ret 4` stub); View::setLocation 0x0025EBB9 passes the ViewLocation's extra float (name not evidenced)
	virtual Real slot68() const;	// slot 68, target 0x0008BB23: returns the float at +0x30; role not evidenced
	virtual void setAngleAndPitchToDefault();	// slot 69, target 0x0025EB36 (copies +0x48 / +0x4C into the angle and pitch); Zero Hour's name
	virtual void getPosition(Coord3D *pos);	// slot 70, View 0x0008BB27; xfer 0x0025F020 reads it through slot 70
	virtual void slot71() = 0;	// slot 71, target 0x0003B810 (name and signature not evidenced)
	virtual void slot72() = 0;	// slot 72, target 0x0003B810 (name and signature not evidenced)
	virtual Real getZoom();	// slot 73, W3DView target 0x0008E10E; Zero Hour's name: View::getLocation 0x0025EB43 reads the zoom through it
	virtual void setZoom(Real zoom);	// slot 74, W3DView 0x0008D3C1
	virtual Real getHeightAboveGround();	// slot 75, zoomIn 0x0025EA2E reads it through slot 75 and writes the result through slot 76
	virtual void setHeightAboveGround(Real height);	// slot 76, W3DView 0x0008D2B8
	virtual void zoomIn();	// slot 77, View 0x0025EA2E
	virtual void zoomOut();	// slot 78, View 0x0025EA55
	virtual void slot79();	// slot 79, target 0x000B3FD0 (name and signature not evidenced)
	virtual void slot80();	// slot 80, target 0x004987FA (name and signature not evidenced)
	virtual void slot81(Bool value);	// slot 81, target 0x0008BB3C: stores the byte at +0x75
	virtual Real slot82();	// slot 82, target 0x0008BB46: returns the float at +0x54
	virtual void slot83(Real value);	// slot 83, target 0x0008BB4A: stores the float at +0x54
	virtual Real slot84();	// slot 84, target 0x0008BB58: returns the float at +0x50
	virtual void slot85(Real value);	// slot 85, target 0x0008BB5C: stores the float at +0x50
	virtual void setFieldOfView(Real fov);	// slot 86, View 0x00086191
	virtual Real getFieldOfView();	// slot 87, target 0x0008BD0F: returns the float at +0x6C that setFieldOfView (slot 86) stores
	virtual WorldToScreenReturn worldToScreenTriReturn(const Coord3D *w, ICoord2D *s) = 0;	// slot 88, W3DView 0x0008619F
	// 0x00050F29: true when the point is inside the frustum (the tri-state result is 0).
	Bool worldToScreen(const Coord3D *w, ICoord2D *s) { return (Int)worldToScreenTriReturn(w, s) == 0; }
	virtual void slot89() = 0;	// slot 89, target 0x0003B810 (name and signature not evidenced)
	virtual Bool screenToTerrain(const ICoord2D *screen, Coord3D *world, Bool flag) = 0;	// slot 90, W3DView 0x0008DE7A
	virtual void screenToWorldAtZ(const ICoord2D *screen, Coord3D *world, Real z) = 0;	// slot 91, W3DView 0x0008A0E5
	virtual void getLocation(ViewLocation *location);	// slot 92, target 0x0025EB43 (Zero Hour's getLocation; evidence InGameUI::resetCamera 0x0029B4F9: [view+0x170](&currentView))
	virtual void setLocation(const ViewLocation *location);	// slot 93, target 0x0025EBB9 (Zero Hour's setLocation: when the record is valid it restores position, angle, pitch, zoom and redraws)
	virtual void slot94() = 0;	// slot 94, target 0x0003B810 (name and signature not evidenced)
	virtual void slot95() = 0;	// slot 95, target 0x0003B810 (name and signature not evidenced)
	virtual void slot96();	// slot 96, target 0x003F3FCE (name and signature not evidenced)
	virtual void slot97(ObjectID id);	// slot 97, target 0x000851B7; ScriptActions 0x003BAA46 / 0x003BC1B2 pass the ObjectID (+0x74) of the unit to follow (name not evidenced)
	virtual void slot98();	// slot 98, target 0x0008BB6A; no argument: sets the byte at +0x76 (name not evidenced)
	virtual void slot99(Int mode, Real value);	// slot 99, target 0x000851CD; (Int, Real) as ScriptActions 0x003BAA46 calls it with (0, 0.0f) (name not evidenced)
	virtual void slot100(Real value);	// slot 100, target 0x0008BB6F; one Real (name not evidenced)
	virtual Real slot101();	// slot 101, target 0x0008BB7D: returns the float at +0x68 that slot 100 stores
	virtual DrawableID slot102();	// slot 102, target 0x0008BB81: returns the DrawableID at +0x5C
	virtual void slot103(Int id);	// slot 103, target 0x003FF0E7; one Int (name not evidenced)
	virtual void slot104();	// slot 104, target 0x000851E2 (name and signature not evidenced)
	virtual void slot105(Bool value);	// slot 105, target 0x0008BB85: stores the byte at +0x74
	virtual Bool slot106();	// slot 106, target 0x0008BB8F: returns the byte at +0x74
	virtual void shake(const Coord3D *epicenter, CameraShakeType type);	// slot 107, W3DView 0x00087452
	virtual Real slot108();	// slot 108, target 0x0008BD27: returns the float at +0x70
	virtual void slot109(Real value);	// slot 109, target 0x0008BB93: stores the float at +0x70
	virtual void slot110();	// slot 110, target 0x000B3FD0 (name and signature not evidenced)
	virtual void slot111(Coord2D *value) = 0;	// slot 111, target 0x0003B810; ScriptActions 0x003BCC08 passes the address of a stack {float,float} (name not evidenced)
	virtual Coord2D *slot112() = 0;	// slot 112, target 0x0003B810 (name and signature not evidenced)
	virtual void slot113();	// slot 113, target 0x005DAA6C (name and signature not evidenced)
	virtual void slot114(const AsciiString &markerName);	// slot 114, target 0x0047A69C (name and signature not evidenced)
	virtual Bool slot115();	// slot 115, target 0x0010857B; Mouse 0x001EDE9C tests the result (name not evidenced)
	virtual void slot116();	// slot 116, target 0x001F01E8 (name and signature not evidenced)
	virtual void slot117();	// slot 117, target 0x0021914D (name and signature not evidenced)
	virtual void slot118();	// slot 118, target 0x001F34E9 (name and signature not evidenced)
	virtual void slot119();	// slot 119, target 0x000B3FD0 (name and signature not evidenced)
	virtual void slot120(Int a, Int b);	// slot 120, target 0x0008BBA1: stores the first two ints of the ICoord3D at +0x8C
	virtual void slot121();	// slot 121, target 0x0008BBB8: sets the first two ints of the ICoord3D at +0x8C to -1
	virtual Bool slot122();	// slot 122, target 0x0008BBC7: true when neither of those two ints is -1
	virtual ICoord3D *slot123(ICoord3D *out);	// slot 123, target 0x0008BBE0: copies the ICoord3D at +0x8C into *out and returns out
	virtual void slot124();	// slot 124, target 0x0047A69C (name and signature not evidenced)
	virtual void slot125();	// slot 125, target 0x0047A69C (name and signature not evidenced)
	virtual void slot126();	// slot 126, target 0x004647D1 (name and signature not evidenced)
	virtual void slot127(Real value);	// slot 127, target 0x0008BBF6 (name and signature not evidenced)
	virtual void slot128();	// slot 128, target 0x0008BC2B: zeroes the float at +0x98
	virtual Real slot129();	// slot 129, target 0x0008BC37: returns the float at +0x98
	virtual Bool slot130();	// slot 130, target 0x0047A699 (name and signature not evidenced)
	virtual void slot131(Bool value);	// slot 131, target 0x0047A69C (name and signature not evidenced)
	virtual void slot132(Real a, Real b);	// slot 132, target 0x0008BC3E: stores the floats at +0xAC and +0xB0
	virtual Real slot133(Real value);	// slot 133, target 0x0025ED90: forwards to fovDegreesToFocalLength (name not evidenced)
	virtual Real slot134(Real value);	// slot 134, target 0x0025EDA1: forwards to fovToFocalLength (name not evidenced)
	virtual Real slot135(Real value);	// slot 135, target 0x0025EDB2: forwards to focalLengthToFovDegrees (name not evidenced)
	virtual Real slot136(Real value);	// slot 136, target 0x0025EDC3: forwards to focalLengthToFov (name not evidenced)
	virtual Bool slot137(const AsciiString &name);	// slot 137, target 0x0047A69C (a shared stub); GameClient::loadPostProcess 0x00239A9D calls it with the string at GameClient +0xBC (name not evidenced)
	virtual void slot138();	// slot 138, target 0x000B3FD0 (name and signature not evidenced)
	virtual Real slot139();	// slot 139, target 0x0008BC5D: returns the float at +0x9C
	virtual Real slot140();	// slot 140, target 0x0008BC64: returns the float at +0xA4
	virtual void slot141(Real value);	// slot 141, target 0x0008BC6B: stores the float at +0x9C
	virtual void slot142(Real value);	// slot 142, target 0x0008BC7C: stores the float at +0xA4
	virtual Real slot143();	// slot 143, TheTacticalView slot read by ScriptConditions 0x003E4ED5 (compared with a condition Real); W3DView field +0x24F4; name not evidenced
	virtual Real slot144();	// slot 144, ScriptConditions 0x003E4F05 (condition Real in radians, so an angle); W3DView field +0x24F8; name not evidenced
	virtual Real slot145();	// slot 145, ScriptConditions 0x003E4F3B; W3DView field +0x24FC; name not evidenced
	virtual Bool slot146();	// slot 146, ScriptConditions 0x003E4F6B (a bool-returning tail call); W3DView byte +0x2500; name not evidenced
	virtual void slot147(Bool flag);	// slot 147, target 0x0047A69C; ScriptActions 0x003BCFE7 forwards its Bool with a jmp (name not evidenced)
	virtual void slot148(Bool flag);	// slot 148, target 0x0047A69C; ScriptActions 0x003BCFF5 (name not evidenced)
	virtual void slot149(Bool flag);	// slot 149, target 0x0047A69C; ScriptActions 0x003BD003 (name not evidenced)
	virtual void slot150(Bool flag);	// slot 150, target 0x0047A69C; ScriptActions 0x003BD011 (name not evidenced)
	virtual void slot151();	// slot 151, target 0x0056DE1F (name and signature not evidenced)
	virtual View *getNextView();	// slot 152, target 0x005801F2: `mov eax,[ecx+4]; ret`, the next view of the display's list (Display::reset 0x0025D9E3 walks the list with it); Zero Hour's name

	View *m_next;				// +0x04 (0 in the constructor)
	UnsignedInt m_id;			// +0x08 (the global view counter)
	Coord3D m_pos;				// +0x0C (the position slot 70 returns; lookAt 0x0025EA7C writes it)
	Int m_width;				// +0x18 (lookAt 0x0025EA7C centres the position on it)
	Int m_height;				// +0x1C (lookAt 0x0025EA7C centres the position on it)
	Int m_originX;				// +0x20 (constructor 0x0025EDE6 zeroes it; xfer 0x0025F020 reads the four ints below version 3)
	Int m_originY;				// +0x24
	Real m_angle;				// +0x28 (slots 63 / 64)
	Real m_pitchAngle;			// +0x2C (setPitch 0x0025EB09 clamps it to +-PI/5)
	// The members below are typed by the constructor 0x0025EDE6 (its store widths and initial values) and
	// by View::xfer. Role names follow the camera operations and the corresponding Zero Hour fields.
	Real m_scriptedPitchAngle;       // +0x30  pitchCamera and pitchOneFrame interpolate this angle separately from m_pitchAngle.
	Real m_maxZoom;                  // +0x34  W3DView::setZoom clamps to this upper bound.
	Real m_minZoom;                  // +0x38  W3DView::setZoom clamps to this lower bound.
	Real m_zoom;                     // +0x3C  W3DView::setZoom stores the unscaled zoom used by getZoom.
	Real m_heightAboveGround;        // +0x40  View::setHeightAboveGround stores the requested height here.
	Bool m_zoomLimited;              // +0x44  W3DView::setZoom applies the bounds when this flag is set.
	Real m_defaultAngle;             // +0x48  View::setAngleAndPitchToDefault restores the angle from this field.
	Real m_defaultPitchAngle;        // +0x4C  View::setAngleAndPitchToDefault restores the pitch from this field.
	Real m_currentHeightAboveGround; // +0x50  Height cache in the matching Zero Hour View layout.
	Real m_terrainHeightUnderCamera; // +0x54  Terrain-height cache in the matching Zero Hour View layout.
	ObjectID m_cameraLockObjectID;   // +0x58  The scripted tether sets this ID from the followed object.
	DrawableID m_cameraLockDrawableID; // +0x5C  The scripted tether tracks both the object and its drawable.
	Int m_cameraLockMode;            // +0x60  Setting the lock object clears the mode. Setting the scripted lock updates it.
	Real m_cameraLockDistance;       // +0x64  Zero Hour lock distance. The lock-object setter resets it alongside the mode.
	Real m_unk0x68;				// +0x68 float, xfer'd
	Real m_fieldOfView;              // +0x6C  The view accessors use radians. The animation converts degrees before storing.
	Real m_fxPitch;                  // +0x70  The effect-pitch step interpolates this scale. Real-zoom mode resets it to 1.
	Bool m_mouseLocked;              // +0x74  Corresponds to the Zero Hour mouse-lock get/set pair.
	Bool m_okToAdjustHeight;         // +0x75  Corresponds to the Zero Hour height-adjustment setter and defaults to true.
	Bool m_snapCameraToTarget;       // +0x76  The scripted tether sets this flag when snapToUnit is true.
	Coord2D m_guardBandBias;         // +0x78  The view expands drawable culling bounds by this extra margin.
	CameraMarkerHead m_markers;	// +0x80 head of the singly linked marker list (xfer 0x0025F020 counts and walks it; 0x0025F30D clears it, 0x0025F385 finds by name)
	Bool m_unk0x84;				// +0x84 bool, xfer'd
	ObjectID m_unk0x88;			// +0x88 xfer'd through XferObjectID
	ICoord3D m_unk0x8C;			// +0x8C three ints, xfer'd through the ICoord3D slot
	Real m_unk0x98;				// +0x98 float, xfer'd
	Real m_unk0x9C;				// +0x9C float, 1.0 in the constructor
	Real m_cameraOffsetScale;        // +0xA0  setAngleAndPitchToDefault scales the horizontal camera offset by this value.
	Real m_unk0xA4;				// +0xA4 float, 1.0
	Real m_zoomScale;                // +0xA8  W3DView::getZoom multiplies the stored zoom by this scale.
	Real m_unk0xAC;				// +0xAC float, xfer'd
	Real m_unk0xB0;				// +0xB0 float, xfer'd (the last transfer)
};
// SIZE 0xB4
typedef View::CameraShakeType CameraShakeType;	// the older callers (FXList.cpp, ScriptActions.cpp) spell it without the class
typedef char ViewSize[(sizeof(View) == 0xB4) ? 1 : -1];
typedef char ViewIdOffset[(offsetof(View, m_id) == 0x08) ? 1 : -1];
typedef char ViewPosOffset[(offsetof(View, m_pos) == 0x0C) ? 1 : -1];
typedef char ViewAngleOffset[(offsetof(View, m_angle) == 0x28) ? 1 : -1];

extern View *TheTacticalView;

#endif
