/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef SCRIPTS_INCLUDE__ENGINE_3DRE_H
#define SCRIPTS_INCLUDE__ENGINE_3DRE_H

#include "engine_vector.h"
#include "engine_io.h"
#include "coltest.h"
#include "Vector3i.h"
#include "Vector2.h"
#include "Crc32.h"
#include "W3D.h"
#ifndef TTLE_EXPORTS
extern REF_DECL1(_PolygonCullMode, unsigned int);
#endif
class DX8TextureCategoryClass;
class MeshModelClass;
class Font3DInstanceClass;
class Font3DDataClass;
class AABTreeClass;
class MaterialInfoClass;
class LightEnvironmentClass;
class HTreeClass;
class ProgrammableShader;
class TextureClass;
class SurfaceClass;
class FontCharsClass;
class RenderInfoClass;

#define FOURCC_INTZ ((D3DFORMAT)(MAKEFOURCC('I','N','T','Z')))

enum WW3DFormat {
	WW3D_FORMAT_UNKNOWN = 0x0,
	WW3D_FORMAT_R8G8B8 = 0x1,
	WW3D_FORMAT_A8R8G8B8 = 0x2,
	WW3D_FORMAT_X8R8G8B8 = 0x3,
	WW3D_FORMAT_R5G6B5 = 0x4,
	WW3D_FORMAT_X1R5G5B5 = 0x5,
	WW3D_FORMAT_A1R5G5B5 = 0x6,
	WW3D_FORMAT_A4R4G4B4 = 0x7,
	WW3D_FORMAT_R3G3B2 = 0x8,
	WW3D_FORMAT_A8 = 0x9,
	WW3D_FORMAT_A8R3G3B2 = 0xa,
	WW3D_FORMAT_X4R4G4B4 = 0xb,
	WW3D_FORMAT_A8P8 = 0xc,
	WW3D_FORMAT_P8 = 0xd,
	WW3D_FORMAT_L8 = 0xe,
	WW3D_FORMAT_A8L8 = 0xf,
	WW3D_FORMAT_A4L4 = 0x10,
	WW3D_FORMAT_U8V8 = 0x11,
	WW3D_FORMAT_L6V5U5 = 0x12,
	WW3D_FORMAT_X8L8V8U8 = 0x13,
	WW3D_FORMAT_DXT1 = 0x14,
	WW3D_FORMAT_DXT2 = 0x15,
	WW3D_FORMAT_DXT3 = 0x16,
	WW3D_FORMAT_DXT4 = 0x17,
	WW3D_FORMAT_DXT5 = 0x18,
	// Depth buffer formats
	WW3D_FORMAT_D32,
	WW3D_FORMAT_D24S8,
	WW3D_FORMAT_D24X8,
	WW3D_FORMAT_D24X4S4,
	WW3D_FORMAT_D16,
	WW3D_FORMAT_D15S1,
	// HDR/floating-point formats
	WW3D_FORMAT_R16F,
	WW3D_FORMAT_GR16F,
	WW3D_FORMAT_ABGR16F,
	WW3D_FORMAT_R32F,
	WW3D_FORMAT_GR32F,
	WW3D_FORMAT_ABGR32F,
	// Vendor hack formats
	WW3D_FORMAT_INTZ,

	WW3D_FORMAT_COUNT,
};

inline unsigned int GetBytesPerPixel(WW3DFormat format)
{
	switch (format)
	{
	case WW3D_FORMAT_ABGR32F:
		return 16;
	case WW3D_FORMAT_ABGR16F:
	case WW3D_FORMAT_GR32F:
		return 8;
	case WW3D_FORMAT_A8R8G8B8:
	case WW3D_FORMAT_X8R8G8B8:
	case WW3D_FORMAT_X8L8V8U8:
	case WW3D_FORMAT_R32F:	
	case WW3D_FORMAT_GR16F:
	case WW3D_FORMAT_INTZ:
		return 4;
	case WW3D_FORMAT_R8G8B8:
		return 3;
	case WW3D_FORMAT_R5G6B5:
	case WW3D_FORMAT_A1R5G5B5:
	case WW3D_FORMAT_A4R4G4B4:
	case WW3D_FORMAT_U8V8:
	case WW3D_FORMAT_L6V5U5:
	case WW3D_FORMAT_R16F:
		return 2;
	case WW3D_FORMAT_R3G3B2:
	case WW3D_FORMAT_A8:
	case WW3D_FORMAT_P8:
	case WW3D_FORMAT_L8:
		return 1;
	default: 
		return 0;
	};
};

