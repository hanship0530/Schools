package kr.ac.knu.lecture.study;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

/**
 * Created by rokim on 2018. 11. 16..
 */
@Component
public class Calculator {
    private final MultiplyOperator multiplyOperator;
    private final DivideOperator divideOperator;

    public Calculator(MultiplyOperator multiplyOperator, DivideOperator divideOperator) {
        this.multiplyOperator = multiplyOperator;
        this.divideOperator = divideOperator;
    }

    public int plus(int a, int b) {
        return a + b;
    }

    public int multiply(int a, int b) {
        return multiplyOperator.multiply(a, b);
    }

    public int divide(int a, int b) {
        return divideOperator.divide(a, b);
    }
}
