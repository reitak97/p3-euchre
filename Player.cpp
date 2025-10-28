#include "Player.hpp"
#include "Card.hpp"
#include <cassert>
#include <vector>
#include <algorithm>

using namespace std;
class Simple : public Player{
    public:
        Simple(const string &name) : name(name) {}

        const std::string & get_name() const override{
            return name;
        }

        void add_card (const Card &c) override {
            assert(hand.size() < MAX_HAND_SIZE);
            hand.push_back(c);
        }
        void print_hand(){
            for(size_t i = 0; i < hand.size(); i++){
                cout << "Human player " << name << "'s hand: "
                << "[" << i << "] " << hand[i] << endl;
            }
        }
        

        bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
            assert(round == 1 || round == 2);
            if(round == 1){
                int up_count = 0;
                for(int i = 0; i < hand.size(); i++){
                    if(hand[i].get_suit() == upcard.get_suit()
                     && hand[i].is_face_or_ace()){
                        up_count++;
                    }
                    else if(hand[i].is_left_bower(upcard.get_suit())){
                        up_count++;
                    } 
                }
                if(up_count >=2){
                    order_up_suit = upcard.get_suit();
                    return true;
                }
            }

            else if(round ==2){
                
                int up_count = 0;
                for(int i = 0; i < hand.size(); i++){
                    if(hand[i].get_suit() == Suit_next(upcard.get_suit())
                     && hand[i].is_face_or_ace()){
                        up_count++;
                    }
                    else if(hand[i].is_left_bower(Suit_next(upcard.get_suit()))){
                        up_count++;
                    }
                }
                if(up_count >=1 and !is_dealer){
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
                if(is_dealer){
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
                
            }
            return false;
        }


        void add_and_discard(const Card &upcard) override {
            assert(hand.size() >0);
            hand.push_back(upcard);
            Card lowest = hand[0];
            for(int i = 1; i < hand.size(); i++){
                if(Card_less(hand[i], lowest, upcard.get_suit())){
                    lowest = hand[i];
                }
            }
            for(int i = 0; i < hand.size(); i++){
                if(hand[i] == lowest){
                    hand.erase(hand.begin() + i);
                    break;
                }
            }
        }
        Card lead_card(Suit trump) override {
            assert(hand.size() >0);
            Card highest = hand[0];
            int trump_count = 0;
            for(int i =0; i <hand.size(); i++){
                if(hand[i].is_trump(trump)){
                    trump_count++;
                }
                
            }
            if(trump_count == hand.size()){
                for(int i =0; i <hand.size(); i++){
                    if(Card_less(highest, hand[i], trump)){
                        highest = hand[i];
                    }
                }
                
            }
            else{
                vector<Card> non_trump;
                for(int i =0; i < hand.size(); i++){
                    if(!hand[i].is_trump(trump)){
                        non_trump.push_back(hand[i]);
                    }
                }
                highest = non_trump[0];
                for(int i =1; i < non_trump.size(); i++){
                    if(Card_less(highest, non_trump[i], trump)){
                        highest = non_trump[i];
                    }
                }
            }
            //Remove highest from hand
            for(int i = 0; i < hand.size(); i++){
                if(hand[i] == highest){
                    hand.erase(hand.begin() + i);
                    break;
                }
            }
            return highest;
            
        }
        Card play_card(const Card &led_card, Suit trump) override {
            Suit led_suit;
            if(led_card.is_left_bower(trump)){
                led_suit = trump;
            }
            else{
                led_suit = led_card.get_suit();
            }
            assert(hand.size() >0);
            Card highest = hand[0];
       
            vector<Card> follow_suit;
            for(int i =0; i < hand.size(); i++){
                if(hand[i].get_suit(trump) == led_suit){ 
                    follow_suit.push_back(hand[i]);
                }
            }
            if(follow_suit.size() > 0){
                highest = follow_suit[0];
                for(int i = 1; i < follow_suit.size(); i++){
                    if(Card_less(highest, follow_suit[i], led_card, trump)){
                        highest = follow_suit[i];
                    }
                }
                for(int i = 0; i < hand.size(); i++){
                if(hand[i] == highest){
                    hand.erase(hand.begin() + i);
                    break;
                }
            }
                return highest;
                
            }
            //play lowest non trump
            else{
                for(int i = 1; i < hand.size(); i++){
                    if(Card_less(hand[i], highest, trump)){
                        highest = hand[i];
                    }
                }
                for(int i = 0; i < hand.size(); i++){
                    if(hand[i] == highest){
                        hand.erase(hand.begin() + i);
                        break;
                    }
                }
                return highest;
                
            }
            //save highest card to play
            //Remove highest from hand
            
        }
      
       

    private:
        string name;
        vector<Card> hand;
        
       
};


class Human : public Player{
    public:
        Human(const string &name) : name(name) {}

        void add_card (const Card &c) override {
            assert(hand.size() < MAX_HAND_SIZE);
            hand.push_back(c);
            sort(hand.begin(), hand.end());
        }


        bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
            assert(round == 1 || round == 2);
            print_hand();
            cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
            string decision;
            cin >> decision;
            if(decision != "pass"){
                order_up_suit = string_to_suit(decision);
                return true;
            }
            else{
                return false;
            }

        }


        void add_and_discard(const Card &upcard) override {
            assert(hand.size()> 0);

            sort(hand.begin(), hand.end());
            print_hand();
            hand.push_back(upcard);
            cout << "Discard upcard: [-1]\n";
            cout << "Human player " << name << ", please select a card to discard:\n";
            int discard_index;
            cin >> discard_index;
            if(discard_index == -1){
                hand.pop_back();
            }
            else{
                hand.erase(hand.begin() + discard_index);
            }

        }
        Card lead_card(Suit trump) override {
            assert(hand.size() >0);
            sort(hand.begin(), hand.end());
            print_hand();
            cout << "Human player " << name << ", please select a card:\n";
            int lead_index;
            cin >> lead_index;
            Card lead_card = hand[lead_index];
            hand.erase(hand.begin() + lead_index);
            return lead_card;
        }
        Card play_card(const Card &led_card, Suit trump) override {
            assert(hand.size() > 0);
            sort(hand.begin(), hand.end());
            print_hand();
            cout << "Human player " << name << ", please select a card:\n";
            int play_index;
            cin >> play_index;
            Card play_card = hand[play_index];
            hand.erase(hand.begin() + play_index);
            return play_card;
        }
        const string & get_name() const override {
            return name;
        }

    private:
        string name;
        vector<Card> hand;

        void print_hand() const{
            for(size_t i = 0; i < hand.size(); i++){
                cout << "Human player " << name << "'s hand: "
                << "[" << i << "] " << hand[i] << endl;
            }
        }
};

Player * Player_factory(const std::string &name, const std::string &strategy){
    if (strategy == "Simple") {
        return new Simple(name);
    }
    else if (strategy == "Human") {
        return new Human(name);
    }
    else{
        assert(false);
        return nullptr;
    }

}
std::ostream & operator<<(std::ostream &os, const Player &p){
    os << p.get_name();
    return os;
}

