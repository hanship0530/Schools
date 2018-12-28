package kr.ac.knu.lecture.study;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Component;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Component
public class DivideOperator {
    @Autowired
    private ArithmeticExceptionHandler arithmeticExceptionHandler;

    public int divide(int a, int b) {
        try {
            return a / b;
        } catch(ArithmeticException e) {
            return arithmeticExceptionHandler.handlerException(a);
        }
    }
}
