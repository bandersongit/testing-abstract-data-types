include PaymentMethodTest.Make({
  include Cash;
  let name = "Cash";
  let initialize = x => x;
});
