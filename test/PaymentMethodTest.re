module Make =
       (
         SUT: {
           module PaymentMethod: Types.PaymentMethod;
           let name: string;
           let initializeWithFunds: float => PaymentMethod.t;
         },
       ) => {
  open TestFramework;
  module PaymentMethod = SUT.PaymentMethod;

  describe(
    SUT.name,
    ({test}) => {
      test(
        "Initializing with 10 should leave 10 in available funds", ({expect}) => {
        let paymentMethod = SUT.initializeWithFunds(10.);

        let availableFunds = PaymentMethod.getAvailableFunds(paymentMethod);

        expect.float(availableFunds).toBeCloseTo(10.);
      });
      test(
        "Spending the initialized balance should leave 0 funds available",
        ({expect}) => {
        let paymentMethod = SUT.initializeWithFunds(10.);

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
      test("Spending 5 with 10 available should leave 5", ({expect}) => {
        let paymentMethod = SUT.initializeWithFunds(10.);

        let transactionResult = PaymentMethod.charge(5., paymentMethod);

        expect.result(transactionResult).toBeOk();

        let paymentMethod =
          switch (transactionResult) {
          | Ok(method) => method
          | Error(e) => raise(Invalid_argument(e))
          };

        let availableFunds = PaymentMethod.getAvailableFunds(paymentMethod);

        expect.float(availableFunds).toBeCloseTo(5.);
      });
      test(
        "Spending more than the available balance should return Error",
        ({expect}) => {
        let paymentMethod = SUT.initializeWithFunds(10.);

        let transactionResult = PaymentMethod.charge(42., paymentMethod);

        expect.result(transactionResult).toBeError();
      });
    },
  );
};
