
public class Character 
{
	
	//Github Mod gijin
	private float attackPoint;
	public Elemental elemental;
	
	public Character(){
		attackPoint = 10;
		elemental = new Normal();
	}
	public Character(float attackPoint, Elemental elemental){
		this.attackPoint = attackPoint;
		this.elemental = elemental;
	}
	
	public Elemental getElemental(){ return elemental; }
	public void setElemental(Elemental elemental){
		this.elemental = elemental;
	}
	
	public float attack(){
		return attackPoint + elemental.getEffect();
	}
}