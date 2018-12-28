package ac.knu.service;

import ac.knu.exceptions.InvalidFriendAgeSyntaxException;
import ac.knu.exceptions.InvalidFriendNameException;
import ac.knu.exceptions.InvalidGenderSyntaxException;
import ac.knu.exceptions.NoCommandOptionException;
import org.junit.Before;
import org.junit.Test;

import java.util.Date;

import static org.junit.Assert.*;

public class CommandParsingServiceTest {
    private CommandParsingService commandParsingService;
    private FriendDataBase friendDataBase;
    @Before
    public void setup() {
        friendDataBase = new FriendDataBase();
        friendDataBase.addFriendToDataBase("hangeul",11,Friend.Gender.MAN);
        friendDataBase.addFriendToDataBase("hankook",21,Friend.Gender.MAN);
        commandParsingService = new CommandParsingService();
    }
    @Test
    public void addCommandExecuteAndReturnMessageWithFriendName() {
        assertTrue(commandParsingService.classifyAndExecuteCommand(
                friendDataBase, "slackUserId(OnlyExistOnTest) add jessica 15 girl").equalsIgnoreCase("jessica is added successfully!"));
    }
    @Test
    public void timeCommandExecuteAndReturnLocalTime() {
        assertTrue(commandParsingService.classifyAndExecuteCommand(
                friendDataBase, "slackUserId(OnlyExistOnTest) time").equalsIgnoreCase("Current Time: " + new Date().toString()));
    }
    @Test
    public void removeCommandExecuteAndReturnMessageWithFriendName() {
        assertTrue(commandParsingService.classifyAndExecuteCommand(
                friendDataBase, "slackUserId(OnlyExistOnTest) remove hangeul").equalsIgnoreCase("hangeul is removed successfully!"));
    }
    @Test
    public void findCommandExecuteAndReturnFriendInformation() {
        assertTrue(commandParsingService.classifyAndExecuteCommand(
                friendDataBase, "slackUserId(OnlyExistOnTest) find hangeul").equalsIgnoreCase("hangeul, 11, MAN"));
    }
    @Test
    public void listCommandExecuteAndReturnFriendsListOfDatabase() {
        assertTrue(commandParsingService.classifyAndExecuteCommand(
                friendDataBase,"slackUserId(OnlyExistOnTest) list").equalsIgnoreCase("hangeul, 11, MAN\nhankook, 21, MAN\n"));
    }
    @Test(expected = InvalidFriendNameException.class)
    public void makeErrorWhenFriendNameOptionIsInvalid() {
       assertFalse(commandParsingService.friendNameValidCheck("hang2121").equalsIgnoreCase("hang2121"));
    }
    @Test
    public void returnStringFriendNameIfNameIsValid() {
        assertTrue(commandParsingService.friendNameValidCheck("hangeul").equalsIgnoreCase("hangeul"));
    }
    @Test(expected = InvalidFriendAgeSyntaxException.class)
    public void makeErrorWhenFriendAgeIsInvalid() {
        assertFalse(commandParsingService.friendAgeValidCheck("aa")==22);
    }
    @Test
    public void returnIntFriendAgeIfAgeIsValid() {
        assertEquals(11, commandParsingService.friendAgeValidCheck("11"));
    }
    @Test(expected = InvalidGenderSyntaxException.class)
    public void makeErrorWhenFriendGenderIsInvalid() {
        assertNotSame(commandParsingService.getGenderFromCommandOption(friendDataBase, "stringMan"), Friend.Gender.MAN);
        assertNotSame(commandParsingService.getGenderFromCommandOption(friendDataBase,"prettyGirl"), Friend.Gender.FEMALE);
    }
    @Test
    public void returnGenderEnumIfFriendGenderIsValidWord() {
        assertEquals(commandParsingService.getGenderFromCommandOption(friendDataBase, "male"), Friend.Gender.MAN);
        assertEquals(commandParsingService.getGenderFromCommandOption(friendDataBase, "Woman"), Friend.Gender.FEMALE);
    }
    @Test(expected = NoCommandOptionException.class)
    public void makeErrorWhenNoOptionToCommand() {
        commandParsingService.classifyAndExecuteCommand(friendDataBase, "slackUserId(OnlyExistOnTest) add");
        commandParsingService.classifyAndExecuteCommand(friendDataBase, "slackUserId(OnlyExistOnTest) find");
        commandParsingService.classifyAndExecuteCommand(friendDataBase, "slackUserId(OnlyExistOnTest) remove");
    }
    @Test(expected = NoCommandOptionException.class)
    public void makeErrorWhenNoOptionToCommandInCheckOptionIsExistMethod() {
        commandParsingService.checkIfCommandOptionIsExist("slackUserId(OnlyExistOnTest) add   ".split("\\s+"), 3);
        commandParsingService.checkIfCommandOptionIsExist("slackUserId(OnlyExistOnTest) find  ".split("\\s+"), 1);
        commandParsingService.checkIfCommandOptionIsExist("slackUserId(OnlyExistOnTest) remove  ".split("\\s+"), 1);
    }
}
