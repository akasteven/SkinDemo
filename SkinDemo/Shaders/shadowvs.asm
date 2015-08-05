//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer cbPerObject
// {
//
//   float4x4 lightWVP;                 // Offset:    0 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// cbPerObject                       cbuffer      NA          NA    3        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// NORMAL                   0   xyz         1     NONE   float       
// TEXCOORD                 0   xy          2     NONE   float   xy  
// TANGENT                  0   xyz         3     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
vs_5_0
dcl_globalFlags refactoringAllowed | skipOptimization
dcl_constantbuffer cb3[4], immediateIndexed
dcl_input v0.xyz
dcl_input v2.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xy
dcl_temps 2
//
// Initial variable locations:
//   v0.x <- input.PosL.x; v0.y <- input.PosL.y; v0.z <- input.PosL.z; 
//   v1.x <- input.NorL.x; v1.y <- input.NorL.y; v1.z <- input.NorL.z; 
//   v2.x <- input.Tex.x; v2.y <- input.Tex.y; 
//   v3.x <- input.TangentL.x; v3.y <- input.TangentL.y; v3.z <- input.TangentL.z; 
//   o1.x <- <VS return value>.Tex.x; o1.y <- <VS return value>.Tex.y; 
//   o0.x <- <VS return value>.PosH.x; o0.y <- <VS return value>.PosH.y; o0.z <- <VS return value>.PosH.z; o0.w <- <VS return value>.PosH.w
//
#line 34 "D:\Projects\Demo\Demo\SkinDemo\Shaders\ShadowMap.hlsl"
mov r0.xyz, v0.xyzx
mov r0.w, l(1.000000)
dp4 r1.x, r0.xyzw, cb3[0].xyzw  // r1.x <- output.PosH.x
dp4 r1.y, r0.xyzw, cb3[1].xyzw  // r1.y <- output.PosH.y
dp4 r1.z, r0.xyzw, cb3[2].xyzw  // r1.z <- output.PosH.z
dp4 r1.w, r0.xyzw, cb3[3].xyzw  // r1.w <- output.PosH.w
mov r0.xy, v2.xyxx  // r0.x <- output.Tex.x; r0.y <- output.Tex.y
mov o0.xyzw, r1.xyzw
mov o1.xy, r0.xyxx
ret 
// Approximately 10 instruction slots used
