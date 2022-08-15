#pragma once
#ifndef __CAMERA_OBSERVER_H__

#include "Observer.h"

BEGIN(Client)
class CCamera_Observer final : public Engine::CObserver
{
public:
	explicit CCamera_Observer();
	virtual ~CCamera_Observer() = default;
public:
	CAMERA_VIEW Get_Cam_View() { return m_eCam_View; }
	CAMERA_EFFECT Get_Cam_Effect() { return m_eCam_Effect; }
public:
	virtual void Update_Observer(int iMessage, void * pData) override;

private:
	CAMERA_EFFECT m_eCam_Effect; 
	CAMERA_VIEW m_eCam_View;
};
END
#define __CAMERA_OBSERVER_H__
#endif // !__CAMERA_OBSERVER_H__