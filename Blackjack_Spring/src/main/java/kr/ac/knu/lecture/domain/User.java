package kr.ac.knu.lecture.domain;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import javax.persistence.Entity;
import javax.persistence.Id;

/**
 * Created by rokim on 2018. 11. 30..
 */
@Data
@Entity
@AllArgsConstructor
@NoArgsConstructor
public class User {
    @Id
    private String name;
    private OAuthProvider oAuthProvider;
    private String oAuthProviderId;
    private long account;
}
