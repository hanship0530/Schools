package ac.knu.service;

import ac.knu.exceptions.FriendExistException;
import ac.knu.exceptions.NotFriendExistException;
import lombok.Data;

import java.util.ArrayList;
import java.util.HashMap;

@Data
public class FriendDataBase {
    private HashMap<String, Friend> friendDataBase;
    private ArrayList<String> manGenderDictionary;
    private ArrayList<String> femaleGenderDictionary;
    private final int maximumLimitOfFriends = 10;
    public FriendDataBase() {
        friendDataBase = new HashMap<>();
        manGenderDictionary = new ArrayList<>();
        femaleGenderDictionary = new ArrayList<>();
        manGenderDictionary.add("male");
        manGenderDictionary.add("man");
        manGenderDictionary.add("boy");
        femaleGenderDictionary.add("lady");
        femaleGenderDictionary.add("girl");
        femaleGenderDictionary.add("female");
        femaleGenderDictionary.add("woman");
    }

    public void addFriendToDataBase(String friendName, int age, Friend.Gender gender) {
        if(!friendDataBase.containsKey(friendName)){
            friendDataBase.put(friendName,new Friend(friendName,age,gender));
        } else {
            throw new FriendExistException("'"+friendName+"'");
        }
    }

    public Friend searchAndGetFriendByName(String friendName) {
        if(friendDataBase.containsKey(friendName)){
            return friendDataBase.get(friendName);
        } else {
            throw new NotFriendExistException("'"+friendName+"'");
        }
    }

    public String removeFriendByName(String friendName) {
        if(friendDataBase.containsKey(friendName)){
            Friend friend = friendDataBase.get(friendName);
            friendDataBase.remove(friendName);
            return friend.getName();
        } else {
            throw new NotFriendExistException("'"+friendName+"'");
        }
    }

    public boolean canSaveFriendMore() {
        return friendDataBase.size() < maximumLimitOfFriends;
    }

    public int getFriendsNumber() {
        return friendDataBase.size();
    }
}
