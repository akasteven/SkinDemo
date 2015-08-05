//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer cbNeverChanges
// {
//
//   struct DirectionalLight
//   {
//       
//       float4 Ambient;                // Offset:    0
//       float4 Diffuse;                // Offset:   16
//       float4 Specular;               // Offset:   32
//       float3 Direction;              // Offset:   48
//       float pad;                     // Offset:   60
//
//   } DirLight;                        // Offset:    0 Size:    64
//   
//   struct PointLight
//   {
//       
//       float4 Ambient;                // Offset:   64
//       float4 Diffuse;                // Offset:   80
//       float4 Specular;               // Offset:   96
//       float3 Position;               // Offset:  112
//       float Range;                   // Offset:  124
//       float3 Att;                    // Offset:  128
//       float pad;                     // Offset:  140
//
//   } PLight;                          // Offset:   64 Size:    80
//   float ShadowMapSize;               // Offset:  144 Size:     4
//   float3 pad3;                       // Offset:  148 Size:    12 [unused]
//
// }
//
// cbuffer cbPerFrame
// {
//
//   float3 eyePos;                     // Offset:    0 Size:    12
//   float pad;                         // Offset:   12 Size:     4 [unused]
//
// }
//
// cbuffer cbPerObject
// {
//
//   float4x4 matWorld;                 // Offset:    0 Size:    64 [unused]
//   float4x4 matWorldInvTranspose;     // Offset:   64 Size:    64 [unused]
//   float4x4 matWVP;                   // Offset:  128 Size:    64 [unused]
//   
//   struct Material
//   {
//       
//       float4 Ambient;                // Offset:  192
//       float4 Diffuse;                // Offset:  208
//       float4 Specular;               // Offset:  224
//       float4 Reflect;                // Offset:  240
//
//   } material;                        // Offset:  192 Size:    64
//   float4x4 lightWVPT;                // Offset:  256 Size:    64 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// samLinear                         sampler      NA          NA    0        1
// samShadowMap                    sampler_c      NA          NA    1        1
// txDiffuse                         texture  float4          2d    0        1
// txNormal                          texture  float4          2d    1        1
// txShadowMap                       texture  float4          2d    2        1
// cbNeverChanges                    cbuffer      NA          NA    0        1
// cbPerFrame                        cbuffer      NA          NA    2        1
// cbPerObject                       cbuffer      NA          NA    3        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// POSITION                 0   xyz         1     NONE   float   xyz 
// NORMAL                   0   xyz         2     NONE   float   xyz 
// TANGENT                  0   xyz         3     NONE   float   xyz 
// TEXCOORD                 0   xy          4     NONE   float   xy  
// TEXCOORD                 1   xyzw        5     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed | skipOptimization
dcl_constantbuffer cb0[10], immediateIndexed
dcl_constantbuffer cb2[1], immediateIndexed
dcl_constantbuffer cb3[15], immediateIndexed
dcl_sampler s0, mode_default
dcl_sampler s1, mode_comparison
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texture2d (float,float,float,float) t2
dcl_input_ps linear v1.xyz
dcl_input_ps linear v2.xyz
dcl_input_ps linear v3.xyz
dcl_input_ps linear v4.xy
dcl_input_ps linear v5.xyzw
dcl_output o0.xyzw
dcl_temps 18
dcl_indexableTemp x0[9], 4
//
// Initial variable locations:
//   v0.x <- input.PosH.x; v0.y <- input.PosH.y; v0.z <- input.PosH.z; v0.w <- input.PosH.w; 
//   v1.x <- input.PosW.x; v1.y <- input.PosW.y; v1.z <- input.PosW.z; 
//   v2.x <- input.NorW.x; v2.y <- input.NorW.y; v2.z <- input.NorW.z; 
//   v3.x <- input.TangentW.x; v3.y <- input.TangentW.y; v3.z <- input.TangentW.z; 
//   v4.x <- input.Tex.x; v4.y <- input.Tex.y; 
//   v5.x <- input.ShadowH.x; v5.y <- input.ShadowH.y; v5.z <- input.ShadowH.z; v5.w <- input.ShadowH.w; 
//   o0.x <- <PS return value>.x; o0.y <- <PS return value>.y; o0.z <- <PS return value>.z; o0.w <- <PS return value>.w
//
#line 136 "D:\Projects\Demo\Demo\SkinDemo\Shaders\DemoShader.hlsl"
dp3 r0.x, v2.xyzx, v2.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, v2.xyzx  // r0.x <- input.NorW.x; r0.y <- input.NorW.y; r0.z <- input.NorW.z
mov r1.xyz, -v1.xyzx
add r1.xyz, r1.xyzx, cb2[0].xyzx  // r1.x <- toEye.x; r1.y <- toEye.y; r1.z <- toEye.z
dp3 r0.w, r1.xyzx, r1.xyzx
sqrt r0.w, r0.w  // r0.w <- distToEye
div r1.xyz, r1.xyzx, r0.wwww
sample_indexable(texture2d)(float,float,float,float) r2.xyzw, v4.xyxx, t0.xyzw, s0  // r2.x <- texColor.x; r2.y <- texColor.y; r2.z <- texColor.z; r2.w <- texColor.w
sample_indexable(texture2d)(float,float,float,float) r3.xyz, v4.xyxx, t1.xyzw, s0
mov r3.xyz, r3.xyzx  // r3.x <- normalMapSample.x; r3.y <- normalMapSample.y; r3.z <- normalMapSample.z
nop 
mov r3.xyz, r3.xyzx
mov r0.xyz, r0.xyzx
mov r4.xyz, v3.yzxy

