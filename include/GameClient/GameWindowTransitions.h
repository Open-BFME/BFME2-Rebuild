#pragma once
class GameWindow;
// Transition, size10: native ctor1DBAA4 and table7DBC10 (8 slots).
// Former Rva001DBAA4 is this base, NOT GameWindowTransitionsHandler.
// Names/field roles: donor(ZH), corroborated by TextOnFrameTransition's
// ctor35FA03, init35FA90, update35FACE and reverse35FB18. BFME2 adds
// slots5 and7 to ZH's surface; their signatures remain unresolved.
class Transition {
public:
    Transition();
    virtual ~Transition();
    virtual void init(GameWindow *) = 0;
    virtual void update(int frame) = 0;
    virtual void reverse() = 0;
    virtual void draw() = 0;
    virtual void unknownTransitionSlot5(); // native folded empty body
    virtual void skip() = 0; // slot6:35F63B calls update(endFrame) unless finished
    virtual void unknownTransitionSlot7(); // native folded return-zero; ABI unknown
    bool isFinished() const { return m_isFinished; }
    int getFrameLength() const { return m_frameLength; }
protected:
    int m_frameLength;
    bool m_isFinished,m_isForward,m_isReversed;
    GameWindow *m_win;
};
typedef char TransitionSizeCheck[sizeof(Transition)==0x10 ? 1 : -1];
// Native allocation35FA39 is18; ctor35FA03 stores frame endpoints10/14;
// table81669C has the same eight slots. Name is native, not an address alias.
class TextOnFrameTransition : public Transition {
public:
    TextOnFrameTransition();
    virtual ~TextOnFrameTransition();
    virtual void init(GameWindow *);
    virtual void update(int frame);
    virtual void reverse();
    virtual void draw() {}
    virtual void skip();
protected:
    int m_startFrame,m_endFrame;
};
typedef char TextOnFrameTransitionSizeCheck[sizeof(TextOnFrameTransition)==0x18 ? 1 : -1];
