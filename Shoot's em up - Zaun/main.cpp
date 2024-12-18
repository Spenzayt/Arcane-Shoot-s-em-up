﻿#include <iostream>
#include <map>
#include "parallax.hpp"
#include "menu.hpp"
#include "Classes.hpp"
#include "hud.hpp"
#include "buff.hpp"
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

Ekko Ekko_Class;
Marcus Marcus_Class;
Soldier Soldier_Class;
MediumSoldier MediumSoldier_Class;
HardSoldier HardSoldier_Class;
Game game;

std::vector<sf::Keyboard::Key> konamiCode = {
    sf::Keyboard::Up, sf::Keyboard::Up,
    sf::Keyboard::Down, sf::Keyboard::Down,
    sf::Keyboard::Left, sf::Keyboard::Right,
    sf::Keyboard::Left, sf::Keyboard::Right,
    sf::Keyboard::B, sf::Keyboard::A
}; 

std::vector<sf::Keyboard::Key> inputSequence;

bool konamiCodeActivated = false;

void checkKonamiCode() {
    if (inputSequence == konamiCode) {
        if (!konamiCodeActivated) {
            std::cout << "Konami Code active !" << std::endl;
            Ekko_Class.Ekko_invincibility = true;
            konamiCodeActivated = true;
        }
        else {
            std::cout << "Konami Code desactivated !" << std::endl;
            Ekko_Class.Ekko_invincibility = false;
            konamiCodeActivated = false;
        }
    }
}