#line 60
mul r3.xyz, r3.xyzx, l(2.000000, 2.000000, 2.000000, 0.000000)
mov r5.xyz, l(-1.000000,-1.000000,-1.000000,-0.000000)
add r3.xyz, r3.xyzx, r5.xyzx  // r3.x <- normalT.x; r3.y <- normalT.y; r3.z <- normalT.z
mov r0.xyz, r0.xyzx  // r0.x <- N.x; r0.y <- N.y; r0.z <- N.z
dp3 r0.w, r4.zxyz, r0.xyzx
mul r5.xyz, r0.yzxy, r0.wwww
mov r5.xyz, -r5.xyzx
add r4.xyz, r4.xyzx, r5.xyzx
dp3 r0.w, r4.xyzx, r4.xyzx
rsq r0.w, r0.w
mul r4.xyz, r0.wwww, r4.xyzx  // r4.x <- T.y; r4.y <- T.z; r4.z <- T.x
mul r5.xyz, r0.yzxy, r4.yzxy
mul r6.xyz, r0.zxyz, r4.xyzx
mov r6.xyz, -r6.xyzx
add r5.xyz, r5.xyzx, r6.xyzx  // r5.x <- B.x; r5.y <- B.y; r5.z <- B.z
mov r6.x, r4.z  // r6.x <- TBN._m00
mov r6.y, r5.x  // r6.y <- TBN._m10
mov r6.z, r0.x  // r6.z <- TBN._m20
mov r7.x, r4.x  // r7.x <- TBN._m01
mov r7.y, r5.y  // r7.y <- TBN._m11
mov r7.z, r0.y  // r7.z <- TBN._m21
mov r0.x, r4.y  // r0.x <- TBN._m02
mov r0.y, r5.z  // r0.y <- TBN._m12
mov r0.z, r0.z  // r0.z <- TBN._m22
dp3 r4.x, r3.xyzx, r6.xyzx
dp3 r4.y, r3.xyzx, r7.xyzx
dp3 r4.z, r3.xyzx, r0.xyzx
dp3 r0.x, r4.xyzx, r4.xyzx
rsq r0.x, r0.x
mul r0.xyz, r0.xxxx, r4.xyzx  // r0.x <- bumpedNormalW.x; r0.y <- bumpedNormalW.y; r0.z <- bumpedNormalW.z
mov r0.xyz, r0.xyzx  // r0.x <- <NormalSampleToWorldSpace return value>.x; r0.y <- <NormalSampleToWorldSpace return value>.y; r0.z <- <NormalSampleToWorldSpace return value>.z

