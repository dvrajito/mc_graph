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
import os.path
import filenames

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

def makeSpace(parent):
    blank_space=Tkinter.Label(parent,font = "6x9", text=" ")
    blank_space.pack()

class MCGraph(GeomWindow):
    def __init__(self,parent=None,cnf={},**kw):
	apply(GeomWindow.__init__,(self,parent),kw)
	self.geom.set_problem("mc_graph")

    def UserWidgets(self):
        # Variables to be used in a separate dialog (Display).
	self.geom_comp = Tkinter.IntVar()
	self.geom_comp.set(7)
	self.light_comp = Tkinter.IntVar()
	self.light_comp.set(7)
	self.sphere_factor = Tkinter.DoubleVar()
        self.sphere_factor.set(0.1667)
        
        aframe = Tkinter.Frame(self.UserTop)
        aframe.pack(fill='x')
        aframe1=Tkinter.Frame(aframe)
        aframe1.pack(side="left",anchor="w")
	#file selection dialog
	self.filename = Tkinter.StringVar()
	self.filename.set("Problems/DrawGraph/dg10dv.txt")
        self.file_label = Tkinter.Label(aframe1,text="Graph file: dg10dv.txt")
        self.file_label.pack(anchor="w")
        button = Tkinter.Button(aframe1,text="Set graph file...",
                                command=self.setGraphFile)
        button.pack(anchor="w")

        aframe1=Tkinter.Frame(aframe)
        aframe1.pack(side="right",anchor="e")
        # The widgets for the result file name
        self.result_file=Tkinter.StringVar()
        self.result_file.set("./Results/dg10res.txt")
        self.res_file_label = Tkinter.Label(aframe1,
                                            text="Result file: dg10res.txt")
        self.res_file_label.pack(anchor="e")
        rfile_browser = Tkinter.Button(aframe1,text="Set result file...",
                                       command = self.setResultFile)
	rfile_browser.pack(anchor="e")

        makeSpace(self.UserTop)

        aframe = Tkinter.Frame(self.UserTop)
        aframe.pack()
        wrap_frame=Tkinter.Frame(aframe)
        wrap_frame.pack(side="left")
        method_label=Tkinter.Label(wrap_frame, text="Method")
        method_label.pack(side="top",anchor="w")
        
        #choose the method: breadth first or random
        self.method = Tkinter.IntVar()
	self.method.set(0)
        method_rad1 = Tkinter.Radiobutton(wrap_frame,text="Breadth first")
        method_rad2 = Tkinter.Radiobutton(wrap_frame,text="Random")
        method_rad3 = Tkinter.Radiobutton(wrap_frame,text="Tension vector")
	method_rad4 = Tkinter.Radiobutton(wrap_frame,text="Repulsion vector")
        method_rad1["variable"]=self.method 
        method_rad2["variable"]=self.method 
        method_rad3["variable"]=self.method 
        method_rad4["variable"]=self.method 
        method_rad1["value"]=0
        method_rad2["value"]=1
        method_rad3["value"]=2
        method_rad4["value"]=3
        method_rad1.pack(side="top",anchor="w")
        method_rad2.pack(side="top",anchor="w")
        method_rad3.pack(side="top",anchor="w")
        method_rad4.pack(side="top",anchor="w")

        wrap_frame=Tkinter.Frame(aframe)
        wrap_frame.pack(side="left")
        set_label=Tkinter.Label(wrap_frame, text="Settings")
        set_label.pack(side="top",anchor="e")
	self.epsilon = Tkinter.DoubleVar()
	self.epsilon.set(0.1)
        eps_box=makeComboBox(wrap_frame,"Epsilon", self.epsilon)
        eps_box.component('listbox').insert(1,0.05,0.1,0.2,0.5,0.9,1)
        eps_box.pack(side="top",anchor="e")

        self.iterations=Tkinter.IntVar()
        self.iterations.set(10)
        iter_box=makeComboBox(wrap_frame,"Number of iterations",
                              self.iterations)
        iter_box.pack(side="top",anchor="e")
        iter_box.component('listbox').insert(1,100,500,1000,5000,10000)
        
        #self.intern=Tkinter.IntVar()
        #self.intern.set(1)
        #iter_box=makeComboBox(self.UserTop,"Number of interior iterations",
        #                      self.intern)
        #iter_box.component('listbox').insert(1,1,5,10,25,50,100)
        
        self.step=Tkinter.IntVar()
        self.step.set(1)
        step_box=makeComboBox(wrap_frame,"Animation step",
                              self.step)
        step_box.component('listbox').insert(1,1,5,10,25,50,100)
        step_box.pack(side="top",anchor="e")

        self.trials=Tkinter.IntVar()
        self.trials.set(1)
        tr_box=makeComboBox(wrap_frame,"Number of trials",
                              self.trials)
        tr_box.component('listbox').insert(1,1,5,10,25,50,100)
        tr_box.pack(side="top",anchor="e")

        set_button=Tkinter.Button(wrap_frame,text="Display options...",
                                  command = self.CallDisplay)
	set_button.pack(side="top",anchor="e")

        makeSpace(self.UserTop)
        
        # Build the layout from current settings
        button = Tkinter.Button(self.UserTop,text="Build layout",
                                command=self.Draw)
        button.pack()

        set_button=Tkinter.Button(self.UserTop,text="Loop...",
                                  command = self.CallLoop)
	set_button.pack()

        self.loop_ctrl=None
        self.display_ctrl=None

    def CallLoop(self):
        self.loop_ctrl=LoopControl(self)

    def CallDisplay(self):
        self.display_ctrl=DisplayControl(self)

    def update_data(self,event):
	self.data = self.entry_contents.get()

    def setResultFile(self):
        old_name=self.result_file.get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.asksaveasfilename(initialdir="%s" %(old_dir),
                                             initialfile="%s"%(old_file))
        if fname:
            self.result_file.set(fname)
            ffname = os.path.basename(fname)
            self.res_file_label.config(text="Result file: %s"%(ffname))

    def setGraphFile(self):
        old_name=self.filename.get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.askopenfilename(initialdir="%s" %(old_dir),
                                           initialfile="%s"%(old_file))
        if fname:
            self.filename.set(fname)
            ffname = os.path.basename(fname)
            self.file_label.config(text="Graph file: %s"%(ffname))
            
    def Draw(self,filename=None):
        if filename:
            time1=time.time()
            file_string=os.path.basename(filename)
            print file_string
            self.UserTop.title(file_string)
            self.filename.set(filename)
            fout=open(self.result_file.get(), 'a')
            fout.write("Graph file: ")
            fout.write(filename)
            fout.write("\n\n")
            fout.close()
            self.data = "%s %s %f %d %d %d %d %d %f" %(filename,
                                                       self.result_file.get(),
                                                       self.epsilon.get(),
                                                       self.method.get(),
                                                       self.iterations.get(),
                                                       self.geom_comp.get(),
                                                       self.light_comp.get(),
                                                       self.step.get(),
                                                       self.sphere_factor.get())

            self.geom.set_problem("mc_graph")
            for i in range(self.trials.get()):
                self.geom.file(self.data)
            total_time = time.time() - time1
            fout=open(self.result_file.get(), 'a')
            fout.write("Total time: ")
            fout.write("%f" %(total_time))
            fout.write("\n\n")
            fout.close()
            print "Total time:", total_time
        elif self.filename.get():
            self.Draw(self.filename.get())
        
class LoopControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Loop Control')
        #self.var=parent.var
        self.top=parent

        self.loop_opt=Tkinter.IntVar()
        self.loop_opt.set(1)
        opt_rad1 = Tkinter.Radiobutton(self,text="epsilon")
        opt_rad2 = Tkinter.Radiobutton(self,text="all graph files")
        opt_rad3 = Tkinter.Radiobutton(self,text="start from reference file")
        opt_rad1["variable"]=self.loop_opt
        opt_rad2["variable"]=self.loop_opt
        opt_rad3["variable"]=self.loop_opt
        opt_rad1["value"]=0
        opt_rad2["value"]=1
        opt_rad3["value"]=2
        opt_rad1.pack()
        opt_rad2.pack()
        opt_rad3.pack()

        self.ep_start = Tkinter.DoubleVar()
        self.ep_start.set(self.top.epsilon.get())
        self.ep_end = Tkinter.DoubleVar()
        self.ep_end.set(self.top.epsilon.get())
        self.ep_step = Tkinter.DoubleVar()
        self.ep_step.set(self.top.epsilon.get())

        svbox=makeComboBox(self,"Starting value",self.ep_start)
        svbox.component('listbox').insert(1,0,0.1,0.01,1,2,3)
        evbox=makeComboBox(self,"Ending value",self.ep_end)
        evbox.component('listbox').insert(1,0.2,0.5,1)
        stbox=makeComboBox(self,"Step",self.ep_step)
        stbox.component('listbox').insert(1,0.1,0.01)

        quit_button=Tkinter.Button(self,text="Run", command = self.Run)
	quit_button.pack()

        quit_button=Tkinter.Button(self,text="Cancel", command = self.Quit)
	quit_button.pack()

    def Run(self):
        if self.loop_opt.get()==0:
            if (self.ep_end.get() < self.ep_start.get() and
                self.ep_step.get() >= 0):
                self.ep_end.set(self.ep_start.get())
            if self.ep_step.get() == 0:
                self.ep_step.get(self.ep_start.get())
            epsilon=self.ep_start.get()
            old_epsilon = self.top.epsilon.get()
            old_res_file = self.top.result_file.get()
            while epsilon <= self.ep_end.get():
                self.top.epsilon.set(epsilon)
                res_file = filenames.append_name(old_res_file, epsilon)
                self.top.result_file.set(res_file)
                self.top.Draw()
                epsilon = epsilon + self.ep_step.get()
            self.top.epsilon.set(old_epsilon)
            self.top.result_file.set(old_res_file)
        else:
            initial_file = self.top.filename.get()
            the_path = os.path.dirname(initial_file)
            the_file = os.path.basename(initial_file)
            init_ext = filenames.get_extension(the_file)
            files = os.listdir(the_path)
            files.sort()
            i=0
            if self.loop_opt.get()==2:
                while (i < len(files)) and files[i] != the_file:
                    i=i+1
            j=i
            res_file = self.top.result_file.get()
            path_res=os.path.dirname(res_file)
            res_base=os.path.basename(res_file)
            epsilon = self.top.epsilon.get()
            if self.loop_opt.get()==1:
                start = 1
            else:
                start = 0
            while (j < len(files)):
                extension = filenames.get_extension(files[j])
                if not start:
                    if files[j] == the_file:
                        start = 1
                if extension == init_ext and start:
                    res_new=os.path.join(path_res, files[j])
                    rfilename = filenames.append_name(res_new, "r")
                    problem = os.path.join(the_path, files[j])
                    self.top.filename.set(problem)
                    self.top.result_file.set(rfilename)
                    self.top.Draw()
                j=j+1
            self.top.filename.set(initial_file)
            self.top.result_file.set(res_file)

    def Quit(self):
        self.top.loop_ctrl=None
        self.destroy()

class DisplayControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Display Control')
        #self.var=parent.var
        self.top=parent

        some_frame=Tkinter.Frame(self)
        some_frame.pack()
        
	#Control widget for the geometry complexity
	slider1 = Tkinter.Scale(some_frame,width=15,length=200)
	slider1["from"] = 1
	slider1["to"] = 9
	slider1["variable"] = self.top.geom_comp
	slider1["label"] = "Geom Complexity"
	slider1["orient"] = "horizontal"
	slider1["tickinterval"] = 1
	slider1["relief"] = "raised"
	slider1.pack(side="left",padx="2m")

	#Control widget for the light complexity
	slider2 = Tkinter.Scale(some_frame,width=15,length=200)
	slider2["from"] = 1
	slider2["to"] = 9
	slider2["variable"] = self.top.light_comp
	slider2["label"] = "Light Complexity"
	slider2["orient"] = "horizontal"
	slider2["tickinterval"] = 1
	slider2.pack(side="left")
	slider2["relief"] = "raised"
	slider2.pack(side="left",padx="2m")
        
        makeComboBox(self,"Sphere radius factor",self.top.sphere_factor)

        quit_button=Tkinter.Button(self,text="OK", command = self.Quit)
	quit_button.pack()

    def Quit(self):
        self.top.display_ctrl=None
        self.destroy()

if __name__ == '__main__' : MCGraph().mainloop()
