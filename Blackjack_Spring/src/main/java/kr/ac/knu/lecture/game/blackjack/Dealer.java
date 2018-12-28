package kr.ac.knu.lecture.game.blackjack;

import lombok.Getter;

/**
 * Created by rokim on 2018. 5. 26..
 */
public class Dealer {
    @Getter
    private Hand hand;

    public Dealer(Hand hand) {
        this.hand = hand;
    }

    public void reset() {
        hand.reset();
    }

    public void deal() {
        hand.drawCard();
    }

    public void play() {
        while (hand.getCardSum() < 17) {
            hand.drawCard();
        }
    }
}
