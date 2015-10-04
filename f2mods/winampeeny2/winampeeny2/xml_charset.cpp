/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#include "xml_charset.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cassert>

namespace xml_charset {

using namespace std;

//////////////////////////////////////////////////////////////////////////

struct Entity
{
   const char* const    ref;
   const unsigned short ch;
};

//////////////////////////////////////////////////////////////////////////

enum { ENTITY_MAP_BUCKETS = 6, ENTITY_MAP_LEN = 73 };

/* Precalculated hash map. */
static const Entity g_entity_map[ ENTITY_MAP_LEN ][ ENTITY_MAP_BUCKETS ] =
{
   { {"&thetasym;", 0x03D1}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Igrave;", 0x00CC}, {"&Scaron;", 0x0160}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&shy;", 0x00AD}, {"&agrave;", 0x00E0}, {"&Dagger;", 0x2021}, {"&weierp;", 0x2118}, {"&prop;", 0x221D}, {NULL, 0} },
   { {"&acute;", 0x00B4}, {"&yuml;", 0x00FF}, {"&Psi;", 0x03A8}, {"&zwnj;", 0x200C}, {"&le;", 0x2264}, {NULL, 0} },
   { {"&Gamma;", 0x0393}, {"&sigmaf;", 0x03C2}, {"&frasl;", 0x2044}, {"&crarr;", 0x21B5}, {NULL, 0}, {NULL, 0} },
   { {"&OElig;", 0x0152}, {"&Pi;", 0x03A0}, {"&emsp;", 0x2003}, {"&harr;", 0x2194}, {"&lowast;", 0x2217}, {"&otimes;", 0x2297} },
   { {"&sigma;", 0x03C3}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&diams;", 0x2666}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Iuml;", 0x00CF}, {"&exist;", 0x2203}, {"&ni;", 0x220B}, {"&nsub;", 0x2284}, {NULL, 0}, {NULL, 0} },
   { {"&xi;", 0x03BE}, {"&part;", 0x2202}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&reg;", 0x00AE}, {"&Egrave;", 0x00C8}, {"&szlig;", 0x00DF}, {"&acirc;", 0x00E2}, {"&rceil;", 0x2309}, {NULL, 0} },
   { {"&ordm;", 0x00BA}, {"&frac34;", 0x00BE}, {"&iacute;", 0x00ED}, {"&forall;", 0x2200}, {NULL, 0}, {NULL, 0} },
   { {"&Otilde;", 0x00D5}, {"&zwj;", 0x200D}, {"&permil;", 0x2030}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&sect;", 0x00A7}, {"&uml;", 0x00A8}, {"&hArr;", 0x21D4}, {"&spades;", 0x2660}, {NULL, 0}, {NULL, 0} },
   { {"&Ocirc;", 0x00D4}, {"&iota;", 0x03B9}, {"&mdash;", 0x2014}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&mu;", 0x03BC}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Atilde;", 0x00C3}, {"&igrave;", 0x00EC}, {"&ocirc;", 0x00F4}, {"&scaron;", 0x0161}, {"&chi;", 0x03C7}, {"&lrm;", 0x200E} },
   { {"&macr;", 0x00AF}, {"&dagger;", 0x2020}, {"&asymp;", 0x2248}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&curren;", 0x00A4}, {"&raquo;", 0x00BB}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Alpha;", 0x0391}, {"&omicron;", 0x03BF}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&eacute;", 0x00E9}, {"&lambda;", 0x03BB}, {"&hellip;", 0x2026}, {"&radic;", 0x221A}, {"&clubs;", 0x2663}, {NULL, 0} },
   { {"&apos;", 0x0027}, {"&lt;", 0x003C}, {"&Auml;", 0x00C4}, {"&theta;", 0x03B8}, {"&equiv;", 0x2261}, {NULL, 0} },
   { {"&upsih;", 0x03D2}, {"&trade;", 0x2122}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&frac12;", 0x00BD}, {"&rsquo;", 0x2019}, {"&Prime;", 0x2033}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&aring;", 0x00E5}, {"&Chi;", 0x03A7}, {"&loz;", 0x25CA}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Uuml;", 0x00DC}, {"&egrave;", 0x00E8}, {"&oline;", 0x203E}, {"&euro;", 0x20AC}, {"&int;", 0x222B}, {NULL, 0} },
   { {"&brvbar;", 0x00A6}, {"&sup2;", 0x00B2}, {"&notin;", 0x2209}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Kappa;", 0x039A}, {"&bdquo;", 0x201E}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&copy;", 0x00A9}, {"&Beta;", 0x0392}, {"&rdquo;", 0x201D}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&iuml;", 0x00EF}, {"&ensp;", 0x2002}, {"&image;", 0x2111}, {"&empty;", 0x2205}, {"&sup;", 0x2283}, {"&rfloor;", 0x230B} },
   { {"&deg;", 0x00B0}, {"&para;", 0x00B6}, {"&THORN;", 0x00DE}, {"&there4;", 0x2234}, {NULL, 0}, {NULL, 0} },
   { {"&atilde;", 0x00E3}, {"&icirc;", 0x00EE}, {"&sbquo;", 0x201A}, {"&nabla;", 0x2207}, {"&cap;", 0x2229}, {NULL, 0} },
   { {"&delta;", 0x03B4}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Uacute;", 0x00DA}, {"&cup;", 0x222A}, {"&hearts;", 0x2665}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Oslash;", 0x00D8}, {"&infin;", 0x221E}, {"&sdot;", 0x22C5}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ccedil;", 0x00E7}, {"&Xi;", 0x039E}, {"&larr;", 0x2190}, {"&oplus;", 0x2295}, {NULL, 0}, {NULL, 0} },
   { {"&iexcl;", 0x00A1}, {"&Sigma;", 0x03A3}, {"&lang;", 0x2329}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&oelig;", 0x0153}, {"&tilde;", 0x02DC}, {"&nu;", 0x03BD}, {"&rlm;", 0x200F}, {NULL, 0}, {NULL, 0} },
   { {"&Ugrave;", 0x00D9}, {"&lsquo;", 0x2018}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&rho;", 0x03C1}, {"&ge;", 0x2265}, {"&perp;", 0x22A5}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&micro;", 0x00B5}, {"&frac14;", 0x00BC}, {"&Acirc;", 0x00C2}, {"&thorn;", 0x00FE}, {"&Epsilon;", 0x0395}, {"&sum;", 0x2211} },
   { {"&ecirc;", 0x00EA}, {"&Mu;", 0x039C}, {"&eta;", 0x03B7}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&auml;", 0x00E4}, {"&ucirc;", 0x00FB}, {"&Omega;", 0x03A9}, {"&uarr;", 0x2191}, {"&ang;", 0x2220}, {"&sube;", 0x2286} },
   { {"&ntilde;", 0x00F1}, {"&ldquo;", 0x201C}, {"&lArr;", 0x21D0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&omega;", 0x03C9}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ETH;", 0x00D0}, {"&phi;", 0x03C6}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Eacute;", 0x00C9}, {"&uuml;", 0x00FC}, {"&Lambda;", 0x039B}, {"&prod;", 0x220F}, {NULL, 0}, {NULL, 0} },
   { {"&Rho;", 0x03A1}, {"&gamma;", 0x03B3}, {"&ne;", 0x2260}, {"&lfloor;", 0x230A}, {NULL, 0}, {NULL, 0} },
   { {"&plusmn;", 0x00B1}, {"&uacute;", 0x00FA}, {"&epsilon;", 0x03B5}, {"&ndash;", 0x2013}, {"&darr;", 0x2193}, {"&minus;", 0x2212} },
   { {"&oslash;", 0x00F8}, {"&Eta;", 0x0397}, {"&beta;", 0x03B2}, {"&rsaquo;", 0x203A}, {NULL, 0}, {NULL, 0} },
   { {"&cedil;", 0x00B8}, {"&uArr;", 0x21D1}, {"&cong;", 0x2245}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Euml;", 0x00CB}, {"&Oacute;", 0x00D3}, {"&Theta;", 0x0398}, {"&zeta;", 0x03B6}, {NULL, 0}, {NULL, 0} },
   { {"&Omicron;", 0x039F}, {"&pi;", 0x03C0}, {"&thinsp;", 0x2009}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ugrave;", 0x00F9}, {"&Phi;", 0x03A6}, {"&and;", 0x2227}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&sup1;", 0x00B9}, {"&Aring;", 0x00C5}, {"&fnof;", 0x0192}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&quot;", 0x0022}, {"&Aacute;", 0x00C1}, {"&Yuml;", 0x0178}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ograve;", 0x00D2}, {"&times;", 0x00D7}, {"&Yacute;", 0x00DD}, {"&dArr;", 0x21D3}, {"&isin;", 0x2208}, {"&sub;", 0x2282} },
   { {"&gt;", 0x003E}, {"&tau;", 0x03C4}, {"&lceil;", 0x2308}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&iquest;", 0x00BF}, {"&ouml;", 0x00F6}, {"&divide;", 0x00F7}, {"&supe;", 0x2287}, {NULL, 0}, {NULL, 0} },
   { {"&otilde;", 0x00F5}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&amp;", 0x0026}, {"&Agrave;", 0x00C0}, {"&Upsilon;", 0x03A5}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ordf;", 0x00AA}, {"&Ccedil;", 0x00C7}, {"&Icirc;", 0x00CE}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&cent;", 0x00A2}, {"&not;", 0x00AC}, {"&Delta;", 0x0394}, {"&Zeta;", 0x0396}, {"&alpha;", 0x03B1}, {NULL, 0} },
   { {"&nbsp;", 0x00A0}, {"&aelig;", 0x00E6}, {"&Nu;", 0x039D}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&yen;", 0x00A5}, {"&eth;", 0x00F0}, {"&rarr;", 0x2192}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&laquo;", 0x00AB}, {"&circ;", 0x02C6}, {"&Tau;", 0x03A4}, {"&rang;", 0x0232}, {NULL, 0}, {NULL, 0} },
   { {"&oacute;", 0x00F3}, {"&Iota;", 0x0399}, {"&prime;", 0x2032}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&lsaquo;", 0x2039}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&middot;", 0x00B7}, {"&upsilon;", 0x03C5}, {"&psi;", 0x03C8}, {"&piv;", 0x03D6}, {"&real;", 0x211C}, {"&sim;", 0x223C} },
   { {"&Iacute;", 0x00CD}, {"&Ntilde;", 0x00D1}, {"&Ouml;", 0x00D6}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&pound;", 0x00A3}, {"&aacute;", 0x00E1}, {"&kappa;", 0x03BA}, {"&or;", 0x2228}, {NULL, 0}, {NULL, 0} },
   { {"&sup3;", 0x00B3}, {"&Ecirc;", 0x00CA}, {"&ograve;", 0x00F2}, {"&yacute;", 0x00FD}, {"&alefsym;", 0x2135}, {NULL, 0} },
   { {"&AElig;", 0x00C6}, {"&Ucirc;", 0x00DB}, {"&euml;", 0x00EB}, {"&bull;", 0x2022}, {"&rArr;", 0x21D2}, {NULL, 0} },
};

enum { ORD_MAP_BUCKETS = 6, ORD_MAP_LEN = 73 };

/* Precalculated hash map. */
static const Entity g_ord_map[ ORD_MAP_LEN ][ ORD_MAP_BUCKETS ] =
{
   { {"&Ucirc;", 0x00DB}, {"&epsilon;", 0x03B5}, {"&lsaquo;", 0x2039}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Uuml;", 0x00DC}, {"&zeta;", 0x03B6}, {"&rsaquo;", 0x203A}, {"&sub;", 0x2282}, {NULL, 0}, {NULL, 0} },
   { {"&Yacute;", 0x00DD}, {"&tilde;", 0x02DC}, {"&eta;", 0x03B7}, {"&sup;", 0x2283}, {NULL, 0}, {NULL, 0} },
   { {"&THORN;", 0x00DE}, {"&theta;", 0x03B8}, {"&nsub;", 0x2284}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&szlig;", 0x00DF}, {"&iota;", 0x03B9}, {"&weierp;", 0x2118}, {"&sim;", 0x223C}, {NULL, 0}, {NULL, 0} },
   { {"&agrave;", 0x00E0}, {"&kappa;", 0x03BA}, {"&oline;", 0x203E}, {"&sube;", 0x2286}, {NULL, 0}, {NULL, 0} },
   { {"&aacute;", 0x00E1}, {"&lambda;", 0x03BB}, {"&supe;", 0x2287}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&acirc;", 0x00E2}, {"&mu;", 0x03BC}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&atilde;", 0x00E3}, {"&nu;", 0x03BD}, {"&real;", 0x211C}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&auml;", 0x00E4}, {"&xi;", 0x03BE}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&aring;", 0x00E5}, {"&omicron;", 0x03BF}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&aelig;", 0x00E6}, {"&Yuml;", 0x0178}, {"&pi;", 0x03C0}, {"&frasl;", 0x2044}, {NULL, 0}, {NULL, 0} },
   { {"&ccedil;", 0x00E7}, {"&rho;", 0x03C1}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&egrave;", 0x00E8}, {"&sigmaf;", 0x03C2}, {"&cong;", 0x2245}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&nbsp;", 0x00A0}, {"&eacute;", 0x00E9}, {"&sigma;", 0x03C3}, {"&trade;", 0x2122}, {NULL, 0}, {NULL, 0} },
   { {"&iexcl;", 0x00A1}, {"&ecirc;", 0x00EA}, {"&tau;", 0x03C4}, {"&crarr;", 0x21B5}, {NULL, 0}, {NULL, 0} },
   { {"&cent;", 0x00A2}, {"&euml;", 0x00EB}, {"&upsilon;", 0x03C5}, {"&asymp;", 0x2248}, {NULL, 0}, {NULL, 0} },
   { {"&pound;", 0x00A3}, {"&igrave;", 0x00EC}, {"&phi;", 0x03C6}, {"&forall;", 0x2200}, {NULL, 0}, {NULL, 0} },
   { {"&curren;", 0x00A4}, {"&iacute;", 0x00ED}, {"&chi;", 0x03C7}, {"&ensp;", 0x2002}, {NULL, 0}, {NULL, 0} },
   { {"&yen;", 0x00A5}, {"&icirc;", 0x00EE}, {"&psi;", 0x03C8}, {"&emsp;", 0x2003}, {"&part;", 0x2202}, {NULL, 0} },
   { {"&brvbar;", 0x00A6}, {"&iuml;", 0x00EF}, {"&omega;", 0x03C9}, {"&exist;", 0x2203}, {"&oplus;", 0x2295}, {NULL, 0} },
   { {"&sect;", 0x00A7}, {"&eth;", 0x00F0}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&uml;", 0x00A8}, {"&ntilde;", 0x00F1}, {"&empty;", 0x2205}, {"&otimes;", 0x2297}, {"&lang;", 0x2329}, {NULL, 0} },
   { {"&copy;", 0x00A9}, {"&ograve;", 0x00F2}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ordf;", 0x00AA}, {"&oacute;", 0x00F3}, {"&nabla;", 0x2207}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&laquo;", 0x00AB}, {"&ocirc;", 0x00F4}, {"&thinsp;", 0x2009}, {"&isin;", 0x2208}, {NULL, 0}, {NULL, 0} },
   { {"&not;", 0x00AC}, {"&otilde;", 0x00F5}, {"&notin;", 0x2209}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&shy;", 0x00AD}, {"&ouml;", 0x00F6}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&reg;", 0x00AE}, {"&divide;", 0x00F7}, {"&thetasym;", 0x03D1}, {"&zwnj;", 0x200C}, {"&ni;", 0x220B}, {NULL, 0} },
   { {"&macr;", 0x00AF}, {"&oslash;", 0x00F8}, {"&upsih;", 0x03D2}, {"&zwj;", 0x200D}, {NULL, 0}, {NULL, 0} },
   { {"&deg;", 0x00B0}, {"&ugrave;", 0x00F9}, {"&lrm;", 0x200E}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&plusmn;", 0x00B1}, {"&uacute;", 0x00FA}, {"&rlm;", 0x200F}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&sup2;", 0x00B2}, {"&ucirc;", 0x00FB}, {"&prod;", 0x220F}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&sup3;", 0x00B3}, {"&uuml;", 0x00FC}, {"&piv;", 0x03D6}, {"&alefsym;", 0x2135}, {NULL, 0}, {NULL, 0} },
   { {"&quot;", 0x0022}, {"&acute;", 0x00B4}, {"&yacute;", 0x00FD}, {"&sum;", 0x2211}, {NULL, 0}, {NULL, 0} },
   { {"&micro;", 0x00B5}, {"&thorn;", 0x00FE}, {"&ndash;", 0x2013}, {"&minus;", 0x2212}, {NULL, 0}, {NULL, 0} },
   { {"&para;", 0x00B6}, {"&yuml;", 0x00FF}, {"&mdash;", 0x2014}, {"&perp;", 0x22A5}, {NULL, 0}, {NULL, 0} },
   { {"&middot;", 0x00B7}, {"&fnof;", 0x0192}, {"&Alpha;", 0x0391}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&amp;", 0x0026}, {"&cedil;", 0x00B8}, {"&Beta;", 0x0392}, {"&loz;", 0x25CA}, {NULL, 0}, {NULL, 0} },
   { {"&apos;", 0x0027}, {"&sup1;", 0x00B9}, {"&Gamma;", 0x0393}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&ordm;", 0x00BA}, {"&Delta;", 0x0394}, {"&lsquo;", 0x2018}, {"&lowast;", 0x2217}, {"&ne;", 0x2260}, {NULL, 0} },
   { {"&raquo;", 0x00BB}, {"&Epsilon;", 0x0395}, {"&rsquo;", 0x2019}, {"&equiv;", 0x2261}, {NULL, 0}, {NULL, 0} },
   { {"&frac14;", 0x00BC}, {"&Zeta;", 0x0396}, {"&sbquo;", 0x201A}, {"&euro;", 0x20AC}, {"&lArr;", 0x21D0}, {"&spades;", 0x2660} },
   { {"&frac12;", 0x00BD}, {"&Eta;", 0x0397}, {"&uArr;", 0x21D1}, {"&radic;", 0x221A}, {NULL, 0}, {NULL, 0} },
   { {"&frac34;", 0x00BE}, {"&Theta;", 0x0398}, {"&ldquo;", 0x201C}, {"&rArr;", 0x21D2}, {"&le;", 0x2264}, {NULL, 0} },
   { {"&iquest;", 0x00BF}, {"&Iota;", 0x0399}, {"&rdquo;", 0x201D}, {"&dArr;", 0x21D3}, {"&ge;", 0x2265}, {"&clubs;", 0x2663} },
   { {"&Agrave;", 0x00C0}, {"&OElig;", 0x0152}, {"&Kappa;", 0x039A}, {"&bdquo;", 0x201E}, {"&hArr;", 0x21D4}, {"&prop;", 0x221D} },
   { {"&Aacute;", 0x00C1}, {"&oelig;", 0x0153}, {"&Lambda;", 0x039B}, {"&infin;", 0x221E}, {"&hearts;", 0x2665}, {NULL, 0} },
   { {"&Acirc;", 0x00C2}, {"&Mu;", 0x039C}, {"&dagger;", 0x2020}, {"&diams;", 0x2666}, {NULL, 0}, {NULL, 0} },
   { {"&Atilde;", 0x00C3}, {"&Nu;", 0x039D}, {"&Dagger;", 0x2021}, {"&ang;", 0x2220}, {NULL, 0}, {NULL, 0} },
   { {"&Auml;", 0x00C4}, {"&Xi;", 0x039E}, {"&bull;", 0x2022}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Aring;", 0x00C5}, {"&Omicron;", 0x039F}, {"&larr;", 0x2190}, {"&rang;", 0x0232}, {NULL, 0}, {NULL, 0} },
   { {"&AElig;", 0x00C6}, {"&Pi;", 0x03A0}, {"&uarr;", 0x2191}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ccedil;", 0x00C7}, {"&circ;", 0x02C6}, {"&Rho;", 0x03A1}, {"&rarr;", 0x2192}, {NULL, 0}, {NULL, 0} },
   { {"&Egrave;", 0x00C8}, {"&hellip;", 0x2026}, {"&darr;", 0x2193}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Eacute;", 0x00C9}, {"&Sigma;", 0x03A3}, {"&harr;", 0x2194}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ecirc;", 0x00CA}, {"&Tau;", 0x03A4}, {"&and;", 0x2227}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Euml;", 0x00CB}, {"&Upsilon;", 0x03A5}, {"&or;", 0x2228}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Igrave;", 0x00CC}, {"&Phi;", 0x03A6}, {"&cap;", 0x2229}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Iacute;", 0x00CD}, {"&Chi;", 0x03A7}, {"&cup;", 0x222A}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&lt;", 0x003C}, {"&Icirc;", 0x00CE}, {"&Scaron;", 0x0160}, {"&Psi;", 0x03A8}, {"&int;", 0x222B}, {NULL, 0} },
   { {"&Iuml;", 0x00CF}, {"&scaron;", 0x0161}, {"&Omega;", 0x03A9}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&gt;", 0x003E}, {"&ETH;", 0x00D0}, {"&lceil;", 0x2308}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ntilde;", 0x00D1}, {"&rceil;", 0x2309}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ograve;", 0x00D2}, {"&permil;", 0x2030}, {"&lfloor;", 0x230A}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Oacute;", 0x00D3}, {"&rfloor;", 0x230B}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ocirc;", 0x00D4}, {"&prime;", 0x2032}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Otilde;", 0x00D5}, {"&Prime;", 0x2033}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ouml;", 0x00D6}, {"&sdot;", 0x22C5}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&times;", 0x00D7}, {"&alpha;", 0x03B1}, {"&there4;", 0x2234}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Oslash;", 0x00D8}, {"&beta;", 0x03B2}, {"&image;", 0x2111}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Ugrave;", 0x00D9}, {"&gamma;", 0x03B3}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
   { {"&Uacute;", 0x00DA}, {"&delta;", 0x03B4}, {NULL, 0}, {NULL, 0}, {NULL, 0}, {NULL, 0} },
};

