#!/usr/local/bin/python

lut = [];

for c in range( 256 ):
   if c >= 0x30 and c <= 0x39: # Numbers.
      lut.append( "'" + chr( c ) + "'" )
   elif c >= 0x41 and c <= 0x5A: # Uppercase letters.
      lut.append( "'" + chr( c + 0x20 ) + "'" )
   elif c >= 0x61 and c <= 0x7A: # Lowercase letters.
      lut.append( "'" + chr( c ) + "'" )
   elif c >= 0xC0 and c <= 0xC6: # A variants.
      lut.append( "'a'" )
   elif c == 0xC7: # C variant
      lut.append( "'c'" )
   elif c >= 0xC8 and c <= 0xCB: # E variants
      lut.append( "'e'" )
   elif c >= 0xCC and c <= 0xCF: # I variants
      lut.append( "'i'" )
   elif c == 0xD0: # D variant
      lut.append( "'d'" )
   elif c == 0xD1: # N variant
      lut.append( "'n'" )
   elif c >= 0xD2 and c <= 0xD6: # O variants
      lut.append( "'o'" )
   elif c == 0xD8: # O variant
      lut.append( "'o'" )
   elif c >= 0xD9 and c <= 0xDC: # U variants
      lut.append( "'u'" )
   elif c == 0xDD: # Y variant
      lut.append( "'y'" )
   elif c >= 0xE0 and c <= 0xE6: # A variants.
      lut.append( "'a'" )
   elif c == 0xE7: # C variant
      lut.append( "'c'" )
   elif c >= 0xE8 and c <= 0xEB: # E variants
      lut.append( "'e'" )
   elif c >= 0xEC and c <= 0xEF: # I variants
      lut.append( "'i'" )
   elif c == 0xF0: # o variant
      lut.append( "'o'" )
   elif c == 0xF1: # n variant
      lut.append( "'n'" )
   elif c >= 0xF2 and c <= 0xF6: # O variants.
      lut.append( "'o'" )
   elif c == 0xF8: # O variant
      lut.append( "'o'" )
   elif c >= 0xF9 and c <= 0xFC: # U variants
      lut.append( "'u'" )
   elif c == 0xFD: # Y variant.
      lut.append( "'y'" )
   elif c == 0xFF: # Y variant.
      lut.append( "'y'" )
   else:
      lut.append( r"'\0'" )
   
print "const char lut[256] ="
print "{"

for i in range( len( lut ) ):
   if i and i % 8 == 0:
      print "\n   " + lut[ i ] + ", ",
   else:
      print lut[ i ] + ", ",

print "};"
   
      
      
      
