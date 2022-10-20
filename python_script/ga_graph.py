#!/usr/bin/env python
import Tkinter
import tkFileDialog
import FileDialog
import Pmw
import os
import string
import types
import os.path
import whrandom
from process_res import *

def get_font():
    return "6x13"

class GADcontrol(Tkinter.Toplevel):
    def __init__(self):
        self.generator = whrandom.whrandom()
        aframe=Tkinter.Frame()
        aframe.pack()
        Tkinter.Toplevel.__init__(self,aframe)
        self.title('GA control')
        self.filename=Tkinter.StringVar()
        self.filename.set("../bin/GAPrefs.txt")
        self.binDir=Tkinter.StringVar()
        self.binDir.set("../bin")
        self.bin=Tkinter.StringVar()
        self.bin.set("../bin/gadg")
        self.findBinDir()
        self.file_label = Tkinter.Label(self,text="Bin directory:\n %s"%(self.binDir.get()),font=get_font()) 
        self.file_label.pack()
        self.file_browser = Tkinter.Button(self,text="Set bin directory...",
                                           font=get_font(),
                                           command = self.setBinDir)
	self.file_browser.pack(fill='x')

        oper_button=Tkinter.Button(self,text="Operator options",
                                   font=get_font(),
                                   command = self.CallOpers)
	oper_button.pack(fill='x')

        eval_button=Tkinter.Button(self,text="Evaluation options",
                                   font=get_font(),
                                   command = self.CallEvals)
	eval_button.pack(fill='x')

        oper_button=Tkinter.Button(self,text="Running options",
                                   font=get_font(),
                                   command = self.CallRun)
	oper_button.pack(fill='x')

        oper_button=Tkinter.Button(self,text="MCG tail options",
                                   font=get_font(),
                                   command = self.CallMCG)
	oper_button.pack(fill='x')

        set_button=Tkinter.Button(self,text="Auto set",
                                  font=get_font(),
                                  command = self.AutoSet)
	set_button.pack()

        run_frame=Tkinter.Frame(self)
        run_frame.pack()
        run_left=Tkinter.Frame(run_frame)
        run_left.pack(side='left')
        run_right=Tkinter.Frame(run_frame)
        run_right.pack(side='right')

        set_button=Tkinter.Button(run_right,text="Loop",
                                  font=get_font(),
                                  command = self.CallLoop)
	set_button.pack()

        quit_button=Tkinter.Button(run_left,text="Save",
                                   font=get_font(),command = self.Save)
	quit_button.pack()

        quit_button=Tkinter.Button(run_right,text="Run",
                                   font=get_font(),command = self.Run)
	quit_button.pack()

        quit_button=Tkinter.Button(run_left,text="Quit",
                                   font=get_font(),command = self.Quit)
	quit_button.pack()
        
        self.initVars()
        self.Read()
        self.oper_ctrl = None
        self.eval_ctrl = None
        self.run_ctrl = None
        self.mcg_control = None
        self.loop_ctrl = None
        
        self.grab_set()
        self.focus_set()
        self.wait_window()

    def setBinDir(self):
        fname=FileDialog.FileDialog(self).go()
        if fname:
            self.binDir.set(fname)
            self.filename.set(os.path.join(self.binDir.get(),"GAPrefs.txt"))
            self.bin.set(os.path.join(self.binDir.get(),"gadg"))
            self.file_label.config(text="Bin directory:\n %s"%(self.binDir.get()))
            self.Read()

    def findBinDir(self):
        path=self.binDir.get()
        binfile=os.path.join(path, "gadg")
        preffile = os.path.join(path, "GAPrefs.txt")
        if os.path.exists(binfile) and os.path.isfile(binfile):
            self.bin.set(binfile)
            self.filename.set(preffile)
            return
        else:
            paths = ("./bin/","../bin/", "./", "../")
            for path in paths:
                binfile=os.path.join(path, "gadg")
                preffile = os.path.join(path, "GAPrefs.txt")
                if os.path.exists(binfile) and os.path.isfile(binfile):
                    self.bin.set(binfile)
                    self.filename.set(preffile)
                    return
            self.binDir.set("")
            self.bin.set("")
            self.filename.set("")

    def Read(self):
        try:
            input=open(self.filename.get(),"r")
            line=input.readline()
            while line:
                x=string.split(line,'\n')[0]
                if self.var.has_key(x):
                    line=input.readline()
                    y=string.split(line,'\n')[0]
                    if type(self.var[x].get()) == types.IntType:
                        self.var[x].set(string.atoi(y))
                    elif type(self.var[x].get()) == types.FloatType:
                        self.var[x].set(string.atof(y))
                    else:
                        self.var[x].set(y)
                else:
                    print "unknown key: ",line
                    line=input.readline()
                line=input.readline()
            input.close()
        except:
            print "Could not read the preference file: set the bin directory"
        #self.PrintVars()

    def Save(self):
        if self.randseed.get() == 0:
            seed=int(self.generator.random()*1000)
            self.var["random seed"].set(seed)
            self.ReConfigure()
        output=open(self.filename.get(),"w")
        for x in self.var.keys():
            save_data=self.var[x].get()
            output.write(x)
            output.write("\n")
            if type(save_data) == types.IntType:
                output.write("%d" %(save_data))
            elif type(save_data) == types.FloatType:
                output.write("%f" %(save_data))
            else:
                output.write(save_data)
            output.write("\n")
        output.close()

    def Run(self):
        cmd = "cd "+self.binDir.get()+";gadg&"
        print "running ", cmd
        self.Save()
        os.system(cmd)
        #os.system("cd ../python_script")

    def Run_seq(self):
        cmd = "cd "+self.binDir.get()+";gadg"
        print "running ", cmd
        self.Save()
        os.system(cmd)
        #os.system("cd ../python_script")

    def Quit(self):
        #self.Save()
        self.destroy()

    def PrintVars(self):
        print "options:"
        for x in self.var.keys():
            print x,": ",self.var[x].get()

    def initVars(self):
        self.var={}
        self.var["inferior limit"]=Tkinter.DoubleVar()
        self.var["inferior limit"].set(0.0)
        self.var["superior limit"]=Tkinter.DoubleVar()
        self.var["superior limit"].set(10.0)
        self.var["probability of crossover"]=Tkinter.DoubleVar()
        self.var["probability of crossover"].set(1.0)
        self.var["probability of mutation"]=Tkinter.DoubleVar()
        self.var["probability of mutation"].set(0.0)
        self.var["epsilon"]=Tkinter.DoubleVar()
        self.var["epsilon"].set(0.0)
        self.var["mutation form"]=Tkinter.IntVar()
        self.var["mutation form"].set(0)
        self.var["individual size"]=Tkinter.IntVar()
        self.var["individual size"].set(10)
        self.var["population size"]=Tkinter.IntVar()
        self.var["population size"].set(10)
        self.var["generation number"]=Tkinter.IntVar()
        self.var["generation number"].set(10)
        self.var["history"]=Tkinter.IntVar()
        self.var["history"].set(1)
        self.var["history file"]=Tkinter.StringVar()
        self.var["history file"].set("")
        self.var["reproduction form"]=Tkinter.IntVar()
        self.var["reproduction form"].set(2)
        self.var["crossover form"]=Tkinter.StringVar()
        self.var["crossover form"].set("combined")
        self.var["diss form"]=Tkinter.StringVar()
        self.var["diss form"].set("and")
        self.var["combined form"]=Tkinter.StringVar()
        self.var["combined form"].set("balanced")
        self.var["crossover sites"]=Tkinter.IntVar()
        self.var["crossover sites"].set(1)
        self.var["swap probability"]=Tkinter.DoubleVar()
        self.var["swap probability"].set(0.5)
        self.var["coef volume"]=Tkinter.DoubleVar()
        self.var["coef volume"].set(0.25)
        self.var["coef surface"]=Tkinter.DoubleVar()
        self.var["coef surface"].set(0.25) 
        self.var["coef angle"]=Tkinter.DoubleVar()
        self.var["coef angle"].set(0.25) 
        self.var["coef error"]=Tkinter.DoubleVar()
        self.var["coef error"].set(0.25)     
        self.var["problem file"]=Tkinter.StringVar()
        self.var["problem file"].set("")
        self.randseed = Tkinter.IntVar()
        self.randseed.set(0)
        self.var["random seed"]=Tkinter.IntVar()
        self.var["random seed"].set(10)
        self.var["number of trials"]=Tkinter.IntVar()
        self.var["number of trials"].set(1)
        self.var["number of reports"]=Tkinter.IntVar()
        self.var["number of reports"].set(1)
        self.var["result file"]=Tkinter.StringVar()
        self.var["result file"].set("res.txt")
        self.var["mcg iterations"] = Tkinter.IntVar()
        self.var["mcg iterations"].set(0)
        self.var["mcg method"] = Tkinter.IntVar()
        self.var["mcg method"].set(0)
        self.var["mcg epsilon"] = Tkinter.DoubleVar()
        self.var["mcg epsilon"].set(0.0)

    def setIndSize(self):
        problem=self.var["problem file"].get()
        file=open(problem)
        if file:
            line=file.readline()
            x=string.split(line)[0]
            self.var["individual size"].set(int(x))
            file.close()

    def setLimits(self):
        self.var["inferior limit"].set(0.0)
        self.var["superior limit"].set(10)

    def ReConfigure(self):
        if self.oper_ctrl:
            self.oper_ctrl.ReConfigure()
        if self.eval_ctrl:
            self.eval_ctrl.ReConfigure()
        if self.run_ctrl:
            self.run_ctrl.ReConfigure()

    def AutoSet(self):
        self.setIndSize()
        self.setLimits()
        self.ReConfigure()

    def CallOpers(self):
        self.oper_ctrl=OperatorControl(self)
        
    def CallEvals(self):
        self.eval_ctrl=EvaluationControl(self)
        
    def CallRun(self):
        self.run_ctrl=RunControl(self)
        
    def CallMCG(self):
        self.mcg_ctrl=MCGControl(self)

    def CallLoop(self):
        self.loop_ctrl=LoopControl(self)
        
