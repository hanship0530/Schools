package kr.ac.knu.lecture.game.blackjack;

import java.util.Map;

/**
 * Created by rokim on 2018. 5. 27..
 */
public class Evaluator {
    private Map<String, Player> playerMap;
    private Dealer dealer;
    public Evaluator(Map<String, Player> playerMap, Dealer dealer) {
        this.playerMap = playerMap;
        this.dealer = dealer;
    }

    public boolean evaluate() {
        if (playerMap.values().stream().anyMatch(player -> player.isPlaying())) {
            return false;
        }

        int dealerResult = dealer.getHand().getCardSum();

        if (dealerResult > 21) {
            playerMap.forEach((s, player) -> {
                int playerResult = player.getHand().getCardSum();
                if (playerResult==21 && player.getHand().getCardList().size()==2) {
                    player.win(1.5);
                } else if (playerResult<=21) {
                    player.win(2);
                }
                else {
                    player.lost();
                }
            });
        } else {
            playerMap.forEach((s, player) -> {
                int playerResult = player.getHand().getCardSum();
                if (playerResult > 21) {
                    player.lost();
                } else if (playerResult == 21 && playerResult > dealerResult && player.getHand().getCardList().size() == 2) {
                    player.win(1.5);
                } else if (playerResult > dealerResult) {
                    player.win(2);
                } else if (playerResult == dealerResult) {
                    player.tie();
                } else {
                    player.lost();
                }
            });
        }
        return true;
    }


}
