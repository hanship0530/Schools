package ac.knu.service;

import ac.knu.exceptions.FriendDataBaseAlreadyFullException;
import ac.knu.exceptions.FriendDataBaseEmptyException;

import java.util.Date;

public class CommandService {
    public String addCommandExecute(FriendDataBase friendDataBase, String friendName, int age, Friend.Gender gender) {
        if(friendDataBase.canSaveFriendMore()){
            friendDataBase.addFriendToDataBase(friendName, age, gender);
            return friendName+" is added successfully!";
        } else {
            throw new FriendDataBaseAlreadyFullException();
        }
    }

    public String removeCommandExecute(FriendDataBase friendDataBase, String friendName) {
        return friendDataBase.removeFriendByName(friendName)+" is removed successfully!";
    }

    public String findCommandExecute(FriendDataBase friendDataBase, String friendName) {
        return friendDataBase.searchAndGetFriendByName(friendName).getFriendInformation();
    }

    public String timeCommandExecute() {
        return "Current Time: " + new Date().toString();
    }

    public String listCommandExecute(FriendDataBase friendDataBase) {
        if( friendDataBase.getFriendsNumber() != 0) {
            StringBuilder friendListTable = new StringBuilder();
            for(Friend friend : friendDataBase.getFriendDataBase().values()) {
                friendListTable.append(friend.getFriendInformation()).append("\n");
            }
            return friendListTable.toString();
        } else {
            throw new FriendDataBaseEmptyException();
        }
    }
}
