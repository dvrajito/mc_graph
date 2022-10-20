#ifndef AFSTREAM_H
#define ASTREAM_H

#include <iostream>

class afstream : public fstreambase, public ostream {
  public:
    afstream() : fstreambase() { }
    afstream(int fd) : fstreambase(fd) { }
    afstream(int fd, char *p, int l) : fstreambase(fd, p, l) { } /*Deprecated*/
    afstream(const afstream &data): fstreambase() { }
    afstream(const char *name, int mode=ios::app, int prot=0664)
        : fstreambase(name, mode | ios::app, prot) { }
    void open(const char *name, int mode=ios::app, int prot=0664)
        { fstreambase::open(name, mode | ios::app, prot); }
};

#endif