int mainGame() {

#pragma region Game Initialisation
    ParallaxBackground background1("assets/backgrounds/ground-zaunV2.png", 150.0f, 630, 1.1, 1.1);
    ParallaxBackground background2("assets/backgrounds/background-zaun.jpeg", 20.0f, -1890, 2, 2);

    Clock clock;

    game.init();

    Ekko_Class.ekkoInitAnimations();
    Ekko_Class.bulletInit();
    Ekko_Class.initializeSpells();

    Marcus_Class.marcusInitAnimations();
    Marcus_Class.marcusBulletInit();

    Soldier_Class.soldierInitAnimations();
    Soldier_Class.soldierBulletInit();

    Soldier_Class.createSoldiers(3);
    MediumSoldier_Class.createSoldiers(2);

    MediumSoldier_Class.mediumSoldierInitAnimations();
    MediumSoldier_Class.mediumSoldierBulletInit();

    HardSoldier_Class.hardSoldierInitAnimations();
    HardSoldier_Class.hardSoldierBulletInit();

    HUD healthBar(100, 100, 3);

    int animationDelay = 20;
    int animationDelayAttack = 50;

    bool invincibilityTimer = false;

    BlueBuff blueBuff(sf::Vector2f(1000, 800));
    RedBuff redBuff(sf::Vector2f(1000, 1000));

    bool BlueBuffTimer = false;
    bool RedBuffTimer = false;

#pragma endregion Game Initialisation

#pragma region Clocks

    animationDelay = static_cast<int>(20.0f / Ekko_Class.Ekko_speed);
    animationDelayAttack = static_cast<int>(50.0f / Ekko_Class.Ekko_attackSpeed);

    auto startTime = chrono::steady_clock::now();
    auto waitTime = chrono::milliseconds(70);
    auto startAttTime = chrono::steady_clock::now();
    auto waitAttTime = chrono::milliseconds(animationDelayAttack);
    auto startReadyToAttackTime = chrono::steady_clock::now();
    auto waitReadyToAttackTime = chrono::seconds(1);
    auto startDashTime = chrono::steady_clock::now();
    auto waitDashTime = chrono::milliseconds(5);

    auto startHitTime = chrono::steady_clock::now();
    auto waitHitTime = chrono::milliseconds(160);

    chrono::steady_clock::time_point invincibilityStartTime;

    auto M_startAttTime = chrono::steady_clock::now();
    auto M_waitAttTime = chrono::milliseconds(Marcus_Class.attackSpeed);
    auto M_startDying = chrono::steady_clock::now();
    auto M_waitDying = chrono::seconds(3);
    auto M_startReadyToAttackTime = chrono::steady_clock::now();
    auto M_waitReadyToAttackTime = chrono::seconds(1);

    auto M_startTrans = chrono::steady_clock::now();
    auto M_waitTrans = chrono::milliseconds(250);

    auto S_startTime = chrono::steady_clock::now();
    auto S_waitTime = chrono::milliseconds(Soldier_Class.attackSpeed);
    auto S_startAttTime = chrono::steady_clock::now();
    auto S_waitAttTime = chrono::milliseconds(800);
    auto S_startDying = chrono::steady_clock::now();
    auto S_waitDying = chrono::seconds(3);
    auto S_startReadyToAttackTime = chrono::steady_clock::now();
    auto S_waitReadyToAttackTime = chrono::seconds(1);

    auto startNewWaveSoldier = chrono::steady_clock::now();
    auto waitNewWaveSoldier = chrono::seconds(6);

    auto startNewWaveMediumSoldier = chrono::steady_clock::now();
    auto waitNewWaveMediumSoldier = chrono::seconds(10);

    auto startNewWaveHardSoldier = chrono::steady_clock::now();
    auto waitNewWaveHardSoldier = chrono::seconds(15);

    auto startNewWaveMarcus = chrono::steady_clock::now();
    auto waitNewWaveMarcus = chrono::seconds(120);  ////////////////////////////

    auto MS_startTime = chrono::steady_clock::now();
    auto MS_waitTime = chrono::milliseconds(MediumSoldier_Class.attackSpeed);
    auto MS_startAttTime = chrono::steady_clock::now();
    auto MS_waitAttTime = chrono::milliseconds(800);
    auto MS_startDying = chrono::steady_clock::now();
    auto MS_waitDying = chrono::seconds(3);
    auto MS_startReadyToAttackTime = chrono::steady_clock::now();
    auto MS_waitReadyToAttackTime = chrono::seconds(1);

    auto HS_startTime = chrono::steady_clock::now();
    auto HS_waitTime = chrono::milliseconds(HardSoldier_Class.attackSpeed);
    auto HS_startAttTime = chrono::steady_clock::now();
    auto HS_waitAttTime = chrono::milliseconds(800);
    auto HS_startDying = chrono::steady_clock::now();
    auto HS_waitDying = chrono::seconds(3);
    auto HS_startReadyToAttackTime = chrono::steady_clock::now();
    auto HS_waitReadyToAttackTime = chrono::seconds(1);

    auto BlueBuffStartTime = chrono::steady_clock::now();
    auto RedBuffStartTime = chrono::steady_clock::now();

#pragma endregion Clocks

    while (game.window.isOpen()) {
        Time deltaTime = clock.restart();
        Event event;
        while (game.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                game.window.close();
        }
        game.window.clear();

        if (!Marcus_Class.marcusApparition) {
            auto newWaveNowTimeSoldier = chrono::steady_clock::now();
            if (newWaveNowTimeSoldier >= startNewWaveSoldier + waitNewWaveSoldier) {
                for (int i = 0; i < rand() % 1 + 4; i++)
                {
                    Soldier_Class.createSoldiers(1);
                }
                startNewWaveSoldier = newWaveNowTimeSoldier;
            }

            auto newWaveNowTimeMediumSoldier = chrono::steady_clock::now();
            if (newWaveNowTimeMediumSoldier >= startNewWaveMediumSoldier + waitNewWaveMediumSoldier) {
                for (int i = 0; i < rand() % 1 + 3; i++) 
                {
                    MediumSoldier_Class.createSoldiers(1);
                }
                startNewWaveMediumSoldier = newWaveNowTimeMediumSoldier;
            }

            auto newWaveNowTimeHardSoldier = chrono::steady_clock::now();
            if (newWaveNowTimeHardSoldier >= startNewWaveHardSoldier + waitNewWaveHardSoldier) {
                for (int i = 0; i < rand() % 1 + 2; i++)
                {
                    HardSoldier_Class.createSoldiers(1);
                }                
                startNewWaveHardSoldier = newWaveNowTimeHardSoldier;
            }
        }

        auto newWaveNowTimeMarcus = chrono::steady_clock::now();
        if (newWaveNowTimeMarcus >= startNewWaveMarcus + waitNewWaveMarcus) {
            Marcus_Class.marcusApparition = true;
        }

#pragma region KonamiCode
        if (event.type == sf::Event::KeyPressed) {
            inputSequence.push_back(event.key.code);

            // Limiter la taille de la séquence à celle du Konami Code
            if (inputSequence.size() > konamiCode.size()) {
                inputSequence.erase(inputSequence.begin());
            }

            // Vérifier si la séquence correspond au Konami Code
            checkKonamiCode();
        }

#pragma endregion KonamiCode

#pragma region Background
        background1.update(deltaTime.asSeconds());
        background2.update(deltaTime.asSeconds());

        background2.draw(game.window);
        background1.draw(game.window);

#pragma endregion Background

#pragma region Ekko
        if (Ekko_Class.getAlive()) {
            Ekko_Class.ekkoCommand();
            Ekko_Class.ekkoDontExitFromScreen();
            Ekko_Class.updatePositionHistory();
            Ekko_Class.updateTeleport();

            int xHealth = Ekko_Class.ekko_walk_sprite.getPosition().x - 10;
            int yHeatlh = Ekko_Class.ekko_walk_sprite.getPosition().y - 15;
            healthBar.updatePosition(xHealth, yHeatlh);

            auto nowTime = chrono::steady_clock::now();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) waitTime = chrono::milliseconds(animationDelay);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) waitTime = chrono::milliseconds(animationDelay);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) waitTime = chrono::milliseconds(animationDelay);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) waitTime = chrono::milliseconds(animationDelay);
            else {
                waitTime = chrono::milliseconds(70);
            }
            if (nowTime >= startTime + waitTime) {
                Ekko_Class.ekko_anim.x++;
                startTime = nowTime;
            }
            if (Ekko_Class.Ekko_invincibility) {
                auto nowHitTime = chrono::steady_clock::now();
                if (nowHitTime >= startHitTime + waitHitTime) {
                    Ekko_Class.ekko_walk_sprite.setColor(sf::Color::Transparent);
                }
                if (nowHitTime >= startHitTime + waitHitTime + chrono::milliseconds(160)) {
                    Ekko_Class.ekko_walk_sprite.setColor(sf::Color::White);
                    startHitTime = nowHitTime;
                }
            }
            if (!Ekko_Class.Ekko_invincibility && !blueBuff.BlueBuffActivated && !redBuff.RedBuffActivated) {
                Ekko_Class.ekko_walk_sprite.setColor(sf::Color::White);
                Ekko_Class.ekko_Auto_Attack_sprite.setColor(sf::Color::White);
            }

            if (Ekko_Class.ekko_S.ekko_anim_isAttacking) {
                auto nowAttTime = chrono::steady_clock::now();
                if (nowAttTime >= startAttTime + waitAttTime) {
                    Ekko_Class.ekko_S.countAnimAtk++;
                    Ekko_Class.ekko_anim_Auto_Attack.x++;
                    if (Ekko_Class.ekko_S.countAnimAtk == 1) {
                        Ekko_Class.bullets.push_back(sf::CircleShape());
                        Ekko_Class.bullets.back().setFillColor(sf::Color::Transparent);
                        Ekko_Class.bullets.back().setRadius(10);
                        Ekko_Class.bullets.back().setPosition(Ekko_Class.ekko_Auto_Attack_sprite.getPosition().x + 128, Ekko_Class.ekko_Auto_Attack_sprite.getPosition().y + 32);
                    }
                    if (Ekko_Class.ekko_S.countAnimAtk == 9) {
                        Ekko_Class.ekko_S.countAnimAtk = 0;
                        Ekko_Class.ekko_S.ekko_anim_isAttacking = false;
                    }
                    startAttTime = nowAttTime;
                }
            }

            Ekko_Class.ekkoPrintWindow(game.window);
            healthBar.draw(game.window);

            for (int i = 0; i < Ekko_Class.bullets.size(); i++) {
                bool destroy = false;
                game.window.draw(Ekko_Class.bullets[i]);
                Ekko_Class.bullets[i].move(20 * Ekko_Class.Ekko_attackSpeed, 0);
                Ekko_Class.ekko_Bullet_Auto_Attack_sprite.setPosition(Ekko_Class.bullets[i].getPosition().x - 30, Ekko_Class.bullets[i].getPosition().y - 6);

                //Soldier
                for (auto& soldier : Soldier_Class.soldiers_vector) {
                    if (Ekko_Class.bullets[i].getGlobalBounds().intersects(soldier.soldier_walk_sprite.getGlobalBounds()) && soldier.getAlive()) {
                        soldier.losePV(1);
                        destroy = true;
                    }
                }
                //MediumSoldier
                for (auto& mediumSoldier : MediumSoldier_Class.mediumSoldiers_vector) {
                    if (Ekko_Class.bullets[i].getGlobalBounds().intersects(mediumSoldier.medium_soldier_walk_sprite.getGlobalBounds()) && mediumSoldier.getAlive()) {
                        mediumSoldier.losePV(1);
                        destroy = true;
                    }
                }
                //HardSoldier
                for (auto& hardSoldier : HardSoldier_Class.hardSoldiers_vector) {
                    if (Ekko_Class.bullets[i].getGlobalBounds().intersects(hardSoldier.hard_soldier_walk_sprite.getGlobalBounds()) && hardSoldier.getAlive()) {
                        hardSoldier.losePV(1);
                        destroy = true;
                    }
                }
                //Marcus
                if (Ekko_Class.bullets[i].getGlobalBounds().intersects(Marcus_Class.marcus_Auto_Attack_sprite.getGlobalBounds()) && Marcus_Class.getAlive()) {
                    Marcus_Class.losePV(1);
                    destroy = true;
                }

                if (destroy)
                    Ekko_Class.bullets.erase(Ekko_Class.bullets.begin() + i);

                else if (Ekko_Class.ekko_Bullet_Auto_Attack_sprite.getPosition().x >= 1850) {
                    Ekko_Class.bullets.erase(Ekko_Class.bullets.begin() + i);
                }

                Ekko_Class.ekko_Bullet_Auto_Attack_sprite.move(20 * Ekko_Class.Ekko_attackSpeed, 0);
                game.window.draw(Ekko_Class.ekko_Bullet_Auto_Attack_sprite);
            }

            if (invincibilityTimer && Ekko_Class.getHealth() > 0) {
                auto now = chrono::steady_clock::now();
                auto duration = chrono::duration_cast<chrono::seconds>(now - invincibilityStartTime).count();
                if (duration == 2) {
                    Ekko_Class.Ekko_invincibility = false;
                    invincibilityTimer = false;
                }
            }
        }

