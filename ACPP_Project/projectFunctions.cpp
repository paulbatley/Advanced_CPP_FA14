#include "projectHeader.h"



boardMember::boardMember()
{
	level = 0;
	HP = 0;
	ATK = 0;
	DFN = 0;
	POW = 0;
	std::cout << "member created\n";
}

boardMember::~boardMember()
{
	std::cout << "member destroyed\n";

}

Allie::Allie()
{
	expr = 0;
	std::cout << "Allie created\n";
}

Allie::~Allie()
{
	std::cout << "Allie destroyed\n";
}

Foe::Foe()
{

	xPos = 0;
	yPos = 0;
	std::cout << "Foe created\n";
}

Foe::~Foe()
{
	std::cout << " foe has been obliviated\n";

}

int Foe::XPgiven(Foe deadDude)
{
	int XP;
	int curLVL = deadDude.getLevel();
	int curATK = deadDude.getATK();
	XP = curATK / curLVL;
	return XP;
}

unsigned int boardMember::ATKcalc(boardMember defender)
{
	unsigned int HPdecreased = 0;
	int attackerATK = getATK();
	int defenderDEF = defender.getDEF();
	HPdecreased = defenderDEF - attackerATK;
	std::cout << "HP : " << defender.getHP() << std::endl;
	return HPdecreased;
}

int Allie::checkForLvlUp()
{
	int a = getLevel();
	int level = getLevel();
	switch (a)
	{
	case 0:
		if (expr > 1)
			level = levelUP();
	case 1:
		if (expr > 100)
			level = levelUP();
	case 2:
		if (expr > 300)
			level = levelUP();
	case 3:
		if (expr > 900)
			level = levelUP();
	case 4:
		if (expr > 2700)
			level = levelUP();
	default:
		break;
	}
	return level;
}

void boardMember::statSet(int newHP, int newPOW, int newATK, int newDFN)
{
	HP = newHP;
	POW = newPOW;
	ATK = newATK;
	DFN = newDFN;
}

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}