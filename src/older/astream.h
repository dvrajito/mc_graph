#ifndef AFSTREAM_H
#define ASTREAM_H

class afstream : public fstreambase, public iostream {
  public:
    afstream() : fstreambase() { }
    afstream(int fd) : fstreambase(fd) { }
    afstream(int fd, char *p, int l) : fstreambase(fd, p, l) { } /*Deprecated*/
    afstream(const char *name, int mode=ios::app, int prot=0664)
        : fstreambase(name, mode, prot) { }
    void open(const char *name, int mode=ios::app, int prot=0664)
        { fstreambase::open(name, mode, prot); }
};

#endif
