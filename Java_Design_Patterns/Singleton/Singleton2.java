import java.util.Random;

public class Singleton2 {
	private static Singleton2 instance = null;
	
	
	private int randomNumber;
	
	private Singleton2(){}
	
	public static Singleton2 getInstance(){
	if(instance == null){
	instance = new Singleton2();
	
	Random random = new Random();
	instance.randomNumber = random.nextInt(10)+1;
	}
	return instance;
}

public int getRandomNumber(){ return randomNumber; }
}
 