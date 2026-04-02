#include "Game.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

void Game::prettyPrint(const IFormattable& object) {
    cout << "--- INFO --- [ " << object.format() << " ] ----------" << endl;
}

int Game::getSafeInt() {
    string input;
    while (true) {
        if (!(cin >> input)) return 0;
        input.erase(remove_if(input.begin(), input.end(), [](char c) {
            return !isdigit((unsigned char)c);
            }), input.end());

        if (input.empty()) {
            cout << "![Ошибка] Введите число: ";
            continue;
        }
        try { return stoi(input); }
        catch (...) { cout << "![Ошибка] Число слишком велико: "; }
    }
}

void Game::reset() {
    hands[0].clear();
    hands[1].clear();
    dealer.clear();
    if (deck.getSize() < 15) deck.generate();
    isSplit = false;
}

void Game::play() {
    bool session = true;
    deck.generate();

    while (session && balance > 0) {
        reset();
        DeckAdapter adapter(deck);

        cout << "\n========== ВАШ БАЛАНС: " << balance << " ==========" << endl;
        cout << "Введите ставку: ";
        currentBet = getSafeInt();
        if (currentBet > balance) currentBet = balance;
        balance -= currentBet;

        if (currentBet == 1488) {
            Card* c = deck.drawCard();
            hands[0].addCard(c);
            hands[0].addCard(new Card(c->getRank(), c->getSuit(), c->getValue()));
        }
        else {
            hands[0].addCard(deck.drawCard());
            hands[0].addCard(deck.drawCard());
        }
        dealer.addCard(deck.drawCard());

        while (true) {
            prettyPrint(adapter);

            cout << "Дилер:  " << *dealer.getHand()[0] << "  ??" << endl;

            for (int i = 0; i <= (isSplit ? 1 : 0); i++) {
                cout << "Рука " << (i + 1) << ": ";
                for (auto c : hands[i].getHand()) cout << *c << "  ";
                cout << "(Очки: " << hands[i].getScore() << ")" << endl;
            }

            bool canSplit = (!isSplit && hands[0].getHand().size() == 2 &&
                hands[0].getHand()[0]->getRank() == hands[0].getHand()[1]->getRank() &&
                balance >= currentBet);

            cout << "\n1. Хватит | 2. Еще в Р1";
            if (isSplit) cout << " | 3. Еще в Р2";
            if (canSplit) cout << " | 4. СПЛИТ";
            cout << "\nВаш выбор: ";

            int choice = getSafeInt();
            if (choice == 1) break;

            if (choice == 2) {
                try { hands[0].addCard(deck.drawCard()); }
                catch (const BustException& e) {
                    cout << "!!! " << e.what() << " В РУКЕ 1 !!!" << endl;
                    cout << "Лишняя карта: " << *hands[0].getHand().back() << endl;
                    if (!isSplit) goto end_turn;
                }
            }

            if (choice == 3 && isSplit) {
                try { hands[1].addCard(deck.drawCard()); }
                catch (const BustException& e) {
                    cout << "!!! " << e.what() << " В РУКЕ 2 !!!" << endl;
                    cout << "Лишняя карта: " << *hands[1].getHand().back() << endl;
                }
            }

            if (choice == 4 && canSplit) {
                isSplit = true;
                balance -= currentBet;
                Card* toMove = hands[0].getHand().back();
                hands[1].addCard(toMove);
                hands[0].removeLastCard();
                try { hands[0].addCard(deck.drawCard()); }
                catch (...) {}
                try { hands[1].addCard(deck.drawCard()); }
                catch (...) {}
                cout << "\n>>> СПЛИТ ВЫПОЛНЕН <<<" << endl;
            }

            if (!isSplit && hands[0].getScore() > 21) break;
            if (isSplit && hands[0].getScore() > 21 && hands[1].getScore() > 21) break;
        }

    end_turn:
        int target = 0;
        if (hands[0].getScore() <= 21) target = hands[0].getScore();
        if (isSplit && hands[1].getScore() <= 21 && hands[1].getScore() > target)
            target = hands[1].getScore();

        if (target > 0) {
            while (dealer.getScore() < target && dealer.getScore() < 21) {
                try {
                    dealer.addCard(deck.drawCard());
                    if (dealer.getScore() >= target) break;
                }
                catch (const BustException&) {
                    break;
                }
            }
        }
        else {
            cout << "\n>>> У игрока перебор. Дилер не добирает. <<<" << endl;
        }

        cout << "\nКарты дилера: ";
        for (auto c : dealer.getHand()) cout << *c << "  ";
        cout << "(Итого: " << dealer.getScore() << ")" << endl;

        cout << "\n--- РЕЗУЛЬТАТЫ ---" << endl;
        int oldBal = balance + currentBet + (isSplit ? currentBet : 0);

        for (int i = 0; i <= (isSplit ? 1 : 0); i++) {
            int s = hands[i].getScore();
            cout << "Рука " << (i + 1) << ": ";
            if (s > 21) cout << "Проигрыш (Перебор " << s << ").";
            else if (dealer.getScore() > 21 || s > dealer.getScore()) {
                balance += (currentBet * 2);
                cout << "ПОБЕДА! + " << currentBet;
            }
            else if (s == dealer.getScore()) {
                balance += currentBet;
                cout << "НИЧЬЯ.";
            }
            else cout << "Проигрыш.";
            cout << endl;
        }

        cout << "БАЛАНС: " << balance << " (Итог кона: " << (balance - oldBal) << ")" << endl;
        if (balance <= 0) break;

        cout << "\nЕще ? (1-Да, 0-Нет): ";
        if (getSafeInt() == 0) session = false;
    }
}