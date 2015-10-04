<?xml version="1.0"?><doc>
<members>
<member name="T:yiffurc.MAJORMINOR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="80">
Defines a major and minor revision. 
</member>
<member name="T:yiffurc.IDXY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="87">
An object/floor/wall ID and a position pair. 
</member>
<member name="T:yiffurc.DSXY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="95">
A DS line and parameter pair. 
</member>
<member name="T:yiffurc.Move_Dir" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="103">
Player movement directions.
*  \todo In the wrong place? 
</member>
<member name="T:yiffurc.Go_Map" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="113">
Some main map numbers.
* add to ' ' to make the correct gomap.
*  \sa c2s_enc_gomap 
</member>
<member name="M:yiffurc.ftoi3(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="134">
Converts a 3-digit base-95 furcadian number to unsigned int.
*  \note
*     ftoi3( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.ftoi2(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="146">
Converts a 2-digit base-95 furcadian number to an integer.
*  \note
*     ftoi2( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.ftoi1(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="156">
Converts a 1-digit base-95 furcadian number to an integer.
*  \param   f  Base-95 Furcadian digit.

</member>
<member name="M:yiffurc.itof3(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="163">
Converts an integer to a 3-digit base-95 furcadian number.
*  \note
*     itof3( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-95 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.itof2(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="175">
Converts an integer to a 2-digit base-95 furcadian number.
*  \note
*     itof2( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-95 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.itof1(System.UInt32)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="186">
Converts an integer to a 1-digit base-95 furcadian number.
*  param    i     integer number.
*  \returns Base-95 Furcadian digit.

</member>
<member name="M:yiffurc.ftoi4_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="195">
Converts a 4-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi4_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[4])

</member>
<member name="M:yiffurc.ftoi3_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="208">
Converts a 3-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi3_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.ftoi2_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="220">
Converts a 2-digit base-220 furcadian number to an integer.
*  \note
*     ftoi2_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.ftoi1_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="230">
Converts a 1-digit base-220 furcadian number to an integer.
*  \param   f  Base-220 Furcadian digit.

</member>
<member name="M:yiffurc.itof4_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="237">
Converts an integer to a 4-digit base-220 furcadian number.
*  \note
*     itof4_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[4])

</member>
<member name="M:yiffurc.itof3_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="250">
Converts an integer to a 3-digit base-220 furcadian number.
*  \note
*     itof3_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.itof2_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="262">
Converts an integer to a 2-digit base-220 furcadian number.
*  \note
*     itof2_220( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-220 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.itof1_220(System.UInt32)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="273">
Converts an integer to a 1-digit base-220 furcadian number.
*  param    i     integer number.
*  \returns Base-220 Furcadian digit.

</member>
<member name="M:yiffurc.get_color_len(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32!System.Runtime.CompilerServices.IsConst)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="282">
Compute the length of a color string. 
*  \remarks This doesn't include the gender-species-digo bytes that
*           sometimes follow the color string. Those are always 3 bytes
*           long.

</member>
<member name="M:yiffurc.yiffurc_release(System.Void!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="289">
\addtogroup memory Memory Management
*  \{ 
Releases any dynamically allocated object returned by a YIFFurc API function. 
</member>
</members>
</doc>