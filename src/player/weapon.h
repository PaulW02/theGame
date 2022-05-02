#ifndef weapon_h
#define weapon_h


typedef struct weapon *Weapon;
    Weapon createWeapon(int range, int power, int speed);
    void setWeaponRange(Weapon w, int range);
    int getWeaponRange(Weapon w);
    void setWeaponPower(Weapon w, int power);
    int getWeaponPower(Weapon w);
    void setWeaponSpeed(Weapon w, int speed);
    int getWeaponSpeed(Weapon w);

#endif