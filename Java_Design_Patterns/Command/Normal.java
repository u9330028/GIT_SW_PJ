import java.io.Writer;
import java.util.List;
import java.util.Map;

import javax.lang.model.element.AnnotationMirror;
import javax.lang.model.element.AnnotationValue;
import javax.lang.model.element.Element;
import javax.lang.model.element.ExecutableElement;
import javax.lang.model.element.Name;
import javax.lang.model.element.PackageElement;
import javax.lang.model.element.TypeElement;
import javax.lang.model.util.Elements;

public class Normal implements Elements, Elemental {
	private float effectPoint;
	
	public Normal(){
		effectPoint = 0;
	}
	
	//@Override
	public float getEffect() {
		return effectPoint;
	}

	@Override
	public List<? extends AnnotationMirror> getAllAnnotationMirrors(Element e) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public List<? extends Element> getAllMembers(TypeElement type) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Name getBinaryName(TypeElement type) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getConstantExpression(Object value) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getDocComment(Element e) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Map<? extends ExecutableElement, ? extends AnnotationValue> getElementValuesWithDefaults(
			AnnotationMirror a) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Name getName(CharSequence cs) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public PackageElement getPackageElement(CharSequence name) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public PackageElement getPackageOf(Element type) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public TypeElement getTypeElement(CharSequence name) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public boolean hides(Element hider, Element hidden) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean isDeprecated(Element e) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean isFunctionalInterface(TypeElement type) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean overrides(ExecutableElement overrider, ExecutableElement overridden, TypeElement type) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void printElements(Writer w, Element... elements) {
		// TODO Auto-generated method stub
		
	}
}