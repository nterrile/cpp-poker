#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

using namespace std;

class Card {
public:
	Card() {};
	Card(int suit, int rank) { //suit is 0-3, rank is 0-12
		this->suit = suit;
		this->rank = rank;
	}
	int getRank() {
		return rank;
	}
	int getSuit() {
		return suit;
	}
	string toString() {
		return ranks[rank] + " of " + suits[suit];
	}
	void printCard() {
		cout << " ___ " << endl;
		if (rank == 0 || rank > 9) {
			cout << "|" << (char)toupper(ranks[rank][0]) << "  |" << endl;
		} else if (rank == 9) {
			cout << "|10 |" << endl;
		} else {
			cout << "|" << rank + 1 << "  |" << endl;
		}
		if (suit == 0) {
			cout << "| \u2663 |" << endl;
		} else if (suit == 1) {
			cout << "| \u2666 |" << endl;
		} else if (suit == 2) {
			cout << "| \u2665 |" << endl;
		} else {
			cout << "| \u2660 |" << endl;
		}
		cout << "'–––'" << endl;
	}
private:
  int suit;
	int rank;
  string suits[4] = {"clubs", "diamonds", "hearts", "spades"};
	string ranks[13] = {"ace", "two", "three", "four", "five", "six", "seven",
		"eight", "nine", "ten", "jack", "queen", "king"};
};

class Deck {
public:
	Deck() {
		srand(time(0));
		size = 52;
		for (int i = 0; i < 13; i++) {
			pile[i] = Card(0, i);	// clubs
			pile[i+13] = Card(1, i);	// diamonds
			pile[i+26] = Card(2, i);	// hearts
			pile[i+39] = Card(3, i);	// spades
		}
	}
  void shuffle() {
		Deck();
	}
  Card drawCard() {
		Card thePick;
		Card temp;
		int pos = rand() % size;
		thePick = pile[pos];
		temp = pile[size - 1];
		pile[size - 1] = thePick;
		pile[pos] = temp;
		size--;
		return thePick;
	}
  Card getCard(int i) {
		return pile[i];
	}
private:
	Card pile[52];
	int size;
};

class Player {
public:
  Player() {
		chips = 0;
    chipsInPot = 0;
    folded = false;
		allIn = false;
		bet = 0;
		rank = 0;
  }
  int getChips() {
    return chips;
  }
  void setChips(int value) {
    chips = value;
  }
  void addChips(int value) {
    chips += value;
  }
  void subtractChips(int value) {
    chips -= value;
  }
	int getTotalChips() {
		return chips + chipsInPot;
	}
  Card getCard1() {
    return cards[0];
  }
  Card getCard2() {
    return cards[1];
  }
	Card* getCards() {
		return cards;
	}
  void setCard1(Card c) {
    cards[0] = c;
  }
  void setCard2(Card c) {
    cards[1] = c;
  }
  string getName() {
    return name;
  }
  void setName(string n) {
    name = n;
  }
  int getChipsInPot() {
    return chipsInPot;
  }
  void addChipsToPot(int value) {
		if (value >= chips) {
			value = chips;
			allIn = true;
		}
    subtractChips(value);
    chipsInPot += value;
		bet += value;
  }
	int getBet() {
		return bet;
	}
	void resetBet() {
		bet = 0;
	}
  void fold() {
    folded = true;
  }
  bool hasFolded() {
    return folded;
  }
	bool isAllIn() {
		return allIn;
	}
	void goOut() {
		out = true;
	}
	bool isOut() {
		return out;
	}
  void newRound() {
    chipsInPot = 0;
    folded = false;
		allIn = false;
  }
	void setRank(int num) {
		rank = num;
	}
	int getRank() {
		return rank;
	}
private:
  Card cards[2];
  int chips;
  int chipsInPot;
	int bet;
  string name;
  bool folded;
	bool allIn;
	bool out;
	int rank;
};

