
public class FireElemental extends SpecialElemental {

public FireElemental(Elemental elemental){
super(elemental);
}

@Override
public float getEffect() {
return elemental.getEffect() + 20;  //additional effect point
}
}