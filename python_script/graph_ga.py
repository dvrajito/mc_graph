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
import string
import time

from ga_conf import *

from geom_window import GeomWindow

def get_font():
    return "6x13"

def makeComboBox(parent,test_str,var):
    combo_box=Pmw.ComboBox(parent,labelpos="w",label_text=test_str,
                           selectioncommand = lambda val,obj=var:obj.set(val),
                           entryfield_entry_width = 5)
    combo_box._entryWidget.config(font=get_font())
    combo_box._list.component('listbox').config(font=get_font())
    combo_box.component('label').config(font=get_font())
    combo_box.component('entryfield').setentry(var.get())
    combo_box.pack()
    return combo_box

class GraphGA(GeomWindow):
    def __init__(self,parent=None,cnf={},**kw):
	apply(GeomWindow.__init__,(self,parent),kw)
	self.geom.set_problem("graph_ga")

    def UserWidgets(self):

	#file selection dialog
	self.filename = Tkinter.StringVar()
	self.filename.set("Problems/DrawGraph/dg10dv.txt")
        button = Tkinter.Button(self.UserTop,text="Select file to open....",
                                command=lambda obj=self:obj.Draw(tkFileDialog.askopenfilename(filetypes=[("graph files", "*.txt"),("all files", "*")])))
        button.pack()
        #redraw with current filename
        button = Tkinter.Button(self.UserTop,text="Redraw",
                                command=lambda obj=self:obj.Draw(obj.filename.get()))
        button.pack()

        some_frame=Tkinter.Frame(self.UserTop)
        some_frame.pack()
        
	self.sphere_factor = Tkinter.DoubleVar()
        self.sphere_factor.set(0.1667)
        line_frame=Tkinter.Frame(self.UserTop)
        line_frame.pack()
        line_typein = Pmw.ComboBox(line_frame,entry_textvariable=self.sphere_factor)
        line_typein.pack(side="right")
        line_label=Tkinter.Label(line_frame,text="Sphere radius factor")
        line_label.pack(side="left")

        #choose the method: breadth first or random
        self.method = Tkinter.IntVar()
	self.method.set(0)
        method_rad1 = Tkinter.Radiobutton(self.UserTop,text="Breadth first")
        method_rad2 = Tkinter.Radiobutton(self.UserTop,text="Random")
        method_rad3 = Tkinter.Radiobutton(self.UserTop,text="Tension vector")
        method_rad1["variable"]=self.method 
        method_rad2["variable"]=self.method 
        method_rad3["variable"]=self.method 
        method_rad1["value"]=0
        method_rad2["value"]=1
        method_rad3["value"]=2
        method_rad1.pack()
        method_rad2.pack()
        method_rad3.pack()

        ga_conf_obj = GAcontrol(self.UserTop)
        print ga_conf_obj.var

	self.epsilon = Tkinter.DoubleVar()
	self.epsilon.set(0.1)
        eps_box=makeComboBox(self.UserTop,"Epsilon",
                              self.epsilon)
        eps_box.component('listbox').insert(1,0.05,0.1,0.2,0.5,0.9,1)

        self.iterations=Tkinter.IntVar()
        self.iterations.set(10)
        iter_box=makeComboBox(self.UserTop,"Number of iterations",
                              self.iterations)
        iter_box.component('listbox').insert(1,100,500,1000,5000,10000)
        
        self.intern=Tkinter.IntVar()
        self.intern.set(1)
        iter_box=makeComboBox(self.UserTop,"Number of interior iterations",
                              self.intern)
        iter_box.component('listbox').insert(1,1,5,10,25,50,100)
        
        self.step=Tkinter.IntVar()
        self.step.set(1)
        step_box=makeComboBox(self.UserTop,"Animation step",
                              self.step)
        step_box.component('listbox').insert(1,1,5,10,25,50,100)
        
	#Control widget for the geometry complexity
	self.geom_comp = Tkinter.IntVar()
	self.geom_comp.set(7)
	slider1 = Tkinter.Scale(some_frame,width=15,length=200)
	slider1["from"] = 1
	slider1["to"] = 9
	slider1["variable"] = self.geom_comp
	slider1["label"] = "Geom Complexity"
	slider1["orient"] = "horizontal"
	slider1["tickinterval"] = 1
	slider1["relief"] = "raised"
	slider1.pack(side="left",padx="2m")

	#Control widget for the light complexity
	self.light_comp = Tkinter.IntVar()
	self.light_comp.set(7)
	slider2 = Tkinter.Scale(some_frame,width=15,length=200)
	slider2["from"] = 1
	slider2["to"] = 9
	slider2["variable"] = self.light_comp
	slider2["label"] = "Light Complexity"
	slider2["orient"] = "horizontal"
	slider2["tickinterval"] = 1
	slider2.pack(side="left")
	slider2["relief"] = "raised"
	slider2.pack(side="left",padx="2m")


    def update_data(self,event):
	self.data = self.entry_contents.get()
    
    def Draw(self,filename=None):
        if filename:
            time1=time.time()
            fn=string.split(filename,'/')
            file_string=fn[len(fn)-1]
            print file_string
            self.UserTop.title(file_string)
            self.filename.set(filename)
            self.data = "%s %f %d %d %d %d %d %d %f" %(filename,
                                                       self.epsilon.get(),
                                                       self.method.get(),
                                                       self.iterations.get(),
                                                       self.intern.get(),
                                                       self.geom_comp.get(),
                                                       self.light_comp.get(),
                                                       self.step.get(),
                                                       self.sphere_factor.get())
            self.geom.set_problem("graph_ga")
            self.geom.file(self.data)
            print "Total time:", time.time() - time1
        elif self.filename.get():
            self.Draw(self.filename.get())
        

if __name__ == '__main__' : GraphGA().mainloop()