class Poker {
public:
	Poker(Player* somePlayers, int numPlayers, Deck aDeck, int bigBlind) {
		players = somePlayers;
		theDeck = aDeck;
		this->numPlayers = numPlayers;
		//changes by bet
	  playersMatchingBet = 0;
		bet = 0;
		//changes by round
		playersFolded = 0;
		playersAllIn = 0;
		dealer = 0;
		pot = 0;
		//---------------
		playersOut = 0;
		this->bigBlind = bigBlind;
		littleBlind = ceil(bigBlind/2);
		theDeck.shuffle();
    for (int i = 0; i < numPlayers; i++) {
      players[i].setCard1(theDeck.drawCard());
      players[i].setCard2(theDeck.drawCard());
			players[i].setRank(i+1);
    }
		for (int i = 0; i < 5; i++) {
			cardsOnTable[i] = theDeck.drawCard();
		}
	}
	Card* getCardsOnTable() {
		return cardsOnTable;
	}
	void newRound() {
		theDeck.shuffle();
    for (int i=0; i<numPlayers; i++) {
      players[i].setCard1(theDeck.drawCard());
      players[i].setCard2(theDeck.drawCard());
    }
		//the flop
		theDeck.drawCard(); //burn
		for (int i = 0; i < 3; i++) {
			cardsOnTable[i] = theDeck.drawCard(); //return
		}
		//the turn
		theDeck.drawCard(); //burn
		cardsOnTable[3] = theDeck.drawCard(); //return
		//the river
		theDeck.drawCard(); //burn
		cardsOnTable[4] = theDeck.drawCard(); //return
		pot = 0;
		playersFolded = 0;
		//switch dealer
		dealer++;
		if (dealer > numPlayers-1) {
			dealer = 0;
		}
		while (players[dealer].isOut()) {
			dealer++;
			if (dealer > numPlayers-1) {
				dealer = 0;
			}
		}
		for (int i = 0; i < numPlayers; i++) {
			players[i].newRound();
		}
	}
	void printCards(Card* cards, int numCards) {
		for (int i = 0; i < numCards; i++) {
			cout << " ___   ";
		}
		cout << endl;
		for (int i = 0; i < numCards; i++) {
			if (cards[i].getRank() == 0 || cards[i].getRank() > 9) {
				cout << "|" << (char)toupper(cards[i].toString()[0]) << "  |  ";
			} else if (cards[i].getRank() == 9) {
				cout << "|10 |  ";
			} else {
				cout << "|" << cards[i].getRank() + 1 << "  |  ";
			}
		}
		cout << endl;
		for (int i = 0; i < numCards; i++) {
			if (cards[i].getSuit() == 0) {
				cout << "| \u2663 |  ";
			} else if (cards[i].getSuit() == 1) {
				cout << "| \u2666 |  ";
			} else if (cards[i].getSuit() == 2) {
				cout << "| \u2665 |  ";
			} else {
				cout << "| \u2660 |  ";
			}
		}
		cout << endl;
		for (int i = 0; i < numCards; i++) {
			cout << "'–––'  ";
		}
		cout << endl;
	}
	void blind() {
		int turn = dealer;
		cout << "--------------------------------------------------" << endl;
		cout << players[turn].getName() << " is dealer." << endl;
		turn++;
		if (turn > numPlayers-1) {
			turn = 0;
		}
		cout << "--------------------------------------------------" << endl;
		cout << players[turn].getName() << " is little blind." << endl;
		players[turn].addChipsToPot(littleBlind);
		pot += players[turn].getChipsInPot();
		turn++;
		if (turn > numPlayers-1) {
			turn = 0;
		}
		cout << "--------------------------------------------------" << endl;
		cout << players[turn].getName() << " is big blind." << endl;
		players[turn].addChipsToPot(bigBlind);
		pot += players[turn].getChipsInPot();
		bet = bigBlind;
	}
	int bettingRound(int round) { //0 = carry on, 1 = everyone folded, 2 =
																//everyone went all in
		int turn;
		if (round == 1) {
			blind();
			turn = dealer + 3;
		} else {
			turn = dealer + 1;
		}
		if (turn > numPlayers-1) {
			turn = turn % numPlayers;
		}
		while (true) {
      if (players[turn].hasFolded() || players[turn].isAllIn() ||
				players[turn].isOut()) {
        continue;
      }
			cout << "--------------------------------------------------" << endl;
			printPlayerPrompt(turn, round);
      while (true) {
        bool exitLoop = false;
        string move;
        cout << "What would you like to do?" << endl << "-> ";
        cin >> move;
        if (move.compare("check") == 0) {
          if (bet != 0 && players[turn].getBet() != bet) {
            cout << "You must call the bet, raise, or fold." << endl;
						continue;
          } else {
						playersMatchingBet++;
            exitLoop = true;
          }
        } else if (move.compare("bet") == 0) {
          int newBet;
					int toCall = bet-players[turn].getBet();
					if (toCall >= players[turn].getChips()) {
						cout << "You don't have enough chips to bet." << endl;
						continue;
					} else if (toCall + bigBlind > players[turn].getChips()) {
						string response;
						cout << "Go all in?" << endl << "-> ";
						cin >> response;
						if (response.compare("yes") == 0) {
							pot += players[turn].getChips();
							bet += players[turn].getChips() - toCall;
							playersMatchingBet = 0;
							players[turn].addChipsToPot(players[turn].getChips());
							playersAllIn++;
							exitLoop = true;
						} else {
							continue;
						}
					} else {
						while (true) {
							if (bet == 0) {
								cout << "How much would you like to bet?";
							} else {
								cout << "How much would you like to raise?";
							}
							/*cout << " (max: " << players[turn].getChips() - toCall << ")" <<
								endl << "-> ";*/
		          cin >> newBet;
							if (newBet < bigBlind) {
								cout << "The minimum bet is " << bigBlind << endl;
								continue;
							} else if (newBet >= players[turn].getChips() - toCall){
								string response;
	              cout << "You don't have enough chips. Go all in?" << endl << "-> ";
	              cin >> response;
	              if (response.compare("yes") == 0) {
	                pot += players[turn].getChips();
									bet += players[turn].getChips() - toCall;
									playersMatchingBet = 0;
									players[turn].addChipsToPot(players[turn].getChips());
	                playersAllIn++;
	                exitLoop = true;
	              }
							} else {
								pot += newBet;
								bet += newBet;
			          players[turn].addChipsToPot(newBet);
			          playersMatchingBet = 1;
			          exitLoop = true;
							}
							break;
						}
					}
        } else if (move.compare("call") == 0) {
          if (bet == players[turn].getBet()) {
            cout << "There is no bet to call. You must check, bet, or fold." <<
							endl;
						continue;
          } else {
            int toCall = bet-players[turn].getBet();
            if (players[turn].getChips() > toCall) {
              players[turn].addChipsToPot(toCall);
              pot += toCall;
              playersMatchingBet++;
              exitLoop = true;
            } else {
              string response;
              cout << "Go all in?" << endl << "-> ";
              cin >> response;
              if (response.compare("yes") == 0) {
                pot += players[turn].getChips();
								bet += players[turn].getChips() - toCall;
								players[turn].addChipsToPot(players[turn].getChips());
                playersAllIn++;
                exitLoop = true;
              } else {
								continue;
							}
            }
          }
        } else if (move.compare("fold") == 0) {
          players[turn].fold();
          playersFolded++;
					exitLoop = true;
        } else if (move.compare("q") == 0) {
					return 3;
				} else {
          cout << "Invalid input. Options: check, call, bet, fold." << endl;
        }
        if (exitLoop) {
          break;
        }
      }
      if (playersMatchingBet + playersFolded + playersAllIn + playersOut ==
				numPlayers || playersFolded == numPlayers - playersOut - 1) {
        break;
      }
      turn++;
      if (turn >= numPlayers) {
        turn = 0;
      }
    }
	  playersMatchingBet = 0;
		bet = 0;
		for (int i = 0; i < numPlayers; i++) {
			players[i].resetBet();
		}
		if (playersFolded == numPlayers - playersOut - 1) {
			return 1;
		} else if (playersAllIn == numPlayers - playersOut - playersFolded) {
			return 2;
		} else {
			return 0;
		}
	}
	void printPlayerPrompt(int player, int round) {
		cout << players[player].getName() << ", your turn." << endl;
		if (round > 1) {
			cout << "Cards on the table:" << endl;
			printCards(cardsOnTable, round+1);
		}
		cout << "Your hand: " << endl;
		printCards(players[player].getCards(), 2);
		cout << "Your chips: " << players[player].getChips() << endl;
		cout << "Chips in the pot: " << pot << endl;
		cout << "The current bet is " << bet << ". ";
		if (bet != 0 && bet-players[player].getBet() != 0) {
			cout << bet-players[player].getBet() << " to call.";
		}
		cout << endl;
	}
	Player getWinner() {
		//returns winner, gives out money, determines players who are out
		//rank all players. first out's rank = numPlayers
		return players[dealer];
	}
	int getPot() {
		return pot;
	}
	bool isGameOver() {
		return playersOut == numPlayers-1;
	}
private:
	Player* players;
	int numPlayers;
	Deck theDeck;
	Card cardsOnTable[5];
	int pot; //total chips on the table
	int bet; //amount each player must bet during a betting round
	int playersFolded;
	int playersMatchingBet;
	int playersAllIn;
	int playersOut;
	int dealer;
	int bigBlind;
	int littleBlind;
};

