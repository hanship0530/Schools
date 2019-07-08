package kr.ac.knu.lecture.game.blackjack;

import lombok.Getter;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Created by rokim on 2018. 5. 26..
 */
public class Hand {
    private Deck deck;
    @Getter
    private List<Card> cardList = new ArrayList<>();

    public Hand(Deck deck) {
        this.deck = deck;
    }

    public Card drawCard() {
        Card card = deck.drawCard();
        cardList.add(card);
        return card;
    }

    public int getCardSum() {
        AtomicInteger numberOfAce = new AtomicInteger();
        int cardSum = cardList.stream().mapToInt(card -> {
            int rank = card.getRank();
            if (rank > 10) {
                return 10;
            }
            if (rank == 1) {
                numberOfAce.addAndGet(1);
                return 11;
            }
            return rank;
        }).sum();

        return setAceValue(cardSum, numberOfAce);
    }

    public int setAceValue(int cardSum, AtomicInteger numberOfAce) {
        if(numberOfAce.get() == 0 || cardSum <= 21) {
            return cardSum;
        }
        return cardSum - numberOfAce.get()*11 + numberOfAce.get()*1;

    }
    public void reset() {
        cardList.clear();
    }
}
