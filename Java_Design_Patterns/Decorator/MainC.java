
public class MainC {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Player player = new Player();

		MacroPattern pattern = new MacroPattern();
		pattern.add(new Movement(player));
		pattern.add(new Attack(player));

		pattern.run();
	}

}
