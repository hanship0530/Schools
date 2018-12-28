package kr.ac.knu.lecture.domain;

import lombok.Data;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Entity
@Data
public class Board {
    @Id
    @GeneratedValue
    private long nid;
    private String title;
    private String content;
}
