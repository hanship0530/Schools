package ac.knu.exceptions;

public class FriendExistException extends RuntimeException {
    public FriendExistException(String message){
        super("Sorry!\n"+
                message+" is already exist in database\n"+
                "friend name is not allowed repetition please add another name");
    }
}
