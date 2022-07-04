#include <iostream>
#include <vector>
#include <string>

#include "salad_shop.h"
#include "components.h"

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
    NovosibirskSaladShop nsk_shop;
    MoscowSaladShop      msk_shop;

    cout << "NSK Style:\n";
    nsk_shop.CreateSalad(SaladType::Olivie);
    nsk_shop.CreateSalad(SaladType::SeldyPodShuboy);
    cout << "\nMSK Style:\n";
    msk_shop.CreateSalad(SaladType::Olivie);
    msk_shop.CreateSalad(SaladType::SeldyPodShuboy);

    (void)argc;
    (void)argv;

    return 0;
}