#line 144
mov r0.xyz, r0.xyzx  // r0.x <- bumpedNormal.x; r0.y <- bumpedNormal.y; r0.z <- bumpedNormal.z
nop 
nop 
nop 
mov r3.xyzw, v5.xyzw

#line 78
div r3.xyz, r3.zxyz, r3.wwww  // r3.x <- shadowPosH.z; r3.y <- shadowPosH.x; r3.z <- shadowPosH.y

#line 81
mov r3.x, r3.x  // r3.x <- depth

#line 84
div r4.y, l(1.000000), cb0[9].x  // r4.y <- dx
mov r0.w, l(0)  // r0.w <- percentLit
mov r5.x, -r4.y
mov r5.y, -r4.y
mov r6.y, -r4.y
mov r4.x, -r4.y
mov r7.x, -r4.y
mov r8.x, -r4.y
mov r8.y, r4.y
mov r4.w, r4.y
mov x0[0].xy, r5.xyxx
mov r6.x, l(0)
mov x0[1].xy, r6.xyxx
mov x0[2].xy, r4.yxyy
mov r7.y, l(0)
mov x0[3].xy, r7.xyxx
mov x0[4].xy, l(0,0,0,0)
mov r4.z, l(0)
mov x0[5].xy, r4.yzyy
mov x0[6].xy, r8.xyxx
mov r4.x, l(0)
mov x0[7].xy, r4.xyxx
mov x0[8].xy, r4.ywyy

#line 97
mov r4.xy, x0[0].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[1].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[2].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[3].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[4].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[5].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[6].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[7].xyxx
add r4.xy, r3.yzyy, r4.xyxx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r4.xyxx, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w
mov r4.xy, x0[8].xyxx
add r3.yz, r3.yyzy, r4.xxyx
sample_c_lz_indexable(texture2d)(float,float,float,float) r1.w, r3.yzyy, t2.xxxx, s1, r3.x
add r0.w, r0.w, r1.w

#line 101
div r0.w, r0.w, l(9.000000)
mov r0.w, r0.w  // r0.w <- <ShadowFactorPCF return value>

#line 145
mov r0.w, r0.w  // r0.w <- PCF
mov r3.xyz, l(0,0,0,0)  // r3.x <- ambient.x; r3.y <- ambient.y; r3.z <- ambient.z
mov r4.xyz, l(0,0,0,0)  // r4.x <- diffuse.x; r4.y <- diffuse.y; r4.z <- diffuse.z
mov r5.xyz, l(0,0,0,0)  // r5.x <- specular.x; r5.y <- specular.y; r5.z <- specular.z

#line 152
nop 
mov r6.xyz, cb3[12].xyzx
mov r7.xyz, cb3[13].xyzx
mov r8.xyzw, cb3[14].xyzw
mov r9.xyz, cb0[0].xyzx
mov r10.xyz, cb0[1].xyzx
mov r11.xyz, cb0[2].xyzx
mov r12.xyz, cb0[3].xyzx
mov r0.xyz, r0.xyzx
mov r1.xyz, r1.xyzx

#line 17 "D:\Projects\Demo\Demo\SkinDemo\Shaders\SimpleLighting.hlsl"
mov r13.xyz, l(0,0,0,0)  // r13.x <- diffuse.x; r13.y <- diffuse.y; r13.z <- diffuse.z
mov r14.xyz, l(0,0,0,0)  // r14.x <- spec.x; r14.y <- spec.y; r14.z <- spec.z

