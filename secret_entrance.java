import java.nio.file.Files;                 
import java.nio.file.Path;
import java.io.IOException;

public class SecretEntrance {
    
    static String[] example = new String[]{"L68", "L30", "R48", "L5", "R60", "L55", "L1", "L99", "R14", "L82"};
    static String fileName = "secret_entrance.txt";
    static int startingPosition = 50;
    static int minPosition = 0;
    static int maxPosition = 99;
    static int loopBack = (maxPosition - minPosition) + 1;


    public static void main(String[] args) {
        numberOfExactZeros(fileName);
        numberOfPassesOverZero(fileName);
    }

    public static void numberOfExactZeros(String filepath){

        String[] codeCombination;
        try {
            codeCombination = Files.readString(Path.of(filepath)).split(System.lineSeparator());
        }
        catch(IOException e) {
            System.out.println(e);
            return;
        }
        int currentPosition = startingPosition;
        int zeros_count = 0;
        for (String stepEntry : codeCombination) {
            int step_size = Integer.parseInt(stepEntry.substring(1));
            int step = stepEntry.charAt(0) == 'R' ? step_size : -step_size;
            currentPosition = (currentPosition + step) % loopBack;
            if (currentPosition < 0) {
                currentPosition += loopBack;
            }
            zeros_count += (currentPosition == 0) ? 1 : 0;

            //System.out.println(stepEntry);
            //System.out.println(currentPosition);
        }
        System.out.println(String.format("numberOfExactZeros: Found %d zeros (0)", zeros_count));
    }

 public static void numberOfPassesOverZero(String filepath){

        String[] codeCombination;
        try {
            codeCombination = Files.readString(Path.of(filepath)).split(System.lineSeparator());
        }
        catch(IOException e) {
            System.out.println(e);
            return;
        }

        int currentPosition = startingPosition;
        int zeros_count = 0;
        for (String stepEntry : codeCombination) {
            int step_size = Integer.parseInt(stepEntry.substring(1));
            
            int full_rotations = step_size / loopBack;
            zeros_count += full_rotations;
            
            int partial_rotation = step_size % loopBack;
            int step = stepEntry.charAt(0) == 'R' ? partial_rotation : -partial_rotation;
            
            boolean was_at_zero = currentPosition == 0;
            boolean crossed_zero = false;
            
            currentPosition += step;
            if (currentPosition < 0) {
                currentPosition += loopBack;
                crossed_zero = true;
            } else if (currentPosition >= loopBack){
                currentPosition %= loopBack;
                crossed_zero = true;
            } else if (currentPosition == 0) {
                crossed_zero = true;
            }
            zeros_count += (!was_at_zero && crossed_zero) ? 1 : 0;
        }
        System.out.println(String.format("numberOfPassesOverZero: Found %d zeros (0)", zeros_count));
    }
}