module type PaymentMethod = {
  type t;
  let charge: (float, t) => result(t, string);
  let getAvailableFunds: t => float;
};