class ShaderCaps
{
public:
	static void Initialize();

	static D3DCAPS9 Caps;
	static int PixelShaderVersion;				// Pixel shader version * 10
	static int VertexShaderVersion;				// Vertex shader version * 10

	static bool DepthBiasSupported;				// True if device supports D3DRS_DEPTHBIAS 
	static bool SlopeScaleDepthBiasSupported;	// True if device supports D3DRS_SLOPESCALEDEPTHBIAS

	static bool FogSupported;					// True if device supports fog
	static bool RangeFogSupported;				// True if device supports range fog (not all video cards do. Examples include some cards by S3, Intel, Matrox)
	static bool BumpEnvMapSupported;            // bump env map supported

	static int MaxAnisotropy;					// Max anisotropy level supported
	
	static unsigned int MaxTextureWidth;		// Max texture width
	static unsigned int MaxTextureHeight;		// Max texture height

	static int MaxStreams;						// Max number of seperate vertex streams supported
	
	static bool SupportRenderToTextureFormat[WW3D_FORMAT_COUNT]; //render to texture format support
	static bool SupportDXTC; // is DXTC supported
	static bool SupportTextureFormat[WW3D_FORMAT_COUNT]; // texture format support
	static bool SupportGamma;
	static bool SupportINTZ; // is native depth buffer access supported
};

enum ShaderShiftConstants
{
	SHIFT_DEPTHCOMPARE			= 0,	// bit shift for depth comparison setting
	SHIFT_DEPTHMASK				= 3,	// bit shift for depth mask setting
	SHIFT_COLORMASK				= 4,	// bit shift for color mask setting
	SHIFT_DSTBLEND					= 5,	// bit shift for destination blend setting
	SHIFT_FOG						= 8,	// bit shift for fog setting
	SHIFT_PRIGRADIENT				= 10,	// bit shift for primary gradient setting
	SHIFT_SECGRADIENT				= 13,	// bit shift for secondary gradient setting
	SHIFT_SRCBLEND					= 14,	// bit shift for source blend setting
	SHIFT_TEXTURING				= 16,	// bit shift for texturing setting (1 bit)
	SHIFT_ALPHATEST				= 18,	// bit shift for alpha test setting
	SHIFT_CULLMODE					= 19,	// bit shift for cullmode setting
	SHIFT_POSTDETAILCOLORFUNC	= 20,	// bit shift for post-detail color function setting
	SHIFT_POSTDETAILALPHAFUNC	= 24	// bit shift for post-detail alpha function setting
};
#define SHADE_CNST(depth_compare, depth_mask, color_mask, src_blend, dst_blend, fog, pri_grad, sec_grad, texture, alpha_test, cullmode, post_det_color, post_det_alpha) \
	(	(depth_compare) << SHIFT_DEPTHCOMPARE | (depth_mask) << SHIFT_DEPTHMASK | \
		(color_mask) << SHIFT_COLORMASK | (dst_blend) << SHIFT_DSTBLEND | (fog) << SHIFT_FOG | \
		(pri_grad) << SHIFT_PRIGRADIENT | (sec_grad) << SHIFT_SECGRADIENT | \
		(src_blend) << SHIFT_SRCBLEND | (texture) << SHIFT_TEXTURING | \
		(alpha_test) << SHIFT_ALPHATEST | (cullmode) << SHIFT_CULLMODE | \
		(post_det_color) << SHIFT_POSTDETAILCOLORFUNC | \
		(post_det_alpha) << SHIFT_POSTDETAILALPHAFUNC)
