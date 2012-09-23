import owl
from tkinter import *
from tkinter import messagebox

##################################################################
# Used to wrap the checkboxes with useful information that aren't
# normally accessible
##################################################################
class CBox:
  def __init__(self, master, text):#, command):
    self.var = IntVar()
    self.text = text
    self.box = Checkbutton(master, text=text, variable=self.var)#, command=command)
    self.box.pack(side=TOP, padx=1, pady=1)
##################################################################

##################################################################
# Ctor for the screech class - basically the checkbox widgets 
# live here
##################################################################
class Screech:
  def __init__(self, master, tests):
    self.tests = tests
    self.boxes = []
    
    self.container = Canvas(master)
    self.container.grid(row=0, column=0, sticky="nswe")
    
    self.vScroll = Scrollbar(master, orient=VERTICAL, command=self.container.yview)
    self.vScroll.grid(row=0, column=1, sticky="ns")

    self.container.configure(yscrollcommand=self.vScroll.set)
    self.containerFrame = Frame(self.container)
    self.container.create_window(0, 0, window=self.containerFrame, anchor='nw')

    for test in tests:
      self.boxes.append(CBox(self.containerFrame, text=tests[test][1]['name']))#, command=self.pressed))

    self.containerFrame.update_idletasks()
    self.container.configure(scrollregion=(0, 0, self.containerFrame.winfo_width(), self.containerFrame.winfo_height()))
##################################################################

##################################################################
# This function allows for querying our checkbox widgets and
# retrieving the actual objects that are selected
##################################################################
  def GetAllSelectedTests(self):
    allPressed = []
    for box in self.boxes:
      if box.var.get() == 1:  
        allPressed.append(self.tests[box.text])
    return allPressed
##################################################################

##################################################################
# This is just a utility function to allow the gui to interact
# with the underlying owl engine. We need to take the json objects
# and use the test name as a key to the dictionary, that way
# when it is time to get all the information from the checkboxes
# we can look them up directly.
##################################################################
def ConvertTestsToDictionaryForScreech(tests):
  resultDict = {}

  for test in tests:
    if not test[1]['name'] in resultDict:
      resultDict[test[1]['name']] = test
    else:
      messagebox.showwarning("Test already defined", "Test named \"%s\" already exists in file <%s>, and is redefined in <%s>" % (test[1]['name'], resultDict[test[1]['name']][0], test[0]))

  return resultDict
##################################################################

if __name__ == "__main__":
  root = Tk()
  root.grid_rowconfigure(1, weight=1)
  root.grid_columnconfigure(0, weight=1)
  root.title("Screech Owl")
  screech = Screech(root, ConvertTestsToDictionaryForScreech(owl.ParseTests(owl.GetAllFiles())))

  ##################################################################
  # inlined to have access to screech
  # performs the actual run - I want to eventually turn this into a
  # window as well
  ##################################################################
  def Run():
    passed = 0
    failed = 0
    print("--------------Testing Begins---------------")
    for result in owl.Execute(owl.DisplayResult, screech.GetAllSelectedTests()):
      if result[0]['StdoutMatch'] and result[0]['StderrMatch']:  
        print("[O]" + result[1]['name'] + " passed")
        passed += 1
      else:
        failed += 1
        errorStr = " "
        if not result[0]['StdoutMatch']:
          errorStr += "stdout error "
        if not result[0]['StderrMatch']:
          errorStr += "stderr error"
        print("[X]" + result[1]['name'] + errorStr) 
    print("passed:%s\nfailed:%s" % (passed, failed))
    print("---------------Testing Ends----------------")
  ##################################################################

  ##################################################################
  # inlined to have access to screech
  # This should probably issue a warning in the future?
  ##################################################################
  def Generate():
    owl.Execute(owl.GenerateResultFiles, screech.GetAllSelectedTests())
  ##################################################################

  runTests = Button(root, text="Run", command=Run)
  runTests.grid(row=1, column=0, sticky='w')
  generateTests = Button(root, text="Generate", command=Generate)
  generateTests.grid(row=1, column=0, sticky='e')
  root.mainloop()
