#!/usr/bin/python

from os import listdir, system
from shutil import copyfile

contents = listdir( "." )

headers = []
for f in contents:
   if f.endswith( ".hpp" ) or f.endswith( ".h" ):
      headers.append( f )

print "Updating the following files:", headers

for f in headers:
   print "Copying from ../yiffurc/" + f + " to " + f
   try:
      copyfile( "../yiffurc/" + f, f );
   except:
      print "Couldn't copy file " + f + "!"

print "Finished."
system( "pause" )