struct W3dMaterial3Struct;
class ShaderClass
{
public:
	void Apply();
	void _Apply_Shader();
	enum AlphaTestType
	{
		ALPHATEST_DISABLE= 0,// disable alpha testing (default)
		ALPHATEST_ENABLE,		// enable alpha testing
		ALPHATEST_MAX			// end of enumeration
	};
	enum DepthCompareType
	{
		PASS_NEVER=0,        	// pass never
		PASS_LESS,        	// pass if incoming less than stored
		PASS_EQUAL,        	// pass if incoming equal to stored
		PASS_LEQUAL,			// pass if incoming less than or equal to stored (default)
		PASS_GREATER,        // pass if incoming greater than stored	
		PASS_NOTEQUAL,       // pass if incoming not equal to stored
		PASS_GEQUAL,			// pass if incoming greater than or equal to stored
		PASS_ALWAYS,			// pass always
		PASS_MAX					// end of enumeration
	};
	enum DepthMaskType
	{
		DEPTH_WRITE_DISABLE=0,	// disable depth buffer writes 
		DEPTH_WRITE_ENABLE,		// enable depth buffer writes		(default)
		DEPTH_WRITE_MAX			// end of enumeration
	};
	enum ColorMaskType
	{
		COLOR_WRITE_DISABLE=0,	// disable color buffer writes 
		COLOR_WRITE_ENABLE,		// enable color buffer writes		(default)
		COLOR_WRITE_MAX			// end of enumeration
	};
 	enum DetailAlphaFuncType
 	{
		DETAILALPHA_DISABLE=0,	// local (default)
		DETAILALPHA_DETAIL,		// other
		DETAILALPHA_SCALE,		// local * other
		DETAILALPHA_INVSCALE,	// ~(~local * ~other) = local + (1-local)*other
		DETAILALPHA_MAX			// end of enumeration
	};
	enum DetailColorFuncType
 	{
 		DETAILCOLOR_DISABLE=0,		// 0000	local (default)
		DETAILCOLOR_DETAIL,			// 0001	other
		DETAILCOLOR_SCALE,			// 0010	local * other
		DETAILCOLOR_INVSCALE,		// 0011	~(~local * ~other) = local + (1-local)*other
		DETAILCOLOR_ADD,				// 0100	local + other
		DETAILCOLOR_SUB,				// 0101	local - other
		DETAILCOLOR_SUBR,				// 0110	other - local
		DETAILCOLOR_BLEND,			// 0111	(localAlpha)*local + (~localAlpha)*other
		DETAILCOLOR_DETAILBLEND,	//	1000	(otherAlpha)*local + (~otherAlpha)*other
		DETAILCOLOR_MAX				//			end of enumeration
	};
	enum CullModeType
	{
		CULL_MODE_DISABLE=0,
		CULL_MODE_ENABLE,
		CULL_MODE_MAX
	};
  	enum DstBlendFuncType
  	{
  		DSTBLEND_ZERO=0,					// destination pixel doesn't affect blending (default)
  		DSTBLEND_ONE,						// destination pixel added unmodified
 		DSTBLEND_SRC_COLOR,				// destination pixel multiplied by fragment RGB components
 		DSTBLEND_ONE_MINUS_SRC_COLOR,	// destination pixel multiplied by one minus (i.e. inverse) fragment RGB components
 		DSTBLEND_SRC_ALPHA,        	// destination pixel multiplied by fragment alpha component
 		DSTBLEND_ONE_MINUS_SRC_ALPHA, // destination pixel multiplied by fragment inverse alpha
		DSTBLEND_MAX						// end of enumeration
  	};
	enum FogFuncType
 	{
 		FOG_DISABLE=0,			// don't perform fogging (default)
 		FOG_ENABLE,        	// apply fog, f*fogColor + (1-f)*fragment
 		FOG_SCALE_FRAGMENT,  // fog scalar value multiplies fragment, (1-f)*fragment
 		FOG_WHITE,				// fog scalar value replaces fragment, f*fogColor
		FOG_MAX					// end of enumeration
 	};
 	enum PriGradientType
 	{
 		GRADIENT_DISABLE=0,  //	000	disable primary gradient (same as OpenGL 'decal' texture blend)
		GRADIENT_MODULATE,   //	001	modulate fragment ARGB by gradient ARGB (default)
		GRADIENT_ADD,        //	010	add gradient RGB to fragment RGB, copy gradient A to fragment A
		GRADIENT_BUMPENVMAP,	// 011
		GRADIENT_BUMPENVMAPLUMINANCE,	// 100
		GRADIENT_DOTPRODUCT3,	// 101
		GRADIENT_MAX			// end of enumeration
 	};
	enum SecGradientType
	{
		SECONDARY_GRADIENT_DISABLE=0,	// don't draw secondary gradient (default)
		SECONDARY_GRADIENT_ENABLE,    // add secondary gradient RGB to fragment RGB 
		SECONDARY_GRADIENT_MAX			// end of enumeration
	};
	enum SrcBlendFuncType	
  	{
  		SRCBLEND_ZERO=0,						// fragment not added to color buffer
  		SRCBLEND_ONE,							// fragment added unmodified to color buffer (default)
 		SRCBLEND_SRC_ALPHA,					// fragment RGB components multiplied by fragment A
 		SRCBLEND_ONE_MINUS_SRC_ALPHA,		// fragment RGB components multiplied by fragment inverse (one minus) A
		SRCBLEND_MAX							// end of enumeration
  	};
	enum TexturingType
	{
		TEXTURING_DISABLE=0, // no texturing (treat fragment initial color as 1,1,1,1)
		TEXTURING_ENABLE,    // enable texturing
		TEXTURING_MAX			// end of enumeration
	};
	enum StaticSortCategoryType
	{
		SSCAT_OPAQUE,
		SSCAT_ALPHA_TEST,
		SSCAT_ADDITIVE,
		SSCAT_OTHER
	};
	enum														
	{
		MASK_DEPTHCOMPARE			= (7<<0),			// mask for depth comparison setting
		MASK_DEPTHMASK				= (1<<3),			// mask for depth mask setting
		MASK_COLORMASK				= (1<<4),			// mask for color mask setting
		MASK_DSTBLEND				= (7<<5),			// mask for destination blend setting
		MASK_FOG						= (3<<8),			// mask for fog setting
		MASK_PRIGRADIENT			= (7<<10),			// mask for primary gradient setting
		MASK_SECGRADIENT			= (1<<13),			// mask for secondary gradient setting
		MASK_SRCBLEND				= (3<<14),			// mask for source blend setting
		MASK_TEXTURING				= (1<<16),			// mask for texturing setting
		MASK_ALPHATEST				= (1<<18),			// mask for alpha test enable
		MASK_CULLMODE				= (1<<19),			// mask for cullmode setting
		MASK_POSTDETAILCOLORFUNC= (15<<20),			// mask for post detail color function setting
		MASK_POSTDETAILALPHAFUNC= (7<<24)			// mask for post detail alpha function setting
	};
	ShaderClass(void)
	{	Reset(); }
	ShaderClass(const ShaderClass & s)
	{	ShaderBits=s.ShaderBits; }
	ShaderClass(const unsigned int d)
	{	ShaderBits=d;	}
	bool operator == (const ShaderClass & s) { return ShaderBits == s.ShaderBits; }
	bool operator != (const ShaderClass & s) { return ShaderBits != s.ShaderBits; }
	inline unsigned int Get_Bits(void) const
	{	return ShaderBits; }
	inline int Uses_Alpha(void) const
	{
		if (Get_Alpha_Test() != ALPHATEST_DISABLE)
		return true;
		DstBlendFuncType dst = Get_Dst_Blend_Func();
		if (dst == DSTBLEND_SRC_ALPHA || dst == DSTBLEND_ONE_MINUS_SRC_ALPHA)
		return true;
		SrcBlendFuncType src = Get_Src_Blend_Func();
		return (src == SRCBLEND_SRC_ALPHA || src == SRCBLEND_ONE_MINUS_SRC_ALPHA);
	}
	inline int	Uses_Fog(void) const
	{
		return (Get_Fog_Func() != FOG_DISABLE);
	}
	inline int	Uses_Primary_Gradient(void) const
	{
		return (Get_Primary_Gradient() != GRADIENT_DISABLE);
	}
	inline int	Uses_Secondary_Gradient(void) const
	{
		return (Get_Secondary_Gradient() != SECONDARY_GRADIENT_DISABLE);
	}
	inline int	Uses_Texture(void) const
	{ return (Get_Texturing() != TEXTURING_DISABLE); }
	inline int	Uses_Post_Detail_Texture(void) const
	{
		if (Get_Texturing() == TEXTURING_DISABLE)
		return false;
		return ((Get_Post_Detail_Color_Func() != DETAILCOLOR_DISABLE) || (Get_Post_Detail_Alpha_Func() != DETAILALPHA_DISABLE));
	}
	inline void	Reset(void);
	inline DepthCompareType		Get_Depth_Compare(void)	const								{ return (DepthCompareType)(ShaderBits&MASK_DEPTHCOMPARE>>SHIFT_DEPTHCOMPARE); }
	inline DepthMaskType			Get_Depth_Mask(void) const									{ return (DepthMaskType)((ShaderBits&MASK_DEPTHMASK)>>SHIFT_DEPTHMASK); }
	inline ColorMaskType			Get_Color_Mask(void) const									{ return (ColorMaskType)((ShaderBits&MASK_COLORMASK)>>SHIFT_COLORMASK); }
	inline DetailAlphaFuncType	Get_Post_Detail_Alpha_Func(void) const					{ return (DetailAlphaFuncType)((ShaderBits&MASK_POSTDETAILALPHAFUNC)>>SHIFT_POSTDETAILALPHAFUNC); }
	inline DetailColorFuncType	Get_Post_Detail_Color_Func(void) const					{ return (DetailColorFuncType)((ShaderBits&MASK_POSTDETAILCOLORFUNC)>>SHIFT_POSTDETAILCOLORFUNC); }
	inline AlphaTestType			Get_Alpha_Test(void) const									{ return (AlphaTestType)((ShaderBits&MASK_ALPHATEST)>>SHIFT_ALPHATEST); }
	inline CullModeType			Get_Cull_Mode(void) const									{ return (CullModeType)((ShaderBits&MASK_CULLMODE)>>SHIFT_CULLMODE); }
	inline DstBlendFuncType		Get_Dst_Blend_Func(void) const							{ return (DstBlendFuncType)((ShaderBits&MASK_DSTBLEND)>>SHIFT_DSTBLEND); }
	inline FogFuncType			Get_Fog_Func(void) const									{ return (FogFuncType)((ShaderBits&MASK_FOG)>>SHIFT_FOG); }
	inline PriGradientType		Get_Primary_Gradient(void) const							{ return (PriGradientType)((ShaderBits&MASK_PRIGRADIENT)>>SHIFT_PRIGRADIENT); }
	inline SecGradientType		Get_Secondary_Gradient(void) const						{ return (SecGradientType)((ShaderBits&MASK_SECGRADIENT)>>SHIFT_SECGRADIENT); }
	inline SrcBlendFuncType		Get_Src_Blend_Func(void) const							{ return (SrcBlendFuncType)((ShaderBits&MASK_SRCBLEND)>>SHIFT_SRCBLEND); }
	inline TexturingType			Get_Texturing(void) const									{ return (TexturingType)((ShaderBits&MASK_TEXTURING)>>SHIFT_TEXTURING); }
	inline	void	Set_Depth_Compare(DepthCompareType x)					{ ShaderBits&=~MASK_DEPTHCOMPARE;ShaderBits|=(x<<SHIFT_DEPTHCOMPARE);	}
	inline	void	Set_Depth_Mask(DepthMaskType x)							{ ShaderBits&=~MASK_DEPTHMASK; ShaderBits|=(x<<SHIFT_DEPTHMASK);	}
	inline	void	Set_Color_Mask(ColorMaskType x)							{ ShaderBits&=~MASK_COLORMASK; ShaderBits|=(x<<SHIFT_COLORMASK);	}
	inline	void	Set_Post_Detail_Alpha_Func(DetailAlphaFuncType x)	{ ShaderBits&=~MASK_POSTDETAILALPHAFUNC;ShaderBits|=(x<<SHIFT_POSTDETAILALPHAFUNC);	}
	inline	void	Set_Post_Detail_Color_Func(DetailColorFuncType x)	{ ShaderBits&=~MASK_POSTDETAILCOLORFUNC;ShaderBits|=(x<<SHIFT_POSTDETAILCOLORFUNC);	}
	inline	void	Set_Alpha_Test(AlphaTestType x)							{ ShaderBits&=~MASK_ALPHATEST; ShaderBits|=(x<<SHIFT_ALPHATEST);		}
	inline	void	Set_Cull_Mode(CullModeType x)								{ ShaderBits&=~MASK_CULLMODE; ShaderBits|=(x<<SHIFT_CULLMODE);		}
	inline	void	Set_Dst_Blend_Func(DstBlendFuncType x)					{ ShaderBits&=~MASK_DSTBLEND; ShaderBits|=(x<<SHIFT_DSTBLEND);		}
	inline	void	Set_Fog_Func(FogFuncType x)								{ ShaderBits&=~MASK_FOG; ShaderBits|=(x<<SHIFT_FOG);			}
	inline	void	Set_Primary_Gradient(PriGradientType x)				{ ShaderBits&=~MASK_PRIGRADIENT;ShaderBits|=(x<<SHIFT_PRIGRADIENT);	}
	inline	void	Set_Secondary_Gradient(SecGradientType x)				{ ShaderBits&=~MASK_SECGRADIENT;ShaderBits|=(x<<SHIFT_SECGRADIENT);	}
	inline	void	Set_Src_Blend_Func(SrcBlendFuncType x)					{ ShaderBits&=~MASK_SRCBLEND;ShaderBits|=(x<<SHIFT_SRCBLEND);		}
	inline	void	Set_Texturing(TexturingType x)							{ ShaderBits&=~MASK_TEXTURING; ShaderBits|=(x<<SHIFT_TEXTURING);	}
	void	Init_From_Material3(const W3dMaterial3Struct & mat3);
	void	Enable_Fog (const char *source);
	StaticSortCategoryType	Get_SS_Category(void) const;
	int							Guess_Sort_Level(void) const;
#ifndef TTLE_EXPORTS
	static inline void	Invalidate() { ShaderDirty=true; }
	static void				Invert_Backface_Culling(bool onoff)
	{
		_PolygonCullMode = onoff ? D3DCULL_CCW : D3DCULL_CW;
		ShaderDirty = true;
	}
	static bool				Is_Backface_Culling_Inverted(void)
	{
		return _PolygonCullMode == D3DCULL_CCW;
	}
#endif

