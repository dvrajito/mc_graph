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

class MCGraph(GeomWindow):
    def __init__(self,parent=None,cnf={},**kw):
	apply(GeomWindow.__init__,(self,parent),kw)
	self.geom.set_problem("mc_combine")

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

	self.epsilon_bf = Tkinter.DoubleVar()
	self.epsilon_bf.set(0.1)
        eps_box=makeComboBox(self.UserTop,"Epsilon bf",
                              self.epsilon_bf)
        eps_box.component('listbox').insert(1,0.05,0.01,0.1,0.2,0.5,0.9,1)

        self.iterations_bf=Tkinter.IntVar()
        self.iterations_bf.set(10)
        iter_box=makeComboBox(self.UserTop,"Number of iterations for bf",
                              self.iterations_bf)
        iter_box.component('listbox').insert(1,100,500,1000,5000,10000)

	self.epsilon_tv = Tkinter.DoubleVar()
	self.epsilon_tv.set(0.1)
        eps_box=makeComboBox(self.UserTop,"Epsilon tv",
                              self.epsilon_tv)
        eps_box.component('listbox').insert(1,0.05,0.01,0.005,0.001,0.1,0.2,0.5,0.9,1)

        self.iterations_tv =Tkinter.IntVar()
        self.iterations_tv.set(10)
        iter_box=makeComboBox(self.UserTop,"Number of iterations for tv",
                              self.iterations_tv)
        iter_box.component('listbox').insert(10,50,100,500,1000,5000,10000)
        
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
            self.data = "%s %f %d %f %d %d %d %d %f" %(filename,
                                                       self.epsilon_bf.get(),
                                                       self.iterations_bf.get(),
                                                       self.epsilon_tv.get(),
                                                       self.iterations_tv.get(),
                                                       self.geom_comp.get(),
                                                       self.light_comp.get(),
                                                       self.step.get(),
                                                       self.sphere_factor.get())
            self.geom.set_problem("mc_combine")
            self.geom.file(self.data)
            print "Total time:", time.time() - time1
        elif self.filename.get():
            self.Draw(self.filename.get())
        

if __name__ == '__main__' : MCGraph().mainloop()
