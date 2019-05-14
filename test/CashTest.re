include PaymentMethodTest.Make({
  include Cash;
  let name = "Cash";
  let initializeWithFunds = x => x;
});
