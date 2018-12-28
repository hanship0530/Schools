package ac.knu.service;

import ac.knu.exceptions.FriendDataBaseAlreadyFullException;
import ac.knu.exceptions.FriendDataBaseEmptyException;
import org.junit.Before;
import org.junit.Test;

import java.util.Date;

import static org.junit.Assert.assertTrue;

public class CommandServiceTest {
    private FriendDataBase friendDataBase;
    private CommandService commandService;
    @Before
    public void setup() {
        friendDataBase = new FriendDataBase();
        friendDataBase.addFriendToDataBase("hangeul",11,Friend.Gender.MAN);
        friendDataBase.addFriendToDataBase("hankook",21,Friend.Gender.MAN);
        friendDataBase.addFriendToDataBase("kim",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("bae",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("jung",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("hun",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("choi",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("david",11,Friend.Gender.FEMALE);
        friendDataBase.addFriendToDataBase("kang",11,Friend.Gender.FEMALE);
        commandService = new CommandService();
    }
    @Test
    public void executeAddCommandReturnMessageWithFriendName() {
        String result = commandService.addCommandExecute(friendDataBase,"jessica",21,Friend.Gender.FEMALE);
        assertTrue(result.equalsIgnoreCase("jessica is added successfully!"));
    }
    @Test(expected = FriendDataBaseAlreadyFullException.class)
    public void makeErrorAfterMaximumLimitOfFriendWhenAdding(){
        assertTrue(commandService.addCommandExecute(
                friendDataBase,"james",11,Friend.Gender.MAN).equalsIgnoreCase("james is added successfully!"));
        commandService.addCommandExecute(
                friendDataBase,"mina",11,Friend.Gender.FEMALE);
    }
    @Test
    public void executeRemoveCommandReturnMessageWithFriendName() {
        String result = commandService.removeCommandExecute(friendDataBase,"hangeul");
        assertTrue(result.equalsIgnoreCase("hangeul is removed successfully!"));
    }
    @Test
    public void executeFindCommandReturnFriendInformation() {
        String result = commandService.findCommandExecute(friendDataBase,"hangeul");
        assertTrue(result.equalsIgnoreCase("hangeul, 11, MAN"));
    }
    @Test
    public void executeTimeCommandReturnCurrentLocalTime() {
        assertTrue(commandService.timeCommandExecute().equalsIgnoreCase("Current Time: "+new Date().toString()));
    }
    @Test(expected = FriendDataBaseEmptyException.class)
    public void makeErrorDatabaseEmptyWhenTryToListCommandWhenNoData() {
        friendDataBase.getFriendDataBase().clear();
        commandService.listCommandExecute(friendDataBase);
    }
    @Test
    public void executeListCommandAndReturnFriendInformationByRows() {
        friendDataBase.getFriendDataBase().clear();
        commandService.addCommandExecute(friendDataBase, "jessica",21,Friend.Gender.FEMALE);
        commandService.addCommandExecute(friendDataBase, "joey",22,Friend.Gender.MAN);
        assertTrue(commandService.listCommandExecute(
                friendDataBase).equalsIgnoreCase("jessica, 21, FEMALE\njoey, 22, MAN\n"));
    }
}
