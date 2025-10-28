#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <sstream>
#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_main){
    Player * Reita = Player_factory("Reita", "Simple");
    ASSERT_EQUAL("Reita", Reita->get_name());
    ostringstream oss;
    oss << *Reita;
    ASSERT_EQUAL("Reita", oss.str());
    Reita->add_card(Card(ACE, HEARTS));
    Reita->add_card(Card(KING, HEARTS));
    Reita->add_card(Card(QUEEN, HEARTS));
    Reita->add_card(Card(JACK, HEARTS));
    Reita->add_card(Card(TEN, HEARTS));

    Suit trump;
    bool orderup = Reita->make_trump(Card(NINE, SPADES), false, 1, trump);
    ASSERT_FALSE(orderup);

    orderup = Reita->make_trump(Card(NINE, SPADES), false, 2, trump);
    ASSERT_FALSE(orderup);


    Player * Bob = Player_factory("Bob", "Simple");
    Bob->add_card(Card(JACK, SPADES));
    Bob->add_card(Card(JACK, CLUBS));
    orderup = Bob->make_trump(Card(NINE, SPADES), false, 1, trump);
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(SPADES, trump);

    orderup = Bob->make_trump(Card(NINE, SPADES), false, 2, trump);
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(CLUBS, trump);

    orderup = Bob->make_trump(Card(NINE, SPADES), true, 2, trump);
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(CLUBS, trump);

    orderup = Bob->make_trump(Card(NINE, DIAMONDS), true, 2, trump);
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(HEARTS, trump);

    Player * Alice = Player_factory("Alice", "Simple");
    Alice->add_card(Card(ACE, SPADES));
    Alice->add_card(Card(KING, SPADES));
    Alice->add_card(Card(QUEEN, SPADES));
    Alice->add_card(Card(JACK, HEARTS));
    Alice->add_card(Card(TEN, SPADES));
    Alice->add_and_discard(Card(NINE, HEARTS));
    Card lead = Alice->lead_card(HEARTS);
    ASSERT_EQUAL(Card(ACE, SPADES), lead);


    //All trump cards
    Player * Hiro = Player_factory("Hiro", "Simple");
    Hiro->add_card(Card(ACE, SPADES));
    Hiro->add_card(Card(KING, SPADES));
    Hiro->add_card(Card(QUEEN, SPADES));
    Hiro->add_card(Card(JACK, SPADES));
    Hiro->add_card(Card(TEN, SPADES));


    
    Hiro->add_and_discard(Card(NINE, SPADES));
    Card lead_hiro = Hiro->lead_card(SPADES);
    ASSERT_EQUAL(Card(JACK, SPADES), lead_hiro);

    //Play lowest non suit card
    Card play_hiro = Hiro->play_card(Card(NINE, DIAMONDS), SPADES);
    ASSERT_EQUAL(Card(TEN, SPADES), play_hiro);

    //Play highest of follow suit
    play_hiro = Hiro->play_card(Card(NINE, SPADES), SPADES);
    ASSERT_EQUAL(Card(ACE, SPADES), play_hiro);

    //delete players
    delete Reita;
    delete Bob;
    delete Alice;
    delete Hiro;
    

    

  
}
TEST_MAIN()