#pragma endregion Ekko

#pragma region Marcus
        if (Marcus_Class.marcusApparition) {
            if (!Marcus_Class.getAlive()) {
                auto M_nowDying = chrono::steady_clock::now();
                if (M_nowDying >= M_startDying + M_waitDying) {
                    Marcus_Class.marcus_Auto_Attack_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                    Marcus_Class.marcus_SecondPhase_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                    Marcus_Class.marcus_SecondPhase_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                }
            }
            if (Marcus_Class.getAlive()) {
                if (Marcus_Class.moveToFight == true) {
                    Marcus_Class.marcus_Auto_Attack_sprite.move(-2, 0);
                }
                if (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().x <= 1300) {
                    Marcus_Class.moveToFight = false;
                }

                //ici fait en sorte que le Marcus puisse bouger tout le temps pendant qu'il attaque quand il est en deuxieme phase
                //fait bouger les trois sprite comme ça pas d'emmerde

                ///////////////////////////////////////////////////////////////////////


                if (Marcus_Class.m_health <= 30 && !Marcus_Class.isAttackingV2) {
                    Marcus_Class.isAttacking = false;
                    Marcus_Class.transIsIn = true;

                    auto M_nowTrans = chrono::steady_clock::now();
                    if (M_nowTrans >= M_startTrans + M_waitTrans) {
                        Marcus_Class.marcus_anim_TransSecondPhase.x++;
                        Marcus_Class.countAnimTrans++;
                        M_startTrans = M_nowTrans;
                    }

                    if (Marcus_Class.countAnimTrans == 13) {
                        Marcus_Class.isAttackingV2 = true;
                        Marcus_Class.transIsIn = false;
                    }
                }

                auto M_nowAttTime = chrono::steady_clock::now();
                if (M_nowAttTime >= M_startAttTime + M_waitAttTime) {

                    if (Marcus_Class.isAttackingV2) {
                        Marcus_Class.marcus_anim_SecondPhase.x++;
                        Marcus_Class.countLaserTime++;
                        if (Marcus_Class.countLaserTime == 30) {
                            Marcus_Class.laserActive = true;
                        }
                        if (Marcus_Class.laserActive && !Marcus_Class.moveToFight && !Marcus_Class.transIsIn) {
                            Marcus_Class.MarcusLaser.push_back(sf::CircleShape());
                            Marcus_Class.MarcusLaser.back().setFillColor(sf::Color::Red);
                            Marcus_Class.MarcusLaser.back().setRadius(15);
                            Marcus_Class.MarcusLaser.back().setPosition(Marcus_Class.marcus_Auto_Attack_sprite.getPosition().x, Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y + 260);
                            if (Marcus_Class.countLaserTime == 300) {
                                Marcus_Class.laserActive = false;
                            }
                        }
                    }
                    else if (Marcus_Class.isAttacking) {
                        Marcus_Class.marcus_anim_Auto_Attack.x++;
                    }
                    ///////////////////////////////////////////////////////////////////////////////////////////////////////
                    Marcus_Class.countAnimAtk++;
                    Marcus_Class.reload = false;
                    if (Marcus_Class.countAnimAtk == 1 && !Marcus_Class.moveToFight && !Marcus_Class.transIsIn) {
                        Marcus_Class.countBulletsMarcus++;
                        Marcus_Class.MarcusBullets.push_back(sf::CircleShape());
                        Marcus_Class.MarcusBullets.back().setTexture(&Marcus_Class.marcus_Bullet_Auto_Attack_texture);
                        Marcus_Class.MarcusBullets.back().setTextureRect(sf::IntRect(32, 0, 32, 32));
                        Marcus_Class.MarcusBullets.back().setRadius(15);
                        Marcus_Class.MarcusBullets.back().setPosition(Marcus_Class.marcus_Auto_Attack_sprite.getPosition().x, Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y + 260);
                    }
                    if (Marcus_Class.countAnimAtk == 5) {
                        Marcus_Class.countAnimAtk = 0;
                    }

                    if (Marcus_Class.isAttackingV2) {
                        if (Marcus_Class.countBulletsMarcus >= 25) {
                            Marcus_Class.reload = true;
                        }
                    }
                    else if (!Marcus_Class.isAttackingV2) {
                        if (Marcus_Class.countBulletsMarcus >= 17) {
                            Marcus_Class.reload = true;
                        }
                    }
                    if (Marcus_Class.isAttackingV2) {
                        if (Marcus_Class.reload) {
                            M_waitAttTime = chrono::seconds(2);
                            Marcus_Class.countBulletsMarcus = 0;
                        }
                        if (!Marcus_Class.reload) {
                            M_waitAttTime = chrono::milliseconds(Marcus_Class.attackSpeed2);
                        }
                    }
                    if (!Marcus_Class.isAttackingV2) {
                        if (Marcus_Class.reload) {
                            M_waitAttTime = chrono::seconds(2);
                            Marcus_Class.countBulletsMarcus = 0;
                        }
                        if (!Marcus_Class.reload) {
                            M_waitAttTime = chrono::milliseconds(Marcus_Class.attackSpeed);
                        }
                    }
                    M_startAttTime = M_nowAttTime;
                }

                Marcus_Class.marcusPrintWindow(game.window);

                for (int i = 0; i < Marcus_Class.MarcusBullets.size(); i++) {
                    bool destroyBulletsMarcus = false;
                    if (Ekko_Class.ekko_walk_sprite.getPosition().y <= 700) Marcus_Class.MarcusBullets[i].move(-15 * Marcus_Class.bulletSpeed, (Ekko_Class.ekko_walk_sprite.getPosition().x) / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y - Ekko_Class.ekko_walk_sprite.getPosition().y));/* / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y / Ekko_Class.ekko_walk_sprite.getPosition().y - 260)*/
                    else Marcus_Class.MarcusBullets[i].move(-15 * Marcus_Class.bulletSpeed, ((Ekko_Class.ekko_walk_sprite.getPosition().x + Ekko_Class.ekko_walk_sprite.getPosition().y) / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y + 128)));

                    game.window.draw(Marcus_Class.MarcusBullets[i]);

                    if (Marcus_Class.MarcusBullets[i].getGlobalBounds().intersects(Ekko_Class.ekko_walk_sprite.getGlobalBounds()) && !Ekko_Class.Ekko_invincibility && Ekko_Class.getAlive()) {
                        healthBar.updateLife(Ekko_Class.losePV(1));
                        Ekko_Class.Ekko_invincibility = true;
                        invincibilityTimer = true;
                        invincibilityStartTime = chrono::steady_clock::now();
                        destroyBulletsMarcus = true;
                        //Marcus_Class.MarcusBullets.erase(Marcus_Class.MarcusBullets.begin() + i);
                    }
                    else if (Marcus_Class.MarcusBullets[i].getPosition().x < 0 || Marcus_Class.MarcusBullets[i].getPosition().y < 0 || Marcus_Class.MarcusBullets[i].getPosition().y > 1080) {
                        destroyBulletsMarcus = true;
                        //Marcus_Class.MarcusBullets.erase(Marcus_Class.MarcusBullets.begin() + i);
                    }


                    if (destroyBulletsMarcus == true) {
                        Marcus_Class.MarcusBullets.erase(Marcus_Class.MarcusBullets.begin() + i);
                    }
                }

                for (int i = 0; i < Marcus_Class.MarcusLaser.size(); i++) {
                    if (Ekko_Class.ekko_walk_sprite.getPosition().y < 700) Marcus_Class.MarcusLaser[i].move(-10 * Marcus_Class.laserSpeed, (Ekko_Class.ekko_walk_sprite.getPosition().x) / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y - Ekko_Class.ekko_walk_sprite.getPosition().y)/* / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y / Ekko_Class.ekko_walk_sprite.getPosition().y - 260)*/);
                    else Marcus_Class.MarcusLaser[i].move(-10 * Marcus_Class.laserSpeed, ((Ekko_Class.ekko_walk_sprite.getPosition().x + Ekko_Class.ekko_walk_sprite.getPosition().y) / (Marcus_Class.marcus_Auto_Attack_sprite.getPosition().y + 128)));

                    game.window.draw(Marcus_Class.MarcusLaser[i]);

                    if (Marcus_Class.MarcusLaser[i].getGlobalBounds().intersects(Ekko_Class.ekko_walk_sprite.getGlobalBounds()) && !Ekko_Class.Ekko_invincibility && Ekko_Class.getAlive()) {
                        healthBar.updateLife(Ekko_Class.losePV(1));
                        Ekko_Class.Ekko_invincibility = true;
                        invincibilityTimer = true;
                        invincibilityStartTime = chrono::steady_clock::now();
                        Marcus_Class.MarcusLaser.erase(Marcus_Class.MarcusLaser.begin() + i);
                    }
                    else if (Marcus_Class.MarcusLaser[i].getPosition().x < 0 || Marcus_Class.MarcusLaser[i].getPosition().y < 0 || Marcus_Class.MarcusLaser[i].getPosition().y > 1080) {
                        Marcus_Class.MarcusLaser.erase(Marcus_Class.MarcusLaser.begin() + i);
                    }
                }
            }
        }
