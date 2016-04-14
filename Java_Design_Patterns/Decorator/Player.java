
public class Player {
	private String name;


	public void move(){
		System.out.println("move character");
	}
	
	public void attack(){
		System.out.println("attack Monster");
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
}