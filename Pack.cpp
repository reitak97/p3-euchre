#include "Pack.hpp"
#include "Card.hpp"
#include <cassert> 

Pack::Pack(){
    next = 0;
    int index = 0;
    for(int s = SPADES; s <= DIAMONDS; ++s) {
      for(int r = NINE; r <= ACE; ++r) {
        cards[index] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
        index++;
    }

}
}
Pack::Pack(std::istream& pack_input){
    next = 0;
    for(int i = 0; i < PACK_SIZE; i++){
        pack_input >> cards[i];
    }
  
}

Card Pack::deal_one(){
    assert(!empty());

    return cards[next++];
}

void Pack::reset(){
    next = 0;

}

void Pack::shuffle(){
    std::array<Card, PACK_SIZE> shuffled;
    for(int shuffle_count = 0; shuffle_count < 7; shuffle_count++){
        int half = PACK_SIZE / 2;
        for(int i = 0; i < half; i++){
            shuffled[2*i + 1] = cards[i];
            shuffled[2*i] = cards[i + half];
            
        }
        cards = shuffled;
    }
    
    
}
bool Pack::empty() const{
    return (next >= PACK_SIZE);
}