type t = {
  creditLimit: float,
  balance: float,
};
let charge = (charge, card) => {
  let remainingCredit = card.creditLimit -. card.balance -. charge;
  if (remainingCredit >= 0.) {
    Ok({...card, balance: card.balance +. charge});
  } else {
    Error("Insufficient available credit");
  };
};
let getAvailableFunds = card => card.creditLimit -. card.balance;
