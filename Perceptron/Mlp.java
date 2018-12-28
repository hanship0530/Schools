import java.util.Random;

public class Mlp {
    private static int[][] inputs; //MNIST.txt file contents
    private static int[] handDigit; //MNIST.txt file digit
    private int hidNeuronN; //hidden layer node number
    private int inNeuronN; //input layer node number
    private int outNeuronN; //output layer node number
    private double[] deltaK; //output layer error gredient
    private double[] deltaJ; //hidden layer error gredient
    private double[] thetaK; //output threshold
    private double[] thetaJ; //hidden threshold
    private double[] yK; //output
    private double[] yJ; //hidden layer output
    private double[] errK; //output error
    private double[] yDk; //desired output
    private double[][] wIj; //weight input to hidden layer
    private double[][] wJk; //weight hidden layer to output
    private double[][] prevWij; //previous weight input to hidden layer
    private double[][] prevWjk; //previous weight hidden layer to output
    private double max; //max random number
    private double min; //min random number
    private double[] accuracyCount;
    private double[] totalCount;

    public Mlp(int[][] input, int[] out, int hidN, int inN, int outN) {
        inputs = input;
        handDigit = out;
        hidNeuronN = hidN;
        inNeuronN = inN;
        outNeuronN = outN;
        deltaK = new double[outN];
        deltaJ = new double[hidN];
        thetaK = new double[outN];
        thetaJ = new double[hidN];
        yK = new double[outN];
        yJ = new double[hidN];
        errK = new double[outN];
        yDk = new double[]{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        wIj = new double[inN][hidN];
        wJk = new double[hidN][outN];
        accuracyCount = new double[]{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        totalCount = new double[]{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        prevWij = new double[inN][hidN];
        prevWjk = new double[hidN][outN];
        max = 4.0 / inNeuronN; //max random number
        min = -4.0 / inNeuronN; //min random number
    }

    //Initialize weights, threshold
    public void stepOne() {
        Random rand = new Random();
        for(int i=0; i<inNeuronN; i++) {
            for (int j = 0; j < hidNeuronN; j++) {
                wIj[i][j] = min + (max - min) * rand.nextDouble();
            }
        }
        for(int j=0; j<hidNeuronN; j++) {
            for (int k = 0; k < outNeuronN; k++) {
                wJk[j][k] = min + (max - min) * rand.nextDouble();
            }
        }
        for(int j=0; j<hidNeuronN; j++){
            thetaJ[j] = min + (max - min) * rand.nextDouble();
        }
        for(int k=0; k<outNeuronN; k++){
            thetaK[k] = min + (max - min) * rand.nextDouble();
        }
    }

    //proceed outputs
    public void stepTwo(int ir) {
        for(int j=0; j<hidNeuronN; j++) {
            double sum = 0.0;
            for (int i = 0; i < inNeuronN; i++) {
                sum += inputs[ir][i] * wIj[i][j];
            }
            yJ[j] = sig(sum - thetaJ[j]);
        }
        for(int k=0; k<outNeuronN; k++) {
            double sum = 0.0;
            for (int j = 0; j < hidNeuronN; j++){
                sum += yJ[j]*wJk[j][k];
            }
            yK[k] = sig(sum - thetaK[k]);
        }
    }

    //Back propagation(weight training, threshold training)
    public double stepThree(int ir, double alpha, double beta) {
        double err=0.0;

        //Save previous weights
        for(int i=0; i<inNeuronN; i++){
            for(int j=0;j<hidNeuronN;j++){
                prevWij[i][j] = wIj[i][j];
            }
        }
        for(int j=0; j<hidNeuronN; j++){
            for(int k=0;k<outNeuronN;k++){
                prevWjk[j][k] = wJk[j][k];
            }
        }

        //set desired output is 1.0
        yDk[handDigit[ir]] = 1.0;

        //get error
        for(int k=0; k<outNeuronN; k++) {
            errK[k] = yDk[k] - yK[k];
        }
        for(int k=0; k<outNeuronN; k++){
            err += Math.pow(errK[k],2);
        }

        //
        for(int k=0; k<outNeuronN; k++){
            deltaK[k] = yK[k]*(1.0-yK[k])*errK[k];
            thetaK[k] = thetaK[k] - alpha*deltaK[k];
        }

        //turn back desired output
        yDk[handDigit[ir]] = 0.0;


        for(int j=0; j<hidNeuronN; j++){
            for(int k=0; k<outNeuronN; k++){
                wJk[j][k] += beta*prevWjk[j][k]+alpha*yJ[j]*deltaK[k];
            }
        }

        for(int j=0; j<hidNeuronN; j++){
            double sum = 0.0;
            for(int k=0; k<outNeuronN; k++){
                sum += deltaK[k]*wJk[j][k];
            }
            deltaJ[j] = yJ[j]*(1.0-yJ[j])*sum;
            thetaJ[j] = thetaJ[j] - alpha*deltaJ[j];
        }

        for(int i=0; i<inNeuronN; i++){
            for(int j=0; j<hidNeuronN; j++){
                wIj[i][j] += beta*prevWij[i][j] + alpha*inputs[ir][i]*deltaJ[j];
            }
        }
        return err;
    }

    //testing Test data
    public void testing(int ir){
        for(int j=0; j<hidNeuronN; j++) {
            double sum = 0.0;
            for (int i = 0; i < inNeuronN; i++) {
                sum += inputs[ir][i] * wIj[i][j];
            }
            yJ[j] = sig(sum - thetaJ[j]);
        }
        for(int k=0; k<outNeuronN; k++) {
            double sum = 0.0;
            for (int j = 0; j < hidNeuronN; j++){
                sum += yJ[j]*wJk[j][k];
            }
            yK[k] = sig(sum - thetaK[k]);
        }

        int maxIndex = getMaxIndex(yK);

        if(handDigit[ir] == maxIndex) {
            accuracyCount[maxIndex] += 1.0;
            totalCount[maxIndex] += 1.0;
        } else {
            totalCount[handDigit[ir]] += 1.0;
        }
    }


    public void printResult() {
        System.out.println("Result...........................................");
        for(int k=0; k<outNeuronN; k++){
            System.out.println(k+"'s accuracy rate is "+String.format("%.2f",accuracyCount[k]/totalCount[k]*100)+"%");
            System.out.println(k+"'s AccuracyCount is "+accuracyCount[k]);
            System.out.println(k+"'s Total Count is "+totalCount[k]);
        }
    }

    //Get highest output value Index
    private int getMaxIndex(double[] arr) {
        double maxValue = arr[0];
        int maxIndex = 0;
        for(int k=1; k<arr.length; k++){
            if(arr[k] > maxValue){
                maxValue = arr[k];
                maxIndex = k;
            }
        }
        return maxIndex;
    }
    private double sig(double x){
        x = (-1.0)*x;
        return 1 / (1 + Math.exp(x));
    }
}
