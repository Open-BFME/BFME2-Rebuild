#pragma once
class GameWindow;
// SIZE0xC. Native winSetModal2C182D allocates12; table7FE5B4 has one
// deleting destructor2C1283. Window+4 and next+8 are used by2C12A0.
// name: donor (ZH), corroborated by the manager's modal-stack operations.
// The old Rva002C1283 placeholder denotes this same record.
class ModalWindow {
public:
    virtual ~ModalWindow() {}
    GameWindow *window;
    ModalWindow *next;
};
typedef char ModalWindowSizeCheck[sizeof(ModalWindow)==0xC ? 1 : -1];
