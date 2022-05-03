#ifndef weapon_h
#define weapon_h

#include <stdbool.h>

typedef struct weapon *Weapon;
    Weapon createWeapon(int range, int power, int speed, int magazine_size, int firerate, int reloadTime);
    void manageFireRate(Weapon w);
    void manageReload(Weapon w);
    void setWeaponRange(Weapon w, int range);
    int getWeaponRange(Weapon w);
    void setWeaponPower(Weapon w, int power);
    int getWeaponPower(Weapon w);
    void setWeaponSpeed(Weapon w, int speed);
    int getWeaponSpeed(Weapon w);
    void setWeaponBullet(Weapon w, char weaponBullet[]);
    char* getWeaponBullet(Weapon w);
    void setWeaponMagazine_Size(Weapon w, int magazine_size);
    int getWeaponMagazine_Size(Weapon w);
    void setWeaponMagazine(Weapon w, int magazine);
    int getWeaponMagazine(Weapon w);
    void setWeaponFirerate(Weapon w, int firerate);
    int getWeaponFirerate(Weapon w);
    void setWeaponBulletTimer(Weapon w, int bulletTimer);
    int getWeaponBulletTimer(Weapon w);
    void setWeaponReloadTime(Weapon w, int reloadTime);
    int getWeaponReloadTime(Weapon w);
    void setWeaponShotCooldown(Weapon w, bool shotCooldown);
    bool getWeaponShotCooldown(Weapon w);
    void setWeaponReload(Weapon w, bool reload);
    bool getWeaponReload(Weapon w);

#endif