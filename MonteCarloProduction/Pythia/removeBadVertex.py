#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections

def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('-f', '--file', dest='FILE_NAME', type='string', default='' ,help='file name')
  

  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()



if __name__ == "__main__":
  parseOptions()


  totalCounterGood = 0
  totalCounter = 0

  for filename in open(opt.FILE_NAME,'r'):
      f = filename.split()
      if len(f) < 1: continue
      if f[0].startswith("#"): continue
      out = open(f[0].replace('.txt','_clean.txt'),'w')
      counter = 0
      counterGood = 0
      rowNumber = []
      posX = []
      posY = []
      posZ = []
      nine = []
      for vertex in open(f[0],'r'):
        g = vertex.split()
        if len(g) < 5: continue
        if g[0].startswith("#"): continue
        totalCounter += 1
        counter += 1
        rowNumber.append(g[0])
        posX.append(g[1])
        posY.append(g[2])
        posZ.append(g[3])
        nine.append(g[4])

        if posX[counter-1] == "nan" or posY[counter-1] == "nan" or posZ[counter-1] == "nan": continue
        counterGood += 1
        totalCounterGood += 1

        myString = '         {0}    {1}    {2}  {3} -9999.00000\n'.format(counter,posX[counter-1],posY[counter-1],posZ[counter-1])
        out.write(myString)
      print filename, '{0} good vertices out of {1}'.format(counterGood,counter)

  print '{0} total good vertices out of {1}'.format(totalCounterGood,totalCounter)
  eff = totalCounterGood/float(totalCounter)
  print eff,'% efficient'
