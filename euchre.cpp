#include <iostream>
#include "Card.hpp"
#include "Player.hpp"
#include "Pack.hpp"
#include <fstream>
using namespace std;

class Game
{
public:
    Game(istream &pack_input, int points_to_win,
         const string &shuffle_str, const vector<Player *> &players)
    {
        this->pack = Pack(pack_input);
        this->points_to_win = points_to_win;
        for (int i = 0; i < players.size(); i++)
        {
            this->players.push_back(players[i]);
        }
        if (shuffle_str == "shuffle")
        {
            this->shuffle = true;
        }
        else
        {
            this->shuffle = false;
        }
    }

    void play()
    {
        
        while (team1_points < points_to_win 
            && team2_points < points_to_win)
        {

            
            pack_shuffle();
            int dealer = hand % 4;
            deal_cards(dealer);
            make_trump();
            int eldest = (dealer + 1) % 4;
            while (trick_round <= 5)
            {
               play_trick(eldest);
            }
            scoring(team1_trump_made);
            reset_trick();
           
        }
        announce_winner();
        delete_players();
    }

private:
    vector<Player *> players;

    Pack pack;
    int points_to_win;
    int team1_points = 0;
    int team2_points = 0;
    bool shuffle;
    Card upcard;
    Suit order_up_suit;
    bool trump_made = false;
    bool team1_trump_made = false;
    int trump_round;
    int team1_tricks = 0;
    int team2_tricks = 0;
    Player *player0;
    Player *player1;
    Player *player2;
    Player *player3;
    int trick_round = 1;
    int hand = 0;

    void play_trick(int &eldest){
        int curr_highest = eldest;
            Card led_card = players[eldest]
            ->lead_card(order_up_suit);
            Card temp_led_card = led_card;
            cout << led_card.get_rank() << " of "
                    << led_card.get_suit() << " led by " 
                    << players[eldest]->get_name()
                    << endl;

            Card card1 = players[(eldest + 1) % 4]
            ->play_card(temp_led_card, order_up_suit);
            cout << card1.get_rank() << " of " 
            << card1.get_suit() 
            << " played by " << players[(eldest + 1) % 4]
            ->get_name() << endl;
            if (Card_less(led_card, card1, 
                led_card, order_up_suit))
            {
                led_card = card1;
                curr_highest = (eldest + 1) % 4;
            }

            Card card2 = players[(eldest + 2) % 4]
            ->play_card(temp_led_card, order_up_suit);
            cout << card2.get_rank() << " of " 
            << card2.get_suit() << " played by "
                    << players[(eldest + 2) % 4]
                    ->get_name() << endl;
            if (Card_less(led_card, card2, 
                led_card, order_up_suit))
            {
                led_card = card2;
                curr_highest = (eldest + 2) % 4;
            }

            Card card3 = players[(eldest + 3) % 4]
            ->play_card
            (temp_led_card, order_up_suit);
            cout << card3.get_rank() << " of " 
            << card3.get_suit() << " played by "
                    << players[(eldest + 3) % 4]
                    ->get_name()
                    << endl;
            if (Card_less(led_card, card3, led_card,
                    order_up_suit))
            {
                led_card = card3;
                curr_highest = (eldest + 3) % 4;
            }
            cout << players[curr_highest]
            ->get_name()
                << " takes the trick" << endl;
            if (players[curr_highest] == players[1] 
                || players[curr_highest] == players[3])
            {
                team1_tricks++;
            }
            else
            {
                team2_tricks++;
            }
            eldest = curr_highest;
            cout << endl;
            trick_round++;
    }
    

    void pack_shuffle(){
        if (shuffle)
        {
            pack.shuffle();
        }
        pack.reset();
    }
    void announce_winner()
    {
         if (team2_points >= points_to_win)
        {
            cout << players[0]->get_name() 
            << " and "
                 << players[2]->get_name() 
                 << " win!" << endl;
        }
        else if (team1_points >= points_to_win)
        {
            cout << players[1]->get_name()
             << " and "
                 << players[3]->get_name() 
                 << " win!" << endl;
        }
    }

    void reset_trick()
    {
        hand++;
        trick_round = 1;
        team1_tricks = 0;
        team2_tricks = 0;
        trump_made = false;
        team1_trump_made = false;

    }

    void deal(Player *p, int amount)
    {
        for (int i = 0; i < amount; i++)
        {
            p->add_card(pack.deal_one());
        }
    }

