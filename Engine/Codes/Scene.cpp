#include "..\Headers\Scene.h"

USING(Engine)

CScene::CScene(LPDIRECT3DDEVICE9 pDevice)
	: m_pDevice(pDevice)
{
	Safe_AddRef(m_pDevice);
}

void CScene::Free()
{
	Safe_Release(m_pDevice);
}