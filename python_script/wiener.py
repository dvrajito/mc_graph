#!/usr/bin/python
import os
import os.path
import string
import sys
    
def multi_wiener(path, pattern, resfile):
    outfile = open(resfile, "a")
    if not outfile:
        outfile = open(resfile, "w")
    outfile.write("\n\nGraph file\tReal Wiener\tInteger Wiener\n\n")
    #outfile.write("\n\nGraph file\tNumber of triangles\n\n")
    outfile.close()
    names=os.listdir(path)
    total = len(names)
    names.sort()
    for name in names:
        match = 0
        if len(pattern) == 0:
            match = 1
        elif len(pattern) == 1 and pattern == "*":
            match = 1
        elif pattern[0] == "*":
            source = string.split(pattern, "*")[1]
            if string.find(name, source)>=0:
                match = 1
        elif string.find(name, pattern)==0:
            match = 1
        if match == 1 and name[len(name)-1] != "~":
            file=os.path.join(path, name)
            cmd = "../bin/wiener %s %s" %(file, resfile)
            os.system(cmd)
 
if __name__ == '__main__' :
    path = "./"
    pattern = "*"
    resfile = None
    sx = -1
    sy = -1
    ex = -1
    ey = -1
    argv=sys.argv
    if len(argv)>1 and (argv[1] == "--help" or argv[1] =="--h"):
        print "wiener.py path filename resultfile"
    if len(argv)>1:
        path = argv[1]
    if len(argv)>2:
        pattern = argv[2]
    if len(argv)>3:
        resfile = argv[3]
    multi_wiener(path, pattern, resfile)