#line 21
mov r12.xyz, -r12.xyzx  // r12.x <- lightVec.x; r12.y <- lightVec.y; r12.z <- lightVec.z

#line 24
mul r6.xyz, r6.xyzx, r9.xyzx  // r6.x <- ambient.x; r6.y <- ambient.y; r6.z <- ambient.z

#line 29
dp3 r1.w, r12.xyzx, r0.xyzx  // r1.w <- diffuseFactor

#line 33
lt r3.w, l(0.000000), r1.w
mov r9.xyz, -r12.xyzx
dp3 r4.w, r9.xyzx, r0.xyzx
add r4.w, r4.w, r4.w
mov r4.w, -r4.w
mul r12.xyz, r0.xyzx, r4.wwww
add r9.xyz, r9.xyzx, r12.xyzx  // r9.x <- v.x; r9.y <- v.y; r9.z <- v.z
dp3 r4.w, r9.xyzx, r1.xyzx
max r4.w, r4.w, l(0.000000)
log r4.w, r4.w
mul r4.w, r4.w, r8.w
exp r4.w, r4.w  // r4.w <- specFactor
mul r7.xyz, r7.xyzx, r1.wwww
mul r7.xyz, r10.xyzx, r7.xyzx  // r7.x <- diffuse.x; r7.y <- diffuse.y; r7.z <- diffuse.z
mul r8.xyz, r8.xyzx, r4.wwww
mul r8.xyz, r11.xyzx, r8.xyzx  // r8.x <- spec.x; r8.y <- spec.y; r8.z <- spec.z
movc r7.xyz, r3.wwww, r7.xyzx, r13.xyzx
movc r8.xyz, r3.wwww, r8.xyzx, r14.xyzx

#line 152 "D:\Projects\Demo\Demo\SkinDemo\Shaders\DemoShader.hlsl"
mov r6.xyz, r6.xyzx  // r6.x <- A.x; r6.y <- A.y; r6.z <- A.z
mov r7.xyz, r7.xyzx  // r7.x <- D.x; r7.y <- D.y; r7.z <- D.z
mov r8.xyz, r8.xyzx  // r8.x <- S.x; r8.y <- S.y; r8.z <- S.z
add r3.xyz, r3.xyzx, r6.xyzx
mul r6.xyz, r0.wwww, r7.xyzx
add r4.xyz, r4.xyzx, r6.xyzx
mul r6.xyz, r0.wwww, r8.xyzx
add r5.xyz, r5.xyzx, r6.xyzx
nop 
mov r6.xyz, cb3[12].xyzx
mov r7.xyz, cb3[13].xyzx
mov r8.xyzw, cb3[14].xyzw
mov r9.xyz, cb0[4].xyzx
mov r10.xyz, cb0[5].xyzx
mov r11.xyz, cb0[6].xyzx
mov r12.xyz, cb0[7].xyzx
mov r0.w, cb0[7].w
mov r13.xyz, cb0[8].xyzx
mov r14.xyz, v1.xyzx
mov r0.xyz, r0.xyzx
mov r1.xyz, r1.xyzx

#line 52 "D:\Projects\Demo\Demo\SkinDemo\Shaders\SimpleLighting.hlsl"
mov r15.xyz, l(0,0,0,0)  // r15.x <- ambient.x; r15.y <- ambient.y; r15.z <- ambient.z
mov r16.xyz, l(0,0,0,0)  // r16.x <- diffuse.x; r16.y <- diffuse.y; r16.z <- diffuse.z
mov r17.xyz, l(0,0,0,0)  // r17.x <- spec.x; r17.y <- spec.y; r17.z <- spec.z

#line 57
mov r14.xyz, -r14.xyzx
add r12.xyz, r12.xyzx, r14.xyzx  // r12.x <- lightVec.x; r12.y <- lightVec.y; r12.z <- lightVec.z