//////////////////////////////////////////////////////////////////////////

/* Important that this matches up with the hash function used to precalc the map. */
template <typename T_Ch>
inline size_t hash_sz( const T_Ch* sz )
{
   size_t h = 0;
   for (size_t c; (c = *sz); ++sz)
      h = c + (h << 6) + (h << 16) - h;
   return h;
}

/* Allows for comparison between regular c-strings to wide strings by casting up. */
template <typename T_ChA, typename T_ChB>
inline int uni_szcmp( const T_ChA* a, const T_ChB* b )
{
   int r;
   for (; *a && *b && !(r = (unsigned short) *a - (unsigned short) *b); ++a, ++b) ;
   return r;
}

/* Strict hex -> int conversion. */
unsigned hex_to_int( const char* sz_hex, unsigned* digits = NULL )
{
   unsigned r = 0;
   unsigned i = 0;
   for (unsigned c; (c =*sz_hex); ++sz_hex, ++i)
   {
      if ((c >= '0' && c <= '9'))
         c -= '0';
      else if ((c >= 'A' && c <= 'F'))
         c = c - 'A' + 10;
      else if ((c >= 'a' && c <= 'f'))
         c = c - 'a' + 10;
      else
         break;

      r = (r << 4) | c;
   }

   if (digits)
      *digits = i;
   return r;
}

