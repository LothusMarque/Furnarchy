unit CursesHTMLUtils;
{
This unit is created from stripped and slightly modified portions of Alcinoe HTML Functions.
Author(s):    Stéphane Vander Clock (svanderclock@arkadia.com)
Sponsor(s):   Arkadia SA (http://www.arkadia.com)

Most of this originally came directly from ALFcnHTML.pas.
It has been modified to use the HTML entity list for character decoding.
}
interface
uses
 classes, sysutils, Windows;

function ALUTF8XMLTextElementDecode(Src: string): string;

implementation

var
 LstEntities : TStrings;

procedure InitHtmlEntitiesLst(HtmlEntities: Tstrings);
Begin
  HtmlEntities.Clear;
  HtmlEntities.AddObject('zwnj',pointer(8204)); // zero width non-joiner,   U+200C NEW RFC 2070 -->
  HtmlEntities.AddObject('zwj',pointer(8205)); // zero width joiner, U+200D NEW RFC 2070 -->
  HtmlEntities.AddObject('zeta',pointer(950)); // greek small letter zeta, U+03B6 ISOgrk3 -->
  HtmlEntities.AddObject('Zeta',pointer(918)); // greek capital letter zeta, U+0396 -->
  HtmlEntities.AddObject('yuml',pointer(255)); // latin small letter y with diaeresis, U+00FF ISOlat1 -->
  HtmlEntities.AddObject('Yuml',pointer(376)); // latin capital letter Y with diaeresis,   U+0178 ISOlat2 -->
  HtmlEntities.AddObject('yen',pointer(165)); // yen sign = yuan sign, U+00A5 ISOnum -->
  HtmlEntities.AddObject('yacute',pointer(253)); // latin small letter y with acute, U+00FD ISOlat1 -->
  HtmlEntities.AddObject('Yacute',pointer(221)); // latin capital letter Y with acute, U+00DD ISOlat1 -->
  HtmlEntities.AddObject('xi',pointer(958)); // greek small letter xi, U+03BE ISOgrk3 -->
  HtmlEntities.AddObject('Xi',pointer(926)); // greek capital letter xi, U+039E ISOgrk3 -->
  HtmlEntities.AddObject('weierp',pointer(8472)); // script capital P = power set    = Weierstrass p, U+2118 ISOamso -->
  HtmlEntities.AddObject('uuml',pointer(252)); // latin small letter u with diaeresis, U+00FC ISOlat1 -->
  HtmlEntities.AddObject('Uuml',pointer(220)); // latin capital letter U with diaeresis, U+00DC ISOlat1 -->
  HtmlEntities.AddObject('upsilon',pointer(965)); // greek small letter upsilon,   U+03C5 ISOgrk3 -->
  HtmlEntities.AddObject('Upsilon',pointer(933)); // greek capital letter upsilon,   U+03A5 ISOgrk3 -->
  HtmlEntities.AddObject('upsih',pointer(978)); // greek upsilon with hook symbol,   U+03D2 NEW -->
  HtmlEntities.AddObject('uml',pointer(168)); // diaeresis = spacing diaeresis, U+00A8 ISOdia -->
  HtmlEntities.AddObject('ugrave',pointer(249)); // latin small letter u with grave, U+00F9 ISOlat1 -->
  HtmlEntities.AddObject('Ugrave',pointer(217)); // latin capital letter U with grave, U+00D9 ISOlat1 -->
  HtmlEntities.AddObject('ucirc',pointer(251)); // latin small letter u with circumflex, U+00FB ISOlat1 -->
  HtmlEntities.AddObject('Ucirc',pointer(219)); // latin capital letter U with circumflex, U+00DB ISOlat1 -->
  HtmlEntities.AddObject('uArr',pointer(8657)); // upwards double arrow, U+21D1 ISOamsa -->
  HtmlEntities.AddObject('uarr',pointer(8593)); // upwards arrow, U+2191 ISOnum-->
  HtmlEntities.AddObject('uacute',pointer(250)); // latin small letter u with acute, U+00FA ISOlat1 -->
  HtmlEntities.AddObject('Uacute',pointer(218)); // latin capital letter U with acute, U+00DA ISOlat1 -->
  HtmlEntities.AddObject('trade',pointer(8482)); // trade mark sign, U+2122 ISOnum -->
  HtmlEntities.AddObject('times',pointer(215)); // multiplication sign, U+00D7 ISOnum -->
  HtmlEntities.AddObject('tilde',pointer(732)); // small tilde, U+02DC ISOdia -->
  HtmlEntities.AddObject('thorn',pointer(254)); // latin small letter thorn, U+00FE ISOlat1 -->
  HtmlEntities.AddObject('THORN',pointer(222)); // latin capital letter THORN, U+00DE ISOlat1 -->
  HtmlEntities.AddObject('thinsp',pointer(8201)); // thin space, U+2009 ISOpub -->
  HtmlEntities.AddObject('thetasym',pointer(977)); // greek small letter theta symbol,   U+03D1 NEW -->
  HtmlEntities.AddObject('theta',pointer(952)); // greek small letter theta,   U+03B8 ISOgrk3 -->
  HtmlEntities.AddObject('Theta',pointer(920)); // greek capital letter theta,   U+0398 ISOgrk3 -->
  HtmlEntities.AddObject('there4',pointer(8756)); // therefore, U+2234 ISOtech -->
  HtmlEntities.AddObject('tau',pointer(964)); // greek small letter tau, U+03C4 ISOgrk3 -->
  HtmlEntities.AddObject('Tau',pointer(932)); // greek capital letter tau, U+03A4 -->
  HtmlEntities.AddObject('szlig',pointer(223)); // latin small letter sharp s = ess-zed, U+00DF ISOlat1 -->
  HtmlEntities.AddObject('supe',pointer(8839)); // superset of or equal to,    U+2287 ISOtech -->
  HtmlEntities.AddObject('sup3',pointer(179)); // superscript three = superscript digit three = cubed, U+00B3 ISOnum -->
  HtmlEntities.AddObject('sup2',pointer(178)); // superscript two = superscript digit two = squared, U+00B2 ISOnum -->
  HtmlEntities.AddObject('sup1',pointer(185)); // superscript one = superscript digit one, U+00B9 ISOnum -->
  HtmlEntities.AddObject('sup',pointer(8835)); // superset of, U+2283 ISOtech -->
  HtmlEntities.AddObject('sum',pointer(8721)); // n-ary sumation, U+2211 ISOamsb -->
  HtmlEntities.AddObject('sube',pointer(8838)); // subset of or equal to, U+2286 ISOtech -->
  HtmlEntities.AddObject('sub',pointer(8834)); // subset of, U+2282 ISOtech -->
  HtmlEntities.AddObject('spades',pointer(9824)); // black spade suit, U+2660 ISOpub -->
  HtmlEntities.AddObject('sim',pointer(8764)); // tilde operator = varies with = similar to,    U+223C ISOtech -->
  HtmlEntities.AddObject('sigmaf',pointer(962)); // greek small letter final sigma,   U+03C2 ISOgrk3 -->
  HtmlEntities.AddObject('sigma',pointer(963)); // greek small letter sigma,   U+03C3 ISOgrk3 -->
  HtmlEntities.AddObject('Sigma',pointer(931)); // greek capital letter sigma,   U+03A3 ISOgrk3 -->
  HtmlEntities.AddObject('shy',pointer(173)); // soft hyphen = discretionary hyphen, U+00AD ISOnum -->
  HtmlEntities.AddObject('sect',pointer(167)); // section sign, U+00A7 ISOnum -->
  HtmlEntities.AddObject('sdot',pointer(8901)); // dot operator, U+22C5 ISOamsb -->
  HtmlEntities.AddObject('scaron',pointer(353)); // latin small letter s with caron,   U+0161 ISOlat2 -->
  HtmlEntities.AddObject('Scaron',pointer(352)); // latin capital letter S with caron,   U+0160 ISOlat2 -->
  HtmlEntities.AddObject('sbquo',pointer(8218)); // single low-9 quotation mark, U+201A NEW -->
  HtmlEntities.AddObject('rsquo',pointer(8217)); // right single quotation mark,   U+2019 ISOnum -->
  HtmlEntities.AddObject('rsaquo',pointer(8250)); // single right-pointing angle quotation mark,   U+203A ISO proposed -->
  HtmlEntities.AddObject('rlm',pointer(8207)); // right-to-left mark, U+200F NEW RFC 2070 -->
  HtmlEntities.AddObject('rho',pointer(961)); // greek small letter rho, U+03C1 ISOgrk3 -->
  HtmlEntities.AddObject('Rho',pointer(929)); // greek capital letter rho, U+03A1 -->
  HtmlEntities.AddObject('rfloor',pointer(8971)); // right floor, U+230B ISOamsc  -->
  HtmlEntities.AddObject('reg',pointer(174)); // registered sign = registered trade mark sign, U+00AE ISOnum -->
  HtmlEntities.AddObject('real',pointer(8476)); // blackletter capital R = real part symbol,    U+211C ISOamso -->
  HtmlEntities.AddObject('rdquo',pointer(8221)); // right double quotation mark,   U+201D ISOnum -->
  HtmlEntities.AddObject('rceil',pointer(8969)); // right ceiling, U+2309 ISOamsc  -->
  HtmlEntities.AddObject('rArr',pointer(8658)); // rightwards double arrow,    U+21D2 ISOtech -->
  HtmlEntities.AddObject('rarr',pointer(8594)); // rightwards arrow, U+2192 ISOnum -->
  HtmlEntities.AddObject('raquo',pointer(187)); // right-pointing double angle quotation mark = right pointing guillemet, U+00BB ISOnum -->
  HtmlEntities.AddObject('rang',pointer(9002)); // right-pointing angle bracket = ket,    U+232A ISOtech -->
  HtmlEntities.AddObject('radic',pointer(8730)); // square root = radical sign,    U+221A ISOtech -->
  HtmlEntities.AddObject('quot',pointer(34)); // quotation mark = APL quote,   U+0022 ISOnum -->
  HtmlEntities.AddObject('psi',pointer(968)); // greek small letter psi, U+03C8 ISOgrk3 -->
  HtmlEntities.AddObject('Psi',pointer(936)); // greek capital letter psi,   U+03A8 ISOgrk3 -->
  HtmlEntities.AddObject('prop',pointer(8733)); // proportional to, U+221D ISOtech -->
  HtmlEntities.AddObject('prod',pointer(8719)); // n-ary product = product sign,    U+220F ISOamsb -->
  HtmlEntities.AddObject('Prime',pointer(8243)); // double prime = seconds = inches,    U+2033 ISOtech -->
  HtmlEntities.AddObject('prime',pointer(8242)); // prime = minutes = feet, U+2032 ISOtech -->
  HtmlEntities.AddObject('pound',pointer(163)); // pound sign, U+00A3 ISOnum -->
  HtmlEntities.AddObject('plusmn',pointer(177)); // plus-minus sign = plus-or-minus sign, U+00B1 ISOnum -->
  HtmlEntities.AddObject('piv',pointer(982)); // greek pi symbol, U+03D6 ISOgrk3 -->
  HtmlEntities.AddObject('pi',pointer(960)); // greek small letter pi, U+03C0 ISOgrk3 -->
  HtmlEntities.AddObject('Pi',pointer(928)); // greek capital letter pi, U+03A0 ISOgrk3 -->
  HtmlEntities.AddObject('phi',pointer(966)); // greek small letter phi, U+03C6 ISOgrk3 -->
  HtmlEntities.AddObject('Phi',pointer(934)); // greek capital letter phi,   U+03A6 ISOgrk3 -->
  HtmlEntities.AddObject('perp',pointer(8869)); // up tack = orthogonal to = perpendicular,    U+22A5 ISOtech -->
  HtmlEntities.AddObject('permil',pointer(8240)); // per mille sign, U+2030 ISOtech -->
  HtmlEntities.AddObject('part',pointer(8706)); // partial differential, U+2202 ISOtech  -->
  HtmlEntities.AddObject('para',pointer(182)); // pilcrow sign = paragraph sign, U+00B6 ISOnum -->
  HtmlEntities.AddObject('ouml',pointer(246)); // latin small letter o with diaeresis, U+00F6 ISOlat1 -->
  HtmlEntities.AddObject('Ouml',pointer(214)); // latin capital letter O with diaeresis, U+00D6 ISOlat1 -->
  HtmlEntities.AddObject('otimes',pointer(8855)); // circled times = vector product,    U+2297 ISOamsb -->
  HtmlEntities.AddObject('otilde',pointer(245)); // latin small letter o with tilde, U+00F5 ISOlat1 -->
  HtmlEntities.AddObject('Otilde',pointer(213)); // latin capital letter O with tilde, U+00D5 ISOlat1 -->
  HtmlEntities.AddObject('oslash',pointer(248)); // latin small letter o with stroke, = latin small letter o slash, U+00F8 ISOlat1 -->
  HtmlEntities.AddObject('Oslash',pointer(216)); // latin capital letter O with stroke = latin capital letter O slash, U+00D8 ISOlat1 -->
  HtmlEntities.AddObject('ordm',pointer(186)); // masculine ordinal indicator, U+00BA ISOnum -->
  HtmlEntities.AddObject('ordf',pointer(170)); // feminine ordinal indicator, U+00AA ISOnum -->
  HtmlEntities.AddObject('or',pointer(8744)); // logical or = vee, U+2228 ISOtech -->
  HtmlEntities.AddObject('oplus',pointer(8853)); // circled plus = direct sum,    U+2295 ISOamsb -->
  HtmlEntities.AddObject('omicron',pointer(959)); // greek small letter omicron, U+03BF NEW -->
  HtmlEntities.AddObject('Omicron',pointer(927)); // greek capital letter omicron, U+039F -->
  HtmlEntities.AddObject('omega',pointer(969)); // greek small letter omega,   U+03C9 ISOgrk3 -->
  HtmlEntities.AddObject('Omega',pointer(937)); // greek capital letter omega,   U+03A9 ISOgrk3 -->
  HtmlEntities.AddObject('oline',pointer(8254)); // overline = spacing overscore,    U+203E NEW -->
  HtmlEntities.AddObject('ograve',pointer(242)); // latin small letter o with grave, U+00F2 ISOlat1 -->
  HtmlEntities.AddObject('Ograve',pointer(210)); // latin capital letter O with grave, U+00D2 ISOlat1 -->
  HtmlEntities.AddObject('oelig',pointer(339)); // latin small ligature oe, U+0153 ISOlat2 -->
  HtmlEntities.AddObject('OElig',pointer(338)); // latin capital ligature OE,   U+0152 ISOlat2 -->
  HtmlEntities.AddObject('ocirc',pointer(244)); // latin small letter o with circumflex, U+00F4 ISOlat1 -->
  HtmlEntities.AddObject('Ocirc',pointer(212)); // latin capital letter O with circumflex, U+00D4 ISOlat1 -->
  HtmlEntities.AddObject('oacute',pointer(243)); // latin small letter o with acute, U+00F3 ISOlat1 -->
  HtmlEntities.AddObject('Oacute',pointer(211)); // latin capital letter O with acute, U+00D3 ISOlat1 -->
  HtmlEntities.AddObject('nu',pointer(957)); // greek small letter nu, U+03BD ISOgrk3 -->
  HtmlEntities.AddObject('Nu',pointer(925)); // greek capital letter nu, U+039D -->
  HtmlEntities.AddObject('ntilde',pointer(241)); // latin small letter n with tilde, U+00F1 ISOlat1 -->
  HtmlEntities.AddObject('Ntilde',pointer(209)); // latin capital letter N with tilde, U+00D1 ISOlat1 -->
  HtmlEntities.AddObject('nsub',pointer(8836)); // not a subset of, U+2284 ISOamsn -->
  HtmlEntities.AddObject('notin',pointer(8713)); // not an element of, U+2209 ISOtech -->
  HtmlEntities.AddObject('not',pointer(172)); // not sign, U+00AC ISOnum -->
  HtmlEntities.AddObject('ni',pointer(8715)); // contains as member, U+220B ISOtech -->
  HtmlEntities.AddObject('ne',pointer(8800)); // not equal to, U+2260 ISOtech -->
  HtmlEntities.AddObject('ndash',pointer(8211)); // en dash, U+2013 ISOpub -->
  HtmlEntities.AddObject('nbsp',pointer(160)); // no-break space = non-breaking space, U+00A0 ISOnum -->
  HtmlEntities.AddObject('nabla',pointer(8711)); // nabla = backward difference,    U+2207 ISOtech -->
  HtmlEntities.AddObject('mu',pointer(956)); // greek small letter mu, U+03BC ISOgrk3 -->
  HtmlEntities.AddObject('Mu',pointer(924)); // greek capital letter mu, U+039C -->
  HtmlEntities.AddObject('minus',pointer(8722)); // minus sign, U+2212 ISOtech -->
  HtmlEntities.AddObject('middot',pointer(183)); // middle dot = Georgian comma = Greek middle dot, U+00B7 ISOnum -->
  HtmlEntities.AddObject('micro',pointer(181)); // micro sign, U+00B5 ISOnum -->
  HtmlEntities.AddObject('mdash',pointer(8212)); // em dash, U+2014 ISOpub -->
  HtmlEntities.AddObject('macr',pointer(175)); // macron = spacing macron = overline = APL overbar, U+00AF ISOdia -->
  HtmlEntities.AddObject('lt',pointer(60)); // less-than sign, U+003C ISOnum -->
  HtmlEntities.AddObject('lsquo',pointer(8216)); // left single quotation mark,   U+2018 ISOnum -->
  HtmlEntities.AddObject('lsaquo',pointer(8249)); // single left-pointing angle quotation mark,   U+2039 ISO proposed -->
  HtmlEntities.AddObject('lrm',pointer(8206)); // left-to-right mark, U+200E NEW RFC 2070 -->
  HtmlEntities.AddObject('loz',pointer(9674)); // lozenge, U+25CA ISOpub -->
  HtmlEntities.AddObject('lowast',pointer(8727)); // asterisk operator, U+2217 ISOtech -->
  HtmlEntities.AddObject('lfloor',pointer(8970)); // left floor = apl downstile,    U+230A ISOamsc  -->
  HtmlEntities.AddObject('le',pointer(8804)); // less-than or equal to, U+2264 ISOtech -->
  HtmlEntities.AddObject('ldquo',pointer(8220)); // left double quotation mark,   U+201C ISOnum -->
  HtmlEntities.AddObject('lceil',pointer(8968)); // left ceiling = apl upstile,    U+2308 ISOamsc  -->
  HtmlEntities.AddObject('lArr',pointer(8656)); // leftwards double arrow, U+21D0 ISOtech -->
  HtmlEntities.AddObject('larr',pointer(8592)); // leftwards arrow, U+2190 ISOnum -->
  HtmlEntities.AddObject('laquo',pointer(171)); // left-pointing double angle quotation mark = left pointing guillemet, U+00AB ISOnum -->
  HtmlEntities.AddObject('lang',pointer(9001)); // left-pointing angle bracket = bra,    U+2329 ISOtech -->
  HtmlEntities.AddObject('lambda',pointer(955)); // greek small letter lambda,   U+03BB ISOgrk3 -->
  HtmlEntities.AddObject('Lambda',pointer(923)); // greek capital letter lambda,   U+039B ISOgrk3 -->
  HtmlEntities.AddObject('kappa',pointer(954)); // greek small letter kappa,   U+03BA ISOgrk3 -->
  HtmlEntities.AddObject('Kappa',pointer(922)); // greek capital letter kappa, U+039A -->
  HtmlEntities.AddObject('iuml',pointer(239)); // latin small letter i with diaeresis, U+00EF ISOlat1 -->
  HtmlEntities.AddObject('Iuml',pointer(207)); // latin capital letter I with diaeresis, U+00CF ISOlat1 -->
  HtmlEntities.AddObject('isin',pointer(8712)); // element of, U+2208 ISOtech -->
  HtmlEntities.AddObject('iquest',pointer(191)); // inverted question mark = turned question mark, U+00BF ISOnum -->
  HtmlEntities.AddObject('iota',pointer(953)); // greek small letter iota, U+03B9 ISOgrk3 -->
  HtmlEntities.AddObject('Iota',pointer(921)); // greek capital letter iota, U+0399 -->
  HtmlEntities.AddObject('int',pointer(8747)); // integral, U+222B ISOtech -->
  HtmlEntities.AddObject('infin',pointer(8734)); // infinity, U+221E ISOtech -->
  HtmlEntities.AddObject('image',pointer(8465)); // blackletter capital I = imaginary part,    U+2111 ISOamso -->
  HtmlEntities.AddObject('igrave',pointer(236)); // latin small letter i with grave, U+00EC ISOlat1 -->
  HtmlEntities.AddObject('Igrave',pointer(204)); // latin capital letter I with grave, U+00CC ISOlat1 -->
  HtmlEntities.AddObject('iexcl',pointer(161)); // inverted exclamation mark, U+00A1 ISOnum -->
  HtmlEntities.AddObject('icirc',pointer(238)); // latin small letter i with circumflex, U+00EE ISOlat1 -->
  HtmlEntities.AddObject('Icirc',pointer(206)); // latin capital letter I with circumflex, U+00CE ISOlat1 -->
  HtmlEntities.AddObject('iacute',pointer(237)); // latin small letter i with acute, U+00ED ISOlat1 -->
  HtmlEntities.AddObject('Iacute',pointer(205)); // latin capital letter I with acute, U+00CD ISOlat1 -->
  HtmlEntities.AddObject('hellip',pointer(8230)); // horizontal ellipsis = three dot leader,    U+2026 ISOpub  -->
  HtmlEntities.AddObject('hearts',pointer(9829)); // black heart suit = valentine,    U+2665 ISOpub -->
  HtmlEntities.AddObject('hArr',pointer(8660)); // left right double arrow,    U+21D4 ISOamsa -->
  HtmlEntities.AddObject('harr',pointer(8596)); // left right arrow, U+2194 ISOamsa -->
  HtmlEntities.AddObject('gt',pointer(62)); // greater-than sign, U+003E ISOnum -->
  HtmlEntities.AddObject('ge',pointer(8805)); // greater-than or equal to,    U+2265 ISOtech -->
  HtmlEntities.AddObject('gamma',pointer(947)); // greek small letter gamma,   U+03B3 ISOgrk3 -->
  HtmlEntities.AddObject('Gamma',pointer(915)); // greek capital letter gamma,   U+0393 ISOgrk3 -->
  HtmlEntities.AddObject('frasl',pointer(8260)); // fraction slash, U+2044 NEW -->
  HtmlEntities.AddObject('frac34',pointer(190)); // vulgar fraction three quarters = fraction three quarters, U+00BE ISOnum -->
  HtmlEntities.AddObject('frac14',pointer(188)); // vulgar fraction one quarter = fraction one quarter, U+00BC ISOnum -->
  HtmlEntities.AddObject('frac12',pointer(189)); // vulgar fraction one half = fraction one half, U+00BD ISOnum -->
  HtmlEntities.AddObject('forall',pointer(8704)); // for all, U+2200 ISOtech -->
  HtmlEntities.AddObject('fnof',pointer(402)); // latin small f with hook = function   = florin, U+0192 ISOtech -->
  HtmlEntities.AddObject('exist',pointer(8707)); // there exists, U+2203 ISOtech -->
  HtmlEntities.AddObject('euro',pointer(8364)); // euro sign, U+20AC NEW -->
  HtmlEntities.AddObject('euml',pointer(235)); // latin small letter e with diaeresis, U+00EB ISOlat1 -->
  HtmlEntities.AddObject('Euml',pointer(203)); // latin capital letter E with diaeresis, U+00CB ISOlat1 -->
  HtmlEntities.AddObject('eth',pointer(240)); // latin small letter eth, U+00F0 ISOlat1 -->
  HtmlEntities.AddObject('ETH',pointer(208)); // latin capital letter ETH, U+00D0 ISOlat1 -->
  HtmlEntities.AddObject('eta',pointer(951)); // greek small letter eta, U+03B7 ISOgrk3 -->
  HtmlEntities.AddObject('Eta',pointer(919)); // greek capital letter eta, U+0397 -->
  HtmlEntities.AddObject('equiv',pointer(8801)); // identical to, U+2261 ISOtech -->
  HtmlEntities.AddObject('epsilon',pointer(949)); // greek small letter epsilon,   U+03B5 ISOgrk3 -->
  HtmlEntities.AddObject('Epsilon',pointer(917)); // greek capital letter epsilon, U+0395 -->
  HtmlEntities.AddObject('ensp',pointer(8194)); // en space, U+2002 ISOpub -->
  HtmlEntities.AddObject('emsp',pointer(8195)); // em space, U+2003 ISOpub -->
  HtmlEntities.AddObject('empty',pointer(8709)); // empty set = null set = diameter,    U+2205 ISOamso -->
  HtmlEntities.AddObject('egrave',pointer(232)); // latin small letter e with grave, U+00E8 ISOlat1 -->
  HtmlEntities.AddObject('Egrave',pointer(200)); // latin capital letter E with grave, U+00C8 ISOlat1 -->
  HtmlEntities.AddObject('ecirc',pointer(234)); // latin small letter e with circumflex, U+00EA ISOlat1 -->
  HtmlEntities.AddObject('Ecirc',pointer(202)); // latin capital letter E with circumflex, U+00CA ISOlat1 -->
  HtmlEntities.AddObject('eacute',pointer(233)); // latin small letter e with acute, U+00E9 ISOlat1 -->
  HtmlEntities.AddObject('Eacute',pointer(201)); // latin capital letter E with acute, U+00C9 ISOlat1 -->
  HtmlEntities.AddObject('divide',pointer(247)); // division sign, U+00F7 ISOnum -->
  HtmlEntities.AddObject('diams',pointer(9830)); // black diamond suit, U+2666 ISOpub -->
  HtmlEntities.AddObject('delta',pointer(948)); // greek small letter delta,   U+03B4 ISOgrk3 -->
  HtmlEntities.AddObject('Delta',pointer(916)); // greek capital letter delta,   U+0394 ISOgrk3 -->
  HtmlEntities.AddObject('deg',pointer(176)); // degree sign, U+00B0 ISOnum -->
  HtmlEntities.AddObject('dArr',pointer(8659)); // downwards double arrow, U+21D3 ISOamsa -->
  HtmlEntities.AddObject('darr',pointer(8595)); // downwards arrow, U+2193 ISOnum -->
  HtmlEntities.AddObject('Dagger',pointer(8225)); // double dagger, U+2021 ISOpub -->
  HtmlEntities.AddObject('dagger',pointer(8224)); // dagger, U+2020 ISOpub -->
  HtmlEntities.AddObject('curren',pointer(164)); // currency sign, U+00A4 ISOnum -->
  HtmlEntities.AddObject('cup',pointer(8746)); // union = cup, U+222A ISOtech -->
  HtmlEntities.AddObject('crarr',pointer(8629)); // downwards arrow with corner leftwards    = carriage return, U+21B5 NEW -->
  HtmlEntities.AddObject('copy',pointer(169)); // copyright sign, U+00A9 ISOnum -->
  HtmlEntities.AddObject('cong',pointer(8773)); // approximately equal to, U+2245 ISOtech -->
  HtmlEntities.AddObject('clubs',pointer(9827)); // black club suit = shamrock,    U+2663 ISOpub -->
  HtmlEntities.AddObject('circ',pointer(710)); // modifier letter circumflex accent,   U+02C6 ISOpub -->
  HtmlEntities.AddObject('chi',pointer(967)); // greek small letter chi, U+03C7 ISOgrk3 -->
  HtmlEntities.AddObject('Chi',pointer(935)); // greek capital letter chi, U+03A7 -->
  HtmlEntities.AddObject('cent',pointer(162)); // cent sign, U+00A2 ISOnum -->
  HtmlEntities.AddObject('cedil',pointer(184)); // cedilla = spacing cedilla, U+00B8 ISOdia -->
  HtmlEntities.AddObject('ccedil',pointer(231)); // latin small letter c with cedilla, U+00E7 ISOlat1 -->
  HtmlEntities.AddObject('Ccedil',pointer(199)); // latin capital letter C with cedilla, U+00C7 ISOlat1 -->
  HtmlEntities.AddObject('cap',pointer(8745)); // intersection = cap, U+2229 ISOtech -->
  HtmlEntities.AddObject('bull',pointer(8226)); // bullet = black small circle,    U+2022 ISOpub  -->
  HtmlEntities.AddObject('brvbar',pointer(166)); // broken bar = broken vertical bar, U+00A6 ISOnum -->
  HtmlEntities.AddObject('beta',pointer(946)); // greek small letter beta, U+03B2 ISOgrk3 -->
  HtmlEntities.AddObject('Beta',pointer(914)); // greek capital letter beta, U+0392 -->
  HtmlEntities.AddObject('bdquo',pointer(8222)); // double low-9 quotation mark, U+201E NEW -->
  HtmlEntities.AddObject('auml',pointer(228)); // latin small letter a with diaeresis, U+00E4 ISOlat1 -->
  HtmlEntities.AddObject('Auml',pointer(196)); // latin capital letter A with diaeresis, U+00C4 ISOlat1 -->
  HtmlEntities.AddObject('atilde',pointer(227)); // latin small letter a with tilde, U+00E3 ISOlat1 -->
  HtmlEntities.AddObject('Atilde',pointer(195)); // latin capital letter A with tilde, U+00C3 ISOlat1 -->
  HtmlEntities.AddObject('asymp',pointer(8776)); // almost equal to = asymptotic to,    U+2248 ISOamsr -->
  HtmlEntities.AddObject('aring',pointer(229)); // latin small letter a with ring above = latin small letter a ring, U+00E5 ISOlat1 -->
  HtmlEntities.AddObject('Aring',pointer(197)); // latin capital letter A with ring above = latin capital letter A ring, U+00C5 ISOlat1 -->
  HtmlEntities.AddObject('ang',pointer(8736)); // angle, U+2220 ISOamso -->
  HtmlEntities.AddObject('and',pointer(8743)); // logical and = wedge, U+2227 ISOtech -->
  HtmlEntities.AddObject('amp',pointer(38)); // ampersand, U+0026 ISOnum -->
  HtmlEntities.AddObject('alpha',pointer(945)); // greek small letter alpha,   U+03B1 ISOgrk3 -->
  HtmlEntities.AddObject('Alpha',pointer(913)); // greek capital letter alpha, U+0391 -->
  HtmlEntities.AddObject('alefsym',pointer(8501)); // alef symbol = first transfinite cardinal,    U+2135 NEW -->
  HtmlEntities.AddObject('agrave',pointer(224)); // latin small letter a with grave = latin small letter a grave, U+00E0 ISOlat1 -->
  HtmlEntities.AddObject('Agrave',pointer(192)); // latin capital letter A with grave = latin capital letter A grave, U+00C0 ISOlat1 -->
  HtmlEntities.AddObject('aelig',pointer(230)); // latin small letter ae = latin small ligature ae, U+00E6 ISOlat1 -->
  HtmlEntities.AddObject('AElig',pointer(198)); // latin capital letter AE = latin capital ligature AE, U+00C6 ISOlat1 -->
  HtmlEntities.AddObject('acute',pointer(180)); // acute accent = spacing acute, U+00B4 ISOdia -->
  HtmlEntities.AddObject('acirc',pointer(226)); // latin small letter a with circumflex, U+00E2 ISOlat1 -->
  HtmlEntities.AddObject('Acirc',pointer(194)); // latin capital letter A with circumflex, U+00C2 ISOlat1 -->
  HtmlEntities.AddObject('aacute',pointer(225)); // latin small letter a with acute, U+00E1 ISOlat1 -->
  HtmlEntities.AddObject('Aacute',pointer(193)); // latin capital letter A with acute, U+00C1 ISOlat1 -->
end;

function ALUTF8XMLTextElementDecode(Src: string): string;
var CurrentSrcPos, CurrentResultPos : Integer;
    j: integer;
    aTmpInteger, ti1: Integer;
    SrcLength: integer;
    aEntity: String;

    {--------------------------------------}
    procedure CopyCurrentSrcPosCharToResult;
    Begin
      result[CurrentResultPos] := src[CurrentSrcPos];
      inc(CurrentResultPos);
      inc(CurrentSrcPos);
    end;

    {---------------------------------------------------------------------------------}
    procedure CopyCharToResult(aUnicodeOrdEntity: Integer; aNewCurrentSrcPos: integer);
    Var aString: String;
        K: integer;
    Begin
      aString := Char(aUnicodeOrdEntity);
      For k := 1 to length(aString) do begin
        result[CurrentResultPos] := aString[k];
        inc(CurrentResultPos);
      end;
      CurrentSrcPos := aNewCurrentSrcPos;
    end;

begin
  {init var}
  CurrentSrcPos := 1;
  CurrentResultPos := 1;
  SrcLength := Length(src);
  SetLength(Result,SrcLength);
  {start loop}
  while (CurrentSrcPos <= SrcLength) do begin
    {HTMLentity detected}
    If src[CurrentSrcPos]='&' then begin
      {extract the HTML entity}
      j := CurrentSrcPos;
      while (J <= SrcLength) and (src[j] <> ';') and (j-CurrentSrcPos<=12) do inc(j);
      {HTML entity is valid}
      If (J<=SrcLength) and (j-CurrentSrcPos<=12) then Begin
        {HTML entity is numeric}
        IF (Src[CurrentSrcPos+1] = '#') then begin
          {HTML entity is hexa}
          IF (Src[CurrentSrcPos+2] = 'x') then begin
            if trystrtoint(
                           '$' + copy(//ALCopyStr(
                                           Src,
                                           CurrentSrcPos+3,
                                           j-CurrentSrcPos-3
                                          ),
                           aTmpInteger
                          )
            then CopyCharToResult(aTmpInteger, J+1)
            else CopyCurrentSrcPosCharToResult;
          end
          {HTML entity is numeric}
          else begin
            {numeric HTML entity is valid}
            if trystrtoint(
                           copy(//ALCopyStr(
                                     Src,
                                     CurrentSrcPos+2,
                                     j-CurrentSrcPos-2
                                    ),
                           aTmpInteger
                          )
            then CopyCharToResult(aTmpInteger, J+1)
            else CopyCurrentSrcPosCharToResult;
          end;
        end
        {HTML entity is literal}
        else begin
          //amp
          aEntity := copy(//ALCopyStr(
                               Src,
                               CurrentSrcPos+1,
                               j-CurrentSrcPos-1
                              );
          {If aEntity ='quot' then CopyCharToResult(34, J+1) // "
          else if aEntity = 'apos' then CopyCharToResult(39, J+1) // '
          else if aEntity = 'amp' then CopyCharToResult(38, J+1) // &
          else if aEntity = 'lt' then CopyCharToResult(60, J+1) // <
          else if aEntity = 'gt' then CopyCharToResult(62, J+1) // >     }
          ti1 := LstEntities.IndexOf(aentity);
          if ti1 > -1 then
           CopyCharToResult(integer(LstEntities.Objects[ti1]), J+1)
          else CopyCurrentSrcPosCharToResult;
        end;
      end
      else CopyCurrentSrcPosCharToResult;
    end
    else CopyCurrentSrcPosCharToResult;
  end;
  setLength(Result,CurrentResultPos-1);
end;

initialization
  LstEntities := TstringList.create;
  With (LstEntities as TstringList) do begin
    CaseSensitive := True;
    Duplicates := DupAccept;
    Sorted := True;
  end;
  InitHtmlEntitiesLst(LstEntities);
finalization
 LstEntities.Free;
end.
