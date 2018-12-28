package ac.knu.exceptions;

public class NoCommandOptionException extends RuntimeException {
    public NoCommandOptionException() {
        super("Sorry! No CommandOption Exist\n"+
                "Follow below rules\n"+
                "add friendName friendAge friendGender\n"+
                "find friendName\n"+
                "remove friendName");
    }
}
