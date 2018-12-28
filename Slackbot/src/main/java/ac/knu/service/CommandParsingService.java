package ac.knu.service;

import ac.knu.exceptions.*;
import org.springframework.stereotype.Service;

@Service
public class CommandParsingService {

    private final CommandService commandService = new CommandService();
    private final int numberOfFindCommandOptions = 1;
    private final int numberOfRemoveCommandOptions = 1;
    private final int numberOfAddCommandOptions = 3;


    public String classifyAndExecuteCommand(FriendDataBase friendDataBase, String command) {
        String[] commandLine = command.split("\\s+");
        if (commandLine[1].equalsIgnoreCase("add") && checkIfCommandOptionIsExist(commandLine, numberOfAddCommandOptions)) {
            return commandService.addCommandExecute(friendDataBase, friendNameValidCheck(commandLine[2]),
                    friendAgeValidCheck(commandLine[3]), getGenderFromCommandOption(friendDataBase, commandLine[4]));
        }
        else if(commandLine[1].equalsIgnoreCase("remove") && checkIfCommandOptionIsExist(commandLine, numberOfRemoveCommandOptions)){
            return commandService.removeCommandExecute(friendDataBase, friendNameValidCheck(commandLine[2]));
        }
        else if(commandLine[1].equalsIgnoreCase("find") && checkIfCommandOptionIsExist(commandLine, numberOfFindCommandOptions)){
            return commandService.findCommandExecute(friendDataBase, friendNameValidCheck(commandLine[2]));
        }
        else if(commandLine[1].equalsIgnoreCase("time")){
            return commandService.timeCommandExecute();
        }
        else if(commandLine[1].equalsIgnoreCase("list")){
            return commandService.listCommandExecute(friendDataBase);
        } else {
            throw new InvalidCommandSyntaxException("'"+commandLine[1]+"'");
        }
    }
    public String friendNameValidCheck(String friendName) {
        for(char alphabet : friendName.toCharArray()){
            if(!Character.isAlphabetic(alphabet)||alphabet == ' ') {
                throw new InvalidFriendNameException("'"+friendName+"'");
            }
        }
        return friendName;
    }
    public int friendAgeValidCheck(String age) {
        for(char digit : age.toCharArray()) {
            if(!Character.isDigit(digit) || digit == ' ') {
                throw new InvalidFriendAgeSyntaxException("'"+age+"'");
            }
        }
        return Integer.parseInt(age);
    }
    public Friend.Gender getGenderFromCommandOption(FriendDataBase friendDataBase, String genderSyntax) {
        if(friendDataBase.getManGenderDictionary().contains(genderSyntax.toLowerCase())) {
            return Friend.Gender.MAN;
        } else if(friendDataBase.getFemaleGenderDictionary().contains(genderSyntax.toLowerCase())) {
            return Friend.Gender.FEMALE;
        } else {
            throw new InvalidGenderSyntaxException("'"+genderSyntax+"'");
        }
    }

    public boolean checkIfCommandOptionIsExist(String[] commandLines, int numberOfOptions)  {
        if(commandLines.length == numberOfOptions+2){
            return true;
        } else {
            throw new NoCommandOptionException();
        }
    }
}