#pragma endregion Marcus

#pragma region Soldier

        for (auto& soldier : Soldier_Class.soldiers_vector) {
            if (!soldier.getAlive()) {
                auto S_nowDying = chrono::steady_clock::now();
                if (S_nowDying >= S_startDying + S_waitDying) {
                    soldier.soldier_walk_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                }
            }
            if (soldier.getAlive()) {
                if (soldier.moveToFight == true) {
                    soldier.soldier_walk_sprite.move(-10, 0);
                }
                if (soldier.soldier_walk_sprite.getPosition().x <= 1450) {
                    soldier.moveToFight = false;
                }
                auto S_nowTime = chrono::steady_clock::now();
                if (S_nowTime >= S_startTime + S_waitTime && soldier.getAlive()) {
                    Soldier_Class.soldier_anim.x++;
                    Soldier_Class.attackCountSoldier++;
                    Soldier_Class.reload = false;
                    if (Soldier_Class.attackCountSoldier == 2 && !soldier.moveToFight) {
                        Soldier_Class.bulletCreation();
                        Soldier_Class.countBulletsSoldier++;
                    }
                    if (Soldier_Class.soldier_anim.x >= 3 && Soldier_Class.soldier_anim.x <= 6 && !soldier.moveToFight) {
                        soldier.soldier_walk_sprite.move(0, -5);
                    }
                    if (Soldier_Class.soldier_anim.x >= 6 && Soldier_Class.soldier_anim.x <= 9 && !soldier.moveToFight) {
                        soldier.soldier_walk_sprite.move(0, 5);
                    }
                    if (Soldier_Class.soldier_anim.x == 10) {
                        Soldier_Class.attackCountSoldier = 0;
                    }
                    if (Soldier_Class.countBulletsSoldier == 10 && !soldier.moveToFight) {
                        Soldier_Class.reload = true;
                    }
                    if (Soldier_Class.reload == true && !soldier.moveToFight) {
                        S_waitTime = chrono::seconds(1);
                        Soldier_Class.countBulletsSoldier = 0;
                    }
                    if (Soldier_Class.reload == false && !soldier.moveToFight) {
                        S_waitTime = chrono::milliseconds(Soldier_Class.attackSpeed);
                    }
                    S_startTime = S_nowTime;
                }

                soldier.soldier_walk_sprite.setTextureRect(sf::IntRect(Soldier_Class.soldier_anim.x * 200, 0, -200, 157));

                if (Soldier_Class.soldier_anim.x * 200 >= Soldier_Class.soldier_walk_texture.getSize().x + 200) {
                    Soldier_Class.soldier_anim.x = 2;
                }
                for (int i = 0; i < soldier.SoldierBullets.size(); i++) {
                    soldier.SoldierBullets[i].move(-10 * soldier.bulletSpeed, 0);
                    game.window.draw(soldier.SoldierBullets[i]);

                    if (soldier.SoldierBullets[i].getGlobalBounds().intersects(Ekko_Class.ekko_walk_sprite.getGlobalBounds()) && !Ekko_Class.Ekko_invincibility && Ekko_Class.getAlive()) {
                        healthBar.updateLife(Ekko_Class.losePV(1));
                        Ekko_Class.Ekko_invincibility = true;
                        invincibilityTimer = true;
                        invincibilityStartTime = chrono::steady_clock::now();
                        soldier.SoldierBullets.erase(soldier.SoldierBullets.begin() + i);
                    }
                    else if (soldier.SoldierBullets[i].getPosition().x < 0) {
                        soldier.SoldierBullets.erase(soldier.SoldierBullets.begin() + i);
                    }
                }
            }

            Soldier_Class.soldierPrintWindow(game.window);
        }
