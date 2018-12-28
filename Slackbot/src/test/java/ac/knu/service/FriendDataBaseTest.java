package ac.knu.service;

import ac.knu.exceptions.FriendExistException;
import ac.knu.exceptions.NotFriendExistException;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class FriendDataBaseTest {
    private FriendDataBase friendDataBase;
    @Before
    public void setUp() {
        friendDataBase = new FriendDataBase();
        friendDataBase.addFriendToDataBase("hangeul",11,Friend.Gender.MAN);
        friendDataBase.addFriendToDataBase("hankook",11,Friend.Gender.MAN);
    }
    @Test
    public void addAFriendToFriendDatabase() {
        assertTrue(friendDataBase.getFriendDataBase().containsKey("hangeul"));
    }
    @Test(expected = FriendExistException.class)
    public void makeErrorWhenTryingToAddFriendAlreadyExist() {
        friendDataBase.addFriendToDataBase("hangeul",11,Friend.Gender.MAN);
    }
    @Test(expected = NotFriendExistException.class)
    public void makeErrorWhenTryingToSearchFriendNotExist() {
        friendDataBase.searchAndGetFriendByName("jessie");
    }
    @Test
    public void searchFriendByNameReturnFriendObject() {
        Friend friend = friendDataBase.searchAndGetFriendByName("hangeul");
        assertEquals(friend, new Friend("hangeul", 11, Friend.Gender.MAN));
    }
    @Test(expected = NotFriendExistException.class)
    public void makeErrorWhenTryingToRemoveFriendNotExists() {
        friendDataBase.removeFriendByName("jessie");
    }
    @Test
    public void removeFriendByNameAndReturnFriendName() {
        String friendName = friendDataBase.removeFriendByName("hangeul");
        assertTrue(friendName.equalsIgnoreCase("hangeul"));
    }
    @Test
    public void canSaveFriendMore() {
        assertTrue(friendDataBase.canSaveFriendMore());
    }
    @Test
    public void getNumberOfFriendInDatabase() {
        int friendNumber = friendDataBase.getFriendsNumber();
        assertEquals(2, friendNumber);
    }
}
