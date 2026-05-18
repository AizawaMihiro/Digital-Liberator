#include "AmmoCounter.h"
namespace
{
    const int LIMIT_AMMO = 5;
	static int currentAmmo = 0;
}

void AmmoCounter::AmmoCounter()
{
	currentAmmo = LIMIT_AMMO;
}

void AmmoCounter::CountUp()
{
	currentAmmo++;
}

void AmmoCounter::CountDown()
{
	currentAmmo--;
}

void AmmoCounter::Reset()
{
	currentAmmo = LIMIT_AMMO;
}

int AmmoCounter::GetCurrentAmmo()
{
    return currentAmmo;
}
