#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>


using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_card_basic){
    Card C;
    ASSERT_EQUAL(TWO, C.get_rank());
    ASSERT_EQUAL(SPADES, C.get_suit());
    ASSERT_FALSE(C.is_face_or_ace());
    ASSERT_FALSE(C.is_right_bower(SPADES));
    ASSERT_FALSE(C.is_left_bower(SPADES));
    ASSERT_TRUE(C.is_trump(SPADES));
    ostringstream oss;
    string correct = "Two of Spades";
    oss << C;
    ASSERT_EQUAL(oss.str(), correct);

    Card C1(JACK, HEARTS);
    ASSERT_EQUAL(JACK, C1.get_rank());
    ASSERT_EQUAL(HEARTS, C1.get_suit());   
    ASSERT_TRUE(C1.is_face_or_ace());
    ASSERT_TRUE(C1.is_right_bower(HEARTS)); 
    ASSERT_FALSE(C1.is_right_bower(SPADES)); 
    ASSERT_TRUE(C1.is_left_bower(DIAMONDS));
    ASSERT_FALSE(C1.is_left_bower(HEARTS));
    ASSERT_TRUE(C1.is_trump(HEARTS));
    ASSERT_TRUE(C1.is_trump(DIAMONDS)); 

    string input = "Jack of Clubs";
    istringstream iss(input);
    Card C2;
    iss >> C2;
    ASSERT_EQUAL(JACK, C2.get_rank());
    ASSERT_EQUAL(CLUBS, C2.get_suit()); 
    ASSERT_EQUAL(SPADES, C2.get_suit(SPADES));
    
    ASSERT_TRUE(C < C1);
    ASSERT_TRUE(C1 < C2);
    ASSERT_TRUE(C1<= C2);
    ASSERT_TRUE(C1 > C);
    ASSERT_TRUE(C2 >= C1);
    ASSERT_TRUE(C != C1);
    ASSERT_TRUE(C == C);
    ASSERT_FALSE(C != C);

    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));


    Card C3(JACK, SPADES);
    ASSERT_TRUE(Card_less(C1,C ,SPADES));
    ASSERT_TRUE(Card_less(C, C3, SPADES));
    ASSERT_TRUE(Card_less(C1, C2, SPADES));

    Card led_card(NINE,DIAMONDS);
    ASSERT_TRUE(Card_less(C1, C, led_card, SPADES));
    ASSERT_TRUE(Card_less(C, C3, led_card, SPADES));
    ASSERT_TRUE(Card_less(C1, C2, led_card, SPADES));

    ASSERT_FALSE(Card_less(led_card,C1, led_card, SPADES));

    ASSERT_TRUE(Card_less(led_card,C3,led_card,SPADES));

    Card C4(ACE, HEARTS);
    ASSERT_TRUE(C4.is_face_or_ace());
    ASSERT_TRUE(Card_less(C4,led_card,led_card,SPADES));


    Card led_card2(JACK,SPADES);
    Card C5(ACE, SPADES);
    ASSERT_TRUE(Card_less(C5, led_card2, led_card2, SPADES));
}



TEST_MAIN()
