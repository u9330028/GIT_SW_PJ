import java.util.LinkedList;

//invoker
public class MacroPattern {
	private LinkedList<Command> cmdList;
	
	public MacroPattern(){
	cmdList = new LinkedList<Command>();
	}
	
	public void add(Command command){
	cmdList.add(command);
	}
	
	public void run(){
	for(Command command : cmdList){
	command.execute();
	}
	}
}