// MIT License
// 
// Copyright(c) 2020 Arthur Bacon and Kevin Dill
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright  this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Controller_AI_Viraj.h"


void Controller_AI_Viraj::tick(float deltaTSec)
{
    assert(m_pPlayer);

    // Focus of AI on defense first 

    if (!m_BTree) defensiveTree();
    m_BTree->tick();
}

void Controller_AI_Viraj::defensiveTree() {
    int var = 10; // For break-point
    Selector* rootSelector = new Selector();
    m_BTree = new BehaviourTree(rootSelector);
    
    // Use normal counters for defense - archer for giant, sword for archer
    
    // First strategy
    Selector* defenseStrat = new Selector();
    rootSelector->addChild(defenseStrat);
    // First sequence
    Sequence* stopGiantSequence = new Sequence();
    defenseStrat->addChild(stopGiantSequence);
    checkGiant* ifGiantIncoming = new checkGiant(m_pPlayer);
    killGiant* attackIncomingGiant = new killGiant(m_pPlayer);
    stopGiantSequence->addChild(ifGiantIncoming);
    stopGiantSequence->addChild(attackIncomingGiant);
    // Second sequence
    Sequence* stopArcherSequence = new Sequence();
    defenseStrat->addChild(stopArcherSequence);
    checkArcher* ifArcherIncoming = new checkArcher(m_pPlayer);
    killArcher* attackIncomingArcher = new killArcher(m_pPlayer);
    stopArcherSequence->addChild(ifArcherIncoming);
    stopArcherSequence->addChild(attackIncomingArcher);

    // Second strategy (Attacking if breathing room)
    Selector* attackStrat = new Selector();
    rootSelector->addChild(attackStrat);
    // First sequence
    Sequence* normalDefensiveAttack = new Sequence();
    attackStrat->addChild(normalDefensiveAttack);
    checkElixir* ifEnoughElixir = new checkElixir(m_pPlayer);
    normalAttack* attackNormally = new normalAttack(m_pPlayer);
    normalDefensiveAttack->addChild(ifEnoughElixir);
    normalDefensiveAttack->addChild(attackNormally);
    // Second sequence
    Sequence* fullAttackSequence = new Sequence();
    attackStrat->addChild(fullAttackSequence);
    checkIfEnemyWeak* ifKingWeak = new checkIfEnemyWeak(m_pPlayer);
    fullAttack* attackFully = new fullAttack(m_pPlayer);
    fullAttackSequence->addChild(ifKingWeak);
    fullAttackSequence->addChild(attackFully);
}

void Controller_AI_Viraj::agressiveTree() {
    Selector* rootSelector = new Selector();
    m_BTree = new BehaviourTree(rootSelector);

    // First strategy
    Selector* attackStrat = new Selector();
    rootSelector->addChild(attackStrat);
    // First sequence
    Sequence* normalAttackSequence = new Sequence();
    attackStrat->addChild(normalAttackSequence);
    checkElixir* ifEnoughElixir = new checkElixir(m_pPlayer);
    normalAttack* attackNormally = new normalAttack(m_pPlayer);
    normalAttackSequence->addChild(ifEnoughElixir);
    normalAttackSequence->addChild(attackNormally);
    // Second sequence
    Sequence* fullAttackSequence = new Sequence();
    attackStrat->addChild(fullAttackSequence);
    checkIfEnemyWeak* ifKingWeak = new checkIfEnemyWeak(m_pPlayer);
    fullAttack* attackFully = new fullAttack(m_pPlayer);
    fullAttackSequence->addChild(ifKingWeak);
    fullAttackSequence->addChild(attackFully);

    // Second strategy
    Selector* defenseStrat = new Selector();
    rootSelector->addChild(defenseStrat);
    // First sequence
    Sequence* stopArcherSequence = new Sequence();
    defenseStrat->addChild(stopArcherSequence);
    checkArcher* ifArcherIncoming = new checkArcher(m_pPlayer);
    killArcher* attackIncomingArcher = new killArcher(m_pPlayer);
    stopArcherSequence->addChild(ifArcherIncoming);
    stopArcherSequence->addChild(attackIncomingArcher);
}

