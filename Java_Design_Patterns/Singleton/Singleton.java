
public class Singleton {
private static Singleton instance = new Singleton();
private Singleton(){ }

public static Singleton getInstance(){
return instance;
}
 
//멤버변수 및 메소드
}