class two_setit:
    def __init__(self, var1, var2, value1, value2):
        self.__value1 = value1
        self.__value2 = value2
        self.__var1 = var1
        self.__var2 = var2
    def __call__(self, *args):
        self.__var1.set(self.__value1)
        self.__var2.set(self.__value2)
        
class one_setit:
    def __init__(self, var, value):
        self.__value = value
        self.__var = var
    def __call__(self, *args):
        self.__var.set(self.__value)

def makeOptionMenu(parent,text_str,option_str,var):
    menu_fr=Tkinter.Frame(parent)
    menu_fr.pack(side='top')
    menu_label=Tkinter.Label(menu_fr,font=get_font(),text=text_str)
    menu_label.pack(side='left')
    menu_var=Tkinter.StringVar()
    menu_var.set("")
    cr_menu=Tkinter.OptionMenu(menu_fr,menu_var,0)
    cr_menu["menu"].config(font = get_font())
    cr_menu.config(font = get_font())
    cr_menu["menu"].delete(0)
    for i in range(len(option_str)):
        cr_menu["menu"].add_command(label=option_str[i],
                                    command=two_setit(var,menu_var,i,
                                                      option_str[i]))
    cr_menu.pack(side='right')
    cr_menu["menu"].invoke(var.get())
    return cr_menu