int main() {
  int numPlayers;
  int chips;
  int bigBlind;
	cout << "==================================================" << endl;
	cout << "Poker.cpp (c) Darwin, Jerry, & Nick" << endl;
	cout << "==================================================" << endl;
  cout << "Enter number of players: "; //error trap this stuff
  cin >> numPlayers;
  cout << "Enter number of chips: ";
  cin >> chips;
  cout << "Enter big blind: ";
  cin >> bigBlind;

  // create variables
  Player* players = new Player[numPlayers];
  for (int i = 0; i < numPlayers; i++) {
    Player newPlayer = Player();
    newPlayer.setChips(chips);
    string name;
    cout << "Enter a name for player" << i+1 << ": ";
    cin >> name;
    newPlayer.setName(name);
    players[i] = newPlayer;
  }
  Deck theDeck;
	Poker game(players, numPlayers, theDeck, bigBlind);
	int gameStatus = 0;

  // game loop
  while (true) {
		gameStatus = game.bettingRound(1);
		if (gameStatus == 0) {
			cout << "//////////////////////////////////////////////////" << endl;
			cout << "The flop:" << endl;
			game.printCards(game.getCardsOnTable(), 3);
			gameStatus = game.bettingRound(2);
			if (gameStatus == 0) {
				cout << "//////////////////////////////////////////////////" << endl;
				cout << "The turn:" << endl;
				game.printCards(game.getCardsOnTable(), 4);
				gameStatus = game.bettingRound(3);
				if (gameStatus == 0) {
					cout << "//////////////////////////////////////////////////" << endl;
					cout << "The river:" << endl;
					game.printCards(game.getCardsOnTable(), 5);
					gameStatus = game.bettingRound(4);
				}
			}
		}
		if (gameStatus == 3) {
			cout << "==================================================" << endl;
			cout << "Game over." << endl << "Scores:" << endl;
			for (int i = 0; i < numPlayers; i++) {
				if (players[i].getRank() == i + 1) {
					cout << players[i].getName() << ": "<< players[i].getTotalChips() <<
						endl;
				}
			}
			cout << "==================================================" << endl;
			break;
		} else {
			cout << "//////////////////////////////////////////////////" << endl;
			if (gameStatus == 0) {
				//show hands (showdown)
			}
			cout << game.getWinner().getName() << " wins the hand!" << endl;
		}
		if (game.isGameOver()) {
			cout << "==================================================" << endl;
			cout << "Game Over." << endl;
			cout << game.getWinner().getName() << " wins the game!" << endl;
			for (int i = 1; i < numPlayers; i++) {
				if (i == 1) {
					cout << "2nd: ";
				} else if (i == 2) {
					cout << "3rd: ";
				} else {
					cout << i + 1 << "th: ";
				}
				for (int j = 0; j < numPlayers; j++) {
					if (players[j].getRank() == i + 1) {
						cout << players[i].getName() << endl;
						break;
					}
				}
			}
			cout << "==================================================" << endl;
			break;
		}
		game.newRound();
		gameStatus = 0;
		cout << "==================================================" << endl;
  }

  delete [] players;

  return 0;
}
