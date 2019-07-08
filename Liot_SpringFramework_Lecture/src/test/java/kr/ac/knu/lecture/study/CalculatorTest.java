package kr.ac.knu.lecture.study;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import static org.assertj.core.api.Assertions.assertThat;


/**
 * Created by rokim on 2018. 11. 16..
 */
@RunWith(SpringRunner.class)
@SpringBootTest
public class CalculatorTest {
    @Autowired
    private Calculator calculator;

    @Test
    public void testPlus() {
        int plusResult = calculator.plus(5, 10);

        assertThat(plusResult).isEqualTo(15);
    }

    @Test
    public void testMultiply() {
        int multiplyResult = calculator.multiply(5, 10);

        assertThat(multiplyResult).isEqualTo(50);

    }

    @Test
    public void testDivide() {
        int divideResult = calculator.divide(10, 0);

        assertThat(divideResult).isEqualTo(0);
    }

    @Test
    public void testDivide2() {
        int divideResult = calculator.divide(-10, 0);

        assertThat(divideResult).isEqualTo(1);
    }

}