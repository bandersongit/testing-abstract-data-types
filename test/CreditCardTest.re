include PaymentMethodTest.Make({
  include CreditCard;
  let name = "Credit Card";
  let initializeWithFunds = x => {creditLimit: x, balance: 0.};
});
