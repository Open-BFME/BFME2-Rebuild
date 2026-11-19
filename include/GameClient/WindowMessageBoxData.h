#pragma once
typedef void (*GameWinMsgBoxFunc)();
enum { MSG_BOX_YES=1, MSG_BOX_NO=2, MSG_BOX_CANCEL=4, MSG_BOX_OK=8 };
// gogoMessageBox2C263A allocates16 bytes and writes callbacks in native
// order cancel+C/no+4/ok+8/yes+0. Names and record identity: ZH donor.
struct WindowMessageBoxData {
    GameWinMsgBoxFunc yesCallback, noCallback, okCallback, cancelCallback;
};
