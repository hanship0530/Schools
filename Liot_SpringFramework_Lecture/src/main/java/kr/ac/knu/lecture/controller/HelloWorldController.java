package kr.ac.knu.lecture.controller;

import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by rokim on 2018. 11. 16..
 */
@RestController
@Slf4j
public class HelloWorldController {
    @GetMapping("/hello")
    public String helloWorld() {
        log.info("Hello!!");
        return "Hello, World!";
    }
}