#pragma endregion Soldier

#pragma region MediumSoldier
        for (auto& mediumSoldier : MediumSoldier_Class.mediumSoldiers_vector) {
            if (!mediumSoldier.getAlive()) {
                auto MS_nowDying = chrono::steady_clock::now();
                if (MS_nowDying >= MS_startDying + MS_waitDying) {
                    mediumSoldier.medium_soldier_walk_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                }
            }
            if (mediumSoldier.getAlive()) {
                if (mediumSoldier.moveToFight == true) {
                    mediumSoldier.medium_soldier_walk_sprite.move(-10, 0);
                }
                if (mediumSoldier.medium_soldier_walk_sprite.getPosition().x <= 1500) {
                    mediumSoldier.moveToFight = false;
                }
                auto MS_nowTime = chrono::steady_clock::now();
                if (MS_nowTime >= MS_startTime + MS_waitTime && mediumSoldier.getAlive()) {
                    MediumSoldier_Class.medium_soldier_anim.x++;
                    MediumSoldier_Class.attackCountMediumSoldier++;
                    MediumSoldier_Class.reload = false;
                    if (MediumSoldier_Class.attackCountMediumSoldier == 2 && !mediumSoldier.moveToFight) {
                        MediumSoldier_Class.bulletCreation();
                        MediumSoldier_Class.countBulletsMediumSoldier++;
                    }
                    if (MediumSoldier_Class.medium_soldier_anim.x == 10) {
                        MediumSoldier_Class.attackCountMediumSoldier = 0;
                    }
                    if (MediumSoldier_Class.countBulletsMediumSoldier == 10) {
                        MediumSoldier_Class.reload = true;
                    }
                    if (MediumSoldier_Class.reload == true) {
                        MS_waitTime = chrono::seconds(1);
                        MediumSoldier_Class.countBulletsMediumSoldier = 0;
                    }
                    if (MediumSoldier_Class.reload == false) {
                        MS_waitTime = chrono::milliseconds(MediumSoldier_Class.attackSpeed);
                    }
                    MS_startTime = MS_nowTime;
                }

                mediumSoldier.medium_soldier_walk_sprite.setTextureRect(sf::IntRect(MediumSoldier_Class.medium_soldier_anim.x * 200, 0, -200, 157));

                if (MediumSoldier_Class.medium_soldier_anim.x * 200 >= MediumSoldier_Class.medium_soldier_walk_texture.getSize().x + 200) {
                    MediumSoldier_Class.medium_soldier_anim.x = 2;
                }

                for (int i = 0; i < mediumSoldier.MediumSoldierBullets.size(); i++) {
                    mediumSoldier.MediumSoldierBullets[i].move(-10 * mediumSoldier.bulletSpeed, 0);
                    game.window.draw(mediumSoldier.MediumSoldierBullets[i]);

                    if (mediumSoldier.MediumSoldierBullets[i].getGlobalBounds().intersects(Ekko_Class.ekko_walk_sprite.getGlobalBounds()) && !Ekko_Class.Ekko_invincibility && Ekko_Class.getAlive()) {
                        healthBar.updateLife(Ekko_Class.losePV(1));
                        Ekko_Class.Ekko_invincibility = true;
                        invincibilityTimer = true;
                        invincibilityStartTime = chrono::steady_clock::now();
                        mediumSoldier.MediumSoldierBullets.erase(mediumSoldier.MediumSoldierBullets.begin() + i);
                    }
                    else if (mediumSoldier.MediumSoldierBullets[i].getPosition().x < 0) {
                        mediumSoldier.MediumSoldierBullets.erase(mediumSoldier.MediumSoldierBullets.begin() + i);
                    }
                }
            }
            MediumSoldier_Class.mediumSoldierPrintWindow(game.window);
        }