    void deal_cards(int dealer)
    {
        int first = (dealer + 1) % 4;
        player0 = players[dealer];
        player1 = players[first];
        player2 = players[(first + 1) % 4];
        player3 = players[(first + 2) % 4];
        deal(player1, 3);
        deal(player2, 2);
        deal(player3, 3);
        deal(player0, 2);
        deal(player1, 2);
        deal(player2, 3);
        deal(player3, 2);
        deal(player0, 3);
        upcard = pack.deal_one();
        cout << "Hand " << hand << endl;
        cout << player0->get_name() << " deals" 
        << endl;
        cout << upcard.get_rank() << " of " 
        << upcard.get_suit() << " turned up" 
        << endl;

        ;
    }
    void delete_players()
    {
        for (size_t i = 0; i < players.size(); i++)
        {
            delete players[i];
        }
    }
    void scoring(bool team1_made_trump)
    {
        if (team1_tricks > team2_tricks)
        {
            cout << players[1]->get_name() 
            << " and "
                 << players[3]->get_name()
                  << " win the hand" << endl;
            if (team1_made_trump)
            {
                if (team1_tricks == 5)
                {
                    team1_points += 2;
                    cout << "march!" << endl;
                }
                else
                {
                    team1_points += 1;
                }
            }
            else
            {
                team1_points += 2;
                cout << "euchred!" << endl;
            }
        }
        else
        {
            cout << players[0]->get_name() << " and "
                 << players[2]->get_name() 
                 << " win the hand" << endl;
            if (!team1_made_trump)
            {
                if (team2_tricks == 5)
                {
                    team2_points += 2;
                    cout << "march!" << endl;
                }
                else
                {
                    team2_points += 1;
                }
            }
            else
            {

                team2_points += 2;
                cout << "euchred!" << endl;
            }
        }
        cout << players[0]->get_name() << " and "
         << players[2]->get_name()
             << " have " << team2_points << " points"
              << endl;
        cout << players[1]->get_name() << " and " 
        << players[3]->get_name()
             << " have " << team1_points << " points"
              << endl;

        cout << endl;
    }

   void make_trump()
{
    trump_round = 1;

    while (trump_round <= 2 && !trump_made) 
    {
        std::vector<Player*> order = {player1, player2, 
            player3, player0};
        for (Player* p : order) 
        {
            bool dealer = (p == player0);
            bool made = p->make_trump(upcard, dealer,
                 trump_round, order_up_suit);

            if (!made)
            {
                cout << p->get_name() << " passes" << endl;
                continue; 
            }

            // if we reach here, they made trump
            trump_made = true;
            cout << p->get_name() << " orders up " 
            << order_up_suit << endl;

            team1_trump_made = (p == players[1] 
                || p == players[3]);

            if (made && trump_round == 1)
                player0->add_and_discard(upcard);

            break; 
        }

        trump_round++;
    }

    cout << endl;
}

   
};

int main(int argc, char *argv[])
{
    cout.setf(std::ios::unitbuf); // disables buffering, flushes every cout

    if (argc != 12 || stoi(argv[3]) < 1 || stoi(argv[3]) > 100 ||
        (string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle") ||
        (string(argv[5]) != "Simple" && string(argv[5]) != "Human") ||
        (string(argv[7]) != "Simple" && string(argv[7]) != "Human") ||
        (string(argv[9]) != "Simple" && string(argv[9]) != "Human") ||
        (string(argv[11]) != "Simple" && string(argv[11]) != "Human"))
    {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    string pack_filename = argv[1];
    ifstream ifs(pack_filename);
    if (!ifs)
    {
        cout << "Error opening " << pack_filename << endl;
        return 1;
    }

    string shuffle_str = argv[2];
    int points_to_win = stoi(argv[3]);
    string name1 = argv[4];
    string type1 = argv[5];
    string name2 = argv[6];
    string type2 = argv[7];
    string name3 = argv[8];
    string type3 = argv[9];
    string name4 = argv[10];
    string type4 = argv[11];
    vector<Player *> players;
    players.push_back(Player_factory(name1, type1));
    players.push_back(Player_factory(name2, type2));
    players.push_back(Player_factory(name3, type3));
    players.push_back(Player_factory(name4, type4));
    Game game(ifs, points_to_win, shuffle_str, players);

    for (int i = 0; i < argc; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;

    game.play();
}
