#ifndef FileSystemWatcher_h
#define FileSystemWatcher_h

#include <Path.h>
#include <Map.h>
#include <Mutex.h>
#include <signalslot.h>

class FileSystemWatcher
{
public:
    enum Flag {
        Added = 0x1,
        Removed = 0x2,
        Modified = 0x4
    };
    FileSystemWatcher(unsigned flags);
    ~FileSystemWatcher();

    unsigned flags() const { return mFlags; }
    bool watch(const Path &path);
    bool unwatch(const Path &path);
    signalslot::Signal1<const Path&> &removed() { return mRemoved; }
    signalslot::Signal1<const Path&> &modified() { return mModified; }
private:
    Mutex mMutex;
    static void notifyCallback(int, unsigned int, void *user) { reinterpret_cast<FileSystemWatcher*>(user)->notifyReadyRead(); }
    void notifyReadyRead();
    int mFd;
    Map<Path, int> mWatchedByPath;
    Map<int, Path> mWatchedById;
    signalslot::Signal1<const Path&> mRemoved, mModified;
    const unsigned mFlags;
};
#endif