	static ShaderClass _PresetOpaqueShader;
	static ShaderClass _PresetAdditiveShader;
	static ShaderClass _PresetBumpenvmapShader;
	static ShaderClass _PresetAlphaShader;
	static ShaderClass _PresetMultiplicativeShader;
	static ShaderClass _PresetOpaque2DShader;
	static ShaderClass _PresetOpaqueSpriteShader;
	static ShaderClass _PresetAdditive2DShader;
	static ShaderClass _PresetAlpha2DShader;
	static ShaderClass _PresetAdditiveSpriteShader;
	static ShaderClass _PresetAlphaSpriteShader;
	static ShaderClass _PresetOpaqueSolidShader;
	static ShaderClass _PresetAdditiveSolidShader;
	static ShaderClass _PresetAlphaSolidShader;
	static ShaderClass _PresetATest2DShader;
	static ShaderClass _PresetATestSpriteShader;
	static ShaderClass _PresetATestBlend2DShader;
	static ShaderClass _PresetATestBlendSpriteShader;
	static ShaderClass _PresetScreen2DShader;
	static ShaderClass _PresetScreenSpriteShader;
	static ShaderClass _PresetMultiplicative2DShader;
	static ShaderClass _PresetMultiplicativeSpriteShader;
#ifndef TTLE_EXPORTS
	static REF_DECL1(ShaderDirty, bool);
#endif
	struct {
		union {
			unsigned long	ShaderBits;
			struct {
				unsigned long	DepthCompare		: 3; //0x7			- 0		11111111111111111111111111111000
				unsigned long	DepthMask			: 1; //0x8			- 3		11111111111111111111111111110111
				unsigned long	ColorMask			: 1; //0x10			- 4		11111111111111111111111111101111
				unsigned long	DestBlend			: 3; //0xE0			- 5		11111111111111111111111100011111
				unsigned long	FogFunc				: 2; //0x300		- 8		11111111111111111111110011111111
				unsigned long	PrimaryGradient		: 3; //0x1C00		- 10	11111111111111111110001111111111
				unsigned long	SecondaryGradient	: 1; //0x2000		- 13	11111111111111111101111111111111
				unsigned long	SrcBlend			: 2; //0xC000		- 14	11111111111111110011111111111111
				unsigned long	Texturing			: 1; //0x10000		- 16	11111111111111101111111111111111
				unsigned long	Unknown2			: 1; //0x20000		- 17	11111111111111011111111111111111
				unsigned long	AlphaTest			: 1; //0x40000		- 18	11111111111110111111111111111111
				unsigned long	Unknown3			: 1; //0x80000		- 19	11111111111101111111111111111111
				unsigned long	DetailColorFunc		: 4; //0xF00000		- 20	11111111000011111111111111111111
				unsigned long	DetailAlphaFunc		: 3; //0x7000000	- 24	11111000111111111111111111111111
			};
		};
	};
protected:
	void Report_Unable_To_Fog (const char *source);
	//unsigned int ShaderBits;
	static unsigned long CurrentShader;
};
inline void ShaderClass::Reset()
{
	ShaderBits=0;
	Set_Depth_Compare(PASS_LEQUAL);
	Set_Depth_Mask(DEPTH_WRITE_ENABLE);
	Set_Color_Mask(COLOR_WRITE_ENABLE);
	Set_Dst_Blend_Func(DSTBLEND_ZERO);
	Set_Fog_Func(FOG_DISABLE);
	Set_Primary_Gradient(GRADIENT_MODULATE);
	Set_Secondary_Gradient(SECONDARY_GRADIENT_DISABLE);
	Set_Src_Blend_Func(SRCBLEND_ONE);
	Set_Texturing(TEXTURING_DISABLE);
	Set_Alpha_Test(ALPHATEST_DISABLE);
	Set_Cull_Mode(CULL_MODE_ENABLE);
	Set_Post_Detail_Color_Func(DETAILCOLOR_DISABLE);
	Set_Post_Detail_Alpha_Func(DETAILALPHA_DISABLE);
}

