type variable = {
  coefficient: float,
  minimum: option(float),
  maximum: option(float),
};

type summand = {
  variable: int,
  coefficient: float,
};

type constraint_ = {
  expression: array(summand),
  comparison: string,
  constant: float,
};

type problem = {
  direction: string,
  variables: array(variable),
  constraints: array(constraint_),
};

type solution = {
  values: array(float),
  objective: float,
};

type result = {
  [@bs.as "type"]
  type_: string,
  solution,
  error: string,
};

[@bs.module "@zrho/minilp-wasm"] external solveRaw: problem => result = "solve";

let directionToString = direction =>
  switch (direction) {
  | `Minimize => "minimize"
  | `Maximize => "maximize"
  };

let comparisonToString = comparison =>
  switch (comparison) {
  | `Le => "le"
  | `Ge => "ge"
  | `Eq => "eq"
  };

type comparison = [ | `Le | `Ge | `Eq];
type direction = [ | `Minimize | `Maximize];

let makeVariable = (~minimum=?, ~maximum=?, coefficient) => {minimum, maximum, coefficient};
let makeConstraint = (expression, comparison, constant) => {
  let expression = Array.map(((variable, coefficient)) => {variable, coefficient}, expression);
  let comparison = comparisonToString(comparison);
  {expression, comparison, constant};
};

exception BadFormat;
exception Infeasible;
exception Unbounded;

let solve = (direction, variables, constraints) => {
  let direction = directionToString(direction);
  let result = solveRaw({direction, variables, constraints});
  if (result.type_ == "success") {
    (result.solution.objective, result.solution.values);
  } else if (result.error == "bad_format") {
    raise(BadFormat);
  } else if (result.error == "infeasible") {
    raise(Infeasible);
  } else if (result.error == "unbounded") {
    raise(Unbounded);
  } else {
    assert(false);
  };
};
