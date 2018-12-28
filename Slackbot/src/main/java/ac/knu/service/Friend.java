package ac.knu.service;

import lombok.Data;

@Data
public class Friend {
    private String name;
    private int age;
    public enum Gender {MAN,FEMALE}
    private Gender gender;
    public Friend(String name, int age, Gender gender) {
        this.name = name;
        this.age = age;
        this.gender = gender;
    }
    public String getFriendInformation() {
        return this.name+", "+String.valueOf(this.age)+", "+String.valueOf(this.gender);
    }
}
