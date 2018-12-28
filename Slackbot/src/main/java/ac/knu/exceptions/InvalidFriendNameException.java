package ac.knu.exceptions;

public class InvalidFriendNameException extends RuntimeException {
    public InvalidFriendNameException(String message){
        super("Sorry! Friend Name Error!\n"+message+" is not alphabetic");
    }
}
