package ac.knu.exceptions;

public class FriendDataBaseAlreadyFullException extends RuntimeException {
    public FriendDataBaseAlreadyFullException(){
        super("Sorry! Friend Database is already full\n"+
                "you can add up to 10 friends to database\n"+
                "To insert please remove some friends");
    }
}
