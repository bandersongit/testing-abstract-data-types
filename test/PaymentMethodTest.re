module type PaymentMethodTestConfig = {
  include Types.PaymentMethod;
  let name: string;
  let initializeWithFunds: float => t;
};

module Make = (PaymentMethod: PaymentMethodTestConfig) => {
  open TestFramework;

  describe(
    PaymentMethod.name,
    ({test}) => {
      test("Initializing with 10 should leave 10 in available funds", ({expect}) => {
        let paymentMethod = PaymentMethod.initializeWithFunds(10.);

        let availableFunds = PaymentMethod.getAvailableFunds(paymentMethod);

        expect.float(availableFunds).toBeCloseTo(10.);
      });
      test("Spending the initialized balance should leave 0 funds available", ({expect}) => {
        let paymentMethod = PaymentMethod.initializeWithFunds(10.);

        let transactionResult = PaymentMethod.charge(10., paymentMethod);

        expect.result(transactionResult).toBeOk();

        let paymentMethod =
          switch (transactionResult) {
          | Ok(method) => method
          | Error(e) => raise(Invalid_argument(e))
          };

        let availableFunds = PaymentMethod.getAvailableFunds(paymentMethod);

        expect.float(availableFunds).toBeCloseTo(0.);
      });
      test("Spending more than the available balance should return Error", ({expect}) => {
        let paymentMethod = PaymentMethod.initializeWithFunds(10.);

        let transactionResult = PaymentMethod.charge(42., paymentMethod);

        expect.result(transactionResult).toBeError();
      })
    },
  );
};
