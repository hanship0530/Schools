package ac.knu.exceptions;

public class InvalidGenderSyntaxException extends RuntimeException {
    public InvalidGenderSyntaxException(String message){
        super("Sorry Friend Gender Syntax Error!\n"+
                message+" is not valid gender option\nyou can use \nmale, man, boy, female, girl, woman");
    }
}
