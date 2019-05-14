include PaymentMethodTest.Make({
  module PaymentMethod = Cash;
  let name = "Cash";
  let initializeWithFunds = x => x;
});
