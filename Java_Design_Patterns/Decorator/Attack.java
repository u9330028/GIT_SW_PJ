
//concrete command
public class Attack implements Command {
	private Player player;
	
	public Attack(Player player){
	this.player = player;
	}
	
	@Override
	public void execute() {
	player.attack();
	}
}
