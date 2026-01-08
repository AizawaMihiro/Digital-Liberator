#include "Light.h"
#include "Player.h"

Light::Light()
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
			VECTOR LightDir =VTransform(VGet(0.0f, 0.0f, 1.0f), obj->GetTransform().GetRotationMatrix());
			ChangeLightTypeDir(LightDir);
		}
		else
		{
			ChangeLightTypeDir(VGet(0.0f, 1.0f, 0.0f));
		}
		break;
	case DX_LIGHTTYPE_POINT:
		if (obj != nullptr)
		{
			VECTOR LightPos = obj->GetTransform().position;
			LightPos.y += 100.0f;
			ChangeLightTypePoint(LightPos, 2000.0f, 0.0f, 0.006f, 0.0f);
		}
		else
		{
			ChangeLightTypePoint(VGet(0.0f, 0.0f, 0.0f), 2000.0f, 0.0f, 0.006f, 0.0f);
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
			VECTOR LightDir = VTransform(VGet(0.0f, 0.0f, 1.0f), obj->GetTransform().GetRotationMatrix());
			SetLightDirection(LightDir);
			break;
		case DX_LIGHTTYPE_POINT:
			VECTOR LightPos = obj->GetTransform().position;
			LightPos.y += 100.0f;
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
				VECTOR LightDir = VTransform(VGet(0.0f, 0.0f, 1.0f), obj->GetTransform().GetRotationMatrix());
				SetLightDirectionHandle(hAddLight,LightDir);
				break;
			case DX_LIGHTTYPE_POINT:
				VECTOR LightPos = obj->GetTransform().position;
				LightPos.y += 100.0f;
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
			VECTOR LightDir = VTransform(VGet(0.0f, 0.0f, 1.0f), obj->GetTransform().GetRotationMatrix());
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
			LightPos.y += 100.0f;
			hAddLight = CreatePointLightHandle(LightPos, 2000.0f, 0.0f, 0.006f, 0.0f);
		}
		else
		{
			hAddLight = CreatePointLightHandle(VGet(0.0f, 0.0f, 0.0f), 2000.0f, 0.0f, 0.006f, 0.0f);
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
