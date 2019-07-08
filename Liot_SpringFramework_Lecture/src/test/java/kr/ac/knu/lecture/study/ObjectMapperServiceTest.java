package kr.ac.knu.lecture.study;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import kr.ac.knu.lecture.domain.Student;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import static org.junit.Assert.*;

/**
 * Created by rokim on 2018. 11. 16..
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class ObjectMapperServiceTest {
    @Autowired
    private ObjectMapper mapper;

    @Test
    public void testSerializeJson() throws JsonProcessingException {
        Student student = new Student();
        student.setName("Robin");
        student.setScore(100);

//        ObjectMapper mapper = new ObjectMapper();
        String result = mapper.writeValueAsString(student);

        System.out.println(result);
    }
}