#pragma endregion MediumSoldier

#pragma region HardSoldier
        for (auto& hardSoldier : HardSoldier_Class.hardSoldiers_vector) {
            if (!hardSoldier.getAlive()) {
                auto HS_nowDying = chrono::steady_clock::now();
                if (HS_nowDying >= HS_startDying + HS_waitDying) {
                    hardSoldier.hard_soldier_walk_sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
                }
            }
            if (hardSoldier.getAlive()) {
                if (hardSoldier.moveToFight == true) {
                    hardSoldier.hard_soldier_walk_sprite.move(-10, 0);
                }
                if (hardSoldier.hard_soldier_walk_sprite.getPosition().x <= 1500) {
                    hardSoldier.moveToFight = false;
                }
                auto HS_nowTime = chrono::steady_clock::now();
                if (HS_nowTime >= HS_startTime + HS_waitTime && hardSoldier.getAlive()) {
                    HardSoldier_Class.hard_soldier_anim.x++;
                    HardSoldier_Class.attackCountHardSoldier++;
                    HardSoldier_Class.reload = false;
                    if (HardSoldier_Class.attackCountHardSoldier == 2 && !hardSoldier.moveToFight) {
                        HardSoldier_Class.bulletCreation();
                        HardSoldier_Class.countBulletsHardSoldier++;
                    }
                    if (HardSoldier_Class.hard_soldier_anim.x == 10) {
                        HardSoldier_Class.attackCountHardSoldier = 0;
                    }
                    if (HardSoldier_Class.countBulletsHardSoldier == 10) {
                        HardSoldier_Class.reload = true;
                    }
                    if (HardSoldier_Class.reload == true) {
                        HS_waitTime = chrono::seconds(1);
                        HardSoldier_Class.countBulletsHardSoldier = 0;
                    }
                    if (HardSoldier_Class.reload == false) {
                        HS_waitTime = chrono::milliseconds(HardSoldier_Class.attackSpeed);
                    }
                    HS_startTime = HS_nowTime;
                }

                hardSoldier.hard_soldier_walk_sprite.setTextureRect(sf::IntRect(HardSoldier_Class.hard_soldier_anim.x * 200, 0, -200, 157));

                if (HardSoldier_Class.hard_soldier_anim.x * 200 >= HardSoldier_Class.hard_soldier_walk_texture.getSize().x + 200) {
                    HardSoldier_Class.hard_soldier_anim.x = 2;
                }

                for (int i = 0; i < hardSoldier.HardSoldierBullets.size(); i++) {
                    hardSoldier.HardSoldierBullets[i].move(-10 * hardSoldier.bulletSpeed, 0);
                    game.window.draw(hardSoldier.HardSoldierBullets[i]);

                    if (hardSoldier.HardSoldierBullets[i].getGlobalBounds().intersects(Ekko_Class.ekko_walk_sprite.getGlobalBounds()) && !Ekko_Class.Ekko_invincibility && Ekko_Class.getAlive()) {
                        healthBar.updateLife(Ekko_Class.losePV(1));
                        Ekko_Class.Ekko_invincibility = true;
                        invincibilityTimer = true;
                        invincibilityStartTime = chrono::steady_clock::now();
                        hardSoldier.HardSoldierBullets.erase(hardSoldier.HardSoldierBullets.begin() + i);
                    }
                    else if (hardSoldier.HardSoldierBullets[i].getPosition().x < 0) {
                        hardSoldier.HardSoldierBullets.erase(hardSoldier.HardSoldierBullets.begin() + i);
                    }
                }
            }
            HardSoldier_Class.hardSoldierPrintWindow(game.window);
        }