/* Strict dec -> int conversion. */
unsigned dec_to_int( const char* sz_dec, unsigned* digits = NULL )
{
   unsigned r = 0;
   unsigned i = 0;
   for (unsigned c; (c =*sz_dec); ++sz_dec, ++i)
   {
      if ((c >= '0' && c <= '9'))
         c -= '0';
      else
         break;

      r = (r * 10) + c;
   }

   if (digits)
      *digits = i;
   return r;
}

template <typename T_Ch>
const Entity* find_entity( const T_Ch* ref )
{
   const Entity* entities = g_entity_map[ hash_sz( ref ) % ENTITY_MAP_LEN ];
   // End of buckets is terminated by a null-named entity.
   for (unsigned i = 0; i < ENTITY_MAP_BUCKETS && entities[ i ].ref; ++i)
   {
      if (!uni_szcmp( entities[ i ].ref, ref ))
         return &entities[ i ];
   }
   return NULL;
}

const Entity* find_entity( unsigned short ord )
{
   const Entity* entities = g_ord_map[ (size_t) ord % ORD_MAP_LEN ];
   // End of buckets is terminated by a null-named entity.
   for (unsigned i = 0; i < ORD_MAP_BUCKETS && entities[ i ].ref; ++i)
   {
      if (entities[ i ].ch == ord)
         return &entities[ i ];
   }
   return NULL;
}

