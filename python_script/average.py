#! /usr/bin/python

import sys, re, string
pattern = re.compile('total error:')
TotalError = 0
Count = 0
output = open('averages', 'w')

for file in sys.argv[1:] :
  inputfile = open(file, 'r')
  line = inputfile.readline()
  while line != ""  :
    if pattern.match(line) : 
      line = inputfile.readline()
      string.rstrip(line)
      TotalError += float(line)
      Count = Count + 1
    line = inputfile.readline()
  output.close()
  AverageError = TotalError / Count
  print AverageError

