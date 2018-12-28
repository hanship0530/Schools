package ac.knu.exceptions;

public class FriendDataBaseEmptyException extends RuntimeException {
    public FriendDataBaseEmptyException(){
        super("Sorry! Nobody exist in Database, please insert friend!");
    }
}