enum WW3DErrorType {
	WW3D_ERROR_OK = 0x0,
	WW3D_ERROR_GENERIC = 0x1,
	WW3D_ERROR_LOAD_FAILED = 0x2,
	WW3D_ERROR_SAVE_FAILED = 0x3,
	WW3D_ERROR_WINDOW_NOT_OPEN = 0x4,
	WW3D_ERROR_INITIALIZATION_FAILED = 0x6,
};

class ShaderControllerClass;
class __declspec(novtable) ShaderControllerObserver
{
protected:
	friend class ShaderControllerClass;
	virtual void OnShaderDatabaseLoad() {};
	virtual void OnShaderDatabaseUnload() {};
};

#ifndef TTLE_EXPORTS
class TextureMapperClass : public RefCountClass, ShaderControllerObserver {
#else
class TextureMapperClass : public RefCountClass {
#endif
protected:
	unsigned int Stage; // 0008  0020
#ifndef TTLE_EXPORTS
	ProgrammableShader *Shader; // 000C  0024
	const char* ShaderName; // 0010  0028
#endif
public:
	enum {
		MAPPER_ID_UNKNOWN = 0x0,
		MAPPER_ID_LINEAR_OFFSET = 0x1,
		MAPPER_ID_CLASSIC_ENVIRONMENT = 0x2,
		MAPPER_ID_ENVIRONMENT = 0x3,
		MAPPER_ID_SCREEN = 0x4,
		MAPPER_ID_ANIMATING_1D = 0x5,
		MAPPER_ID_AXIAL = 0x6,
		MAPPER_ID_SILHOUETTE = 0x7,
		MAPPER_ID_SCALE = 0x8,
		MAPPER_ID_GRID = 0x9,
		MAPPER_ID_ROTATE = 0xa,
		MAPPER_ID_SINE_LINEAR_OFFSET = 0xb,
		MAPPER_ID_STEP_LINEAR_OFFSET = 0xc,
		MAPPER_ID_ZIGZAG_LINEAR_OFFSET = 0xd,
		MAPPER_ID_WS_CLASSIC_ENVIRONMENT = 0xe,
		MAPPER_ID_WS_ENVIRONMENT = 0xf,
		MAPPER_ID_GRID_CLASSIC_ENVIRONMENT = 0x10,
		MAPPER_ID_GRID_ENVIRONMENT = 0x11,
		MAPPER_ID_RANDOM = 0x12,
		MAPPER_ID_EDGE = 0x13,
		MAPPER_ID_BUMPENV = 0x14,
		// non-stock ids
		MAPPER_ID_UV = 0x100,
		MAPPER_ID_MATRIX = 0x101,
	};
	TextureMapperClass(TextureMapperClass& src);
	TextureMapperClass(unsigned int stage=0);
	TextureMapperClass(INIClass& ini,char* section,unsigned int stage);
#ifndef TTLE_EXPORTS
	SHADERS_API void Set_Shader(const char* shader_name, ProgrammableShader* shader = nullptr);
	ProgrammableShader *Peek_Shader()
	{
		return Shader;
	}
	ProgrammableShader *Get_Shader();
#endif
	virtual void Reset()
	{
	}
	virtual TextureMapperClass *Clone() = 0;
	virtual int Mapper_ID()
	{
		return MAPPER_ID_UNKNOWN;
	}
	virtual bool Is_Time_Variant()
	{
		return false;
	}
	virtual void Apply(int uv_array_index) = 0;
	virtual bool Needs_Normals()
	{
		return false;
	}
	~TextureMapperClass();
#ifndef TTLE_EXPORTS
	virtual void ApplyToMatrix(Matrix4& mat,int uv_array_index) = 0;

