package ac.knu.exceptions;

public class InvalidCommandSyntaxException extends RuntimeException {
    public InvalidCommandSyntaxException(String message){
        super( "Sorry! Command Syntax Error!\n"+
                message+" is not valid command!\n"+
                "check command list and see guide\n" +
                "add friendname age gender\n"+
                "find friendname\n"+
                "remove friendname\n"+
                "list\n"+
                "time");
    }
}
