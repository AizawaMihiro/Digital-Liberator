#include "Light.h"
#include "Player.h"

Light::Light()
{
	hLight = GetLightType();
}

Light::~Light()
{
}

void Light::ChangeLight(int type)
{
	if (hLight == type)
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
	hLight = type;
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
		switch (hLight)
		{
		case DX_LIGHTTYPE_DIRECTIONAL:
			//Playerの向きをライトの向きにする
			VECTOR LightDir = VTransform(VGet(0.0f, 0.0f, 1.0f), obj->GetTransform().GetRotationMatrix());
			ChangeLightTypeDir(LightDir);
			break;
		case DX_LIGHTTYPE_POINT:
			VECTOR LightPos = obj->GetTransform().position;
			ChangeLightTypePoint(LightPos, 2000.0f, 0.0f, 0.006f, 0.0f);
			break;
		default:
			break;
		}
	}
}
