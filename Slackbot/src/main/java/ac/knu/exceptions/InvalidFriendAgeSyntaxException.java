package ac.knu.exceptions;

public class InvalidFriendAgeSyntaxException extends RuntimeException {
    public InvalidFriendAgeSyntaxException(String message){
        super("Sorry! Friend Age Syntax Error!\n"+message+" is not digit");
    }
}
