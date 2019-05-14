include PaymentMethodTest.Make({
  include CreditCard;
  let name = "Credit Card";
  let initialize = x => {creditLimit: x, balance: 0.};
});
