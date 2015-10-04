#!/usr/bin/python
import re, msvcrt

f = open( 'project.xml' )
contents = f.read( )
f.close( )

members = {}
re_member = re.compile( r'<member name="([A-Z]):([^"]+)"', re.MULTILINE )
iter = re_member.finditer( contents )
for mo in iter:
   print 'Member: %s:%s' % (mo.group( 1 ), mo.group( 2 ))
   members[ mo.group( 2 ) ] = mo.group( 1 )
   
contents = re.sub( r'::', '.', contents )

re_cref = re.compile( r' cref="!:([^"]+)"', re.MULTILINE )
while True:
   mo = re_cref.search( contents )
   if mo is None:
      break
   fullname = 'Furnarchy.%s' % (mo.group( 1 ))
   if fullname in members:
      fullname = '%s:%s' % (members[ fullname ], fullname)
   else:
      print 'Unmatched: %s' % (fullname)
   contents = '%s cref="%s"%s' % (contents[:mo.start(0)], fullname, contents[mo.end(0):])   

f = open( 'project.xml', 'w' )
f.write( contents )
f.close( )

msvcrt.getch( )