#pragma endregion HardSoldier

        /*
        #pragma region Buff

                if (blueBuff.touchByThePlayer(Ekko_Class.ekko_walk_sprite)) {
                    if (!blueBuff.BlueBuffActivated) {
                        //std::cout << "Starting Blue Buff!" << std::endl;
                        blueBuff.BlueBuffActivated = true;
                        BlueBuffTimer = true;
                        BlueBuffStartTime = chrono::steady_clock::now();
                        Ekko_Class.Ekko_speed = 1.5;
                        Ekko_Class.ekko_walk_sprite.setColor(sf::Color::Cyan);
                        Ekko_Class.ekko_Auto_Attack_sprite.setColor(sf::Color::Cyan);
                    }
                }

                if (BlueBuffTimer) {
                    auto now = chrono::steady_clock::now();
                    auto duration = chrono::duration_cast<chrono::seconds>(now - BlueBuffStartTime).count();
                    if (duration > 10) {
                        blueBuff.BlueBuffActivated = false;
                        BlueBuffTimer = false;
                        //std::cout << "Ending Blue Buff!" << std::endl;
                        Ekko_Class.Ekko_speed = 1.0f;
                        Ekko_Class.ekko_walk_sprite.setColor(sf::Color::White);
                        Ekko_Class.ekko_Auto_Attack_sprite.setColor(sf::Color::White);
                    }
                }

                if (redBuff.touchByThePlayer(Ekko_Class.ekko_walk_sprite)) {
                    if (!redBuff.RedBuffActivated) {
                        //std::cout << "Starting Red Buff!" << std::endl;
                        redBuff.RedBuffActivated = true;
                        RedBuffTimer = true;
                        RedBuffStartTime = chrono::steady_clock::now();
                        Ekko_Class.Ekko_attackSpeed = 2.0f;
                        Ekko_Class.ekko_walk_sprite.setColor(sf::Color::Red);
                        Ekko_Class.ekko_Auto_Attack_sprite.setColor(sf::Color::Red);
                    }
                }

                if (RedBuffTimer) {
                    auto now = chrono::steady_clock::now();
                    auto duration = chrono::duration_cast<chrono::seconds>(now - RedBuffStartTime).count();
                    if (duration > 10) {
                        redBuff.RedBuffActivated = false;
                        RedBuffTimer = false;
                        //std::cout << "Ending Red Buff!" << std::endl;
                        Ekko_Class.Ekko_attackSpeed = 1.0f;
                        Ekko_Class.ekko_walk_sprite.setColor(sf::Color::White);
                        Ekko_Class.ekko_Auto_Attack_sprite.setColor(sf::Color::White);
                    }
                }

                blueBuff.draw(game.window);
                redBuff.draw(game.window);

        #pragma endregion Buff
                */
        game.window.display();
    }
    return 0;
}

int main() {
    Menu menu;
    menu.run_menu();
    if (menu.isPlaySelected()) {
        mainGame();
    }
    return EXIT_SUCCESS;
}