# activatorNode
 a vector distance node with a max distance range attribute for the target vector.

---

## Information
This C++ node is useful in rigging for Maya to determine how close the controller object is to the specified targetVector.

## Math
// create a new vector between the origin and target

_MVector_ outputResultVector = inOriginVector - inVector;


// compute distance between the origin and target vector

_double_ array[3] = {

pow(outputResultVector.x, 2);

pow(outputResultVector.y, 2);

pow(outputResultVector.z, 2);

}

_double_ distance = pow(sum(array), 0.5);

_double_ outputUnit = 1 - (distance / maxDistance);

## Demo

![Node Demo](./activator.gif)
