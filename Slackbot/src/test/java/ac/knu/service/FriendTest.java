package ac.knu.service;

import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;

public class FriendTest {
    private Friend friend;
    @Before
    public void setUp() {
        friend = new Friend("hangeul",11,Friend.Gender.FEMALE);
    }
    @Test
    public void getFriendName() {
        assertTrue(friend.getName().equalsIgnoreCase("hangeul"));
    }
    @Test
    public void getFriendAge() {
        assertEquals(11, friend.getAge());
    }
    @Test
    public void getFriendGender() {
        assertSame(friend.getGender(), Friend.Gender.FEMALE);
    }
}
