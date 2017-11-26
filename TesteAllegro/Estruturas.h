enum IDS{PLAYER, BULLET, ENEMY};

typedef struct SpaceShip
{
	int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	int score;
}SpaceShip;

typedef struct Comet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
}Comet;
typedef struct Bullet
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
}Bullet;
