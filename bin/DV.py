#!/usr/bin/env python
#    Visualization for Bifurcation Manifolds
#    Copyright (C) 1997 Randy Paffenroth and John Maddocks
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU  General Public
#    License as published by the Free Software Foundation; either
#    version 2 of the License, or (at your option) any later version.
#
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Library General Public License for more details.
#
#    You should have received a copy of the GNU Library General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#    MA 02111-1307, USA


import os
import sys
import string
#  This is the startup script for DataViewer 
class DV_setup:
    def __init__(self):
        # This figures out where the problem is installed
        DV_PROBLEM_INSTALL_DIR=os.path.join(os.getcwd(),sys.path[0])
        (DV_PROBLEM_INSTALL_DIR,junk)=os.path.split(DV_PROBLEM_INSTALL_DIR)
        os.environ["DV_PROBLEM_INSTALL_DIR"] = DV_PROBLEM_INSTALL_DIR

        name = os.popen("uname")
        DV_ARCH = name.readline()
        DV_ARCH = string.strip(DV_ARCH)
        name.close()

        if os.environ.has_key("DV"):
            DV_INSTALL_DIR = os.environ["DV"]
        else:
            dvfilename = os.path.join(DV_PROBLEM_INSTALL_DIR,"variables")
            dvfilename = os.path.join(dvfilename,"DV.%s"%(DV_ARCH))
            dv = open(dvfilename)
            DV_INSTALL_DIR = string.strip(dv.readline())

        if os.environ.has_key("DV_GRAPHIC_LIB"):
            DV_GRAPHIC_LIB = os.environ["DV_GRAPHIC_LIB"]
        else:
            if os.environ.has_key("GRAPHIC_LIB"):
                DV_GRAPHIC_LIB = os.environ["GRAPHIC_LIB"]
            else:
                print "Neither DV_GRAPHIC_LIB nor GRAPHIC_LIB set, defaulting to OpenGL"
                DV_GRAPHIC_LIB = "OpenGL"

        if os.environ.has_key("PYTHONPATH"):
            os.environ["PYTHONPATH"] = os.environ["PYTHONPATH"]+\
                                       ":%s/lib/%s_%s:%s/Python_interface/python_script:%s/python_script/continuation_modules:%s/python_script"%(DV_PROBLEM_INSTALL_DIR,
                                                                                                                                                 DV_ARCH,
                                                                                                                                                 DV_GRAPHIC_LIB,
                                                                                                                                                 DV_INSTALL_DIR,
                                                                                                                                                 DV_PROBLEM_INSTALL_DIR,
                                                                                                                                                 DV_PROBLEM_INSTALL_DIR
                                                                                                                                                 )
        else:
            os.environ["PYTHONPATH"] = "%s/lib/%s_%s:%s/Python_interface/python_script:%s/python_script/continuation_modules:%s/python_script"%(DV_PROBLEM_INSTALL_DIR,
                                                                                                                               DV_ARCH,
                                                                                                                               DV_GRAPHIC_LIB,
                                                                                                                               DV_INSTALL_DIR,
                                                                                                                               DV_PROBLEM_INSTALL_DIR,
                                                                                                                               DV_PROBLEM_INSTALL_DIR
                                                                                                                                                )
        self.DV_PROBLEM_INSTALL_DIR = DV_PROBLEM_INSTALL_DIR

if __name__ == "__main__":
    setup = DV_setup()
    arguements = ""
    for arg in sys.argv[1:]:
        arguements = arguements + arg + " "
    
    os.system("python %s"%(arguements,))
        

