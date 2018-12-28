package ac.knu.exceptions;

public class NotFriendExistException extends RuntimeException {
    public NotFriendExistException(String message){
        super("Sorry!\n"+message+" is not exist in DataBase");
    }
}
