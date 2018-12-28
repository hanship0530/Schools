import org.knowm.xchart.QuickChart;
import org.knowm.xchart.SwingWrapper;
import org.knowm.xchart.XYChart;
import java.io.IOException;
import java.util.ArrayList;

public class Process {

    public static void main(String[] args) throws IOException, InterruptedException {
        Reader read = new Reader();
        read.readFile();//MNIST file reading
        System.out.println("Start");
        ArrayList<Double> errors= new ArrayList<>();
        ArrayList<Double> epochs= new ArrayList<>();
        double alpha = 0.0009; //Learning rate
        double beta = 0.00000001; //Momentum rate
        double err = 0.0;

        //Set the number of input, hidden, output layer node
        Mlp p = new Mlp(read.getInputs(), read.getHandDigit(), 320, 28*28, 10);

        p.stepOne(); //Step 1
        // First epoch get accuracy rate, error to draw graph
        for (int ir = 0; ir < 49000; ir++) {
            p.stepTwo(ir);
            err += p.stepThree(ir, alpha, beta);
        }
        err = err/49000; //MSE
        epochs.add(1.0);
        errors.add(err);
        System.out.println("epoch: 1");
        System.out.println("squared sum error: "+err);
        //create chart
        final XYChart chart = QuickChart.getChart("Learning Curve", "Epoch", "Mean Squared Sum Error", "error", epochs, errors);
        //show graph
        final SwingWrapper<XYChart> sw = new SwingWrapper<XYChart>(chart);
        sw.displayChart();

        for (int epoch = 2; epoch < 120; epoch++) { //Step 2
            err = 0.0;
            for (int ir = 0; ir < 49000; ir++) {
                p.stepTwo(ir);
                err += p.stepThree(ir, alpha, beta);
            }
            err = err/49000; //MSE
            epochs.add((double)epoch);
            errors.add(err);
            System.out.println("epoch: "+epoch);
            System.out.println("squared sum error: "+err);

            //Graph update
            Thread.sleep(100);
            chart.updateXYSeries("error", epochs, errors, null);
            sw.repaintChart();
        }

        //Test data
        for(int ir=49000; ir<70000; ir++){
            p.testing(ir);
        }

        //Print Result
        p.printResult();
    }
}