string* make_hex_ref( unsigned short ord, string* out )
{
   const static char x_digits[ 16 ] = 
   { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

   char sz[] = "&#x0000;";

   sz[ 3 ] = x_digits[ ((ord >> 12) & 0x000F) ];
   sz[ 4 ] = x_digits[ ((ord >> 8) & 0x000F)  ];
   sz[ 5 ] = x_digits[ ((ord >> 4) & 0x000F)  ];
   sz[ 6 ] = x_digits[ ((ord >> 0) & 0x000F)  ];
   
   out->assign( sz, sizeof( sz ) / sizeof( *sz ) );
   return out;
}

string* extract_ref( const string& src, string::size_type off, string* out )
{
   if (src.length( ) > off && src[ off ] == '&')
   {
      const string::size_type end = src.find_first_of( ";", off + 1 );
      if (end != string::npos)
      {
         out->assign( src, off, (end + 1) - off );
         return out;
      }
   }
   out->clear( );
   return out;
}

unsigned short ref_to_ord( const string& ref )
{
   if (!ref.empty( ) && ref[ 0 ] == '&' && ref[ ref.length( ) - 1 ] == ';' )
   {
      if (ref[ 1 ] == '#') // number reference.
      {
         if (ref[ 2 ] == 'x') // hex reference.
         {
            unsigned digits;
            unsigned short ord = hex_to_int( ref.c_str( ) + 3, &digits );
            if (digits && 3 + digits + 1 == ref.length( ))
               return ord;
            return 0; // Bad digits.
         }
         else // decimal reference.
         {
            unsigned digits;
            unsigned short ord = dec_to_int( ref.c_str( ) + 2, &digits );
            if (digits && 2 + digits + 1 == ref.length( ))
               return ord;
            return 0; // Bad digits.
         }
      }
      else // named reference.
      {
         const Entity* e = find_entity( ref.c_str( ) );
         if (e)
            return e->ch;
      }
   }

   return 0;
}

string* ord_to_ref( unsigned short ord, string* out )
{
   // If there's a name, use it.
   const Entity* ent = find_entity( ord );

   if (ent)
   {
      out->assign( ent->ref );
      return out;
   }

   // Otherwise, use a hex name.
   return make_hex_ref( ord, out );
}

//////////////////////////////////////////////////////////////////////////

string uni_to_latin( const wstring& uni )
{
   string latin, ref;
   latin.reserve( uni.length( ) );

   const unsigned short* sz = (const unsigned short*) uni.c_str( );
   for (unsigned short ch; (ch = *sz); ++sz)
   {
      // If the character value is too big, escape it.
      if (ch > 0xFF)
         latin += *ord_to_ref( ch, &ref );
      else
         latin += (unsigned char) ch;
   }
   return latin;
}

string uni_to_ascii( const wstring& uni )
{
   string ascii, ref;
   ascii.reserve( uni.length( ) );

   const unsigned short* sz = (const unsigned short*) uni.c_str( );
   for (unsigned short ch; (ch = *sz); ++sz)
   {
      // If the character value is too big, escape it.
      if (ch > 0x7E)
         ascii += *ord_to_ref( ch, &ref );
      else
         ascii += (unsigned char) ch;
   }
   return ascii;
}

string ascii_to_latin( const string& ascii )
{
   string latin, ref;
   latin.reserve( ascii.length( ) );

   const unsigned char* sz = (const unsigned char*) ascii.c_str( );
   for (unsigned char ch; (ch = *sz); ++sz)
   {
      // Reference?
      if (ch == '&')
      {
         unsigned short ord = 
            ref_to_ord( *extract_ref( ascii, (uintptr_t) sz - (uintptr_t) ascii.c_str( ), &ref ) );

         // Only convert references that fit.
         if (ord && ord <= 0xFF)
         {
            latin += (unsigned char) ord;
            sz += ref.length( ) - 1; // Skip past the entire reference.
         }
         else
            // Bad reference or won't fit.
            latin += ch;
      }
      else
         latin += ch;
   }
   return latin;
}


string latin_to_ascii( const string& latin )
{
   string ascii, ref;
   ascii.reserve( latin.length( ) );

   const unsigned char* sz = (const unsigned char*) latin.c_str( );
   for (unsigned char ch; (ch = *sz); ++sz)
   {
      // If the character value is too big, escape it.
      if (ch > 0x7E)
         ascii += *ord_to_ref( ch, &ref );
      else
         ascii += ch;
   }
   return ascii;
}

wstring latin_to_uni( const string& latin )
{
   string  ref;
   wstring uni;

   const unsigned char* sz = (const unsigned char*) latin.c_str( );
   for (unsigned short ch; (ch = *sz); ++sz)
   {
      if (ch == '&')
      {
         unsigned short ord = 
            ref_to_ord( *extract_ref( latin, (uintptr_t) sz - (uintptr_t) latin.c_str( ), &ref ) );

         if (ord)
         {
            uni += (unsigned short) ord;
            sz += ref.length( ) - 1; // Skip past the entire reference.
         }
         else
            // Bad reference.
            uni += (unsigned short) ch;
      }
      else
         uni += (unsigned short) ch;
   }
   return uni;
}

//////////////////////////////////////////////////////////////////////////

} // namespace xml_charset