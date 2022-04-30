#include "mtkpch.h"

#include "Example.h"
#include "File/FileHandler.h"
#include "Structures/Map.h"

int main() 
{
    auto map = mtk::Map<uint32_t, Player>([](uint32_t id){ return 0; });

    map.Insert(0, Player { "Bellona", 10, Player::Warrior });
    map.Insert(1, Player { "Poseidon", 20, Player::Mage });
    map.Insert(2, Player { "Ymir", 30, Player::Guardian });
    map.Insert(3, Player { "Hou Yi", 40, Player::Hunter });
    map.Insert(4, Player { "Tsukuyomi", 50, Player::Assassin });

    /*map.InsertRange({
        { 0, Player { "Bellona", 10, Player::Warrior }},
        { 1, Player { "Poseidon", 20, Player::Mage }},
        { 2, Player { "Ymir", 30, Player::Guardian }},
        { 3, Player { "Hou Yi", 40, Player::Hunter }},
        { 4, Player { "Tsukuyomi", 50, Player::Assassin }}
    });*/

    LOG(map);

    LOG(map.At(0));
    
}
