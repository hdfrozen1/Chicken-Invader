#ifndef __DEFINE_BITMASK_H__

class DefineBitmask
{
public:
	static const int NON = 0x00;
	static const int SHIP = 0x08;
	static const int ENEMY = 0x02;
	static const int BULLET = 0x01;
	static const int GIFT = 0x04;
	static const int FRAME = 16;
	static const int EBULLET = 32;
};

#endif // !__DEFINE_BITMASK_H__