	virtual void OnShaderDatabaseLoad();
	virtual void OnShaderDatabaseUnload();
#endif
}; // 0014  002C

typedef void (*ScriptNotify) (int ID,int notify);

#define DEBUG_COLOR1 D3DCOLOR_XRGB(200,100,100)
#define DEBUG_COLOR2 D3DCOLOR_XRGB(100,200,100)
#define DEBUG_COLOR3 D3DCOLOR_XRGB(100,100,200)
void _DebugEventStart(D3DCOLOR color, wchar_t *message,...); //Add a debug event
void _DebugEventEnd(); //End a debug event
void _DebugEventMarker(D3DCOLOR color, wchar_t *message,...); //Add a debug marker
int _DebugGetStatus(); //Get debug status
void _DebugNameObject(IDirect3DResource9* object, const char* name); // Name a D3D object
#ifdef DEBUG
#define DebugEventStart(...)	_DebugEventStart(__VA_ARGS__)
#define DebugEventMarker(...)	_DebugEventMarker(__VA_ARGS__)
#define DebugEventEnd()			_DebugEventEnd()
#define DebugGetStatus()		_DebugGetStatus()
#define DebugNameObject(object, name) _DebugNameObject(object, name)
#else
#define DebugEventStart(...)
#define DebugEventMarker(...)
#define DebugEventEnd()
#define DebugGetStatus()
#define DebugNameObject(object, name)
#endif
Vector4 GetColorVector4(D3DCOLOR color); //Converts a D3DCOLOR into a Vector4
Vector3 GetColorVector3(D3DCOLOR color); //Converts a D3DCOLOR into a Vector3
D3DCOLOR GetD3dColor(const Vector3& color); // Converts a Vector3 into a D3DCOLOR
D3DCOLOR GetD3dColor(const Vector3& color, float alpha); // Converts a Vector3 into a D3DCOLOR with alpha
D3DCOLOR GetD3dColor(const Vector4& color); // Converts a Vector4 into a D3DCOLOR
Vector3 GetColorVector3(D3DCOLORVALUE color); //Converts a D3DCOLORVALUE into a Vector3

//stuff thats in here because the custom HUD code needs it
void Seconds_To_Hms(float secs, int &hours, int &minutes, int &seconds); //Convert seconds to HMS
#ifndef TTLE_EXPORTS
extern REF_ARR_DECL1(Fonts, FontCharsClass *, 15);
#endif
class W3dUtilityClass {
public:
	static void Convert_Shader(W3dShaderStruct const &,ShaderClass *);
	static void Convert_Shader(ShaderClass const &,W3dShaderStruct *);
	static void Convert_Color(W3dRGBStruct  const &, Vector3 *);
	static void Convert_Color(Vector3 const &, W3dRGBStruct *);
	static void Convert_Color(W3dRGBAStruct  const &, Vector4 *);
	static void Convert_Color(Vector4 const &, W3dRGBAStruct *);
};

#endif