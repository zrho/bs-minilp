type variable;
type constraint_;

type comparison = [ | `Le | `Ge | `Eq];
type direction = [ | `Minimize | `Maximize];

exception Infeasible;
exception Unbounded;

let makeVariable: (~minimum: float=?, ~maximum: float=?, float) => variable;
let makeConstraint: (array((int, float)), comparison, float) => constraint_;

let solve: (direction, array(variable), array(constraint_)) => (float, array(float));