#line 60
dp3 r1.w, r12.xyzx, r12.xyzx
sqrt r14.y, r1.w  // r14.y <- d

#line 63
lt r0.w, r0.w, r14.y

#line 67
if_z r0.w
  div r12.xyz, r12.xyzx, r14.yyyy

#line 70
  mul r15.xyz, r6.xyzx, r9.xyzx

#line 75
  dp3 r0.w, r12.xyzx, r0.xyzx  // r0.w <- diffuseFactor

#line 79
  lt r1.w, l(0.000000), r0.w
  mov r6.xyz, -r12.xyzx
  dp3 r3.w, r6.xyzx, r0.xyzx
  add r3.w, r3.w, r3.w
  mov r3.w, -r3.w
  mul r0.xyz, r0.xyzx, r3.wwww
  add r0.xyz, r6.xyzx, r0.xyzx  // r0.x <- v.x; r0.y <- v.y; r0.z <- v.z
  dp3 r0.x, r0.xyzx, r1.xyzx
  max r0.x, r0.x, l(0.000000)
  log r0.x, r0.x
  mul r0.x, r0.x, r8.w
  exp r0.x, r0.x  // r0.x <- specFactor
  mul r0.yzw, r7.xxyz, r0.wwww
  mul r0.yzw, r10.xxyz, r0.yyzw  // r0.y <- diffuse.x; r0.z <- diffuse.y; r0.w <- diffuse.z
  mul r1.xyz, r8.xyzx, r0.xxxx
  mul r1.xyz, r11.xyzx, r1.xyzx  // r1.x <- spec.x; r1.y <- spec.y; r1.z <- spec.z
  movc r0.xyz, r1.wwww, r0.yzwy, r16.xyzx  // r0.x <- diffuse.x; r0.y <- diffuse.y; r0.z <- diffuse.z
  movc r1.xyz, r1.wwww, r1.xyzx, r17.xyzx

#line 89
  mul r14.z, r14.y, r14.y
  mov r14.x, l(1.000000)
  dp3 r0.w, r13.xyzx, r14.xyzx
  div r0.w, l(1.000000), r0.w  // r0.w <- att
  mul r16.xyz, r0.wwww, r0.xyzx  // r16.x <- diffuse.x; r16.y <- diffuse.y; r16.z <- diffuse.z
  mul r17.xyz, r0.wwww, r1.xyzx  // r17.x <- spec.x; r17.y <- spec.y; r17.z <- spec.z
endif 

#line 158 "D:\Projects\Demo\Demo\SkinDemo\Shaders\DemoShader.hlsl"
mov r15.xyz, r15.xyzx  // r15.x <- A.x; r15.y <- A.y; r15.z <- A.z
mov r16.xyz, r16.xyzx  // r16.x <- D.x; r16.y <- D.y; r16.z <- D.z
mov r17.xyz, r17.xyzx  // r17.x <- S.x; r17.y <- S.y; r17.z <- S.z
add r0.xyz, r3.xyzx, r15.xyzx  // r0.x <- ambient.x; r0.y <- ambient.y; r0.z <- ambient.z
add r1.xyz, r4.xyzx, r16.xyzx  // r1.x <- diffuse.x; r1.y <- diffuse.y; r1.z <- diffuse.z
add r3.xyz, r5.xyzx, r17.xyzx  // r3.x <- specular.x; r3.y <- specular.y; r3.z <- specular.z
add r0.xyz, r0.xyzx, r1.xyzx
mul r0.xyz, r0.xyzx, r2.xyzx
add r0.xyz, r3.xyzx, r0.xyzx  // r0.x <- litColor.x; r0.y <- litColor.y; r0.z <- litColor.z
mov r2.w, r2.w  // r2.w <- litColor.w
mov o0.xyz, r0.xyzx
mov o0.w, r2.w
ret 
// Approximately 222 instruction slots used
