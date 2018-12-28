import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;


public class Reader {
    private static int[] handDigit = new int[70000];
    private static int[][] inputs = new int[70000][28*28];

    public Reader() { }

    public void readFile() throws IOException {
        FileReader fr = new FileReader("MNIST.txt");
        BufferedReader br = new BufferedReader(fr);

        String lines;
        int count = 1, i=0, r=0, c=0;
        while((lines = br.readLine()) != null) {
            if((count % 29) == 1){
                handDigit[i++] = Integer.parseInt(lines);
                count++;
            } else if((count % 29) == 0) {
                String[] temp = lines.split(" ");
                for(int a=0; a< temp.length; a++) {
                    inputs[r][c] = Integer.parseInt(temp[a]);
                    c++;
                }
                count++;
                r++;
                c=0;
            } else {
                String[] temp = lines.split(" ");
                for(int a=0; a< temp.length; a++) {
                    inputs[r][c] = Integer.parseInt(temp[a]);
                    c++;
                }
                count++;
            }
        }
        br.close();
        fr.close();
    }

    public int[] getHandDigit() {
        return handDigit;
    }
    public int[][] getInputs() {
        return inputs;
    }
}
