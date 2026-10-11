#include "main/profile/Profile.hpp"
#include "main/status/GameFlag.hpp"


ARM void profile::Profile::presetMember()
{
    this->pSYSTEM = &this->profiledata_.system;
    this->pPARTY = &this->profiledata_.party;
    this->pCHAPTER = this->profiledata_.chapter;
    this->pPLAYER = this->profiledata_.player;
    this->pMONSTER = this->profiledata_.monster;
    this->pHISTORY = this->profiledata_.history;
    this->pENVOY = this->profiledata_.envoy;
}

ARM void profile::Profile::collectGameFlag(unsigned char *flagBuf, status::GameFlag *gameFlag)
{
    for (int i = 0; i < 0x40; i++)
    {
        int bits = 0;
        for (int j = 0; j < 8; j++)
        {
            bits = (bits << 1) | gameFlag->check(i * 8 + j);
        }
        flagBuf[i] = bits;
    }
}

ARM void profile::Profile::deliverGameFlag(status::GameFlag *gameFlag, unsigned char *flagBuf)
{
    for (int i = 0; i < 0x40; i++)
    {
        int bits = flagBuf[i];
        int mask = 0x80;
        for (int j = 0; j < 8; j++)
        {
            if (bits & mask)
            {
                gameFlag->set(i * 8 + j);
            }
            else
            {
                gameFlag->remove(i * 8 + j);
            }
            mask >>= 1;
        }
    }
}

ARM int profile::Profile::calcCheckSum()
{
    unsigned int oldSum = profiledata_.system.CHECKSUM;
    int sum = 0;
    unsigned int i = 0;

    profiledata_.system.CHECKSUM = 0;

    unsigned char *p = (unsigned char *)&profiledata_;
    do
    {
        sum += *p++;
        i++;
    } while (i < 13056);

    profiledata_.system.CHECKSUM = sum;

    if (sum == oldSum)
    {
        return 1;
    }
    return 0;
}

ARM bool profile::Profile::isValidData()
{
    if (profiledata_.system.MAGIC != 1702130273)
    {
        return false;
    }
    if (profiledata_.system.VER == 200802140)
    {
        return true;
    }
    return false;
}