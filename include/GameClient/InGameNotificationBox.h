#pragma once
class UnicodeString;
class InGameNotificationData;
// Role-inferred identity, original class spelling unproved. Five-slot
// notification interface shared by InGameUI+0x10 and the movie-clip object.
// Concrete table862440 and delegates29B558..29B5D8 establish signatures.
// No virtual destructor. A folded five-purecall table is NOT identity evidence.
class InGameNotificationBox {
public:
    virtual bool isNotificationEnabled() const=0; // 578522 reads byte+50
    virtual void setNotificationEnabled(bool)=0; // 4E6B7D false closes then stores
    virtual void openNotification(const UnicodeString &,const InGameNotificationData &,int)=0;
    virtual void DoClose()=0; // 4E6B9B, name from WB
    virtual int hasActiveNotification()=0; // 4E6882 tests owners+40/+44 and state+8
};
