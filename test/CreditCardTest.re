include PaymentMethodTest.Make({
  module PaymentMethod = CreditCard;
  let name = "Credit Card";
  let initializeWithFunds = x => CreditCard.{creditLimit: x, balance: 0.};
});