def makeStringMenu(parent,text_str,option_str,var):
    menu_fr=Tkinter.Frame(parent)
    menu_fr.pack(side='top')
    menu_label=Tkinter.Label(menu_fr,font=get_font(),text=text_str)
    menu_label.pack(side='left')
    cr_menu=Tkinter.OptionMenu(menu_fr,var,0)
    cr_menu["menu"].config(font = get_font())
    cr_menu.config(font = get_font())
    cr_menu["menu"].delete(0)
    for opt in option_str:
        cr_menu["menu"].add_command(label=opt,
                                    command=one_setit(var,opt))
    cr_menu.pack(side='right')
    cr_menu["menu"].invoke(var.get())
    return cr_menu

def makeComboBox(parent,text_str,var):
    combo_box=Pmw.ComboBox(parent,labelpos="w",label_text=text_str,
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
    
class OperatorControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Operator options')
        self.var=parent.var
        self.top=parent
        pc=makeComboBox(self,"Probability of crossover",
                        self.var["probability of crossover"])
        pc.component('listbox').insert(1,0.0,0.8,1.0)
        pm=makeComboBox(self,"Probability of mutation",
                        self.var["probability of mutation"])
        pm.component('listbox').insert(1,0.0,0.01,0.001,0.0005)
        makeSpace(self)
        self.makeMutationFormMenu()
        eps=makeComboBox(self,"Epsilon",
                         self.var["epsilon"])
        eps.component('listbox').insert(1,0.0,0.1,0.05,0.01,0.005,0.001,0.0005)
        makeSpace(self)
        self.makeCrossoverMenu()
        makeComboBox(self,"Number of crossover sites",
                     self.var["crossover sites"])
        makeComboBox(self,"Swap probability",
                     self.var["swap probability"])
        #self.makeDissFormMenu()
        #self.makeCombinedFormMenu()
        makeSpace(self)
        makeOptionMenu(self,"Reproduction form",["simple",
                                                 "elitist",
                                                 "monotonical",
                                                 "diverse"],
                       self.var["reproduction form"])
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()
        
    def makeCrossoverMenu(self):
        command_str=["1-point","n-point","dissociated","uniform"]
        makeOptionMenu(self,"Crossover form",command_str,
                       self.var["crossover form"])
        
    def makeMutationFormMenu(self):
        command_str=["new random", "percentage random", "mirror", "adapt tv",
                     "adapt edge"]
        makeOptionMenu(self,"Mutation form",command_str,
                       self.var["mutation form"])
        
    def makeCombinedFormMenu(self):
        command_str=["performance","balanced","RR50","RR100","adaptive","all"]
        makeOptionMenu(self,"Combination form",command_str,
                       self.var["combined form"])
        
    def makeDissFormMenu(self):
        command_str=["0","and","xor","random","all"]
        makeOptionMenu(self,"Dissociated option",command_str,
                       self.var["diss form"])
        
    def ReConfigure(self):
        return 1

    def quit(self):
        self.top.oper_ctrl=None
        self.destroy()

class RunControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Running options')
        self.var=parent.var
        self.top=parent
        self.si=makeComboBox(self,"Size of the individual",
                        self.var["individual size"])
        self.si.component('listbox').insert(1,10,20,30,50,300,36,45,55,66,78,91,105,122,153,173)
        makeComboBox(self,"Size of the population",self.var["population size"])
        makeComboBox(self,"Number of generations",
                     self.var["generation number"])
        makeSpace(self)
        makeOptionMenu(self,"Keep the evolution history",["no","yes"],
                       self.var["history"])
        self.hist_file_label = Tkinter.Label(self,text="History file:\n %s"%(self.var["history file"].get()),font=get_font())
        self.hist_file_label.pack()
        file_browser = Tkinter.Button(self,text="Set history file...",
                                      font=get_font(),
                                      command = self.setHistoryFile)
	file_browser.pack(fill='x')
        self.res_file_label = Tkinter.Label(self,text="Result file:\n %s"%(self.var["result file"].get()),font=get_font())
        self.res_file_label.pack()
        rfile_browser = Tkinter.Button(self,text="Set result file...",
                                       font=get_font(),
                                       command = self.setResultFile)
	rfile_browser.pack(fill='x')
        makeSpace(self)
        makeComboBox(self,"Number of trials",
                     self.var["number of trials"])
        makeComboBox(self,"Number of reports",
                     self.var["number of reports"])
        makeOptionMenu(self,"Random seed",["automatic","user determined"],
                       parent.randseed)
        self.rand_box = makeComboBox(self,"random seed",
                                     self.var["random seed"])
        makeSpace(self)
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()

    def setHistoryFile(self):
        fname=tkFileDialog.asksaveasfilename()
        if fname:
            self.var["history file"].set(fname)
            self.hist_file_label.config(text="History file:\n %s"%(self.var["history file"].get()))

    def setResultFile(self):
        old_name=self.var["result file"].get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.asksaveasfilename(initialdir="%s" %(old_dir),
                                             initialfile="%s"%(old_file))
        if fname:
            self.var["result file"].set(fname)
            self.res_file_label.config(text="Result file:\n %s"%(self.var["result file"].get()))
        
    def ReConfigure(self):
        self.si.component('entryfield').setentry(self.var["individual size"].get())
        self.rand_box.component('entryfield').setentry(self.var["random seed"].get())

    def quit(self):
        self.top.run_ctrl=None
        self.destroy()
        
class EvaluationControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Evaluation options')
        self.var=parent.var
        self.top=parent
        self.sup_lim=makeComboBox(self,"Containing Box Width",
                                  self.var["superior limit"])
        self.sup_lim.component('listbox').insert(1,10,20,50,75,100)
        makeSpace(self)
        fnr=makeComboBox(self,"Coefficient Error",self.var["coef error"])
        fnr.component('listbox').insert(1,1,2,0.5,0.25,0.1,0.05,0.01)
        fnr=makeComboBox(self,"Coefficient Volume",self.var["coef volume"])
        fnr.component('listbox').insert(1,1,2,0.5,0.25,0.1,0.05,0.01)
        fnr=makeComboBox(self,"Coefficient Angle",self.var["coef angle"])
        fnr.component('listbox').insert(1,1,2,0.5,0.25,0.1,0.05,0.01)
        fnr=makeComboBox(self,"Coefficient Surface",self.var["coef surface"])
        fnr.component('listbox').insert(1,1,2,0.5,0.25,0.1,0.05,0.01)
        makeSpace(self)
        self.problem_file_label = Tkinter.Label(self,font=get_font(),
                                                text="Problem file:\n %s"%(self.var["problem file"].get()))
        self.problem_file_label.pack()
        file_browser = Tkinter.Button(self,text="Set problem file...",
                                      font=get_font(),
                                      command = self.setProblemFile)
	file_browser.pack(fill='x')
        makeSpace(self)
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()

    def setProblemFile(self):
        old_name=self.var["problem file"].get()
        old_dir = os.path.dirname(old_name)
        old_file = os.path.basename(old_name)
        fname=tkFileDialog.askopenfilename(initialdir="%s" %(old_dir),
                                           initialfile="%s"%(old_file))
        if fname:
            self.var["problem file"].set(fname)
            self.problem_file_label.config(text="Problem file:\n %s"%(self.var["problem file"].get()))
            self.top.AutoSet()
        
    def ReConfigure(self):
        self.sup_lim.component('entryfield').setentry(self.var["superior limit"].get())

    def quit(self):
        self.top.eval_ctrl=None
        self.destroy()
    
class MCGControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('MCG Control')
        self.var=parent.var
        self.top=parent
        
        self.makeMethodMenu()
        meps=makeComboBox(self,"MCG Epsilon",
                          self.var["mcg epsilon"])
        meps.component('listbox').insert(1,0.0,0.1,0.05,0.01,0.005,
                                         0.001,0.0005)
        
        makeSpace(self)
        mit=makeComboBox(self,"Number of iterations",
                         self.var["mcg iterations"])
        mit.component('listbox').insert(1,10, 20, 25, 50, 100, 500, 1000)
        
        makeSpace(self)
        ok_button=Tkinter.Button(self,text="OK",font=get_font(),
                                 command = self.quit)
	ok_button.pack()
        
    def makeMethodMenu(self):
        command_str=["breadth-first", "tension vector"]
        makeOptionMenu(self,"MCG form",command_str,
                       self.var["mcg method"])

    def quit(self):
        self.top.mcg_ctrl=None
        self.destroy()
        
def AppendToName(generic_name, current_value):
    split_name=string.split(generic_name,".")
    length = len(split_name)
    split_name[length-2] = "%s%g" %(split_name[length-2],current_value)
    name=string.join(split_name,".")
    return name

def MakeFilename(generic_name, check_value, current_value):
    path = os.path.dirname(generic_name)
    old_file = os.path.basename(generic_name)
    check_str="%g" %(check_value)
    split_name=string.split(old_file,check_str)
    if len(split_name) == 1:
        return AppendToName(generic_name, current_value)
    last_part=split_name[len(split_name) - 1]
    first_list=[]
    for i in range(len(split_name) - 1):
        first_list.append(split_name[i])
    first_part=string.join(first_list, check_str)
    name = string.join([first_part,last_part],"%g" %(current_value))
    return os.path.join(path, name)

def AddToName(old_name, value, new_text):
    split_name=string.split(old_name, "%g" %(value))
    length = len(split_name)
    split_name[length-2] = "%s%s" %(split_name[length-2],new_text)
    name=string.join(split_name,"%g" %(value))
    return name
    
class LoopControl(Tkinter.Toplevel):
    def __init__(self, parent=None):
        Tkinter.Toplevel.__init__(self, parent)
        self.title('Loop Control')
        self.var=parent.var
        self.top=parent
        
        self.loop_var=Tkinter.StringVar()
        self.loop_var.set("none")
        options_array = self.var.keys()
        options_array.append("none")
        loop_menu = makeStringMenu(self,"Loop over",options_array,
                                   self.loop_var)

        self.start = Tkinter.DoubleVar()
        self.start.set(0)
        self.end = Tkinter.DoubleVar()
        self.end.set(0)
        self.step = Tkinter.DoubleVar()
        self.step.set(0)

        svbox=makeComboBox(self,"Starting value",self.start)
        svbox.component('listbox').insert(1,0,1,2,3)
        evbox=makeComboBox(self,"Ending value",self.end)
        evbox.component('listbox').insert(1,10)
        stbox=makeComboBox(self,"Step",self.step)
        stbox.component('listbox').insert(1,1,2,5)

        quit_button=Tkinter.Button(self,text="Run",
                                   font=get_font(),command = self.Run)
	quit_button.pack()

        quit_button=Tkinter.Button(self,text="Quit",
                                   font=get_font(),command = self.Quit)
	quit_button.pack()

    def Run(self):
        if not self.var.has_key(self.loop_var.get()):
            self.top.Run()
        elif self.loop_var.get() == "problem file":
            initial_file = self.var["problem file"].get()
            split_array = os.path.split(initial_file)
            the_path = split_array[0]
            the_file = split_array[1]
            files = os.listdir(the_path)
            Sort(files)
            i=0
            while (i < len(files)) and files[i] != the_file:
                i=i+1
            j=i
            res_file = self.var["result file"].get()
            while (j < len(files)):
                rfilename = "%s.%d" %(res_file, j)
                problem = os.path.join(the_path, files[j])
                self.var["problem file"].set(problem)
                self.var["result file"].set(rfilename)
                self.top.AutoSet()
                self.top.Run_seq()
                j=j+1
            self.var["problem file"].set(initial_file)
            self.var["result file"].set(res_file)
        else:
            generic_value = self.var[self.loop_var.get()].get()
            generic_name=self.var["result file"].get()
            value = self.start.get()
            while value <= self.end.get():
                print self.loop_var.get()
                print self.var[self.loop_var.get()]
                print type(self.var[self.loop_var.get()].get())
                if type(self.var[self.loop_var.get()].get()) is types.IntType:
                    value=int(value)
                print value
                self.var[self.loop_var.get()].set(value)
                name = MakeFilename(generic_name, generic_value, value)
                self.var["result file"].set(name)
                self.top.AutoSet()
                self.top.Run_seq()
                if self.var["reproduction form"].get() == 6:
                    name_rep=AddToName(name, value, "_rep")
                    command="mv ~/temp/gender_stat.txt %s" %(name_rep)
                    os.system(command)
                value = value + self.step.get()
            self.var[self.loop_var.get()].set(generic_value)

    def Quit(self):
        self.top.loop_ctrl=None
        self.destroy()

if __name__ == '__main__' : 
    gad_ctrl = GADcontrol()
