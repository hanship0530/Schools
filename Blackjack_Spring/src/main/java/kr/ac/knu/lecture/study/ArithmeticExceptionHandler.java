package kr.ac.knu.lecture.study;

import org.springframework.stereotype.Component;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Component
public class ArithmeticExceptionHandler {
    public int handlerException(int target) {
        if (target > 0) {
            return 0;
        } else {
            return 1;
        }
    }
}
