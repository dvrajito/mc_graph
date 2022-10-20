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

from geom_window import GeomWindow

class FileTransfer(GeomWindow):
    def __init__(self,parent=None,cnf={},**kw):
	apply(GeomWindow.__init__,(self,parent),kw)

    def UserWidgets(self):

	#file selection dialog
	self.ifilename = Tkinter.StringVar()
	self.ifilename.set("")
        button = Tkinter.Button(self.UserTop,text="Input from file",
                                command=self.get_input_filename)
        button.pack()
        
	#file selection dialog
	self.ofilename = Tkinter.StringVar()
	self.ofilename.set("")
        button = Tkinter.Button(self.UserTop,
                                text="Output to file",
                                command=self.get_output_filename)
        button.pack()

	
	#radio buttons for choosing the transfering direction
	rad1 = Tkinter.Radiobutton(self.UserTop,text="Povray 2 Scene file")
	rad2 = Tkinter.Radiobutton(self.UserTop,text="Scene file 2 Povray")
        self.tr_type = Tkinter.IntVar()
	self.tr_type.set(0)
	rad1["variable"]=self.tr_type
	rad2["variable"]=self.tr_type
        rad1["value"]=0
	rad2["value"]=1
	rad1.pack()
	rad2.pack()

    def get_input_filename(self):
        self.ifilename.set(tkFileDialog.askopenfilename())

    def get_output_filename(self):
        self.ofilename.set(tkFileDialog.asksaveasfilename())
        self.transfer()

    def update_data(self,event):
	self.data = self.entry_contents.get()
    
    def transfer(self):
	self.geom.set_problem("transfer_file")
	self.tr_data = "%d %s %s" % (self.tr_type.get(),
                                     self.ifilename.get(),
                                     self.ofilename.get())
	self.geom.file(self.tr_data)


if __name__ == '__main__' : FileTransfer().mainloop()

