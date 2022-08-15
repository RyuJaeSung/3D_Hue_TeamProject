#pragma once

#ifndef __DOOR_H__
#define __DOOR_H__

#include "MyObject.h"

BEGIN(Client)

// ��� �浹X
class CDoor : public CMyObject
{
public:
	enum DOOR_TYPE
	{
		DOOR_PREV,	// ���������� ���� ��
		DOOR_NEXT,	// ���������� ���� ��
		DOOR_END
	};

private:
	explicit CDoor(LPDIRECT3DDEVICE9 pDevice);
	explicit CDoor(const CDoor& other);
	virtual ~CDoor() = default;

public:
	// Getter
	const DOOR_TYPE& Get_DoorType() const { return m_eDoorType; }

	// Setter
	void Set_DoorType(const DOOR_TYPE& _eDoorType) { m_eDoorType = _eDoorType; }

public:
	// CMyObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject_Prototype() override;
	virtual HRESULT Ready_GameObject(void * pArg) override;
	virtual int Update_GameObject(float fTimeDelta) override;
	virtual int LateUpdate_GameObject(float fTimeDelta) override;
	virtual HRESULT Render_GameObject() override;

private:
	virtual HRESULT Add_Component() override;
	virtual HRESULT Collision(float _fTimeDelta) override;
	HRESULT Collision_ToPlayer();

public:
	static CDoor* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual CGameObject * Clone_GameObject(void * pArg) override;
	virtual void Free() override;

private:
	DOOR_TYPE	m_eDoorType = DOOR_END;
};

END

#endif // !__DOOR_H__


