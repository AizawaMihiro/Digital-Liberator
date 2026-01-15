#include "Light.h"
#include "Player.h"

namespace 
{
	const VECTOR DIRECTIONAL_LIGHT_VEC = VGet(0.0f, 0.0f, 1.0f);
	const VECTOR POINT_LIGHT_POS = VGet(0.0f, 0.0f, 0.0f);
	const float POINT_LIGHT_HEIGHT = 70.0f;
	const float POINT_LIGHT_RANGE = 2000.0f;
	const float POINT_LIGHT_ATTENUATION0 = 0.0f;
	const float POINT_LIGHT_ATTENUATION1 = 0.006f;
	const float POINT_LIGHT_ATTENUATION2 = 0.0f;
}

Light::Light()
	: hAddLight(-1),
	addLightType(DX_LIGHTTYPE_DIRECTIONAL)
{
	lightType = GetLightType();
}

Light::~Light()
{
}

void Light::ChangeLight(int type)
{
	if (lightType == type)
	{
		return;
	}
	Player* obj = ObjectManager::FindGameObject<Player>();
	switch (type)
	{
	case DX_LIGHTTYPE_DIRECTIONAL:
		if (obj != nullptr)
		{
			//Playerの向きをライトの向きにする
			VECTOR LightDir =VTransform(DIRECTIONAL_LIGHT_VEC, obj->GetTransform().GetRotationMatrix());
			ChangeLightTypeDir(LightDir);
		}
		else
		{
			ChangeLightTypeDir(DIRECTIONAL_LIGHT_VEC);
		}
		break;
	case DX_LIGHTTYPE_POINT:
		if (obj != nullptr)
		{
			VECTOR LightPos = obj->GetTransform().position;
			LightPos.y += POINT_LIGHT_HEIGHT;
			ChangeLightTypePoint(LightPos, POINT_LIGHT_RANGE, 
				POINT_LIGHT_ATTENUATION0, POINT_LIGHT_ATTENUATION1, POINT_LIGHT_ATTENUATION2);
		}
		else
		{
			ChangeLightTypePoint(POINT_LIGHT_POS, POINT_LIGHT_RANGE, 
				POINT_LIGHT_ATTENUATION0, POINT_LIGHT_ATTENUATION1, POINT_LIGHT_ATTENUATION2);
		}
		break;
	//case DX_LIGHTTYPE_SPOT:
	//	ChangeLightTypeSpot();
	//	break;
	default:
		break;
	}
	lightType = type;
}

int Light::DefaultLight(bool flag)
{
	return SetLightEnable(flag);
}

void Light::Update()
{
	Player* obj = ObjectManager::FindGameObject<Player>();
	if (obj != nullptr)
	{
		switch (lightType)
		{
		case DX_LIGHTTYPE_DIRECTIONAL:
			//Playerの向きをライトの向きにする
			VECTOR LightDir = VTransform(DIRECTIONAL_LIGHT_VEC, obj->GetTransform().GetRotationMatrix());
			SetLightDirection(LightDir);
			break;
		case DX_LIGHTTYPE_POINT:
			VECTOR LightPos = obj->GetTransform().position;
			LightPos.y += POINT_LIGHT_HEIGHT;
			SetLightPosition(LightPos);
			break;
		default:
			break;
		}
		if (hAddLight >= -1)
		{
			switch (addLightType)
			{
			case DX_LIGHTTYPE_DIRECTIONAL:
				//Playerの向きをライトの向きにする
				VECTOR LightDir = VTransform(DIRECTIONAL_LIGHT_VEC, obj->GetTransform().GetRotationMatrix());
				SetLightDirectionHandle(hAddLight,LightDir);
				break;
			case DX_LIGHTTYPE_POINT:
				VECTOR LightPos = obj->GetTransform().position;
				LightPos.y += POINT_LIGHT_HEIGHT;
				SetLightPositionHandle(hAddLight, LightPos);
				break;
			default:
				break;
			}
		}
	}
}

void Light::CreateAddLight(int type)
{
	Player* obj = ObjectManager::FindGameObject<Player>();
	switch (type)
	{
	case DX_LIGHTTYPE_DIRECTIONAL:
		if (obj != nullptr)
		{
			//Playerの向きをライトの向きにする
			VECTOR LightDir = VTransform(DIRECTIONAL_LIGHT_VEC, obj->GetTransform().GetRotationMatrix());
			hAddLight = CreateDirLightHandle(LightDir);
		}
		else
		{
			hAddLight = CreateDirLightHandle(VGet(0.0f, 1.0f, 0.0f));
		}
		break;
	case DX_LIGHTTYPE_POINT:
		if (obj != nullptr)
		{
			VECTOR LightPos = obj->GetTransform().position;
			LightPos.y += POINT_LIGHT_HEIGHT;
			hAddLight = CreatePointLightHandle(LightPos, POINT_LIGHT_RANGE, 
				POINT_LIGHT_ATTENUATION0, POINT_LIGHT_ATTENUATION1, POINT_LIGHT_ATTENUATION2);
		}
		else
		{
			hAddLight = CreatePointLightHandle(POINT_LIGHT_POS, POINT_LIGHT_RANGE, 
				POINT_LIGHT_ATTENUATION0, POINT_LIGHT_ATTENUATION1, POINT_LIGHT_ATTENUATION2);
		}
		break;
		//case DX_LIGHTTYPE_SPOT:
		//	ChangeLightTypeSpot();
		//	break;
	default:
		break;
	}
	addLightType = type;
}
