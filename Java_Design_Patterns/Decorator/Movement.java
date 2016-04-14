
//concrete command
public class Movement implements Command {
	private Player player;
	
	public Movement(Player player){
	this.player = player;
	}
	
	@Override
	public void execute() {
	player.move();
	}
}