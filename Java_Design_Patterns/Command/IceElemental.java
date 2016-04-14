
public class IceElemental extends SpecialElemental {

public IceElemental(Elemental elemental){
super(elemental);
}

@Override
public float getEffect() {
return elemental.getEffect() + 10; //additional effect point
}
}