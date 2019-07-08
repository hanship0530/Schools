package kr.ac.knu.lecture.game.blackjack;

import kr.ac.knu.lecture.exception.NotEnoughBalanceException;
import lombok.Data;
import lombok.Getter;

/**
 * Created by rokim on 2018. 5. 26..
 */
@Data
public class Player {
    @Getter
    private long balance;
    @Getter
    private long currentBet;
    @Getter
    private boolean isPlaying;
    @Getter
    private Hand hand;
    @Getter
    private int result;
    public Player(long seedMoney, Hand hand) {
        this.balance = seedMoney;
        this.hand = hand;

        isPlaying = false;
    }

    public void reset() {
        hand.reset();
        isPlaying = false;
    }

    public void placeBet(long bet, boolean isDoubleDown) {
        if (balance < bet) {
            //throw new NotEnoughBalanceException();
            bet = balance;
        }
        balance -= bet;
        isPlaying = true;
        if (!isDoubleDown) {
            currentBet = bet;
            return;
        }

        currentBet += bet;

    }

    public void deal() {
        hand.drawCard();
        hand.drawCard();
    }

    public void win(double rate) {
        balance += currentBet * rate;
        result = 0;
    }

    public void tie() {
        balance += currentBet;
        result = 2;
    }

    public void lost() {
        result = 1;
    }

    public Card hitCard() {
        return hand.drawCard();
    }

    public void stand() {
        this.isPlaying = false;
    }

}
