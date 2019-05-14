type t = float;
let charge = (charge, balance) => {
  let remaining = balance -. charge;
  remaining >= 0. ? Ok(remaining) : Error("Insufficient funds");
};
let getAvailableFunds = f => f;
