#include "stdafx.h"
#include "..\Headers\Camera_Observer.h"

USING(Client)
CCamera_Observer::CCamera_Observer()
{
}

void CCamera_Observer::Update_Observer(int iMessage, void * pData)
{
	switch (iMessage)
	{
	case OBJ_DATA::CAM_VIEW:
		m_eCam_View = *static_cast<CAMERA_VIEW*>(pData);
		break;
	case OBJ_DATA::CAM_EFFECT:
		m_eCam_Effect = *static_cast<CAMERA_EFFECT*>(pData);
		break;
	}
}
