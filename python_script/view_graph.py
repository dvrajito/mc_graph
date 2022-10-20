#    DataViewer visualization package
#    Copyright (C) 1997 Randy Paffenroth and Thomas Stone
#
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Library General Public
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
import tkFileDialog
import Tkinter
import Pmw
import graph_handle 
import math
import string

from geom_window import GeomWindow

class ViewGraph(GeomWindow):
    def __init__(self,parent=None,cnf={},**kw):
	apply(GeomWindow.__init__,(self,parent),kw)
	self.geom.set_problem("view_graph")
        self.graph=graph_handle.GraphHandle()
        self.var={}

    def UserWidgets(self):

	#file selection dialog
	self.filename = Tkinter.StringVar()
        button = Tkinter.Button(self.UserTop,text="Select file to open....",
                                command=lambda obj=self:obj.Read(tkFileDialog.askopenfilename()))
        button.pack()
        #redraw with current filename
        button = Tkinter.Button(self.UserTop,text="Redraw",
                                command=lambda obj=self:obj.Draw())
        button.pack()

        some_frame=Tkinter.Frame(self.UserTop)
        some_frame.pack()

	self.var["tube_width"] = Tkinter.DoubleVar()
        self.var["tube_width"].set(0.3)
        width_frame=Tkinter.Frame(self.UserTop)
        width_frame.pack()
        width_typein = Pmw.ComboBox(width_frame,entry_textvariable=self.var["tube_width"])
        width_typein.pack(side="right")
        width_label=Tkinter.Label(width_frame,text="Tube width")
        width_label.pack(side="left")
        
	self.var["line_width"] = Tkinter.IntVar()
        self.var["line_width"].set(2)
        line_frame=Tkinter.Frame(self.UserTop)
        line_frame.pack()
        line_typein = Pmw.ComboBox(line_frame,entry_textvariable=self.var["line_width"])
        line_typein.pack(side="right")
        line_label=Tkinter.Label(line_frame,text="Line width")
        line_label.pack(side="left")
        
	self.var["sphere_radius"] = Tkinter.DoubleVar()
        self.var["sphere_radius"].set(0.03)
        line_frame=Tkinter.Frame(self.UserTop)
        line_frame.pack()
        line_typein = Pmw.ComboBox(line_frame,entry_textvariable=self.var["sphere_radius"])
        line_typein.pack(side="right")
        line_label=Tkinter.Label(line_frame,text="Sphere radius")
        line_label.pack(side="left")
        
	#Control widget for the geometry complexity
	self.var["geom_comp"] = Tkinter.IntVar()
	self.var["geom_comp"].set(7)
	slider1 = Tkinter.Scale(some_frame,width=15,length=200)
	slider1["from"] = 1
	slider1["to"] = 9
	slider1["variable"] = self.var["geom_comp"]
	slider1["label"] = "Geom Complexity"
	slider1["orient"] = "horizontal"
	slider1["tickinterval"] = 1
	slider1["relief"] = "raised"
	slider1.pack(side="left",padx="2m")

	#Control widget for the light complexity
	self.var["light_comp"] = Tkinter.IntVar()
	self.var["light_comp"].set(7)
	slider2 = Tkinter.Scale(some_frame,width=15,length=200)
	slider2["from"] = 1
	slider2["to"] = 9
	slider2["variable"] = self.var["light_comp"]
	slider2["label"] = "Light Complexity"
	slider2["orient"] = "horizontal"
	slider2["tickinterval"] = 1
	slider2.pack(side="left")
	slider2["relief"] = "raised"
	slider2.pack(side="left",padx="2m")

    def update_data(self,event):
	self.data = self.entry_contents.get()

    def Read(self,filename):
        if filename:
            fn=string.split(filename,'/')
            file_string=fn[len(fn)-1]
            print file_string
            self.UserTop.title(file_string)
            self.filename.set(filename)
            self.graph.read_hc_graph(filename)
            if (self.graph.nr_nodes > 10):
                self.graph.make_cluster(int(math.ceil(math.sqrt(self.graph.nr_nodes))))
            if self.graph.cluster:
                self.graph.make_points_layered()
            else:
                self.graph.make_points_simple()
            self.var["points"]=self.graph.points
            self.var["arcs"]=self.graph.graph
            self.var["radius"]=self.graph.first_radius
            self.var["hc"]=self.graph.hc
            self.Draw()
    
    def Draw(self):
	self.var["light"]=self.var["light_comp"].get()
	self.var["geom"]=self.var["geom_comp"].get()
	self.var["line"]=self.var["line_width"].get()
        self.var["tube"]=self.var["tube_width"].get()
        self.var["sphere"]=self.var["sphere_radius"].get()
        data_str=`id(self.var)`
        self.geom.set_problem("view_graph")
        self.geom.file(data_str)

if __name__ == '__main__' : ViewGraph().mainloop()
