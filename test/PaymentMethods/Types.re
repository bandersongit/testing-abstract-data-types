module type PaymentMethod = {
    type t;
    let charge: (float, t) => t;
    let getAvailableFunds: (t) => float;
}
