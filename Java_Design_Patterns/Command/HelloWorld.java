
public class HelloWorld {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		System.out.println("Hello world");
		
		Character character = new Character();
		System.out.println("attack : " + character.attack());

		character.setElemental(new IceElemental(character.getElemental()));
		System.out.println("attack : " + character.attack());

		character.setElemental(new FireElemental(character.getElemental()));
		System.out.println("attack : " + character.attack());	
		
	}

}
