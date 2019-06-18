# Testing Abstract Data Types

Proper abstraction is an important part of writing highly maintainable and usable software. In this article, I aim to address how explicitly testing against abstract data types can help you write more robust tests (that don't depend on implementation details) and avoid duplicating code.

### What is an Abstract Data Type (ADT)?

> An abstract data type is a set of values and associated operations that are specified indepent of any concrete implementation.
>
> -- Paul E. Black, [Dictionary of Algorithms and Data Structures]( https://xlinux.nist.gov/dads/HTML/abstractDataType.html)

While the formal definition of an abstract data type may not be extraordinarily intuitive, it is likely that you are familiar with many concrete examples. Stacks, lists, queues, and dictionaries are all common examples of ADT's.

We can define an immutable Stack ADT in [Reason](https://reasonml.github.io/en/) as a module that satisfies the following signature and informally expressed properties

```reason
module type Stack = {
    type t('a);
    let empty: t('a);
    let push: ('a, t('a)) => t('a);
    let pop: (t('a)) => t('a);
    let top: (t('a)) => 'a;
};
```

- The last thing pushed onto a stack should be returned by top
- The last thing pushed onto a stack should be the first thing returned by Stack.pop
- Popping from an empty stack is invalid

We could also express this ADT using a record type, or with an interface in an object oriented language. These approaches are also valid and can make sense depending on your use case. For the purposes of this article we will be using module types.

### How is this relevant to the applications/libraries that I am creating?

While data structures are certainly the most obvious examples of ADT's, they are in fact quite common in many applications. We will walk through an example that can hopefully be applied more generally. We will be using [Rely](https://reason-native.com/docs/rely/) to write our tests.

### Explicitly Identifying our ADT

When going through this exercise. It is helpful to first explicitly identify both the type of the ADT (which hopefully already exists in some form in your application) as well as the explicit set of rules that we expect to be true of any instance of the ADT. This set of rules will be the basis for our tests cases.

For this example, we define our ADT using the following module type (note that this could certainly be expressed as a record type as well)

```reason
module type PaymentMethod = {
  type t;
  let pay: (float, t) => result(t, string);
  let getAvailableFunds: t => float;
};

/* Note that in a real application you would almost certainly want a separate money type that stores both currency and amount, however for the sake of simplicity, we are using float here instead*/
```

Based on this type and our intuitive understanding of what a payment method is, we can develop some plain English rules to describe what we expect to be true of _any_ payment method.

- If there are $N in available funds and $M is spent, there should be $(N-M) remaining
- Spending more than is available should return an Error

### Translating to test cases

Now we can begin to translate the rules into actual code. In Reason, a natural way to build these tests independent of implementation is by using a functor (module function). Again note that if we defined our ADT using a record type we could use ordinary functions here instead.

### PaymentMethodTest.re

```reason
module Make =
       (
         SUT: {
           module PaymentMethod: Types.PaymentMethod;
            /*
            * In addition to the module we are interested in testing, it can be helpful to
            * include metadata (such as the name of the implementation being tested) so
            * that the resulting tests are descriptively named.
            */
           let name: string;
            /*
            * Additionally when testing ADT's without a clear means of construction
            * (such as empty in the case of data structures), it can be helpful to specify
            * some kind of builder interface independent of the ADT itself for use in tests.
            * This makes sense in the payment method example where we could imagine that
            * the logic for creating a credit card is different from that for creating a
            * gift card or cash.
            */
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
```

Given this functor, we can now easily add test coverage for new payment methods that we make

### CreditCardTest.re

```reason
include PaymentMethodTest.Make({
  module PaymentMethod = CreditCard;

  let name = "Credit Card";
  let initializeWithFunds = x => CreditCard.{creditLimit: x, balance: 0.};
});

```

### CashTest.re

```reason
include PaymentMethodTest.Make({
  module PaymentMethod = Cash;
  let name = "Cash";
  let initializeWithFunds = x => x;
});
```

For the full code example see [the accompanying github repository](https://github.com/bandersongit/testing-abstract-data-types)

I hope that this concept is useful. Please let me know if there are any other testing topics you would like to see my cover by reaching out on twitter(@the_banderson) or the [reason discord](https://discordapp.com/invite/reasonml)


I first learned about testing ADT's (and much more) from Zoran Horvat's excellent puralsight course on [Writing Highly Maintainable Unit Tests](https://www.pluralsight.com/courses/writing-highly-maintainable-unit-tests
) (not an affiliate link), which presents similar material from an object oriented perspective using C#.
