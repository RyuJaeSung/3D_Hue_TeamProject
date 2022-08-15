#pragma once

#ifndef __ENGINE_STRUCT_H__

#pragma region STRUCT_VERTEX
typedef struct tagVertex_Color
{
	D3DXVECTOR3 vPosition;	
	DWORD dwColor;
}VTX_COLOR;

const DWORD FVF_VTX_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertex_Texture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR2 vUV; // UV좌표.
}VTX_TEXTURE;

// 버텍스 텍스쳐랑 같은데 왜 중복으로 만들었는지 이유 작성해주십셔~

typedef struct tagVertex_CubeTexture
{
	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vUV; // UV좌표.
}VTX_CUBE_TEXTURE;

const DWORD FVF_VTX_CUBE_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
const DWORD FVF_VTX_TEXTURE = D3DFVF_XYZ | D3DFVF_TEX1;
#pragma endregion

#pragma region STRUCT_INDEX

typedef struct tagIndex16
{
	WORD _1, _2, _3;
}INDEX16;

typedef struct tagIndex32
{
	DWORD _1, _2, _3;
}INDEX32;

#pragma endregion

#define __ENGINE_STRUCT_H__
#endif